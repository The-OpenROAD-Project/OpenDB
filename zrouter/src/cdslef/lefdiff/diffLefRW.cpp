//
//       Copyright (C) 1997 Cadence Design Systems Inc.
//                     All Rights Reserved.
//
 
// This program is the diffLef core program.  It has all the callback
// routines and write it out to a temporary file

#include <stdio.h>
#include <string.h>
#ifndef WIN32
#   include <unistd.h>
#endif /* not WIN32 */
#include "lefrReader.hpp"
#include "lefwWriter.hpp"
#include "lefiDebug.hpp"

char defaultName[128];
char defaultOut[128];
FILE* fout;
int printing = 0;     // Printing the output.
int userData;

// TX_DIR:TRANSLATION ON
 
void dataError() {
  fprintf(fout, "ERROR: returned user data is not correct!\n");
}
 

void checkType(lefrCallbackType_e c) {
  if (c >= 0 && c <= lefrLibraryEndCbkType) {
    // OK
  } else {
    fprintf(fout, "ERROR: callback type is out of bounds!\n");
  }
}

// 05/24/2001 - Wanda da Rosa.  PCR 373170
// This function is added due to the rounding between machines are
// different.  For a 5, solaries will round down while hppa will roundup.
// This function will make sure it round up for all the machine
double chkNum(double num) {
  long tempNum;
  if ((num > 1000004) || (num < -1000004)) {
     tempNum = (long)num;
     if ((tempNum%5) == 0)
        return num + 3;
  }
  return num;
}

// Print Via
void lefVia(lefiVia* via, char* inName) {
  int i, j;

  fprintf(fout, "%s %s", inName, via->name());
  if (via->hasDefault())
     fprintf(fout, " DEFAULT");
  if (via->hasTopOfStack())
     fprintf(fout, " TOPOFSTACKONLY");
  if (via->hasForeign()) {
     fprintf(fout, " FOREIGN %s", via->foreign());
     if (via->hasForeignPnt()) {
        fprintf(fout, " ( %g %g )", chkNum(via->foreignX()),
                chkNum(via->foreignY()));
        if (via->hasForeignOrient())
           fprintf(fout, " %s", lefiOrientStr(via->foreignOrient()));
     }
  }
  fprintf(fout, "\n");
  if (via->hasProperties()) {
     for (i = 0; i < via->numProperties(); i++) {
        fprintf(fout, "%s %s PROP", inName, via->name());
        fprintf(fout, " %s", via->propName(i));
        if (via->propIsNumber(i))
           fprintf(fout, " %g", chkNum(via->propNumber(i)));
        if (via->propIsString(i))
           fprintf(fout, " %s", via->propValue(i));
        fprintf(fout, "\n");
     }
  }
  if (via->hasResistance())
     fprintf(fout, "%s %s RESISTANCE %g\n",inName, via->name(),
             chkNum(via->resistance()));
  if (via->numLayers() > 0) {
     for (i = 0; i < via->numLayers(); i++) {
        fprintf(fout, "%s %s LAYER %s\n", inName, via->name(),
                via->layerName(i));
        for (j = 0; j < via->numRects(i); j++)
           fprintf(fout, "%s %s RECT ( %g %g ) ( %g %g )\n", inName,
                   via->name(), chkNum(via->xl(i, j)), chkNum(via->yl(i, j)),
                   chkNum(via->xh(i, j)), chkNum(via->yh(i, j)));
     }
  }
  return;
}

// Print Spacing
void lefSpacing(lefiSpacing* spacing, char *inName) {
  fprintf(fout, "%s SAMENET %s %s %g ", inName, spacing->name1(),
          spacing->name2(), chkNum(spacing->distance()));
  if (spacing->hasStack())
     fprintf(fout, " STACK");
  fprintf(fout,"\n");
  return;
}

// Print Via Rule Layer
void lefViaRuleLayer(lefiViaRuleLayer* vLayer, char* viaName) {
  fprintf(fout, "VIARULE %s LAYER %s", viaName, vLayer->name());
  if (vLayer->hasDirection()) {
     if (vLayer->isHorizontal())
        fprintf(fout, " DIRECTION HORIZONTAL");
     if (vLayer->isVertical())
        fprintf(fout, " DIRECTION VERTICAL");
  }
  if (vLayer->hasEnclosure())
     fprintf(fout, " ENCLOSURE %g %g\n", chkNum(vLayer->enclosureOverhang1()),
             chkNum(vLayer->enclosureOverhang2()));
  if (vLayer->hasWidth())
     fprintf(fout, " WIDTH %g TO %g", chkNum(vLayer->widthMin()),
                   chkNum(vLayer->widthMax()));
  if (vLayer->hasResistance())
     fprintf(fout, " RESISTANCE %g", chkNum(vLayer->resistance()));
  if (vLayer->hasOverhang())
     fprintf(fout, " OVERHANG %g", chkNum(vLayer->overhang()));
  if (vLayer->hasMetalOverhang())
     fprintf(fout, " METALOVERHANG %g", chkNum(vLayer->metalOverhang()));
  if (vLayer->hasSpacing())
     fprintf(fout, " SPACING %g BY %g", chkNum(vLayer->spacingStepX()),
             chkNum(vLayer->spacingStepY()));
  if (vLayer->hasRect())
     fprintf(fout, " RECT ( %g %g ) ( %g %g )", chkNum(vLayer->xl()),
             chkNum(vLayer->yl()), chkNum(vLayer->xh()), chkNum(vLayer->yh()));
  fprintf(fout, "\n");
  return;
}
 
// Print Geometry
void prtGeometry(lefiGeometries* geometry, char* inName) {
  int                   numItems = geometry->numItems();
  int                   i, j;
  lefiGeomPath*         path;
  lefiGeomPathIter*     pathIter;
  lefiGeomRect*         rect;
  lefiGeomRectIter*     rectIter;
  lefiGeomPolygon*      polygon;
  lefiGeomPolygonIter*  polygonIter;
  lefiGeomVia*          via;
  lefiGeomViaIter*      viaIter;
 
  for (i = 0; i < numItems; i++) {
     switch (geometry->itemType(i)) {
        case  lefiGeomClassE:
             fprintf(fout, "%s CLASS %s\n", inName, geometry->getClass(i));
             break;
        case lefiGeomLayerE:
             fprintf(fout, "%s LAYER %s\n", inName, geometry->getLayer(i));
             break;
        case lefiGeomLayerMinSpacingE:
             fprintf(fout, "%s SPACING %g\n", inName,
                     chkNum(geometry->getLayerMinSpacing(i)));
             break;
        case lefiGeomLayerRuleWidthE:
             fprintf(fout, "%s DESIGNRULEWIDTH %g\n", inName,
                     chkNum(geometry->getLayerRuleWidth(i)));
             break;
        case lefiGeomWidthE:
             fprintf(fout, "%s WIDTH %g\n", inName,
                     chkNum(geometry->getWidth(i)));
             break;
        case lefiGeomPathE:
             path = geometry->getPath(i);
             fprintf(fout, "%s PATH ", inName);
             for (j = 0; j < path->numPoints; j++) {
                if (j+1 == path->numPoints) // last one on the list
                   fprintf(fout, " ( %g %g )\n", chkNum(path->x[j]),
                           chkNum(path->y[j]));
                else
                   fprintf(fout, " ( %g %g )", chkNum(path->x[j]),
                           chkNum(path->y[j]));
             }
             break;
        case lefiGeomPathIterE:
             pathIter = geometry->getPathIter(i);
             fprintf(fout, "%s PATH ITERATED ", inName);
             for (j = 0; j < pathIter->numPoints; j++)
                fprintf(fout, " ( %g %g )", chkNum(pathIter->x[j]),
                        chkNum(pathIter->y[j]));
             fprintf(fout, " DO %g BY %g STEP %g %g\n",
                     chkNum(pathIter->xStart), chkNum(pathIter->yStart),
                     chkNum(pathIter->xStep), chkNum(pathIter->yStep));
             break;
        case lefiGeomRectE:
             rect = geometry->getRect(i);
             fprintf(fout, "%s RECT ( %g %g ) ( %g %g )\n", inName,
                     chkNum(rect->xl), chkNum(rect->yl), chkNum(rect->xh),
                     chkNum(rect->yh));
             break;
        case lefiGeomRectIterE:
             rectIter = geometry->getRectIter(i);
             fprintf(fout, "%s RECT ITERATE ( %g %g ) ( %g %g )", inName,
                     chkNum(rectIter->xl), chkNum(rectIter->yl),
                     chkNum(rectIter->xh), chkNum(rectIter->yh));
             fprintf(fout, " DO %g BY %g STEP %g %g ;\n",
                     chkNum(rectIter->xStart), chkNum(rectIter->yStart),
                     chkNum(rectIter->xStep), chkNum(rectIter->yStep));
             break;
        case lefiGeomPolygonE:
             polygon = geometry->getPolygon(i);
             fprintf(fout, "%s POLYGON ", inName);
             for (j = 0; j < polygon->numPoints; j++) {
                if (j+1 == polygon->numPoints) // last one on the list
                   fprintf(fout, " ( %g %g )\n", chkNum(polygon->x[j]),
                           chkNum(polygon->y[j]));
                else
                   fprintf(fout, " ( %g %g )", chkNum(polygon->x[j]),
                           chkNum(polygon->y[j]));
             }
             break;
        case lefiGeomPolygonIterE:
             polygonIter = geometry->getPolygonIter(i);
             fprintf(fout, "%s POLYGON ITERATE", inName);
             for (j = 0; j < polygonIter->numPoints; j++)
                   fprintf(fout, " ( %g %g )", chkNum(polygonIter->x[j]),
                           chkNum(polygonIter->y[j]));
             fprintf(fout, " DO %g BY %g STEP %g %g\n",
                     chkNum(polygonIter->xStart), chkNum(polygonIter->yStart),
                     chkNum(polygonIter->xStep), chkNum(polygonIter->yStep));
             break;
        case lefiGeomViaE:
             via = geometry->getVia(i);
             fprintf(fout, "%s VIA ( %g %g ) %s\n", inName, chkNum(via->x),
                     chkNum(via->y), via->name);
             break;
        case lefiGeomViaIterE:
             viaIter = geometry->getViaIter(i);
             fprintf(fout, "%s VIA ITERATE ( %g %g ) %s", inName,
                     chkNum(viaIter->x), chkNum(viaIter->y), viaIter->name);
             fprintf(fout, " DO %g BY %g STEP %g %g\n",
                     chkNum(viaIter->xStart), chkNum(viaIter->yStart),
                     chkNum(viaIter->xStep), chkNum(viaIter->yStep));
             break;
        default:
             fprintf(fout, "BOGUS geometries type.\n");
             break;
     }
  }
}

