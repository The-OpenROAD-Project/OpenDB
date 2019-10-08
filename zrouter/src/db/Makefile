#
#
#
include ../Makefile.defs

DEFINES= -DDB_PRIVATE=public
LIBNAME=db
SUBDIRS=db_tcl WireConn
SRCS=  \
        dbBTerm.cpp \
        dbStream.cpp \
        dbBTermItr.cpp \
        dbBPinItr.cpp \
        dbBlock.cpp \
        dbBlockItr.cpp \
        dbBox.cpp \
        dbBoxItr.cpp \
        dbChip.cpp \
        dbDatabase.cpp \
        dbITerm.cpp \
        dbITermItr.cpp \
        dbInst.cpp \
        dbInstHdr.cpp \
        dbLib.cpp \
        dbMPin.cpp \
        dbMPinItr.cpp \
        dbMTerm.cpp \
        dbMaster.cpp \
        dbNet.cpp \
        dbTech.cpp \
        dbTechLayer.cpp \
        dbTechLayerSpacingRule.cpp \
        dbTechLayerAntennaRule.cpp \
        dbTechMinCutOrAreaRule.cpp \
        dbTechLayerItr.cpp \
        dbTechVia.cpp \
        dbTypes.cpp \
        dbVia.cpp \
        dbWire.cpp \
        dbWireCodec.cpp \
        dbGCellGrid.cpp \
        dbTrackGrid.cpp \
        dbBlockage.cpp \
        dbObstruction.cpp \
        dbObject.cpp \
        dbInstShapeItr.cpp \
        dbTransform.cpp \
        dbITermShapeItr.cpp \
        dbSWire.cpp \
        dbSBox.cpp \
        dbSWireItr.cpp \
        dbSBoxItr.cpp \
        dbDiff.cpp \
        dbSite.cpp \
        dbCCSeg.cpp \
        dbCCSegItr.cpp \
        dbWireShapeItr.cpp \
        dbWirePathItr.cpp \
        dbTarget.cpp \
        dbTargetItr.cpp \
        test_wire.cpp \
        test_wire2.cpp \
        dbTechNonDefaultRule.cpp \
        dbTechLayerRule.cpp \
        dbCapNode.cpp \
        dbCapNodeItr.cpp \
        dbRSeg.cpp \
        dbRSegItr.cpp \
        dbTechSameNetRule.cpp \
        dbRow.cpp \
        dbShape.cpp \
        dbWireGraph.cpp \
	dbJournal.cpp \
	dbTmgJournal.cpp \
	dbJournalLog.cpp \
	dbBlockCallBackObj.cpp \
        dbTmg.cpp \
        dbMetrics.cpp \
        dbRtTree.cpp \
        dbRegion.cpp \
        dbRegionInstItr.cpp \
        dbExtControl.cpp \
        dbPrintControl.cpp \
        dbNullIterator.cpp \
        dbBPin.cpp \
        dbHierInstShapeItr.cpp \
        dbRegionItr.cpp \
        dbHier.cpp \
        dbTechViaRule.cpp \
        dbTechViaLayerRule.cpp \
        dbTechViaGenerateRule.cpp \
        dbViaParams.cpp \
        dbNameCache.cpp \
        dbProperty.cpp \
        dbPropertyItr.cpp \
        dbFlatten.cpp \
        dbUtil.cpp \

EXCLUDE_SRC= dbSearch.cpp dbZgui.cpp 
##############################################
# Add custom targets below the following line.
include ../Makefile.rules

#xtest: xtest.cpp
#	$(CXX) $(CXXFLAGS) xtest.cpp -DDB_TRANSFORM_TEST dbTransform.cpp dbTypes.cpp