// Antenna
int antennaCB(lefrCallbackType_e c, double value, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();

  switch (c) {
        case lefrAntennaInputCbkType:
             fprintf(fout, "ANTINPUTGATEAREA %g\n", chkNum(value));
             break;
        case lefrAntennaInoutCbkType:
             fprintf(fout, "ANTINOUTDIFFAREA %g\n", chkNum(value));
             break;
        case lefrAntennaOutputCbkType:
             fprintf(fout, "ANTOUTPUTDIFFAREA %g\n", chkNum(value));
             break;
        case lefrInputAntennaCbkType:
             fprintf(fout, "INPUTPINANTENNASIZE %g\n", chkNum(value));
             break;
        case lefrOutputAntennaCbkType:
             fprintf(fout, "OUTPUTPINANTENNASIZE %g\n", chkNum(value));
             break;
        case lefrInoutAntennaCbkType:
             fprintf(fout, "INOUTPINANTENNASIZE %g\n", chkNum(value));
             break;
        default:
             break;
  }
  return 0;
}

// Array
int arrayCB(lefrCallbackType_e c, lefiArray* a, lefiUserData ud) {
  int              i, j, defCaps;
  lefiSitePattern* pattern;
  lefiTrackPattern* track;
  lefiGcellPattern* gcell;

  checkType(c);
  if ((long)ud != userData) dataError();
   
  if (a->numSitePattern() > 0) {
     for (i = 0; i < a->numSitePattern(); i++) {
        pattern = a->sitePattern(i);
        fprintf(fout, "ARRAY %s SITE %s %g %g %s DO %g BY %g STEP %g %g\n",
               a->name(), pattern->name(), chkNum(pattern->x()),
               chkNum(pattern->y()), lefiOrientStr(pattern->orient()),
               chkNum(pattern->xStart()), chkNum(pattern->yStart()),
               chkNum(pattern->xStep()), chkNum(pattern->yStep()));
     }
  }
  if (a->numCanPlace() > 0) {
     for (i = 0; i < a->numCanPlace(); i++) {
        pattern = a->canPlace(i);
        fprintf(fout, "ARRAY %s CANPLACE %s %g %g %s DO %g BY %g STEP %g %g\n",
               a->name(), pattern->name(), chkNum(pattern->x()),
               chkNum(pattern->y()), lefiOrientStr(pattern->orient()),
               chkNum(pattern->xStart()), chkNum(pattern->yStart()),
               chkNum(pattern->xStep()), chkNum(pattern->yStep()));
     }
  }
  if (a->numCannotOccupy() > 0) {
     for (i = 0; i < a->numCannotOccupy(); i++) {
        pattern = a->cannotOccupy(i);
        fprintf(fout, "ARRAY %s CANNOTOCCUPY %s %g %g %s DO %g BY %g STEP %g %g\n",
               a->name(), pattern->name(), chkNum(pattern->x()),
               chkNum(pattern->y()), lefiOrientStr(pattern->orient()),
               chkNum(pattern->xStart()), chkNum(pattern->yStart()),
               chkNum(pattern->xStep()), chkNum(pattern->yStep()));
     }
  }

  if (a->numTrack() > 0) {
     for (i = 0; i < a->numTrack(); i++) {
        track = a->track(i);
        // 11/22/99 - Wanda da Rosa, PCR 283781.  Merge the track with
        // layer names.  This way, if user split the layer into different
        // line, the diff program won't generate extra lines.
        if (track->numLayers() == 0) {
           fprintf(fout, "ARRAY %s TRACKS %s, %g DO %d STEP %g\n", a->name(),
                   track->name(), chkNum(track->start()), track->numTracks(), 
                   chkNum(track->space())); 
        } else {
           fprintf(fout, "ARRAY %s TRACKS %s, %g DO %d STEP %g ", a->name(),
                   track->name(), chkNum(track->start()), track->numTracks(), 
                   chkNum(track->space())); 
           for (j = 0; j < track->numLayers(); j++)
              fprintf(fout, "%s ", track->layerName(j));
           fprintf(fout, ";\n"); 
        }
     }
  }

  if (a->numGcell() > 0) {
     for (i = 0; i < a->numGcell(); i++) {
        gcell = a->gcell(i);
        fprintf(fout, "ARRAY %s GCELLGRID %s, %g DO %d STEP %g\n", a->name(),
                gcell->name(), chkNum(gcell->start()), gcell->numCRs(),
                chkNum(gcell->space())); 
     }
  }

  if (a->numFloorPlans() > 0) {
     for (i = 0; i < a->numFloorPlans(); i++) {
        for (j = 0; j < a->numSites(i); j++) {
           pattern = a->site(i, j);
           fprintf(fout,
               "ARRAY %s FLOORPLAN %s %s %s %g %g %s DO %d BY %d STEP %g %g\n",
               a->name(), a->floorPlanName(i), a->siteType(i, j),
               pattern->name(), chkNum(pattern->x()), chkNum(pattern->y()),
               lefiOrientStr(pattern->orient()), pattern->xStart(),
               pattern->yStart(), chkNum(pattern->xStep()),
               chkNum(pattern->yStep()));
        }
     }
  }

  defCaps = a->numDefaultCaps();
  if (defCaps > 0) {
     for (i = 0; i < defCaps; i++)
        fprintf(fout, "ARRAY %s DEFAULTCAP %d MINPINS %g WIRECAP %g\n",
                a->name(), defCaps, chkNum(a->defaultCapMinPins(i)),
                chkNum(a->defaultCap(i)));
  }
  return 0;
}

// Busbitchar
int busBitCharsCB(lefrCallbackType_e c, const char* busBit, lefiUserData ud)
{
  checkType(c);
  if ((long)ud != userData) dataError();
  // use the lef writer to write out the data
  fprintf(fout, "BUSBITCHARS %s\n", busBit);
  return 0;
}

// CaseSensitive
int caseSensCB(lefrCallbackType_e c, int caseSense, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();

  if (caseSense == TRUE)
     fprintf(fout, "NAMESCASESENSITIVE ON\n");
  else
     fprintf(fout, "NAMESCASESENSITIVE OFF\n");
  return 0;
}

// Crearance
int clearanceCB(lefrCallbackType_e c, const char* name, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();

  fprintf(fout, "CLEARANCEMEASURE %s\n", name);
  return 0;
}

// Crosstalk correcttable
int correctTableCB(lefrCallbackType_e c, lefiCorrectionTable* table,
                   lefiUserData ud) {
  int                      i, j, k, l;
  lefiCorrectionEdge       *edge;
  lefiCorrectionResistance *resist;
  lefiCorrectionVictim     *victim;
 
  checkType(c);
  if ((long)ud != userData) dataError();

  fprintf(fout, "CROSSTALK CORRECTIONTABLE %d\n", table->num());
  for (i = 0; i < table->numEdges(); i++) {
     edge = table->edge(i);
     // resistances
     if (edge->numResistances()) {
        for (j = 0; j < edge->numResistances(); j++) {
           resist = edge->resistance(j);
           fprintf(fout, "CROSSTALK CORRECTIONTABLE %d RESISTANCE %g\n",
                   table->num(), chkNum(resist->num(j)));
           // victims
           for (k = 0; k < resist->numVictims(); k++) {
              victim = resist->victim(k);
              if (victim->length() > 0)
                 fprintf(fout,
                    "CROSSTALK CORRECTIONTABLE %d RESISTANCE %g VICTIMLEN %g\n",
                         table->num(), chkNum(resist->num(j)),
                         chkNum(victim->length()));
                 for (l = 0; l < victim->numCorrections(); l++)
                    fprintf(fout,
                    "CROSSTALK CORRECTIONTABLE %d RESISTANCE %g VICTIM GTH %g CORRECTIONFACTOR %g\n",
                       table->num(), chkNum(resist->num(j)),
                       chkNum(victim->length()), chkNum(victim->correction(l)));
           }
        }
     }
  }
  return 0;
}

// Dielectric
int dielectricCB(lefrCallbackType_e c, double dielectric, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();

  fprintf(fout, "DIELECTRIC %g\n", chkNum(dielectric));
  return 0;
}

// Divider
int dividerCB(lefrCallbackType_e c, const char* divideChar, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "DIVIDER %s\n", divideChar);
  return 0;
}

// Crosstalk edgeRate
int edgeRateCB(lefrCallbackType_e c, double rate, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "CROSSTALK EDGERATESCALEFACTOR %g\n", chkNum(rate));
  return 0;
}

// Callback routine for edgeratethreshold1
int edgeRate1CB(lefrCallbackType_e c, double rate, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "CROSSTALK EDGERATETHRESHOLD1 %g\n", chkNum(rate));
  return 0;
}

// Callback routine for edgeratethreshold2
int edgeRate2CB(lefrCallbackType_e c, double rate, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "CROSSTALK EDGERATETHRESHOLD2 %g\n", chkNum(rate));
  return 0;
}

// InputAntenna
int inputAntCB(lefrCallbackType_e c, double antenna, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "INPUTINANTENNASIZE %g\n", chkNum(antenna));
  return 0;
}

// OutputAntenna
int outputAntCB(lefrCallbackType_e c, double antenna, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "OUTPUTINANTENNASIZE %g\n", chkNum(antenna));
  return 0;
}


// InOutAntenna
int inoutAntCB(lefrCallbackType_e c, double antenna, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "INOUTPUTINANTENNASIZE %g\n", chkNum(antenna));
  return 0;
}

// Irdrop
int irdropCB(lefrCallbackType_e c, lefiIRDrop* irdrop, lefiUserData ud) {
  int i;
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "IRDROP TABLE %s ", irdrop->lefiIRDrop::name());
  for (i = 0; i < irdrop->lefiIRDrop::numValues(); i++) 
     fprintf(fout, "%g %g ", chkNum(irdrop->lefiIRDrop::value1(i)),
             chkNum(irdrop->lefiIRDrop::value2(i)));
  fprintf(fout, "\n");
  return 0;
}

// Layer
int layerCB(lefrCallbackType_e c, lefiLayer* layer, lefiUserData ud) {
  int                 i, j, k;
  double              *widths, *current;
  lefiLayerDensity*   density;
  char                pType;
  int                 numPoints, propNum;
  lefiAntennaPWL*     pwl;
  int                 needHeading;
  int                 numMinCut, numMinenclosed;
  lefiSpacingTable*   spTable;
  lefiInfluence*      influence;
  lefiParallel*       parallel;
  lefiAntennaModel*   aModel;

  checkType(c);
  if ((long)ud != userData) dataError();
  if (layer->hasType())
     fprintf(fout, "LAYER %s TYPE %s\n", layer->name(), layer->type());
  if (layer->hasPitch())
     fprintf(fout, "LAYER %s PITCH %g\n", layer->name(), chkNum(layer->pitch()));
  if (layer->hasOffset())
     fprintf(fout, "LAYER %s OFFSET %g\n", layer->name(),
             chkNum(layer->offset()));
  if (layer->hasWidth())
     fprintf(fout, "LAYER %s WIDTH %g\n", layer->name(),
             chkNum(layer->width()));
  if (layer->hasArea())
     fprintf(fout, "LAYER %s AREA %g\n", layer->name(),
             chkNum(layer->area()));
  if (layer->hasSlotWireWidth())
     fprintf(fout, "LAYER %s SLOTWIREWIDTH %g\n", layer->name(),
             chkNum(layer->slotWireWidth()));
  if (layer->hasSlotWireLength())
     fprintf(fout, "LAYER %s SLOTWIRELENGTH %g\n", layer->name(),
             chkNum(layer->slotWireLength()));
  if (layer->hasSlotWidth())
     fprintf(fout, "LAYER %s SLOTWIDTH %g\n", layer->name(),
             chkNum(layer->slotWidth()));
  if (layer->hasSlotLength())
     fprintf(fout, "LAYER %s SLOTLENGTH %g\n", layer->name(),
             chkNum(layer->slotLength()));
  if (layer->hasMaxAdjacentSlotSpacing())
     fprintf(fout, "LAYER %s MAXADJACENTSLOTSPACING %g\n", layer->name(),
             chkNum(layer->maxAdjacentSlotSpacing()));
  if (layer->hasMaxCoaxialSlotSpacing())
     fprintf(fout, "LAYER %s MAXCOAXIALSLOTSPACING %g\n", layer->name(),
             chkNum(layer->maxCoaxialSlotSpacing()));
  if (layer->hasMaxEdgeSlotSpacing())
     fprintf(fout, "LAYER %s MAXEDGESLOTSPACING %g\n", layer->name(),
             chkNum(layer->maxEdgeSlotSpacing()));
  if (layer->hasSplitWireWidth())
     fprintf(fout, "LAYER %s SPLITWIREWIDTH %g\n", layer->name(),
             chkNum(layer->splitWireWidth()));
  if (layer->hasMinimumDensity())
     fprintf(fout, "LAYER %s MINIMUMDENSITY %g\n", layer->name(),
             chkNum(layer->minimumDensity()));
  if (layer->hasMaximumDensity())
     fprintf(fout, "LAYER %s MAXIMUMDENSITY %g\n", layer->name(),
             chkNum(layer->maximumDensity()));
  if (layer->hasDensityCheckWindow())
     fprintf(fout, "LAYER %s DENSITYCHECKWINDOW %g %g\n", layer->name(),
             chkNum(layer->densityCheckWindowLength()),
             chkNum(layer->densityCheckWindowWidth()));
  if (layer->hasDensityCheckStep())
     fprintf(fout, "LAYER %s DENSITYCHECKSTEP %g\n", layer->name(),
             chkNum(layer->densityCheckStep()));
  if (layer->hasFillActiveSpacing())
     fprintf(fout, "LAYER %s FILLACTIVESPACING %g\n", layer->name(),
             chkNum(layer->fillActiveSpacing()));
  numMinCut = layer->lefiLayer::numMinimumcut();    // 5.4.1
  if (numMinCut > 0) {
     for (i = 0; i < numMinCut; i++) {
         fprintf(fout, "LAYER %s MINIMUMCUT %d WIDTH %g", layer->name(),
              layer->lefiLayer::minimumcut(i),
              layer->lefiLayer::minimumcutWidth(i));
         if (layer->lefiLayer::hasMinimumcutConnection(i))
            fprintf(fout, " %s", layer->lefiLayer::minimumcutConnection(i));
         if (layer->lefiLayer::hasMinimumcutNumCuts(i))
            fprintf(fout, " LENGTH %g WITHIN %g",
            layer->lefiLayer::minimumcutLength(i),
            layer->lefiLayer::minimumcutDistance(i));
         fprintf(fout, "\n");
     }
  }
  if (layer->lefiLayer::hasMaxwidth())              // 5.4.1
     fprintf(fout, "LAYER %s MAXWIDTH %g\n", layer->name(), layer->maxwidth()); 
  if (layer->lefiLayer::hasMinwidth())              // 5.5
     fprintf(fout, "LAYER %s MINWIDTH %g\n", layer->name(), layer->minwidth()); 
  numMinenclosed = layer->lefiLayer::numMinenclosedarea();
  if (numMinenclosed > 0) {
     for (i = 0; i < numMinenclosed; i++) {  // 5.5, made it multiples
        fprintf(fout, "LAYER %s MINENCLOSEDAREA %g", layer->name(),
               layer->minenclosedarea(i));
        if (layer->lefiLayer::hasMinenclosedareaWidth(i))
           fprintf(fout, " MINENCLOSEDAREAWIDTH %g",
                   layer->minenclosedareaWidth(i));
        fprintf(fout,"\n");
     }
  }
  if (layer->lefiLayer::hasMinstep())               // 5.4.1
     fprintf(fout, "LAYER %s MINSTEP %g\n", layer->name(), layer->minstep());
  if (layer->lefiLayer::hasProtrusion())            // 5.4.1
     fprintf(fout, "LAYER %s PROTRUSIONWIDTH %g LENGTH %g WIDTH %g\n",
             layer->name(), layer->protrusionWidth1(),
             layer->protrusionLength(), layer->protrusionWidth2());
  if (layer->hasSpacingNumber()) {
     for (i = 0; i < layer->numSpacing(); i++) {
        fprintf(fout, "LAYER %s SPACING %g", layer->name(),
                chkNum(layer->spacing(i)));
        if (layer->spacingName(i))
           fprintf(fout, " LAYER %s", layer->spacingName(i));
        fprintf(fout,"\n");
        if (layer->hasSpacingAdjacent(i))
           fprintf(fout, "LAYER %s ADJACENTCUTS %d WITHIN %g\n", layer->name(),
                   layer->spacingAdjacentCuts(i),
                   layer->spacingAdjacentWithin(i));
        if (layer->hasSpacingRange(i)) {
           fprintf(fout, "LAYER %s RANGE %g %g", layer->name(),
                   chkNum(layer->spacingRangeMin(i)),
                   chkNum(layer->spacingRangeMax(i)));
           if (layer->hasSpacingRangeUseLengthThreshold(i))
              fprintf(fout, "USELENGTHTHRESHOLD ");
          else if (layer->lefiLayer::hasSpacingRangeInfluence(i)) {
              fprintf(fout, "INFLUENCE %g ",
                 layer->lefiLayer::spacingRangeInfluence(i));
              if (layer->lefiLayer::hasSpacingRangeInfluenceRange(i))
                 fprintf(fout, "RANGE %g %g ",
                    layer->lefiLayer::spacingRangeInfluenceMin(i),
                    layer->lefiLayer::spacingRangeInfluenceMax(i));
           } else if (layer->lefiLayer::hasSpacingRangeRange(i))
               fprintf(fout, "RANGE %g %g ",
                 layer->lefiLayer::spacingRangeRangeMin(i),
                 layer->lefiLayer::spacingRangeRangeMax(i));
           fprintf(fout,"\n");
        }
        else if (layer->hasSpacingLengthThreshold(i)) {
           fprintf(fout, "LAYER %s LENGTHTHRESHOLD %g\n", layer->name(),
                   chkNum(layer->spacingLengthThreshold(i)));
           if (layer->lefiLayer::hasSpacingLengthThresholdRange(i))
              fprintf(fout, "RANGE %g %g",
                 layer->lefiLayer::spacingLengthThresholdRangeMin(i),
                 layer->lefiLayer::spacingLengthThresholdRangeMax(i));
           fprintf(fout,"\n");
        }
     }
  }
  if (layer->hasCurrentDensityPoint())
     fprintf(fout, "LAYER %s CURRENTDEN %g\n", layer->name(),
             chkNum(layer->currentDensityPoint()));
  if (layer->hasCurrentDensityArray()) {
     layer->currentDensityArray(&numPoints, &widths, &current);
     for (i = 0; i < numPoints; i++)
         fprintf(fout, "LAYER %s CURRENTDEN ( %g %g )\n", layer->name(),
                 chkNum(widths[i]), chkNum(current[i]));
  }
  if (layer->hasDirection())
     fprintf(fout, "LAYER %s DIRECTION %s\n", layer->name(),
             layer->direction());
  if (layer->hasResistance())
     fprintf(fout, "LAYER %s RESISTANCE RPERSQ %g\n", layer->name(),
             chkNum(layer->resistance()));
  if (layer->hasCapacitance())
     fprintf(fout, "LAYER %s CAPACITANCE CPERSQDIST %g\n", layer->name(),
             chkNum(layer->capacitance()));
  if (layer->hasHeight())
     fprintf(fout, "LAYER %s HEIGHT %g\n", layer->name(), chkNum(layer->height()));
  if (layer->hasThickness())
     fprintf(fout, "LAYER %s THICKNESS %g\n", layer->name(),
             chkNum(layer->thickness()));
  if (layer->hasWireExtension())
     fprintf(fout, "LAYER %s WIREEXTENSION %g\n", layer->name(),
             chkNum(layer->wireExtension()));
  if (layer->hasShrinkage())
     fprintf(fout, "LAYER %s SHRINKAGE %g\n", layer->name(),
             chkNum(layer->shrinkage()));
  if (layer->hasCapMultiplier())
     fprintf(fout, "LAYER %s CAPMULTIPLIER %g\n", layer->name(),
             chkNum(layer->capMultiplier()));
  if (layer->hasEdgeCap())
     fprintf(fout, "LAYER %s EDGECAPACITANCE %g\n", layer->name(),
             chkNum(layer->edgeCap()));
  if (layer->hasAntennaArea())
     fprintf(fout, "LAYER %s ANTAREAFACTOR %g\n", layer->name(),
             chkNum(layer->antennaArea()));
  if (layer->hasAntennaLength())
     fprintf(fout, "LAYER %s ANTLENGTHFACTOR %g\n", layer->name(),
             chkNum(layer->antennaLength()));
  for (j = 0; j < layer->numAntennaModel(); j++) { // 5.5
     aModel = layer->lefiLayer::antennaModel(j);

     if (aModel->lefiAntennaModel::hasAntennaAreaRatio())
        fprintf(fout, "LAYER %s %s ANTAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaAreaRatio()));
     if (aModel->lefiAntennaModel::hasAntennaDiffAreaRatio())
        fprintf(fout, "LAYER %s %s ANTDIFFAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaDiffAreaRatio()));
     else if (aModel->lefiAntennaModel::hasAntennaDiffAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaDiffAreaRatioPWL();
        fprintf(fout, "LAYER %s %s ANTDIFFAREARATIO PWL", layer->name(),
                aModel->lefiAntennaModel::antennaOxide());
        for (i = 0; i < pwl->lefiAntennaPWL::numPWL(); i++)
           fprintf(fout, " %g %g", chkNum(pwl->lefiAntennaPWL::PWLdiffusion(i)),
                   chkNum(pwl->lefiAntennaPWL::PWLratio(i)));
        fprintf(fout, "\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaCumAreaRatio())
        fprintf(fout, "LAYER %s %s ANTCUMAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaCumAreaRatio()));
     if (aModel->lefiAntennaModel::hasAntennaCumDiffAreaRatio())
        fprintf(fout, "LAYER %s %s ANTCUMDIFFAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaCumDiffAreaRatio()));
     else if (aModel->lefiAntennaModel::hasAntennaCumDiffAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaCumDiffAreaRatioPWL();
        fprintf(fout, "LAYER %s %s ANTCUMDIFFAREARATIO PWL", layer->name(),
                aModel->lefiAntennaModel::antennaOxide());
        for (i = 0; i < pwl->lefiAntennaPWL::numPWL(); i++)
           fprintf(fout, " %g %g", chkNum(pwl->lefiAntennaPWL::PWLdiffusion(i)),
                   chkNum(pwl->lefiAntennaPWL::PWLratio(i)));
        fprintf(fout, "\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaAreaFactor()) {
        fprintf(fout, "LAYER %s %s ANTAREAFACTOR %g", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaAreaFactor()));
        if (aModel->lefiAntennaModel::hasAntennaAreaFactorDUO())
           fprintf(fout, "  DIFFUSEONLY");
        fprintf(fout, "\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaSideAreaRatio())
        fprintf(fout, "LAYER %s %s ANTSIDEAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaSideAreaRatio()));
     if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatio())
        fprintf(fout, "LAYER %s %s ANTDIFFSIDEAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaDiffSideAreaRatio()));
     else if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaDiffSideAreaRatioPWL();
        fprintf(fout, "LAYER %s %s ANTDIFFSIDEAREARATIO PWL", layer->name(),
                aModel->lefiAntennaModel::antennaOxide());
        for (i = 0; i < pwl->lefiAntennaPWL::numPWL(); i++)
           fprintf(fout, " %g %g", chkNum(pwl->lefiAntennaPWL::PWLdiffusion(i)),
                   chkNum(pwl->lefiAntennaPWL::PWLratio(i)));
        fprintf(fout, "\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaCumSideAreaRatio())
        fprintf(fout, "LAYER %s %s ANTCUMSIDEAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaCumSideAreaRatio()));
     if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatio())
        fprintf(fout, "LAYER %s %s ANTCUMDIFFSIDEAREARATIO %g\n", layer->name(),
                aModel->lefiAntennaModel::antennaOxide(),
                chkNum(aModel->lefiAntennaModel::antennaCumDiffSideAreaRatio()));
     else if (aModel->lefiAntennaModel::hasAntennaCumDiffSideAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaCumDiffSideAreaRatioPWL();
        fprintf(fout, "LAYER %s %s ANTCUMDIFFSIDEAREARATIO PWL", layer->name(),
                aModel->lefiAntennaModel::antennaOxide());
        for (i = 0; i < pwl->lefiAntennaPWL::numPWL(); i++)
           fprintf(fout, "%g %g ", chkNum(pwl->lefiAntennaPWL::PWLdiffusion(i)),
                   chkNum(pwl->lefiAntennaPWL::PWLratio(i)));
        fprintf(fout, "\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaSideAreaFactor()) {
        fprintf(fout, "  ANTSIDEAREAFACTOR %g ",
                chkNum(aModel->lefiAntennaModel::antennaSideAreaFactor()));
        if (aModel->lefiAntennaModel::hasAntennaSideAreaFactorDUO())
           fprintf(fout, "  DIFFUSEONLY ");
        fprintf(fout, "\n");
     }
  }

  if (layer->numAccurrentDensity()) {
     for (i = 0; i < layer->numAccurrentDensity(); i++) {
         density = layer->accurrent(i);
         fprintf(fout, "LAYER %s ACCURRENTDENSITY %s", layer->name(),
                 density->type());
         needHeading = 0;
         if (density->hasOneEntry())
             fprintf(fout, " %g\n", chkNum(density->oneEntry()));
         else {
             if (density->numFrequency()) {
                if (needHeading) {
                   fprintf(fout, "LAYER %s ACCURRENTDENSITY %s", layer->name(),
                   density->type());
                }
                fprintf(fout, " FREQUENCY");
                for (j = 0; j < density->numFrequency(); j++)
                   fprintf(fout, " %g", chkNum(density->frequency(j)));
                fprintf(fout, "\n");
                needHeading = 1;
             }
             if (density->numWidths()) {
                if (needHeading) {
                   fprintf(fout, "LAYER %s ACCURRENTDENSITY %s", layer->name(),
                   density->type());
                }
                fprintf(fout, " WIDTH");
                for (j = 0; j < density->numWidths(); j++)
                   fprintf(fout, " %g", chkNum(density->width(j)));
                fprintf(fout, "\n");
                needHeading = 1;
             }
             if (density->numTableEntries()) {
                if (needHeading) {
                   fprintf(fout, "LAYER %s ACCURRENTDENSITY %s", layer->name(),
                   density->type());
                   k = 0;
                }
                else k = 5;
                fprintf(fout, " TABLEENTRIES");
                for (j = 0; j < density->numTableEntries(); j++) {
                   if (k > 4) {
                      fprintf(fout,
                          "\nLAYER %s ACCURRENTDENSITY %s TABLEENTRIES %g",
                          layer->name(), density->type(),
                          chkNum(density->tableEntry(j)));
                      k = 1;
                   } else {
                      fprintf(fout, " %g", chkNum(density->tableEntry(j)));
                      k++;
                   }
                }
                fprintf(fout, "\n");
                needHeading = 1;
             }
         }
     }
  }
  if (layer->numDccurrentDensity()) {
     for (i = 0; i < layer->numDccurrentDensity(); i++) {
         density = layer->dccurrent(i);
         fprintf(fout, "LAYER %s DCCURRENTDENSITY %s", layer->name(),
                 density->type());
         needHeading = 0;
         if (density->hasOneEntry())
             fprintf(fout, " %g\n", chkNum(density->oneEntry()));
         else {
             if (density->numCutareas()) {
                if (needHeading) {
                   fprintf(fout, "LAYER %s DCCURRENTDENSITY %s", layer->name(),
                   density->type());
                }
                fprintf(fout, " CUTAREA");
                for (j = 0; j < density->numCutareas(); j++)
                   fprintf(fout, " %g", chkNum(density->cutArea(j)));
                fprintf(fout, "\n");
                needHeading = 1;
             }
             if (density->numWidths()) {
                if (needHeading) {
                   fprintf(fout, "LAYER %s DCCURRENTDENSITY %s", layer->name(),
                   density->type());
                }
                fprintf(fout, " WIDTH");
                for (j = 0; j < density->numWidths(); j++)
                   fprintf(fout, " %g", chkNum(density->width(j)));
                fprintf(fout, "\n");
                needHeading = 1;
             }
             if (density->numTableEntries()) {
                if (needHeading) {
                   fprintf(fout, "LAYER %s DCCURRENTDENSITY %s", layer->name(),
                   density->type());
                }
                fprintf(fout, " TABLEENTRIES");
                for (j = 0; j < density->numTableEntries(); j++)
                   fprintf(fout, " %g", chkNum(density->tableEntry(j)));
                fprintf(fout, "\n");
                needHeading = 1;
             }
         }
     }
  }

  for (i = 0; i < layer->numSpacingTable(); i++) {
     spTable = layer->spacingTable(i);
     if (spTable->isInfluence()) {
        influence = spTable->influence();
        for (j = 0; j < influence->numInfluenceEntry(); j++) {
           fprintf(fout, "LAYER %s SPACINGTABLE INFLUENCE WIDTH %g WITHIN %g SPACING %g\n",
                   layer->name(), influence->width(j), influence->distance(j),
                   influence->spacing(j));
        }
     } else {
        parallel = spTable->parallel();
        fprintf(fout, "LAYER %s SPACINGTABLE PARALLELRUNLENGTH", layer->name());
        for (j = 0; j < parallel->numLength(); j++) {
           fprintf(fout, " %g", parallel->length(j));
        }
        fprintf(fout, "\n");
        for (j = 0; j < parallel->numWidth(); j++) {
           fprintf(fout, "LAYER %s SPACINGTABLE PARALLELRUNLENGTH WIDTH %g",
                   layer->name(), parallel->width(j));
           for (k = 0; k < parallel->numLength(); k++) {
              fprintf(fout, " %g", parallel->widthSpacing(j, k));
           }
        }
       fprintf(fout, "\n");
     }
  }
  propNum = layer->numProps();
  if (propNum > 0) {
     for (i = 0; i < propNum; i++) {
        fprintf(fout, "LAYER %s PROP ", layer->name());
        // value can either be a string or number
        fprintf(fout, "%s %s ", layer->propName(i),
                layer->propValue(i));
        pType = layer->propType(i);
        switch (pType) {
           case 'R': fprintf(fout, "REAL");
                     break;
           case 'I': fprintf(fout, "INTEGER");
                     break;
           case 'S': fprintf(fout, "STRING");
                     break;
           case 'Q': fprintf(fout, "QUOTESTRING");
                     break;
           case 'N': fprintf(fout, "NUMBER");
                     break;
        }
        fprintf(fout, "\n");
     }
  }

  return 0;
}

// Macro
int macroCB(lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud) {
  lefiSitePattern* pattern;
  int              propNum, i, hasPrtSym = 0;

  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "MACRO %s", macro->name());
  if (macro->hasClass())
     fprintf(fout, " CLASS %s", macro->macroClass());
  if (macro->hasEEQ())
     fprintf(fout, " EEQ %s", macro->EEQ());
  if (macro->hasLEQ())
     fprintf(fout, " LEQ %s", macro->LEQ());
  if (macro->hasSource())
     fprintf(fout, " SOURCE %s", macro->source());
  if (macro->hasXSymmetry()) {
     fprintf(fout, " SYMMETRY X ");
     hasPrtSym = 1;
  }
  if (macro->hasYSymmetry()) {   // print X Y & R90 in one line
     if (!hasPrtSym) {
        fprintf(fout, "  SYMMETRY Y ");
        hasPrtSym = 1;
     }
     else
        fprintf(fout, "Y ");
  }
  if (macro->has90Symmetry()) {
     if (!hasPrtSym) {
        fprintf(fout, "  SYMMETRY R90 ");
        hasPrtSym = 1;
     }
     else
        fprintf(fout, "R90 ");
  }
  fprintf (fout, "\n");
  if (macro->hasSiteName())
     fprintf(fout, "MACRO %s SITE %s\n", macro->name(), macro->siteName());
  if (macro->hasSitePattern()) {
     for (i = 0; i < macro->numSitePattern(); i++ ) {
       pattern = macro->sitePattern(i);
       fprintf(fout, "MACRO %s SITE %s %g %g %s DO %g BY %g STEP %g %g\n",
             macro->name(), pattern->name(), chkNum(pattern->x()),
             chkNum(pattern->y()), lefiOrientStr(pattern->orient()),
             chkNum(pattern->xStart()), chkNum(pattern->yStart()),
             chkNum(pattern->xStep()), chkNum(pattern->yStep()));
     }
  }
  if (macro->hasSize())
     fprintf(fout, "MACRO %s SIZE %g BY %g\n", macro->name(), 
             chkNum(macro->sizeX()), chkNum(macro->sizeY()));
  if (macro->hasForeign()) {
     fprintf(fout, "MACRO %s FOREIGN %s", macro->name(), macro->foreignName());
     if (macro->hasForeignPoint()) {
        fprintf(fout, " ( %g %g )", chkNum(macro->foreignX()),
                chkNum(macro->foreignY()));
        if (macro->hasForeignOrient())
           fprintf(fout, " %s", lefiOrientStr(macro->foreignOrient()));
     }
     fprintf(fout, "\n");
  }
  if (macro->hasOrigin())
     fprintf(fout, "MACRO %s ORIGIN ( %g %g )\n", macro->name(),
             chkNum(macro->originX()), chkNum(macro->originY()));
  if (macro->hasPower())
     fprintf(fout, "MACRO %s POWER %g\n", macro->name(), chkNum(macro->power()));
  propNum = macro->numProperties();
  if (propNum > 0) {
     for (i = 0; i < propNum; i++) {
        fprintf(fout, "MACRO %s PROP", macro->name());
        // value can either be a string or number
        if (macro->propValue(i)) {
           fprintf(fout, " %s %s\n", macro->propName(i), macro->propValue(i));
        }
        else
           fprintf(fout, " %s %g\n", macro->propName(i),
                   chkNum(macro->propNum(i)));
     }
  }
  return 0;
}

// Manufacturinggrid
int manufacturingCB(lefrCallbackType_e c, double num, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "MANUFACTURINGGRID %g\n", chkNum(num));
  return 0;
}

// Maxviastack
int maxStackViaCB(lefrCallbackType_e c, lefiMaxStackVia* maxStack,
  lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "MAXVIASTACK %d", maxStack->lefiMaxStackVia::maxStackVia());
  if (maxStack->lefiMaxStackVia::hasMaxStackViaRange())
     fprintf(fout, " RANGE %s %s",
             maxStack->lefiMaxStackVia::maxStackViaBottomLayer(),
             maxStack->lefiMaxStackVia::maxStackViaTopLayer());
  fprintf(fout, "\n");
  return 0;
}

// Minfeature
int minFeatureCB(lefrCallbackType_e c, lefiMinFeature* min, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "MINFEATURE %g %g\n", chkNum(min->one()), chkNum(min->two()));
  return 0;
}

// Universalnoisemargin
int noiseMarginCB(lefrCallbackType_e c, lefiNoiseMargin* margin,
                  lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "UNIVERSALNOISEMARGIN %g %g\n", chkNum(margin->high),
          chkNum(margin->low));
  return 0;
}

// NoiseTable
int noiseTableCB(lefrCallbackType_e c, lefiNoiseTable* table, lefiUserData ud) {
  int                 i, j, k, l;
  lefiNoiseEdge       *edge;
  lefiNoiseResistance *resist;
  lefiNoiseVictim     *victim;
 
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "CROSSTALK NOISETABLE %d\n", table->num());
  if (table->numEdges() > 0) {
     for (i = 0; i < table->numEdges(); i++) {
        edge = table->edge(i);
        // resistances
        if (edge->numResistances()) {
           for (j = 0; j < edge->numResistances(); j++) {
              resist = edge->resistance(j);
              fprintf(fout, "NOISETABLE %d RESISTANCE %g\n",
                      table->num(), chkNum(resist->num(j)));
              // victims
              for (k = 0; k < resist->numVictims(); k++) {
                 victim = resist->victim(k);
                 if (victim->length() > 0)
                    fprintf(fout, "CROSSTALK NOISETABLE %d RESISTANCE %g VICTIMLENGTH % g\n",
                            table->num(), chkNum(resist->num(j)),
                            chkNum(victim->length()));
                    for (l = 0; l < victim->numNoises(); l++)
                       fprintf(fout, "CROSSTALK NOISETABLE %d RESISTANCE %g VICTIMLENGT %g NOISEFACTOR %g\n",
                          table->num(), chkNum(resist->num(j)),
                          chkNum(victim->length()), chkNum(victim->noise(l)));
              }
           }
        }
     }
  }
  return 0;
}

// Nondefault
int nonDefaultCB(lefrCallbackType_e c, lefiNonDefault* def, lefiUserData ud) {
  int          i;
  lefiVia*     via;
  lefiSpacing* spacing;
  char         defName[1024];

  checkType(c);
  if ((long)ud != userData) dataError();
  for (i = 0; i < def->numLayers(); i++) {
     fprintf(fout, "NONDEFAULTRULE %s LAYER %s", def->name(),
             def->layerName(i));
     if (def->hasLayerWidth(i))
        fprintf(fout, " WIDTH %g", chkNum(def->layerWidth(i)));
     if (def->hasLayerSpacing(i))
        fprintf(fout, " SPACING %g", chkNum(def->layerSpacing(i)));
     if (def->hasLayerWireExtension(i))
        fprintf(fout, " WIREEXTENSION %g", chkNum(def->layerWireExtension(i)));
     if (def->hasLayerResistance(i))
        fprintf(fout, " RESISTANCE RPERSQ %g", chkNum(def->layerResistance(i)));
     if (def->hasLayerCapacitance(i))
        fprintf(fout, " CAPACITANCE CPERSQDIST %g",
                chkNum(def->layerCapacitance(i)));
     if (def->hasLayerEdgeCap(i))
        fprintf(fout, " EDGECAPACITANCE %g", chkNum(def->layerEdgeCap(i)));
     fprintf(fout, "\n");
  }

  sprintf(defName, "NONDEFAULTRULE %s VIA", def->name());
  // handle via in nondefaultrule
  for (i = 0; i < def->numVias(); i++) {
     via = def->viaRule(i);
     lefVia(via, defName);
  }

  sprintf(defName, "NONDEFAULTRULE %s SPACING", def->name());
  // handle spacing in nondefaultrule
  for (i = 0; i < def->numSpacingRules(); i++) {
     spacing = def->spacingRule(i);
     lefSpacing(spacing, defName);
  }

  return 0;
}

// Nowireextension
int noWireExtCB(lefrCallbackType_e c, const char* wireExt, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "NOWIREEXTENSION %s\n", wireExt);
  return 0;
}

// Obstruction
int obstructionCB(lefrCallbackType_e c, lefiObstruction* obs,
                  lefiUserData ud) {
  lefiGeometries* geometry;

  checkType(c);
  if ((long)ud != userData) dataError();
  geometry = obs->geometries();
  prtGeometry(geometry, (char*)"OBS");
  return 0;
}

// Pin
int pinCB(lefrCallbackType_e c, lefiPin* pin, lefiUserData ud) {
  int                  numPorts, i, j;
  lefiGeometries*      geometry;
  lefiPinAntennaModel* aModel;
 
  checkType(c);
  if ((long)ud != userData) dataError();
  if (pin->hasForeign()) {
     if (pin->hasForeignOrient())
        fprintf(fout, "PIN %s FOREIGN %s STRUCTURE %g %g %s\n", pin->name(),
                pin->foreignName(), chkNum(pin->foreignX()),
                chkNum(pin->foreignY()), lefiOrientStr(pin->foreignOrient()));
     else if (pin->hasForeignPoint())
        fprintf(fout, "PIN %s FOREIGN %s STRUCTURE %g %g", pin->name(),
                pin->foreignName(), chkNum(pin->foreignX()),
                chkNum(pin->foreignY()));
     else
        fprintf(fout, "PIN %s FOREIGN %s\n", pin->name(), pin->foreignName());
  }
  if (pin->hasLEQ())
     fprintf(fout, "PIN %s LEQ %s\n", pin->name(), pin->LEQ());
  if (pin->hasDirection())
     fprintf(fout, "PIN %s DIRECTION %s\n", pin->name(), pin->direction());
  if (pin->hasUse())
     fprintf(fout, "PIN %s USE %s\n", pin->name(), pin->use());
  if (pin->hasShape())
     fprintf(fout, "PIN %s SHAPE %s\n", pin->name(), pin->shape());
  if (pin->hasMustjoin())
     fprintf(fout, "PIN %s MUSTJOIN %s\n", pin->name(), pin->mustjoin());
  if (pin->hasOutMargin())
     fprintf(fout, "PIN %s OUTPUTNOISEMARGIN %g %g\n", pin->name(),
             chkNum(pin->outMarginHigh()), chkNum(pin->outMarginLow()));
  if (pin->hasOutResistance())
     fprintf(fout, "PIN %s OUTPUTRESISTANCE %g %g\n", pin->name(),
             chkNum(pin->outResistanceHigh()), chkNum(pin->outResistanceLow()));
  if (pin->hasInMargin())
     fprintf(fout, "PIN %s INPUTNOISEMARGIN %g %g\n", pin->name(),
             chkNum(pin->inMarginHigh()), chkNum(pin->inMarginLow()));
  if (pin->hasPower())
     fprintf(fout, "PIN %s POWER %g\n", pin->name(), chkNum(pin->power()));
  if (pin->hasLeakage())
     fprintf(fout, "PIN %s LEAKAGE %g\n", pin->name(), chkNum(pin->leakage()));
  if (pin->hasMaxload())
     fprintf(fout, "PIN %s MAXLOAD %g\n", pin->name(), chkNum(pin->maxload()));
  if (pin->hasCapacitance())
     fprintf(fout, "PIN %s CAPACITANCE %g\n", pin->name(),
             chkNum(pin->capacitance()));
  if (pin->hasResistance())
     fprintf(fout, "PIN %s RESISTANCE %g\n", pin->name(),
             chkNum(pin->resistance()));
  if (pin->hasPulldownres())
     fprintf(fout, "PIN %s PULLDOWNRES %g\n", pin->name(),
             chkNum(pin->pulldownres()));
  if (pin->hasTieoffr())
     fprintf(fout, "PIN %s TIEOFFR %g\n", pin->name(), chkNum(pin->tieoffr()));
  if (pin->hasVHI())
     fprintf(fout, "PIN %s VHI %g\n", pin->name(), chkNum(pin->VHI()));
  if (pin->hasVLO())
     fprintf(fout, "PIN %s VLO %g\n", pin->name(), chkNum(pin->VLO()));
  if (pin->hasRiseVoltage())
     fprintf(fout, "PIN %s RISEVOLTAGETHRESHOLD %g\n", pin->name(),
             chkNum(pin->riseVoltage()));
  if (pin->hasFallVoltage())
     fprintf(fout, "PIN %s FALLVOLTAGETHRESHOLD %g\n", pin->name(),
             chkNum(pin->fallVoltage()));
  if (pin->hasRiseThresh())
     fprintf(fout, "PIN %s RISETHRESH %g\n", pin->name(),
             chkNum(pin->riseThresh()));
  if (pin->hasFallThresh())
     fprintf(fout, "PIN %s FALLTHRESH %g\n", pin->name(),
             chkNum(pin->fallThresh()));
  if (pin->hasRiseSatcur())
     fprintf(fout, "PIN %s RISESATCUR %g\n", pin->name(),
             chkNum(pin->riseSatcur()));
  if (pin->hasFallSatcur())
     fprintf(fout, "PIN %s FALLSATCUR %g\n", pin->name(),
             chkNum(pin->fallSatcur()));
  if (pin->hasRiseSlewLimit())
     fprintf(fout, "PIN %s RISESLEWLIMIT %g\n", pin->name(),
             chkNum(pin->riseSlewLimit()));
  if (pin->hasFallSlewLimit())
     fprintf(fout, "PIN %s FALLSLEWLIMIT %g\n", pin->name(),
             chkNum(pin->fallSlewLimit()));
  if (pin->hasCurrentSource())
     fprintf(fout, "PIN %s CURRENTSOURCE %s\n", pin->name(),
             pin->currentSource());
  if (pin->hasTables())
     fprintf(fout, "PIN %s IV_TABLES %s %s\n", pin->name(),
             pin->tableHighName(), pin->tableLowName());
  if (pin->hasAntennaSize()) {
     for (i = 0; i < pin->numAntennaSize(); i++) {
        fprintf(fout, "PIN %s ANTSIZE %g", pin->name(),
           chkNum(pin->antennaSize(i)));
        if (pin->antennaSizeLayer(i))
           fprintf(fout, " LAYER %s\n", pin->antennaSizeLayer(i));
        else
           fprintf(fout, "\n");
     }
  }
  if (pin->hasAntennaMetalArea()) {
     for (i = 0; i < pin->numAntennaMetalArea(); i++) {
        fprintf(fout, "PIN %s ANTMETALAREA %g", pin->name(),
           chkNum(pin->antennaMetalArea(i)));
        if (pin->antennaMetalAreaLayer(i))
           fprintf(fout, " LAYER %s\n", pin->antennaMetalAreaLayer(i));
        else
           fprintf(fout, "\n");
     }
  }
  if (pin->hasAntennaMetalLength()) {
     for (i = 0; i < pin->numAntennaMetalLength(); i++) {
        fprintf(fout, "PIN %s ANTMETALLENGTH %g", pin->name(),
           chkNum(pin->antennaMetalLength(i)));
        if (pin->antennaMetalLengthLayer(i))
           fprintf(fout, " LAYER %s\n", pin->antennaMetalLengthLayer(i));
        else
           fprintf(fout, "\n");
     }
  }
  if (pin->lefiPin::hasAntennaPartialMetalArea()) {
     for (i = 0; i < pin->lefiPin::numAntennaPartialMetalArea(); i++) {
        fprintf(fout, "PIN %s ANTPARTIALMETALAREA %g", pin->name(),
                chkNum(pin->lefiPin::antennaPartialMetalArea(i)));
        if (pin->lefiPin::antennaPartialMetalAreaLayer(i))
           fprintf(fout, " LAYER %s",
                   pin->lefiPin::antennaPartialMetalAreaLayer(i));
        fprintf(fout, "\n");
     }
  }
  if (pin->lefiPin::hasAntennaPartialMetalSideArea()) {
     for (i = 0; i < pin->lefiPin::numAntennaPartialMetalSideArea(); i++) {
        fprintf(fout, "PIN %s ANTPARTIALMETALSIDEAREA %g", pin->name(),
                chkNum(pin->lefiPin::antennaPartialMetalSideArea(i)));
        if (pin->lefiPin::antennaPartialMetalSideAreaLayer(i))
           fprintf(fout, " LAYER %s ",
                   pin->lefiPin::antennaPartialMetalSideAreaLayer(i));
        fprintf(fout, "\n");
     }
  }
  if (pin->lefiPin::hasAntennaPartialCutArea()) {
     for (i = 0; i < pin->lefiPin::numAntennaPartialCutArea(); i++) {
        fprintf(fout, "PIN %s ANTPARTIALCUTAREA %g", pin->name(),
                chkNum(pin->lefiPin::antennaPartialCutArea(i)));
        if (pin->lefiPin::antennaPartialCutAreaLayer(i))
           fprintf(fout, " LAYER %s ",
                   pin->lefiPin::antennaPartialCutAreaLayer(i));
        fprintf(fout, "\n");
     }
  }
  if (pin->lefiPin::hasAntennaDiffArea()) {
     for (i = 0; i < pin->lefiPin::numAntennaDiffArea(); i++) {
        fprintf(fout, "PIN %s ANTDIFFAREA %g", pin->name(),
                chkNum(pin->lefiPin::antennaDiffArea(i)));
        if (pin->lefiPin::antennaDiffAreaLayer(i))
           fprintf(fout, " LAYER %s ", pin->lefiPin::antennaDiffAreaLayer(i));
        fprintf(fout, "\n");
     }
  }

  for (j = 0; j < pin->lefiPin::numAntennaModel(); j++) {
     aModel = pin->lefiPin::antennaModel(j);

     if (aModel->lefiPinAntennaModel::hasAntennaGateArea()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaGateArea(); i++)
        {
           fprintf(fout, "PIN %s %s ANTGATEAREA %g", pin->name(),
                   aModel->lefiPinAntennaModel::antennaOxide(),
                   chkNum(aModel->lefiPinAntennaModel::antennaGateArea(i)));
           if (aModel->lefiPinAntennaModel::antennaGateAreaLayer(i))
              fprintf(fout, " LAYER %s ",
                      aModel->lefiPinAntennaModel::antennaGateAreaLayer(i));
           fprintf(fout, "\n");
        }
     }
     if (aModel->lefiPinAntennaModel::hasAntennaMaxAreaCar()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxAreaCar();
             i++) {
           fprintf(fout, "PIN %s %s ANTMAXAREACAR %g", pin->name(),
                   aModel->lefiPinAntennaModel::antennaOxide(),
                   chkNum(aModel->lefiPinAntennaModel::antennaMaxAreaCar(i)));
           if (aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i))
              fprintf(fout, " LAYER %s ",
                      aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i));
           fprintf(fout, "\n");
        }
     }
     if (aModel->lefiPinAntennaModel::hasAntennaMaxSideAreaCar()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxSideAreaCar();
             i++) {
           fprintf(fout, "PIN %s %s ANTMAXAREACAR %g", pin->name(),
                 aModel->lefiPinAntennaModel::antennaOxide(),
                 chkNum(aModel->lefiPinAntennaModel::antennaMaxSideAreaCar(i)));
           if (aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i))
              fprintf(fout, " LAYER %s ",
                    aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i));
           fprintf(fout, "\n");
        }
     }
     if (aModel->lefiPinAntennaModel::hasAntennaMaxCutCar()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxCutCar(); i++)
        {
           fprintf(fout, "PIN %s %s ANTMAXCUTCAR %g", pin->name(),
                   aModel->lefiPinAntennaModel::antennaOxide(),
                   chkNum(aModel->lefiPinAntennaModel::antennaMaxCutCar(i)));
           if (aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i))
              fprintf(fout, " LAYER %s ",
                      aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i));
           fprintf(fout, "\n");
        }
     }
  }

  if (pin->numProperties() > 0) {
     for (i = 0; i < pin->numProperties(); i++) {
        fprintf(fout, "PIN %s PROP ", pin->name());
        // value can either be a string or number
        if (pin->propValue(i)) {
           fprintf(fout, "%s %s\n", pin->propName(i), pin->propValue(i));
        }
        else
           fprintf(fout, "%s %g\n", pin->propName(i), chkNum(pin->propNum(i)));
     }
  }
  
  numPorts = pin->numPorts();

  char pinName[1024], pinPortName[1024];

  for (i = 0; i < numPorts; i++) {
     sprintf(pinName, "PIN %s PORT", pin->name());
     geometry = pin->port(i);
     prtGeometry(geometry, pinName);
  }
  return 0;  
}

// Property definition
int propDefCB(lefrCallbackType_e c, lefiProp* prop, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "PROPDEF %s %s",
          prop->propType(), prop->propName());
  if (prop->hasRange())
     fprintf(fout, " RANGE %g %g", chkNum(prop->left()), chkNum(prop->right()));
  if (prop->hasNumber())
     fprintf(fout, " NUMBER %g", chkNum(prop->number()));
  switch (prop->dataType()) {
      case 'I':
           fprintf(fout, " TYPE I");
           break;
      case 'R':
           fprintf(fout, " TYPE R");
           break;
      case 'S':
           fprintf(fout, " TYPE S");
           break;
      case 'Q':
           fprintf(fout, " TYPE Q");
           break;
      case 'N':
           fprintf(fout, " TYPE N");
           break;
  }
  fprintf(fout, "\n");
  return 0;
}

// Site
int siteCB(lefrCallbackType_e c, lefiSite* site, lefiUserData ud) {
  int hasPrtSym = 0;

  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "SITE %s", site->name());
  if (site->hasClass())
     fprintf(fout, " CLASS %s", site->siteClass());
  if (site->hasXSymmetry()) {
     fprintf(fout, " SYMMETRY X ");
     hasPrtSym = 1;
  }
  if (site->hasYSymmetry()) {
     if (hasPrtSym)
        fprintf(fout, "Y ");
     else {
        fprintf(fout, " SYMMETRY Y ");
        hasPrtSym = 1;
     }
  }
  if (site->has90Symmetry()) {
     if (hasPrtSym)
        fprintf(fout, "R90 ");
     else {
        fprintf(fout, " SYMMETRY R90 ");
        hasPrtSym = 1;
     }
  }
  if (site->hasSize())
     fprintf(fout, " SIZE %g BY %g", chkNum(site->sizeX()),
             chkNum(site->sizeY()));
  fprintf(fout, "\n");
  return 0;
}

// Spacing
int spacingCB(lefrCallbackType_e c, lefiSpacing* spacing, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  lefSpacing(spacing, (char*)"SPACING");
  return 0;
}

// Timing
int timingCB(lefrCallbackType_e c, lefiTiming* timing, lefiUserData ud) {
  int i;
  checkType(c);
  if ((long)ud != userData) dataError();
  for (i = 0; i < timing->numFromPins(); i++)
     fprintf(fout, "TIMING FROMPIN %s\n", timing->fromPin(i));
  for (i = 0; i < timing->numToPins(); i++)
     fprintf(fout, "TIMING TOPIN %s\n", timing->toPin(i));
     fprintf(fout, "TIMING RISE SLEW1 %g %g %g %g\n",
             chkNum(timing->riseSlewOne()), chkNum(timing->riseSlewTwo()),
             chkNum(timing->riseSlewThree()), chkNum(timing->riseSlewFour()));
  if (timing->hasRiseSlew2())
     fprintf(fout, "TIMING RISE SLEW2 %g %g %g\n",
             chkNum(timing->riseSlewFive()), chkNum(timing->riseSlewSix()),
             chkNum(timing->riseSlewSeven()));
  if (timing->hasFallSlew())
     fprintf(fout, "TIMING FALL SLEW1 %g %g %g %g\n",
             chkNum(timing->fallSlewOne()), chkNum(timing->fallSlewTwo()),
             chkNum(timing->fallSlewThree()), chkNum(timing->fallSlewFour()));
  if (timing->hasFallSlew2())
     fprintf(fout, "TIMING FALL SLEW2 %g %g %g\n",
             chkNum(timing->fallSlewFive()), chkNum(timing->fallSlewSix()),
             chkNum(timing->riseSlewSeven()));
  if (timing->hasRiseIntrinsic()) {
     fprintf(fout, "TIMING RISE INTRINSIC %g %g\n",
             chkNum(timing->riseIntrinsicOne()),
             chkNum(timing->riseIntrinsicTwo()));
     fprintf(fout, "TIMING RISE VARIABLE %g %g\n",
             chkNum(timing->riseIntrinsicThree()),
             chkNum(timing->riseIntrinsicFour()));
  }
  if (timing->hasFallIntrinsic()) {
     fprintf(fout, "TIMING FALL INTRINSIC %g %g\n",
             chkNum(timing->fallIntrinsicOne()),
             chkNum(timing->fallIntrinsicTwo()));
     fprintf(fout, "TIMING RISE VARIABLE %g %g\n",
             chkNum(timing->fallIntrinsicThree()),
             chkNum(timing->fallIntrinsicFour()));
  }
  if (timing->hasRiseRS())
     fprintf(fout, "TIMING RISERS %g %g\n",
             chkNum(timing->riseRSOne()), chkNum(timing->riseRSTwo()));
     if (timing->hasRiseCS())
     fprintf(fout, "TIMING RISECS %g %g\n",
             chkNum(timing->riseCSOne()), chkNum(timing->riseCSTwo()));
  if (timing->hasFallRS())
     fprintf(fout, "TIMING FALLRS %g %g\n",
             chkNum(timing->fallRSOne()), chkNum(timing->fallRSTwo()));
  if (timing->hasFallCS())
     fprintf(fout, "TIMING FALLCS %g %g\n",
             chkNum(timing->fallCSOne()), chkNum(timing->fallCSTwo()));
  if (timing->hasUnateness())
     fprintf(fout, "TIMING UNATENESS %s\n", timing->unateness());
  if (timing->hasRiseAtt1())
     fprintf(fout, "TIMING RISESATT1 %g %g\n", chkNum(timing->riseAtt1One()),
             chkNum(timing->riseAtt1Two()));
  if (timing->hasFallAtt1())
     fprintf(fout, "TIMING FALLSATT1 %g %g\n", chkNum(timing->fallAtt1One()),
             chkNum(timing->fallAtt1Two()));
  if (timing->hasRiseTo())
     fprintf(fout, "TIMING RISET0 %g %g\n", chkNum(timing->riseToOne()),
             chkNum(timing->riseToTwo()));
  if (timing->hasFallTo())
     fprintf(fout, "TIMING FALLT0 %g %g\n", chkNum(timing->fallToOne()),
             chkNum(timing->fallToTwo()));
  return 0;
}

// Units
int unitsCB(lefrCallbackType_e c, lefiUnits* unit, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  if (unit->hasDatabase())
     fprintf(fout, "UNITS DATABASE %s %g\n", unit->databaseName(),
             chkNum(unit->databaseNumber()));
  if (unit->hasCapacitance())
     fprintf(fout, "UNITS CAPACITANCE PICOFARADS %g\n",
             chkNum(unit->capacitance()));
  if (unit->hasResistance())
     fprintf(fout, "UNITS RESISTANCE OHMS %g\n", chkNum(unit->resistance()));
  if (unit->hasPower())
     fprintf(fout, "UNITS POWER MILLIWATTS %g\n", chkNum(unit->power()));
  if (unit->hasCurrent())
     fprintf(fout, "UNITS CURRENT MILLIAMPS %g\n", chkNum(unit->current()));
  if (unit->hasVoltage())
     fprintf(fout, "UNITS VOLTAGE VOLTS %g\n", chkNum(unit->voltage()));
  if (unit->hasFrequency())
     fprintf(fout, "UNITS FREQUENCY MEGAHERTZ %g\n", chkNum(unit->frequency()));
  return 0;
}

// UseMinSpacing
int useMinSpacingCB(lefrCallbackType_e c, lefiUseMinSpacing* spacing,
                    lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "USEMINSPACING %s ", spacing->lefiUseMinSpacing::name());
  if (spacing->lefiUseMinSpacing::value())
      fprintf(fout, "USEMINSPACING ON\n");
  else
      fprintf(fout, "USEMINSPACING OFF\n");
  return 0;
}

// Version
int versionCB(lefrCallbackType_e c, double num, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "VERSION %g\n", num);
  return 0;
}

// Via
int viaCB(lefrCallbackType_e c, lefiVia* via, lefiUserData ud) {
  checkType(c);
  if ((long)ud != userData) dataError();
  lefVia(via, (char*)"VIA");
  return 0;
}

// Viarule
int viaRuleCB(lefrCallbackType_e c, lefiViaRule* viaRule, lefiUserData ud) {
  int               numLayers, numVias, i;
  lefiViaRuleLayer* vLayer;

  checkType(c);
  if ((long)ud != userData) dataError();
  fprintf(fout, "VIARULE %s", viaRule->name());
  if (viaRule->hasGenerate())
     fprintf(fout, " GENERATE\n");
  else
     fprintf(fout, "\n");

  numLayers = viaRule->numLayers();
  // if numLayers == 2, it is VIARULE without GENERATE and has via name
  // if numLayers == 3, it is VIARULE with GENERATE, and the 3rd layer is cut
  for (i = 0; i < numLayers; i++) {
     vLayer = viaRule->layer(i); 
     lefViaRuleLayer(vLayer, viaRule->name());
  }

  if (numLayers == 2) {    // should have vianames
     numVias = viaRule->numVias();
     if (numVias == 0)
        fprintf(fout, "Should have via names in VIARULE.\n");
     else {
        for (i = 0; i < numVias; i++)
           fprintf(fout, "VIARULE %s  VIA %s ;\n", viaRule->name(),
                   viaRule->viaName(i));
     }
  }
  if (viaRule->numProps() > 0) {
     for (i = 0; i < viaRule->lefiViaRule::numProps(); i++) {
        fprintf(fout, "VIARULE %s PROP %s ", viaRule->name(),
                viaRule->propName(i));
        if (viaRule->lefiViaRule::propValue(i))
           fprintf(fout, "%s ", viaRule->propValue(i));
        switch (viaRule->propType(i)) {
           case 'R': fprintf(fout, "REAL ");
                     break;
           case 'I': fprintf(fout, "INTEGER ");
                     break;
           case 'S': fprintf(fout, "STRING ");
                     break;
           case 'Q': fprintf(fout, "QUOTESTRING ");
                     break;
           case 'N': fprintf(fout, "NUMBER ");
                     break;
        fprintf(fout, "\n");
        }
     }
  }
  return 0;
}

//========
 
int diffLefReadFile(char* inFile, char* outFile) {
  FILE* f;
  int   res;
 
  userData = 0x01020304;
  lefrInit();

  lefrSetAntennaInputCbk(antennaCB);
  lefrSetAntennaInoutCbk(antennaCB);
  lefrSetAntennaOutputCbk(antennaCB);
  lefrSetArrayCbk(arrayCB);
  lefrSetBusBitCharsCbk(busBitCharsCB);
  lefrSetCaseSensitiveCbk(caseSensCB);
  lefrSetClearanceMeasureCbk(clearanceCB);
  lefrSetCorrectionTableCbk(correctTableCB);
  lefrSetDielectricCbk(dielectricCB);
  lefrSetDividerCharCbk(dividerCB);
  lefrSetEdgeRateScaleFactorCbk(edgeRateCB);
  lefrSetEdgeRateThreshold1Cbk(edgeRate1CB);
  lefrSetEdgeRateThreshold2Cbk(edgeRate2CB);
  lefrSetInputAntennaCbk(inputAntCB);
  lefrSetOutputAntennaCbk(outputAntCB);
  lefrSetInoutAntennaCbk(inoutAntCB);
  lefrSetIRDropCbk(irdropCB);
  lefrSetLayerCbk(layerCB);
  lefrSetMacroCbk(macroCB);
  lefrSetManufacturingCbk(manufacturingCB);
  lefrSetMaxStackViaCbk(maxStackViaCB);
  lefrSetMinFeatureCbk(minFeatureCB);
  lefrSetNoiseMarginCbk(noiseMarginCB);
  lefrSetNoiseTableCbk(noiseTableCB);
  lefrSetNonDefaultCbk(nonDefaultCB);
  lefrSetNoWireExtensionCbk(noWireExtCB);
  lefrSetObstructionCbk(obstructionCB);
  lefrSetPinCbk(pinCB);
  lefrSetPropCbk(propDefCB);
  lefrSetSiteCbk(siteCB);
  lefrSetSpacingCbk(spacingCB);
  lefrSetTimingCbk(timingCB);
  lefrSetUnitsCbk(unitsCB);
  lefrSetUseMinSpacingCbk(useMinSpacingCB);
  lefrSetUserData((void*)3);
  lefrSetVersionCbk(versionCB);
  lefrSetViaCbk(viaCB);
  lefrSetViaRuleCbk(viaRuleCB);

  if ((f = fopen(inFile,"r")) == 0) {
    fprintf(stderr,"Couldn't open input file '%s'\n", inFile);
    return(2);
  }

  if ((fout = fopen(outFile,"w")) == 0) {
    fprintf(stderr,"Couldn't open output file '%s'\n", outFile);
    return(2);
  }

  res = lefrRead(f, inFile, (void*)userData);

  fclose(f);
  fclose(fout);

  return 0;
}
