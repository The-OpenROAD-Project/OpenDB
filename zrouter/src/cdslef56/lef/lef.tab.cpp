
/*  A Bison parser, made from lef.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse lefyyparse
#define yylex lefyylex
#define yyerror lefyyerror
#define yylval lefyylval
#define yychar lefyychar
#define yydebug lefyydebug
#define yynerrs lefyynerrs
#define	K_HISTORY	258
#define	K_ABUT	259
#define	K_ABUTMENT	260
#define	K_ACTIVE	261
#define	K_ANALOG	262
#define	K_ARRAY	263
#define	K_AREA	264
#define	K_BLOCK	265
#define	K_BOTTOMLEFT	266
#define	K_BOTTOMRIGHT	267
#define	K_BY	268
#define	K_CAPACITANCE	269
#define	K_CAPMULTIPLIER	270
#define	K_CLASS	271
#define	K_CLOCK	272
#define	K_CLOCKTYPE	273
#define	K_COLUMNMAJOR	274
#define	K_DESIGNRULEWIDTH	275
#define	K_INFLUENCE	276
#define	K_CORE	277
#define	K_CORNER	278
#define	K_COVER	279
#define	K_CPERSQDIST	280
#define	K_CURRENT	281
#define	K_CURRENTSOURCE	282
#define	K_CUT	283
#define	K_DEFAULT	284
#define	K_DATABASE	285
#define	K_DATA	286
#define	K_DIELECTRIC	287
#define	K_DIRECTION	288
#define	K_DO	289
#define	K_EDGECAPACITANCE	290
#define	K_EEQ	291
#define	K_END	292
#define	K_ENDCAP	293
#define	K_FALL	294
#define	K_FALLCS	295
#define	K_FALLT0	296
#define	K_FALLSATT1	297
#define	K_FALLRS	298
#define	K_FALLSATCUR	299
#define	K_FALLTHRESH	300
#define	K_FEEDTHRU	301
#define	K_FIXED	302
#define	K_FOREIGN	303
#define	K_FROMPIN	304
#define	K_GENERATE	305
#define	K_GENERATOR	306
#define	K_GROUND	307
#define	K_HEIGHT	308
#define	K_HORIZONTAL	309
#define	K_INOUT	310
#define	K_INPUT	311
#define	K_INPUTNOISEMARGIN	312
#define	K_COMPONENTPIN	313
#define	K_INTRINSIC	314
#define	K_INVERT	315
#define	K_IRDROP	316
#define	K_ITERATE	317
#define	K_IV_TABLES	318
#define	K_LAYER	319
#define	K_LEAKAGE	320
#define	K_LEQ	321
#define	K_LIBRARY	322
#define	K_MACRO	323
#define	K_MATCH	324
#define	K_MAXDELAY	325
#define	K_MAXLOAD	326
#define	K_METALOVERHANG	327
#define	K_MILLIAMPS	328
#define	K_MILLIWATTS	329
#define	K_MINFEATURE	330
#define	K_MUSTJOIN	331
#define	K_NAMESCASESENSITIVE	332
#define	K_NANOSECONDS	333
#define	K_NETS	334
#define	K_NEW	335
#define	K_NONDEFAULTRULE	336
#define	K_NONINVERT	337
#define	K_NONUNATE	338
#define	K_OBS	339
#define	K_OHMS	340
#define	K_OFFSET	341
#define	K_ORIENTATION	342
#define	K_ORIGIN	343
#define	K_OUTPUT	344
#define	K_OUTPUTNOISEMARGIN	345
#define	K_OUTPUTRESISTANCE	346
#define	K_OVERHANG	347
#define	K_OVERLAP	348
#define	K_OFF	349
#define	K_ON	350
#define	K_OVERLAPS	351
#define	K_PAD	352
#define	K_PATH	353
#define	K_PATTERN	354
#define	K_PICOFARADS	355
#define	K_PIN	356
#define	K_PITCH	357
#define	K_PLACED	358
#define	K_POLYGON	359
#define	K_PORT	360
#define	K_POST	361
#define	K_POWER	362
#define	K_PRE	363
#define	K_PULLDOWNRES	364
#define	K_RECT	365
#define	K_RESISTANCE	366
#define	K_RESISTIVE	367
#define	K_RING	368
#define	K_RISE	369
#define	K_RISECS	370
#define	K_RISERS	371
#define	K_RISESATCUR	372
#define	K_RISETHRESH	373
#define	K_RISESATT1	374
#define	K_RISET0	375
#define	K_RISEVOLTAGETHRESHOLD	376
#define	K_FALLVOLTAGETHRESHOLD	377
#define	K_ROUTING	378
#define	K_ROWMAJOR	379
#define	K_RPERSQ	380
#define	K_SAMENET	381
#define	K_SCANUSE	382
#define	K_SHAPE	383
#define	K_SHRINKAGE	384
#define	K_SIGNAL	385
#define	K_SITE	386
#define	K_SIZE	387
#define	K_SOURCE	388
#define	K_SPACER	389
#define	K_SPACING	390
#define	K_SPECIALNETS	391
#define	K_STACK	392
#define	K_START	393
#define	K_STEP	394
#define	K_STOP	395
#define	K_STRUCTURE	396
#define	K_SYMMETRY	397
#define	K_TABLE	398
#define	K_THICKNESS	399
#define	K_TIEHIGH	400
#define	K_TIELOW	401
#define	K_TIEOFFR	402
#define	K_TIME	403
#define	K_TIMING	404
#define	K_TO	405
#define	K_TOPIN	406
#define	K_TOPLEFT	407
#define	K_TOPRIGHT	408
#define	K_TOPOFSTACKONLY	409
#define	K_TRISTATE	410
#define	K_TYPE	411
#define	K_UNATENESS	412
#define	K_UNITS	413
#define	K_USE	414
#define	K_VARIABLE	415
#define	K_VERTICAL	416
#define	K_VHI	417
#define	K_VIA	418
#define	K_VIARULE	419
#define	K_VLO	420
#define	K_VOLTAGE	421
#define	K_VOLTS	422
#define	K_WIDTH	423
#define	K_X	424
#define	K_Y	425
#define	K_R90	426
#define	T_STRING	427
#define	QSTRING	428
#define	NUMBER	429
#define	K_N	430
#define	K_S	431
#define	K_E	432
#define	K_W	433
#define	K_FN	434
#define	K_FS	435
#define	K_FE	436
#define	K_FW	437
#define	K_R0	438
#define	K_R180	439
#define	K_R270	440
#define	K_MX	441
#define	K_MY	442
#define	K_MXR90	443
#define	K_MYR90	444
#define	K_USER	445
#define	K_MASTERSLICE	446
#define	K_ENDMACRO	447
#define	K_ENDMACROPIN	448
#define	K_ENDVIARULE	449
#define	K_ENDVIA	450
#define	K_ENDLAYER	451
#define	K_ENDSITE	452
#define	K_CANPLACE	453
#define	K_CANNOTOCCUPY	454
#define	K_TRACKS	455
#define	K_FLOORPLAN	456
#define	K_GCELLGRID	457
#define	K_DEFAULTCAP	458
#define	K_MINPINS	459
#define	K_WIRECAP	460
#define	K_STABLE	461
#define	K_SETUP	462
#define	K_HOLD	463
#define	K_DEFINE	464
#define	K_DEFINES	465
#define	K_DEFINEB	466
#define	K_IF	467
#define	K_THEN	468
#define	K_ELSE	469
#define	K_FALSE	470
#define	K_TRUE	471
#define	K_EQ	472
#define	K_NE	473
#define	K_LE	474
#define	K_LT	475
#define	K_GE	476
#define	K_GT	477
#define	K_OR	478
#define	K_AND	479
#define	K_NOT	480
#define	K_DELAY	481
#define	K_TABLEDIMENSION	482
#define	K_TABLEAXIS	483
#define	K_TABLEENTRIES	484
#define	K_TRANSITIONTIME	485
#define	K_EXTENSION	486
#define	K_PROPDEF	487
#define	K_STRING	488
#define	K_INTEGER	489
#define	K_REAL	490
#define	K_RANGE	491
#define	K_PROPERTY	492
#define	K_VIRTUAL	493
#define	K_BUSBITCHARS	494
#define	K_VERSION	495
#define	K_BEGINEXT	496
#define	K_ENDEXT	497
#define	K_UNIVERSALNOISEMARGIN	498
#define	K_EDGERATETHRESHOLD1	499
#define	K_CORRECTIONTABLE	500
#define	K_EDGERATESCALEFACTOR	501
#define	K_EDGERATETHRESHOLD2	502
#define	K_VICTIMNOISE	503
#define	K_NOISETABLE	504
#define	K_EDGERATE	505
#define	K_VICTIMLENGTH	506
#define	K_CORRECTIONFACTOR	507
#define	K_OUTPUTPINANTENNASIZE	508
#define	K_INPUTPINANTENNASIZE	509
#define	K_INOUTPINANTENNASIZE	510
#define	K_CURRENTDEN	511
#define	K_PWL	512
#define	K_ANTENNALENGTHFACTOR	513
#define	K_TAPERRULE	514
#define	K_DIVIDERCHAR	515
#define	K_ANTENNASIZE	516
#define	K_ANTENNAMETALLENGTH	517
#define	K_ANTENNAMETALAREA	518
#define	K_RISESLEWLIMIT	519
#define	K_FALLSLEWLIMIT	520
#define	K_FUNCTION	521
#define	K_BUFFER	522
#define	K_INVERTER	523
#define	K_NAMEMAPSTRING	524
#define	K_NOWIREEXTENSIONATPIN	525
#define	K_WIREEXTENSION	526
#define	K_MESSAGE	527
#define	K_CREATEFILE	528
#define	K_OPENFILE	529
#define	K_CLOSEFILE	530
#define	K_WARNING	531
#define	K_ERROR	532
#define	K_FATALERROR	533
#define	K_RECOVERY	534
#define	K_SKEW	535
#define	K_ANYEDGE	536
#define	K_POSEDGE	537
#define	K_NEGEDGE	538
#define	K_SDFCONDSTART	539
#define	K_SDFCONDEND	540
#define	K_SDFCOND	541
#define	K_MPWH	542
#define	K_MPWL	543
#define	K_PERIOD	544
#define	K_ACCURRENTDENSITY	545
#define	K_DCCURRENTDENSITY	546
#define	K_AVERAGE	547
#define	K_PEAK	548
#define	K_RMS	549
#define	K_FREQUENCY	550
#define	K_CUTAREA	551
#define	K_MEGAHERTZ	552
#define	K_USELENGTHTHRESHOLD	553
#define	K_LENGTHTHRESHOLD	554
#define	K_ANTENNAINPUTGATEAREA	555
#define	K_ANTENNAINOUTDIFFAREA	556
#define	K_ANTENNAOUTPUTDIFFAREA	557
#define	K_ANTENNAAREARATIO	558
#define	K_ANTENNADIFFAREARATIO	559
#define	K_ANTENNACUMAREARATIO	560
#define	K_ANTENNACUMDIFFAREARATIO	561
#define	K_ANTENNAAREAFACTOR	562
#define	K_ANTENNASIDEAREARATIO	563
#define	K_ANTENNADIFFSIDEAREARATIO	564
#define	K_ANTENNACUMSIDEAREARATIO	565
#define	K_ANTENNACUMDIFFSIDEAREARATIO	566
#define	K_ANTENNASIDEAREAFACTOR	567
#define	K_DIFFUSEONLY	568
#define	K_MANUFACTURINGGRID	569
#define	K_ANTENNACELL	570
#define	K_CLEARANCEMEASURE	571
#define	K_EUCLIDEAN	572
#define	K_MAXXY	573
#define	K_USEMINSPACING	574
#define	K_ROWMINSPACING	575
#define	K_ROWABUTSPACING	576
#define	K_FLIP	577
#define	K_NONE	578
#define	K_ANTENNAPARTIALMETALAREA	579
#define	K_ANTENNAPARTIALMETALSIDEAREA	580
#define	K_ANTENNAGATEAREA	581
#define	K_ANTENNADIFFAREA	582
#define	K_ANTENNAMAXAREACAR	583
#define	K_ANTENNAMAXSIDEAREACAR	584
#define	K_ANTENNAPARTIALCUTAREA	585
#define	K_ANTENNAMAXCUTCAR	586
#define	K_SLOTWIREWIDTH	587
#define	K_SLOTWIRELENGTH	588
#define	K_SLOTWIDTH	589
#define	K_SLOTLENGTH	590
#define	K_MAXADJACENTSLOTSPACING	591
#define	K_MAXCOAXIALSLOTSPACING	592
#define	K_MAXEDGESLOTSPACING	593
#define	K_SPLITWIREWIDTH	594
#define	K_MINIMUMDENSITY	595
#define	K_MAXIMUMDENSITY	596
#define	K_DENSITYCHECKWINDOW	597
#define	K_DENSITYCHECKSTEP	598
#define	K_FILLACTIVESPACING	599
#define	K_MINIMUMCUT	600
#define	K_ADJACENTCUTS	601
#define	K_ANTENNAMODEL	602
#define	K_BUMP	603
#define	K_ENCLOSURE	604
#define	K_FROMABOVE	605
#define	K_FROMBELOW	606
#define	K_IMPLANT	607
#define	K_LENGTH	608
#define	K_MAXVIASTACK	609
#define	K_AREAIO	610
#define	K_BLACKBOX	611
#define	K_MAXWIDTH	612
#define	K_MINENCLOSEDAREA	613
#define	K_MINSTEP	614
#define	K_ORIENT	615
#define	K_OXIDE1	616
#define	K_OXIDE2	617
#define	K_OXIDE3	618
#define	K_OXIDE4	619
#define	K_PARALLELRUNLENGTH	620
#define	K_MINWIDTH	621
#define	K_PROTRUSIONWIDTH	622
#define	K_SPACINGTABLE	623
#define	K_WITHIN	624
#define	K_ABOVE	625
#define	K_BELOW	626
#define	K_CENTERTOCENTER	627
#define	K_CUTSIZE	628
#define	K_CUTSPACING	629
#define	K_DENSITY	630
#define	K_DIAG45	631
#define	K_DIAG135	632
#define	K_DIAGMINEDGELENGTH	633
#define	K_DIAGSPACING	634
#define	K_DIAGPITCH	635
#define	K_DIAGWIDTH	636
#define	K_GENERATED	637
#define	K_GROUNDSENSITIVITY	638
#define	K_HARDSPACING	639
#define	K_INSIDECORNER	640
#define	K_LAYERS	641
#define	K_LENGTHSUM	642
#define	K_MICRONS	643
#define	K_MINCUTS	644
#define	K_MINSIZE	645
#define	K_NETEXPR	646
#define	K_OUTSIDECORNER	647
#define	K_PREFERENCLOSURE	648
#define	K_ROWCOL	649
#define	K_ROWPATTERN	650
#define	K_SOFT	651
#define	K_SUPPLYSENSITIVITY	652
#define	K_USEVIA	653
#define	K_USEVIARULE	654
#define	K_WELLTAP	655
#define	IF	656
#define	LNOT	657
#define	UMINUS	658

#line 15 "lef.y"

#include <string.h>
#include <stdlib.h>
#include <math.h>
/* # include <sys/types.h> */
/* # include <sys/stat.h> */
/* # include <fcntl.h> */
/* # include <unistd.h> */
/* # ifndef	WIN32 */
/* # include <sys/param.h> */
/* # endif */
#include "FEF_stringhash.h"
#include "lex.h"
#include "lefiDefs.hpp"
#include "lefiUser.hpp"
#include "lefiUtil.hpp"
#include "lefrCallBacks.hpp"
#ifdef WIN32
#include <malloc.h>
#endif

#define LYPROP_ECAP "EDGE_CAPACITANCE"

#define YYINITDEPTH 5000   /* pcr 640902 - initialize the yystacksize to 300 */
                           /* this may need to increase in a design gets */
                           /* larger and a polygon has around 300 sizes */
                           /* 11/21/2003 - incrreased to 500, design from */
                           /* Artisan is greater than 300, need to find a */
                           /* way to dynamically increase the size */
                           /* 2/10/2004 - increased to 1000 for pcr 686073 */
                           /* 3/22/2004 - increased to 2000 for pcr 695879 */
                           /* 9/29/2004 - double the size for pcr 746865 */
                           /* tried to overwrite the yyoverflow definition */
                           /* it is impossible due to the union structure */

/*
#define YYMAXDEPTH 20000
*/

int lefRetVal;
static char lefPropDefType;    /* save the current type of the property */

/* Macro to describe how we handle a callback.
 * If the function was set then call it.
 * If the function returns non zero then there was an error
 * so call the error routine and exit.
 */
#define CALLBACK(func, typ, data) \
    if (!lef_errors) { \
      if (func) { \
        if ((lefRetVal = (*func)(typ, data, lefrUserData)) == 0) { \
        } else if (lefRetVal == STOP_PARSE) { \
	  return lefRetVal; \
        } else { \
	  yyerror("Error in callback"); \
	  return lefRetVal; \
        } \
      } \
    }

#define CHKERR() \
    if (lef_errors > 20) { \
      yyerror("Too many syntax errors"); \
      lef_errors = 0; \
      return 1; \
    }

extern int lefNlines, lefNtokens;
extern FILE* lefFile;

/**********************************************************************/
/**********************************************************************/

#define C_EQ 0
#define C_NE 1
#define C_LT 2
#define C_LE 3
#define C_GT 4
#define C_GE 5

extern int lefDumbMode;   // for communicating with parser
extern int lefNoNum;      // likewise, says no numbers for the next N tokens
extern int lefRealNum;    // Next token will be a real number
extern int lefNlToken;    // likewise
extern int lefDefIf;      // likewise
extern int lefNdRule;     // for communicating with parser

extern int lef_errors;    // from lex.cpph
extern char* Hist_text;   // for BEGINEXT - extension

extern int doneLib;       // keep track if the file is done parsing
extern int ge56almostDone;// lef file has Extension section
extern int ge56done;      // done reading 5.6 and beyond library

extern char* lefrFileName;

static int ignoreVersion = 0; // ignore checking version number

int lefNamesCaseSensitive = TRUE;  // always true in 5.6
int lefReaderCaseSensitive = FALSE;  // default to 0

int lefrShiftCase = 0;     // if user wants to shift to all uppercase

int lefrRelaxMode = FALSE;    // relax mode?

/* XXXXXXXXXXXXX check out these variables */
double lef_save_x, lef_save_y;  // for interpreting (*) notation of LEF/DEF

/* #define STRING_LIST_SIZE 1024 */
/* char string_list[STRING_LIST_SIZE]; */

static char temp_name[258];
static char* layerName;
static char* viaName;
static char* viaRuleName;
static char* nonDefaultRuleName;
static char* siteName;
static char* arrayName;
static char* macroName;
static char* pinName;

static int siteDef, symDef, sizeDef, pinDef, obsDef, origDef;
static int useLenThr;
static int layerCut, layerMastOver, layerRout, layerDir;
static lefiAntennaEnum antennaType = lefiAntennaAR;  /* 5.4 - antenna type */
static int viaRuleLayer;       /* keep track number of layer in a viarule */
static int viaLayer;           /* keep track number of layer in a via */
static int ndLayer;            /* keep track number of layer in a nondefault */
static int numVia;             /* keep track number of via */
static int viaRuleHasDir;      /* viarule layer has direction construct */
static int viaRuleHasEnc;      /* viarule layer has enclosure construct */
static int ndRule = 0;         /* keep track if inside nondefaultrule */
static int ndLayerWidth;       /* keep track if width is set at ndLayer */
static int ndLayerSpace;       /* keep track if spacing is set at ndLayer */
static int isGenerate;         /* keep track if viarule has generate */
static int hasType;            /* keep track of type in layer */
static int hasPitch;           /* keep track of pitch in layer */
static int hasWidth;           /* keep track of width in layer */
static int hasDirection;       /* keep track of direction in layer */
static int hasParallel;        /* keep track of parallelrunlength */
static int hasInfluence;       /* keep track of influence */
static int hasLayerMincut;     /* keep track of layer minimumcut */
static int hasManufactur = 0;  /* keep track of manufacture is after unit */
static int hasPRP;             /* keep track if path, rect or poly is def */
static int needGeometry;       /* keep track if path/rect/poly is defined */
static int hasViaRule_layer = 0; /* keep track at least viarule or layer */
static int hasSamenet;         /* keep track if samenet is defined in spacing */
static int hasSiteClass = 0;   /* keep track if SITE has CLASS */
static int hasSiteSize = 0;    /* keep track if SITE has SIZE */

// the following variables to keep track the number of warnings printed.
static int antennaInoutWarnings = 0;
static int antennaInputWarnings = 0;
static int antennaOutputWarnings = 0;
static int arrayWarnings = 0;
static int caseSensitiveWarnings = 0;
static int correctionTableWarnings = 0;
static int dielectricWarnings = 0;
static int edgeRateThreshold1Warnings = 0;
static int edgeRateThreshold2Warnings = 0;
static int edgeRateScaleFactorWarnings = 0;
static int inoutAntennaWarnings = 0;
static int inputAntennaWarnings = 0;
static int iRDropWarnings = 0;
static int layerWarnings = 0;
static int macroWarnings = 0;
static int maxStackViaWarnings = 0;
static int minFeatureWarnings = 0;
static int noiseMarginWarnings = 0;
static int noiseTableWarnings = 0;
static int nonDefaultWarnings = 0;
static int noWireExtensionWarnings = 0;
static int outputAntennaWarnings = 0;
static int pinWarnings = 0;
static int siteWarnings = 0;
static int spacingWarnings = 0;
static int timingWarnings = 0;
static int unitsWarnings = 0;
static int useMinSpacingWarnings = 0;
static int viaRuleWarnings = 0;
static int viaWarnings = 0;

int spParallelLength;          /* the number of layer parallelrunlength */

lefiNum macroNum;              /* for origin & size callback */

int comp_str(char *s1, int op, char *s2)
{
    int k = strcmp(s1, s2);
    switch (op) {
	case C_EQ: return k == 0;
	case C_NE: return k != 0;
	case C_GT: return k >  0;
	case C_GE: return k >= 0;
	case C_LT: return k <  0;
	case C_LE: return k <= 0;
	}
    return 0;
}
int comp_num(double s1, int op, double s2)
{
    double k = s1 - s2;
    switch (op) {
	case C_EQ: return k == 0;
	case C_NE: return k != 0;
	case C_GT: return k >  0;
	case C_GE: return k >= 0;
	case C_LT: return k <  0;
	case C_LE: return k <= 0;
	}
    return 0;
}
/*
double convert_name2num(char *versionName)
{
    char majorNm[80];
    char minorNm[80];
    char *subMinorNm = NULL;
    char *p1;
    char *versionNm = strdup(versionName);
  
    double major = 0, minor = 0, subMinor = 0;
    double version;

    sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
    if (p1 = strchr(minorNm, '.')) {
       subMinorNm = p1+1; 
       *p1 = '\0';
    }
    major = atof(majorNm);
    minor = atof(minorNm);
    if (subMinorNm)
       subMinor = atof(subMinorNm);

    version = major;
    
    if (minor > 0)
       version = major + minor/10;

    if (subMinor > 0)
       version = version + subMinor/1000;

    lefFree(versionNm);
    return version;
}
*/

int validNum(int values) {
    char *outMsg;
    switch (values) {
        case 100:
        case 200:
        case 1000:
        case 2000:
                return 1; 
        case 10000:
        case 20000:
             if (versionNum < 5.6) {
                if (lefrUnitsCbk) {
                  if (unitsWarnings++ < lefrUnitsWarnings) {
                    outMsg = (char*)lefMalloc(10000);
                    sprintf (outMsg,
                    "Error found when processing LEF file '%s'\nUnit %d is a 5.6 or later syntax\nYour lef file is not defined as 5.6",
                    lefrFileName, values);
                    yyerror(outMsg);
                    lefFree(outMsg);
                  }
                }
                return 0;
             } else
                return 1;
    }
    if (unitsWarnings++ < lefrUnitsWarnings)
      yyerror("Illegal value for LEF UNITS DATABASE MICRONS");
    CHKERR();
    return 0;
}

int zeroOrGt(double values) {
    if (values < 0)
      return 0;
    return 1;
}


#line 301 "lef.y"
typedef union {
	double    dval ;
	int       integer ;
	char *    string ;
	lefPOINT  pt;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		1915
#define	YYFLAG		-32768
#define	YYNTBASE	415

#define YYTRANSLATE(x) ((unsigned)(x) <= 658 ? yytranslate[x] : 827)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,   412,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,   409,
   410,   405,   404,     2,   403,     2,   406,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,   408,   413,
   411,   414,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
   106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
   116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
   126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
   136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
   146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
   156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
   166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
   176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
   186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
   196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
   206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
   216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
   226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
   236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
   246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
   256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
   266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
   276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
   286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
   296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
   306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
   316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
   326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
   336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
   346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
   356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
   366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
   376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
   386,   387,   388,   389,   390,   391,   392,   393,   394,   395,
   396,   397,   398,   399,   400,   401,   402,   407
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     5,    10,    14,    18,    19,    22,    24,    25,
    28,    30,    32,    34,    36,    38,    40,    42,    44,    46,
    48,    50,    52,    54,    56,    58,    60,    62,    64,    66,
    68,    70,    72,    74,    76,    78,    80,    82,    84,    86,
    88,    90,    92,    94,    96,    98,   100,   102,   104,   108,
   112,   116,   120,   124,   129,   133,   135,   137,   139,   141,
   143,   145,   150,   152,   153,   156,   161,   166,   171,   176,
   181,   186,   191,   196,   200,   201,   205,   206,   210,   211,
   214,   218,   222,   227,   231,   236,   240,   245,   249,   253,
   257,   261,   262,   269,   273,   278,   286,   291,   299,   303,
   307,   311,   315,   319,   323,   327,   331,   338,   345,   346,
   351,   352,   357,   362,   367,   368,   369,   379,   380,   381,
   391,   395,   396,   401,   405,   406,   411,   412,   418,   422,
   423,   428,   432,   433,   438,   439,   445,   446,   451,   455,
   459,   463,   467,   471,   475,   479,   483,   487,   491,   496,
   500,   504,   508,   512,   513,   519,   520,   528,   529,   535,
   543,   544,   549,   550,   558,   559,   567,   571,   575,   576,
   582,   583,   584,   585,   586,   598,   599,   609,   610,   612,
   614,   615,   618,   619,   622,   623,   626,   628,   630,   635,
   636,   639,   641,   644,   646,   648,   650,   652,   653,   661,
   662,   665,   667,   668,   670,   672,   674,   676,   677,   678,
   679,   692,   693,   694,   700,   701,   707,   708,   714,   715,
   718,   720,   723,   726,   729,   732,   734,   737,   742,   744,
   747,   752,   754,   757,   762,   764,   766,   768,   770,   772,
   774,   776,   778,   780,   782,   783,   786,   788,   790,   792,
   794,   795,   798,   799,   804,   805,   808,   815,   819,   820,
   828,   829,   834,   836,   839,   843,   847,   848,   849,   850,
   877,   878,   881,   886,   891,   898,   899,   904,   906,   908,
   911,   912,   915,   917,   919,   923,   924,   929,   931,   933,
   936,   939,   942,   945,   948,   952,   957,   961,   962,   966,
   968,   970,   972,   974,   976,   978,   980,   982,   984,   986,
   988,   990,   992,   994,   996,   998,  1001,  1002,  1007,  1008,
  1011,  1016,  1017,  1025,  1026,  1030,  1031,  1035,  1041,  1042,
  1050,  1051,  1053,  1055,  1058,  1059,  1061,  1063,  1066,  1067,
  1072,  1074,  1077,  1080,  1083,  1086,  1089,  1090,  1093,  1097,
  1098,  1103,  1104,  1107,  1111,  1115,  1120,  1126,  1131,  1137,
  1141,  1145,  1149,  1150,  1154,  1158,  1160,  1163,  1164,  1167,
  1173,  1180,  1182,  1186,  1188,  1191,  1192,  1195,  1199,  1200,
  1203,  1206,  1209,  1214,  1218,  1219,  1220,  1221,  1230,  1232,
  1235,  1236,  1239,  1240,  1243,  1245,  1247,  1249,  1251,  1253,
  1255,  1257,  1261,  1265,  1270,  1271,  1276,  1278,  1281,  1284,
  1287,  1290,  1291,  1292,  1293,  1294,  1307,  1308,  1311,  1315,
  1319,  1324,  1329,  1333,  1337,  1341,  1342,  1346,  1347,  1351,
  1352,  1355,  1361,  1363,  1365,  1367,  1371,  1375,  1379,  1383,
  1384,  1387,  1389,  1391,  1393,  1394,  1399,  1400,  1403,  1404,
  1408,  1411,  1416,  1417,  1422,  1423,  1427,  1428,  1432,  1433,
  1436,  1438,  1440,  1442,  1444,  1446,  1448,  1450,  1452,  1454,
  1456,  1458,  1460,  1462,  1466,  1470,  1472,  1474,  1476,  1478,
  1479,  1484,  1486,  1489,  1493,  1494,  1497,  1499,  1501,  1503,
  1506,  1509,  1512,  1516,  1518,  1521,  1523,  1525,  1528,  1531,
  1533,  1535,  1537,  1540,  1542,  1544,  1547,  1550,  1552,  1554,
  1556,  1558,  1560,  1562,  1564,  1566,  1568,  1570,  1572,  1574,
  1576,  1578,  1580,  1582,  1584,  1586,  1590,  1595,  1599,  1603,
  1607,  1611,  1615,  1618,  1622,  1627,  1631,  1632,  1637,  1638,
  1643,  1647,  1651,  1653,  1655,  1661,  1665,  1666,  1670,  1671,
  1675,  1676,  1679,  1682,  1686,  1691,  1695,  1700,  1706,  1707,
  1712,  1716,  1718,  1722,  1726,  1730,  1734,  1738,  1742,  1746,
  1750,  1754,  1758,  1762,  1763,  1768,  1769,  1775,  1776,  1782,
  1783,  1789,  1793,  1797,  1801,  1805,  1809,  1813,  1817,  1821,
  1825,  1830,  1834,  1835,  1840,  1845,  1848,  1853,  1858,  1863,
  1867,  1871,  1876,  1881,  1886,  1891,  1896,  1901,  1906,  1911,
  1912,  1917,  1918,  1923,  1924,  1929,  1930,  1935,  1937,  1939,
  1941,  1943,  1945,  1948,  1951,  1954,  1957,  1961,  1965,  1970,
  1974,  1978,  1980,  1981,  1985,  1987,  1989,  1991,  1993,  1995,
  1997,  1999,  2001,  2003,  2005,  2006,  2008,  2010,  2012,  2015,
  2016,  2017,  2024,  2028,  2033,  2040,  2045,  2052,  2060,  2070,
  2072,  2073,  2076,  2077,  2080,  2083,  2085,  2087,  2088,  2091,
  2092,  2098,  2099,  2107,  2115,  2127,  2132,  2133,  2134,  2145,
  2146,  2147,  2158,  2165,  2172,  2173,  2176,  2178,  2185,  2192,
  2196,  2199,  2201,  2206,  2207,  2210,  2211,  2212,  2220,  2221,
  2224,  2230,  2231,  2236,  2240,  2242,  2245,  2246,  2249,  2250,
  2255,  2256,  2261,  2262,  2273,  2283,  2287,  2291,  2296,  2301,
  2306,  2311,  2316,  2321,  2326,  2331,  2335,  2343,  2352,  2359,
  2363,  2367,  2371,  2374,  2376,  2378,  2380,  2382,  2384,  2386,
  2388,  2390,  2392,  2394,  2396,  2398,  2400,  2402,  2404,  2406,
  2409,  2415,  2417,  2420,  2421,  2426,  2434,  2436,  2438,  2440,
  2442,  2444,  2446,  2449,  2451,  2454,  2455,  2460,  2461,  2465,
  2466,  2470,  2471,  2474,  2475,  2480,  2481,  2486,  2487,  2492,
  2493,  2498,  2503,  2504,  2509,  2515,  2517,  2520,  2521,  2524,
  2525,  2530,  2531,  2536,  2537,  2540,  2546,  2547,  2554,  2555,
  2562,  2563,  2570,  2571,  2578,  2579,  2586,  2587,  2589,  2591,
  2593,  2596,  2598,  2601,  2605,  2609,  2613,  2617,  2620,  2624,
  2631,  2633,  2637,  2641,  2645,  2649,  2653,  2657,  2661,  2665,
  2669,  2673,  2676,  2680,  2687,  2689,  2691,  2695,  2699,  2706,
  2708,  2710,  2712,  2714,  2716,  2718,  2720,  2722,  2724,  2726,
  2727,  2733,  2734,  2737,  2738,  2744,  2745,  2751,  2752,  2758,
  2759,  2765,  2766,  2772,  2773,  2779,  2780,  2786,  2787,  2793,
  2794,  2799,  2800,  2805,  2807,  2810,  2813,  2814,  2815,  2821,
  2824,  2830,  2831,  2833,  2836,  2842,  2846,  2847,  2851,  2852,
  2854,  2855,  2856,  2860,  2865,  2867,  2868,  2869,  2873,  2874,
  2878,  2883,  2887,  2891,  2895,  2896,  2904,  2907,  2909,  2912,
  2916,  2918,  2919,  2925,  2927,  2930,  2932,  2935,  2936,  2944,
  2946,  2949,  2950,  2958,  2961,  2963,  2966,  2970,  2972,  2973,
  2979,  2981,  2984,  2986,  2989,  2990,  2998,  3000,  3003,  3007,
  3011,  3015,  3019,  3023,  3027,  3028,  3030
};

static const short yyrhs[] = {   420,
   825,   421,     0,     0,   240,   417,   172,   408,     0,   260,
   173,   408,     0,   239,   173,   408,     0,     0,   420,   422,
     0,     1,     0,     0,    37,    67,     0,   416,     0,   419,
     0,   423,     0,   431,     0,   435,     0,   512,     0,   544,
     0,   545,     0,   418,     0,   424,     0,   750,     0,   563,
     0,   578,     0,   577,     0,   569,     0,   600,     0,   617,
     0,   730,     0,   754,     0,   579,     0,   765,     0,   791,
     0,   792,     0,   794,     0,   793,     0,   795,     0,   807,
     0,   819,     0,   820,     0,   821,     0,   822,     0,   823,
     0,   824,     0,   425,     0,   426,     0,   427,     0,   510,
     0,   752,     0,    77,    95,   408,     0,    77,    94,   408,
     0,   270,    95,   408,     0,   270,    94,   408,     0,   314,
   174,   408,     0,   319,   429,   430,   408,     0,   316,   428,
   408,     0,   318,     0,   317,     0,    84,     0,   101,     0,
    95,     0,    94,     0,   432,   433,    37,   158,     0,   158,
     0,     0,   433,   434,     0,   148,    78,   174,   408,     0,
    14,   100,   174,   408,     0,   111,    85,   174,   408,     0,
   107,    74,   174,   408,     0,    26,    73,   174,   408,     0,
   166,   167,   174,   408,     0,    30,   388,   174,   408,     0,
   295,   297,   174,   408,     0,   436,   440,   438,     0,     0,
    64,   437,   172,     0,     0,    37,   439,   172,     0,     0,
   441,   440,     0,   156,   501,   408,     0,   102,   174,   408,
     0,   102,   174,   174,   408,     0,   380,   174,   408,     0,
   380,   174,   174,   408,     0,    86,   174,   408,     0,    86,
   174,   174,   408,     0,   381,   174,   408,     0,   379,   174,
   408,     0,   168,   174,   408,     0,     9,   174,   408,     0,
     0,   135,   174,   442,   780,   785,   408,     0,    33,   502,
   408,     0,   111,   125,   174,   408,     0,   111,   125,   257,
   409,   499,   410,   408,     0,    14,    25,   174,   408,     0,
    14,    25,   257,   409,   497,   410,   408,     0,    53,   174,
   408,     0,   271,   174,   408,     0,   144,   174,   408,     0,
   129,   174,   408,     0,    15,   174,   408,     0,    35,   174,
   408,     0,   258,   174,   408,     0,   256,   174,   408,     0,
   256,   257,   409,   495,   410,   408,     0,   256,   409,   174,
   174,   410,   408,     0,     0,   237,   443,   493,   408,     0,
     0,   290,   482,   444,   483,     0,   290,   482,   174,   408,
     0,   291,   292,   174,   408,     0,     0,     0,   291,   292,
   296,   174,   445,   492,   408,   446,   490,     0,     0,     0,
   291,   292,   168,   174,   447,   492,   408,   448,   490,     0,
   303,   174,   408,     0,     0,   304,   449,   477,   408,     0,
   305,   174,   408,     0,     0,   306,   450,   477,   408,     0,
     0,   307,   174,   451,   481,   408,     0,   308,   174,   408,
     0,     0,   309,   452,   477,   408,     0,   310,   174,   408,
     0,     0,   311,   453,   477,   408,     0,     0,   312,   174,
   454,   481,   408,     0,     0,   347,   455,   504,   408,     0,
   332,   174,   408,     0,   333,   174,   408,     0,   334,   174,
   408,     0,   335,   174,   408,     0,   336,   174,   408,     0,
   337,   174,   408,     0,   338,   174,   408,     0,   339,   174,
   408,     0,   340,   174,   408,     0,   341,   174,   408,     0,
   342,   174,   174,   408,     0,   343,   174,   408,     0,   344,
   174,   408,     0,   357,   174,   408,     0,   366,   174,   408,
     0,     0,   358,   174,   456,   503,   408,     0,     0,   345,
   174,   168,   174,   457,   472,   408,     0,     0,   359,   174,
   458,   474,   408,     0,   367,   174,   353,   174,   168,   174,
   408,     0,     0,   368,   459,   463,   408,     0,     0,   349,
   469,   174,   174,   460,   470,   408,     0,     0,   393,   469,
   174,   174,   461,   471,   408,     0,   111,   174,   408,     0,
   378,   174,   408,     0,     0,   390,   462,   682,   684,   408,
     0,     0,     0,     0,     0,   365,   174,   464,   492,   465,
   168,   174,   466,   492,   467,   505,     0,     0,    21,   168,
   174,   369,   174,   135,   174,   468,   508,     0,     0,   370,
     0,   371,     0,     0,   168,   174,     0,     0,   168,   174,
     0,     0,   473,   472,     0,   350,     0,   351,     0,   353,
   174,   369,   174,     0,     0,   475,   474,     0,   476,     0,
   387,   174,     0,   385,     0,   392,     0,   139,     0,   174,
     0,     0,   257,   409,   616,   616,   478,   479,   410,     0,
     0,   480,   479,     0,   616,     0,     0,   313,     0,   293,
     0,   292,     0,   294,     0,     0,     0,     0,   295,   174,
   484,   492,   408,   485,   487,   229,   174,   486,   492,   408,
     0,     0,     0,   296,   174,   488,   492,   408,     0,     0,
   168,   174,   489,   492,   408,     0,     0,   229,   174,   491,
   492,   408,     0,     0,   492,   174,     0,   494,     0,   493,
   494,     0,   172,   172,     0,   172,   173,     0,   172,   174,
     0,   496,     0,   495,   496,     0,   409,   174,   174,   410,
     0,   498,     0,   497,   498,     0,   409,   174,   174,   410,
     0,   500,     0,   499,   500,     0,   409,   174,   174,   410,
     0,   123,     0,    28,     0,    93,     0,   191,     0,   238,
     0,   352,     0,    54,     0,   161,     0,   376,     0,   377,
     0,     0,   168,   174,     0,   361,     0,   362,     0,   363,
     0,   364,     0,     0,   506,   505,     0,     0,   168,   174,
   507,   492,     0,     0,   508,   509,     0,   168,   174,   369,
   174,   135,   174,     0,   354,   174,   408,     0,     0,   354,
   174,   236,   511,   172,   172,   408,     0,     0,   515,   513,
   523,   540,     0,   163,     0,   514,   172,     0,   514,   172,
    29,     0,   514,   172,   382,     0,     0,     0,     0,   164,
   517,   172,   408,   373,   174,   174,   408,   386,   518,   172,
   172,   172,   408,   374,   174,   174,   408,   349,   174,   174,
   174,   174,   408,   519,   520,     0,     0,   520,   521,     0,
   394,   174,   174,   408,     0,    88,   174,   174,   408,     0,
    86,   174,   174,   174,   174,   408,     0,     0,    99,   522,
   172,   408,     0,   516,     0,   524,     0,   526,   525,     0,
     0,   525,   526,     0,   530,     0,   534,     0,   111,   174,
   408,     0,     0,   237,   527,   528,   408,     0,   154,     0,
   529,     0,   528,   529,     0,   172,   174,     0,   172,   173,
     0,   172,   172,     0,   531,   408,     0,   531,   616,   408,
     0,   531,   616,   533,   408,     0,   531,   533,   408,     0,
     0,    48,   532,   172,     0,   175,     0,   178,     0,   176,
     0,   177,     0,   179,     0,   182,     0,   180,     0,   181,
     0,   183,     0,   171,     0,   184,     0,   185,     0,   187,
     0,   189,     0,   186,     0,   188,     0,   535,   537,     0,
     0,    64,   536,   172,   408,     0,     0,   537,   538,     0,
   110,   616,   616,   408,     0,     0,   104,   539,   682,   683,
   683,   684,   408,     0,     0,    37,   541,   172,     0,     0,
   164,   543,   172,     0,   542,   548,   555,   549,   561,     0,
     0,   542,    50,   547,   546,   548,   549,   561,     0,     0,
    29,     0,   554,     0,   548,   554,     0,     0,   550,     0,
   551,     0,   550,   551,     0,     0,   237,   552,   553,   408,
     0,   551,     0,   553,   551,     0,   172,   172,     0,   172,
   173,     0,   172,   174,     0,   557,   559,     0,     0,   555,
   556,     0,   514,   172,   408,     0,     0,    64,   558,   172,
   408,     0,     0,   559,   560,     0,    33,    54,   408,     0,
    33,   161,   408,     0,   349,   174,   174,   408,     0,   168,
   174,   150,   174,   408,     0,   110,   616,   616,   408,     0,
   135,   174,    13,   174,   408,     0,   111,   174,   408,     0,
    92,   174,   408,     0,    72,   174,   408,     0,     0,    37,
   562,   172,     0,   564,   566,   565,     0,   135,     0,    37,
   135,     0,     0,   566,   567,     0,   568,   172,   172,   174,
   408,     0,   568,   172,   172,   174,   137,   408,     0,   126,
     0,   570,   572,   571,     0,    61,     0,    37,    61,     0,
     0,   572,   573,     0,   576,   574,   408,     0,     0,   574,
   575,     0,   174,   174,     0,   143,   172,     0,    75,   174,
   174,   408,     0,    32,   174,   408,     0,     0,     0,     0,
    81,   580,   172,   581,   584,   585,   582,   583,     0,    37,
     0,    37,   172,     0,     0,   384,   408,     0,     0,   585,
   586,     0,   593,     0,   512,     0,   563,     0,   590,     0,
   587,     0,   588,     0,   589,     0,   398,   172,   408,     0,
   399,   172,   408,     0,   389,   172,   174,   408,     0,     0,
   237,   591,   592,   408,     0,   590,     0,   592,   590,     0,
   172,   172,     0,   172,   173,     0,   172,   174,     0,     0,
     0,     0,     0,    64,   594,   172,   595,   168,   174,   408,
   596,   598,    37,   597,   172,     0,     0,   598,   599,     0,
   135,   174,   408,     0,   271,   174,   408,     0,   111,   125,
   174,   408,     0,    14,    25,   174,   408,     0,    35,   174,
   408,     0,   381,   174,   408,     0,   601,   605,   603,     0,
     0,   131,   602,   172,     0,     0,    37,   604,   172,     0,
     0,   605,   606,     0,   132,   174,    13,   174,   408,     0,
   608,     0,   607,     0,   611,     0,    16,    97,   408,     0,
    16,    22,   408,     0,    16,   238,   408,     0,   142,   609,
   408,     0,     0,   609,   610,     0,   169,     0,   170,     0,
   171,     0,     0,   395,   612,   613,   408,     0,     0,   613,
   614,     0,     0,   172,   533,   615,     0,   174,   174,     0,
   409,   174,   174,   410,     0,     0,   619,   623,   618,   621,
     0,     0,    68,   620,   172,     0,     0,    37,   622,   172,
     0,     0,   624,   623,     0,   631,     0,   636,     0,   637,
     0,   638,     0,   627,     0,   640,     0,   639,     0,   641,
     0,   642,     0,   644,     0,   649,     0,   646,     0,   650,
     0,   266,   267,   408,     0,   266,   268,   408,     0,   698,
     0,   700,     0,   707,     0,   709,     0,     0,   237,   625,
   626,   408,     0,   630,     0,   626,   630,     0,   142,   628,
   408,     0,     0,   628,   629,     0,   169,     0,   170,     0,
   171,     0,   172,   174,     0,   172,   173,     0,   172,   172,
     0,    16,   632,   408,     0,    24,     0,    24,   348,     0,
   113,     0,    10,     0,    10,   356,     0,    10,   396,     0,
   323,     0,    97,     0,   238,     0,    97,   633,     0,    22,
     0,    23,     0,    22,   634,     0,    38,   635,     0,    56,
     0,    89,     0,    55,     0,   107,     0,   134,     0,   355,
     0,    46,     0,   145,     0,   146,     0,   134,     0,   315,
     0,   400,     0,   108,     0,   106,     0,   152,     0,   153,
     0,    11,     0,    12,     0,    51,   172,   408,     0,    50,
   172,   172,   408,     0,   133,   190,   408,     0,   133,    50,
   408,     0,   133,    10,   408,     0,   107,   174,   408,     0,
    88,   616,   408,     0,   531,   408,     0,   531,   616,   408,
     0,   531,   616,   533,   408,     0,   531,   533,   408,     0,
     0,    36,   643,   172,   408,     0,     0,    66,   645,   172,
   408,     0,   647,   172,   408,     0,   647,   689,   408,     0,
   131,     0,   131,     0,   132,   174,    13,   174,   408,     0,
   651,   655,   653,     0,     0,   101,   652,   172,     0,     0,
    37,   654,   172,     0,     0,   655,   656,     0,   531,   408,
     0,   531,   616,   408,     0,   531,   616,   533,   408,     0,
   531,   141,   408,     0,   531,   141,   616,   408,     0,   531,
   141,   616,   533,   408,     0,     0,    66,   657,   172,   408,
     0,   107,   174,   408,     0,   670,     0,   159,   673,   408,
     0,   127,   674,   408,     0,    65,   174,   408,     0,   118,
   174,   408,     0,    45,   174,   408,     0,   117,   174,   408,
     0,    44,   174,   408,     0,   165,   174,   408,     0,   162,
   174,   408,     0,   147,   174,   408,     0,   128,   675,   408,
     0,     0,    76,   658,   172,   408,     0,     0,    90,   659,
   174,   174,   408,     0,     0,    91,   660,   174,   174,   408,
     0,     0,    57,   661,   174,   174,   408,     0,    14,   174,
   408,     0,    70,   174,   408,     0,    71,   174,   408,     0,
   111,   174,   408,     0,   109,   174,   408,     0,    27,     6,
   408,     0,    27,   112,   408,     0,   121,   174,   408,     0,
   122,   174,   408,     0,    63,   172,   172,   408,     0,   259,
   172,   408,     0,     0,   237,   662,   668,   408,     0,   671,
   672,   676,    37,     0,   671,    37,     0,   261,   174,   787,
   408,     0,   263,   174,   787,   408,     0,   262,   174,   787,
   408,     0,   264,   174,   408,     0,   265,   174,   408,     0,
   324,   174,   787,   408,     0,   325,   174,   787,   408,     0,
   330,   174,   787,   408,     0,   327,   174,   787,   408,     0,
   326,   174,   787,   408,     0,   328,   174,   789,   408,     0,
   329,   174,   789,   408,     0,   331,   174,   789,   408,     0,
     0,   347,   663,   667,   408,     0,     0,   391,   664,   173,
   408,     0,     0,   397,   665,   172,   408,     0,     0,   383,
   666,   172,   408,     0,   361,     0,   362,     0,   363,     0,
   364,     0,   669,     0,   668,   669,     0,   172,   174,     0,
   172,   173,     0,   172,   172,     0,    33,    56,   408,     0,
    33,    89,   408,     0,    33,    89,   155,   408,     0,    33,
    55,   408,     0,    33,    46,   408,     0,   105,     0,     0,
    16,   632,   408,     0,   130,     0,     7,     0,   107,     0,
    52,     0,    17,     0,    31,     0,    56,     0,    89,     0,
   138,     0,   140,     0,     0,     5,     0,   113,     0,    46,
     0,   677,   680,     0,     0,     0,    64,   678,   172,   679,
   681,   408,     0,   168,   174,   408,     0,    98,   682,   684,
   408,     0,    98,    62,   682,   684,   688,   408,     0,   110,
   616,   616,   408,     0,   110,    62,   616,   616,   688,   408,
     0,   104,   682,   683,   683,   683,   684,   408,     0,   104,
    62,   682,   683,   683,   683,   684,   688,   408,     0,   685,
     0,     0,   680,   677,     0,     0,   135,   174,     0,    20,
   174,     0,   616,     0,   616,     0,     0,   684,   683,     0,
     0,   163,   616,   686,   172,   408,     0,     0,   163,    62,
   616,   687,   172,   688,   408,     0,    34,   174,    13,   174,
   139,   174,   174,     0,   172,   174,   174,   533,    34,   174,
    13,   174,   139,   174,   174,     0,   172,   174,   174,   533,
     0,     0,     0,   169,   174,    34,   174,   139,   174,   691,
    64,   692,   695,     0,     0,     0,   170,   174,    34,   174,
   139,   174,   693,    64,   694,   695,     0,   169,   174,    34,
   174,   139,   174,     0,   170,   174,    34,   174,   139,   174,
     0,     0,   695,   696,     0,   172,     0,   169,   174,    34,
   174,   139,   174,     0,   170,   174,    34,   174,   139,   174,
     0,   699,   676,    37,     0,   699,    37,     0,    84,     0,
   375,   702,   701,    37,     0,     0,   701,   702,     0,     0,
     0,    64,   703,   172,   408,   704,   706,   705,     0,     0,
   705,   706,     0,   110,   616,   616,   174,   408,     0,     0,
    18,   708,   172,   408,     0,   710,   712,   711,     0,   149,
     0,    37,   149,     0,     0,   712,   713,     0,     0,    49,
   714,   728,   408,     0,     0,   151,   715,   729,   408,     0,
     0,   726,    59,   174,   174,   716,   725,   160,   174,   174,
   408,     0,   726,   721,   157,   727,   227,   174,   174,   174,
   408,     0,   228,   724,   408,     0,   229,   722,   408,     0,
   116,   174,   174,   408,     0,    43,   174,   174,   408,     0,
   115,   174,   174,   408,     0,    40,   174,   174,   408,     0,
   119,   174,   174,   408,     0,    42,   174,   174,   408,     0,
   120,   174,   174,   408,     0,    41,   174,   174,   408,     0,
   157,   727,   408,     0,   206,   207,   174,   208,   174,   726,
   408,     0,   718,   719,   720,   227,   174,   174,   174,   408,
     0,   717,   227,   174,   174,   174,   408,     0,   284,   173,
   408,     0,   285,   173,   408,     0,   286,   173,   408,     0,
   231,   408,     0,   287,     0,   288,     0,   289,     0,   207,
     0,   208,     0,   279,     0,   280,     0,   281,     0,   282,
     0,   283,     0,   281,     0,   282,     0,   283,     0,   226,
     0,   230,     0,   723,     0,   722,   723,     0,   409,   174,
   174,   174,   410,     0,   174,     0,   724,   174,     0,     0,
   174,   174,   174,   174,     0,   174,   174,   174,   174,   174,
   174,   174,     0,   114,     0,    39,     0,    60,     0,    82,
     0,    83,     0,   172,     0,   728,   172,     0,   172,     0,
   729,   172,     0,     0,   732,   736,   731,   734,     0,     0,
     8,   733,   172,     0,     0,    37,   735,   172,     0,     0,
   736,   737,     0,     0,   648,   738,   689,   408,     0,     0,
   198,   739,   689,   408,     0,     0,   199,   740,   689,   408,
     0,     0,   200,   741,   690,   408,     0,   743,   744,    37,
   172,     0,     0,   202,   742,   697,   408,     0,   203,   174,
   748,    37,   203,     0,   754,     0,   201,   172,     0,     0,
   744,   745,     0,     0,   198,   746,   689,   408,     0,     0,
   199,   747,   689,   408,     0,     0,   748,   749,     0,   204,
   174,   205,   174,   408,     0,     0,   272,   751,   172,   411,
   763,   758,     0,     0,   273,   753,   172,   411,   763,   758,
     0,     0,   209,   755,   172,   411,   761,   758,     0,     0,
   210,   756,   172,   411,   763,   758,     0,     0,   211,   757,
   172,   411,   762,   758,     0,     0,   408,     0,   412,     0,
   213,     0,   412,   213,     0,   214,     0,   412,   214,     0,
   761,   404,   761,     0,   761,   403,   761,     0,   761,   405,
   761,     0,   761,   406,   761,     0,   403,   761,     0,   409,
   761,   410,     0,   212,   762,   759,   761,   760,   761,     0,
   174,     0,   761,   764,   761,     0,   761,   224,   761,     0,
   761,   223,   761,     0,   763,   764,   763,     0,   763,   224,
   763,     0,   763,   223,   763,     0,   762,   217,   762,     0,
   762,   218,   762,     0,   762,   224,   762,     0,   762,   223,
   762,     0,   225,   762,     0,   409,   762,   410,     0,   212,
   762,   759,   762,   760,   762,     0,   216,     0,   215,     0,
   763,   404,   763,     0,   409,   763,   410,     0,   212,   762,
   759,   763,   760,   763,     0,   173,     0,   219,     0,   220,
     0,   221,     0,   222,     0,   217,     0,   218,     0,   411,
     0,   413,     0,   414,     0,     0,   232,   766,   767,    37,
   232,     0,     0,   767,   768,     0,     0,    67,   769,   172,
   777,   408,     0,     0,    58,   770,   172,   777,   408,     0,
     0,   101,   771,   172,   777,   408,     0,     0,    68,   772,
   172,   777,   408,     0,     0,   163,   773,   172,   777,   408,
     0,     0,   164,   774,   172,   777,   408,     0,     0,    64,
   775,   172,   777,   408,     0,     0,    81,   776,   172,   777,
   408,     0,     0,   234,   778,   783,   784,     0,     0,   235,
   779,   783,   784,     0,   233,     0,   233,   173,     0,   269,
   172,     0,     0,     0,   236,   174,   174,   781,   782,     0,
   299,   174,     0,   299,   174,   236,   174,   174,     0,     0,
   298,     0,    21,   174,     0,    21,   174,   236,   174,   174,
     0,   236,   174,   174,     0,     0,   236,   174,   174,     0,
     0,   174,     0,     0,     0,    64,   786,   172,     0,   346,
   174,   369,   174,     0,   372,     0,     0,     0,    64,   788,
   172,     0,     0,    64,   790,   172,     0,   243,   174,   174,
   408,     0,   244,   174,   408,     0,   247,   174,   408,     0,
   246,   174,   408,     0,     0,   249,   174,   796,   408,   798,
   797,   758,     0,    37,   249,     0,   799,     0,   798,   799,
     0,   250,   174,   408,     0,   800,     0,     0,    91,   801,
   802,   408,   803,     0,   174,     0,   802,   174,     0,   804,
     0,   803,   804,     0,     0,   251,   174,   408,   805,   248,
   806,   408,     0,   174,     0,   806,   174,     0,     0,   245,
   174,   408,   808,   810,   809,   758,     0,    37,   245,     0,
   811,     0,   810,   811,     0,   250,   174,   408,     0,   812,
     0,     0,    91,   813,   814,   408,   815,     0,   174,     0,
   814,   174,     0,   816,     0,   815,   816,     0,     0,   251,
   174,   408,   817,   252,   818,   408,     0,   174,     0,   818,
   174,     0,   254,   174,   408,     0,   253,   174,   408,     0,
   255,   174,   408,     0,   300,   174,   408,     0,   301,   174,
   408,     0,   302,   174,   408,     0,     0,   826,     0,   241,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   432,   483,   483,   518,   525,   532,   533,   534,   537,   544,
   571,   571,   571,   571,   572,   572,   572,   572,   572,   573,
   573,   574,   574,   574,   574,   574,   574,   574,   575,   575,
   575,   576,   576,   577,   577,   578,   578,   578,   579,   579,
   580,   580,   580,   580,   581,   581,   581,   582,   585,   598,
   616,   626,   637,   644,   659,   662,   664,   666,   668,   670,
   672,   674,   680,   691,   692,   695,   697,   699,   701,   703,
   705,   707,   714,   717,   724,   725,   754,   755,   802,   808,
   811,   818,   823,   828,   832,   836,   840,   844,   848,   852,
   857,   861,   890,   891,   905,   917,   928,   940,   951,   963,
   975,   987,   999,  1011,  1036,  1059,  1071,  1082,  1094,  1094,
  1099,  1112,  1112,  1127,  1142,  1165,  1168,  1168,  1191,  1196,
  1196,  1226,  1256,  1257,  1287,  1317,  1318,  1332,  1333,  1363,
  1393,  1394,  1424,  1454,  1455,  1486,  1487,  1517,  1518,  1532,
  1546,  1560,  1574,  1588,  1602,  1616,  1630,  1644,  1658,  1672,
  1686,  1700,  1721,  1742,  1754,  1755,  1761,  1768,  1772,  1775,
  1787,  1807,  1809,  1823,  1825,  1839,  1840,  1854,  1875,  1882,
  1894,  1917,  1921,  1928,  1940,  1940,  1965,  1965,  1967,  1968,
  1970,  1971,  1978,  1979,  1986,  1990,  1992,  2008,  2022,  2036,
  2040,  2042,  2047,  2052,  2054,  2055,  2057,  2061,  2069,  2074,
  2078,  2081,  2087,  2088,  2113,  2115,  2116,  2118,  2121,  2124,
  2126,  2129,  2130,  2142,  2144,  2156,  2159,  2162,  2165,  2166,
  2169,  2171,  2174,  2183,  2191,  2202,  2205,  2208,  2211,  2213,
  2216,  2219,  2221,  2224,  2227,  2229,  2230,  2231,  2232,  2233,
  2235,  2237,  2238,  2239,  2241,  2242,  2248,  2254,  2259,  2264,
  2270,  2275,  2278,  2285,  2287,  2289,  2292,  2295,  2327,  2328,
  2354,  2354,  2360,  2363,  2372,  2380,  2389,  2389,  2391,  2410,
  2412,  2413,  2416,  2420,  2424,  2428,  2428,  2434,  2435,  2437,
  2440,  2441,  2444,  2447,  2449,  2451,  2451,  2455,  2465,  2467,
  2470,  2481,  2489,  2498,  2508,  2517,  2526,  2536,  2537,  2539,
  2541,  2542,  2543,  2544,  2545,  2546,  2547,  2548,  2549,  2550,
  2551,  2552,  2553,  2554,  2555,  2557,  2560,  2560,  2567,  2569,
  2572,  2576,  2582,  2592,  2593,  2616,  2617,  2625,  2651,  2656,
  2688,  2689,  2702,  2704,  2707,  2709,  2712,  2714,  2717,  2717,
  2722,  2724,  2727,  2736,  2744,  2755,  2786,  2788,  2791,  2794,
  2794,  2800,  2802,  2805,  2825,  2844,  2876,  2878,  2881,  2883,
  2885,  2915,  2943,  2944,  2957,  2960,  2972,  2984,  2986,  2989,
  2998,  3008,  3012,  3015,  3026,  3035,  3037,  3040,  3048,  3050,
  3053,  3056,  3059,  3072,  3083,  3084,  3094,  3095,  3124,  3129,
  3144,  3146,  3161,  3162,  3165,  3167,  3168,  3169,  3170,  3171,
  3172,  3175,  3188,  3203,  3218,  3218,  3223,  3225,  3228,  3237,
  3245,  3256,  3257,  3265,  3270,  3271,  3301,  3303,  3306,  3312,
  3315,  3335,  3354,  3373,  3388,  3394,  3395,  3403,  3404,  3428,
  3430,  3433,  3441,  3443,  3448,  3451,  3453,  3454,  3456,  3459,
  3461,  3464,  3467,  3469,  3472,  3473,  3476,  3478,  3481,  3482,
  3484,  3487,  3490,  3498,  3498,  3499,  3516,  3517,  3532,  3537,
  3540,  3542,  3543,  3544,  3545,  3546,  3548,  3550,  3552,  3553,
  3554,  3556,  3558,  3560,  3562,  3564,  3566,  3568,  3570,  3572,
  3572,  3577,  3579,  3582,  3593,  3595,  3598,  3601,  3603,  3606,
  3617,  3625,  3634,  3641,  3643,  3657,  3658,  3659,  3673,  3688,
  3689,  3690,  3691,  3709,  3710,  3717,  3720,  3724,  3727,  3728,
  3729,  3730,  3731,  3733,  3735,  3736,  3737,  3752,  3767,  3783,
  3785,  3786,  3787,  3788,  3789,  3791,  3794,  3797,  3807,  3816,
  3826,  3836,  3885,  3890,  3894,  3898,  3903,  3903,  3906,  3906,
  3916,  3929,  3944,  3948,  3951,  3972,  3979,  3980,  3985,  3986,
  3999,  4002,  4005,  4015,  4024,  4033,  4042,  4051,  4060,  4060,
  4069,  4078,  4080,  4082,  4084,  4093,  4102,  4111,  4120,  4129,
  4138,  4147,  4156,  4158,  4158,  4160,  4160,  4170,  4170,  4180,
  4180,  4190,  4199,  4201,  4203,  4212,  4221,  4231,  4241,  4251,
  4261,  4270,  4272,  4272,  4276,  4289,  4303,  4320,  4337,  4354,
  4356,  4358,  4380,  4402,  4424,  4446,  4468,  4490,  4512,  4534,
  4555,  4556,  4556,  4568,  4568,  4580,  4580,  4593,  4599,  4604,
  4609,  4615,  4617,  4620,  4631,  4639,  4648,  4650,  4651,  4652,
  4653,  4655,  4666,  4667,  4671,  4673,  4674,  4675,  4676,  4677,
  4679,  4681,  4682,  4683,  4685,  4687,  4688,  4689,  4691,  4693,
  4695,  4703,  4704,  4707,  4713,  4719,  4724,  4729,  4736,  4742,
  4745,  4746,  4748,  4749,  4759,  4770,  4774,  4778,  4783,  4786,
  4787,  4790,  4791,  4796,  4800,  4811,  4822,  4832,  4833,  4834,
  4843,  4844,  4845,  4854,  4864,  4866,  4869,  4872,  4881,  4891,
  4901,  4917,  4928,  4945,  4946,  4949,  4949,  4954,  4956,  4957,
  4960,  4966,  4966,  4969,  4972,  4975,  4989,  4991,  4994,  5004,
  5006,  5006,  5008,  5010,  5012,  5021,  5023,  5025,  5027,  5029,
  5031,  5033,  5035,  5037,  5039,  5041,  5043,  5045,  5047,  5049,
  5051,  5053,  5055,  5058,  5061,  5063,  5066,  5069,  5071,  5073,
  5076,  5079,  5081,  5084,  5087,  5089,  5092,  5095,  5098,  5101,
  5104,  5107,  5110,  5113,  5116,  5118,  5122,  5125,  5128,  5131,
  5133,  5136,  5139,  5142,  5145,  5148,  5158,  5158,  5159,  5168,
  5169,  5184,  5187,  5190,  5192,  5198,  5199,  5205,  5206,  5212,
  5212,  5218,  5221,  5221,  5227,  5233,  5236,  5239,  5242,  5245,
  5247,  5253,  5254,  5261,  5264,  5267,  5270,  5271,  5274,  5275,
  5278,  5279,  5288,  5288,  5297,  5297,  5308,  5309,  5310,  5312,
  5314,  5317,  5319,  5322,  5324,  5325,  5326,  5327,  5328,  5329,
  5331,  5333,  5335,  5336,  5337,  5338,  5339,  5340,  5341,  5342,
  5343,  5344,  5345,  5346,  5348,  5349,  5351,  5358,  5360,  5369,
  5372,  5374,  5375,  5376,  5377,  5378,  5379,  5380,  5381,  5384,
  5389,  5396,  5399,  5402,  5404,  5412,  5413,  5421,  5422,  5431,
  5432,  5440,  5441,  5449,  5450,  5458,  5459,  5467,  5468,  5477,
  5478,  5483,  5483,  5489,  5494,  5499,  5506,  5510,  5516,  5516,
  5522,  5530,  5533,  5538,  5545,  5552,  5558,  5561,  5564,  5567,
  5570,  5572,  5573,  5577,  5588,  5600,  5603,  5604,  5606,  5609,
  5612,  5627,  5640,  5653,  5667,  5669,  5672,  5685,  5687,  5690,
  5698,  5701,  5703,  5706,  5710,  5714,  5716,  5719,  5722,  5725,
  5729,  5733,  5736,  5739,  5752,  5754,  5757,  5765,  5768,  5771,
  5774,  5778,  5782,  5784,  5787,  5791,  5794,  5798,  5804,  5824,
  5844,  5864,  5890,  5916,  5942,  5943,  5945
};

static const char * const yytname[] = {   "$","error","$undefined.","K_HISTORY",
"K_ABUT","K_ABUTMENT","K_ACTIVE","K_ANALOG","K_ARRAY","K_AREA","K_BLOCK","K_BOTTOMLEFT",
"K_BOTTOMRIGHT","K_BY","K_CAPACITANCE","K_CAPMULTIPLIER","K_CLASS","K_CLOCK",
"K_CLOCKTYPE","K_COLUMNMAJOR","K_DESIGNRULEWIDTH","K_INFLUENCE","K_CORE","K_CORNER",
"K_COVER","K_CPERSQDIST","K_CURRENT","K_CURRENTSOURCE","K_CUT","K_DEFAULT","K_DATABASE",
"K_DATA","K_DIELECTRIC","K_DIRECTION","K_DO","K_EDGECAPACITANCE","K_EEQ","K_END",
"K_ENDCAP","K_FALL","K_FALLCS","K_FALLT0","K_FALLSATT1","K_FALLRS","K_FALLSATCUR",
"K_FALLTHRESH","K_FEEDTHRU","K_FIXED","K_FOREIGN","K_FROMPIN","K_GENERATE","K_GENERATOR",
"K_GROUND","K_HEIGHT","K_HORIZONTAL","K_INOUT","K_INPUT","K_INPUTNOISEMARGIN",
"K_COMPONENTPIN","K_INTRINSIC","K_INVERT","K_IRDROP","K_ITERATE","K_IV_TABLES",
"K_LAYER","K_LEAKAGE","K_LEQ","K_LIBRARY","K_MACRO","K_MATCH","K_MAXDELAY","K_MAXLOAD",
"K_METALOVERHANG","K_MILLIAMPS","K_MILLIWATTS","K_MINFEATURE","K_MUSTJOIN","K_NAMESCASESENSITIVE",
"K_NANOSECONDS","K_NETS","K_NEW","K_NONDEFAULTRULE","K_NONINVERT","K_NONUNATE",
"K_OBS","K_OHMS","K_OFFSET","K_ORIENTATION","K_ORIGIN","K_OUTPUT","K_OUTPUTNOISEMARGIN",
"K_OUTPUTRESISTANCE","K_OVERHANG","K_OVERLAP","K_OFF","K_ON","K_OVERLAPS","K_PAD",
"K_PATH","K_PATTERN","K_PICOFARADS","K_PIN","K_PITCH","K_PLACED","K_POLYGON",
"K_PORT","K_POST","K_POWER","K_PRE","K_PULLDOWNRES","K_RECT","K_RESISTANCE",
"K_RESISTIVE","K_RING","K_RISE","K_RISECS","K_RISERS","K_RISESATCUR","K_RISETHRESH",
"K_RISESATT1","K_RISET0","K_RISEVOLTAGETHRESHOLD","K_FALLVOLTAGETHRESHOLD","K_ROUTING",
"K_ROWMAJOR","K_RPERSQ","K_SAMENET","K_SCANUSE","K_SHAPE","K_SHRINKAGE","K_SIGNAL",
"K_SITE","K_SIZE","K_SOURCE","K_SPACER","K_SPACING","K_SPECIALNETS","K_STACK",
"K_START","K_STEP","K_STOP","K_STRUCTURE","K_SYMMETRY","K_TABLE","K_THICKNESS",
"K_TIEHIGH","K_TIELOW","K_TIEOFFR","K_TIME","K_TIMING","K_TO","K_TOPIN","K_TOPLEFT",
"K_TOPRIGHT","K_TOPOFSTACKONLY","K_TRISTATE","K_TYPE","K_UNATENESS","K_UNITS",
"K_USE","K_VARIABLE","K_VERTICAL","K_VHI","K_VIA","K_VIARULE","K_VLO","K_VOLTAGE",
"K_VOLTS","K_WIDTH","K_X","K_Y","K_R90","T_STRING","QSTRING","NUMBER","K_N",
"K_S","K_E","K_W","K_FN","K_FS","K_FE","K_FW","K_R0","K_R180","K_R270","K_MX",
"K_MY","K_MXR90","K_MYR90","K_USER","K_MASTERSLICE","K_ENDMACRO","K_ENDMACROPIN",
"K_ENDVIARULE","K_ENDVIA","K_ENDLAYER","K_ENDSITE","K_CANPLACE","K_CANNOTOCCUPY",
"K_TRACKS","K_FLOORPLAN","K_GCELLGRID","K_DEFAULTCAP","K_MINPINS","K_WIRECAP",
"K_STABLE","K_SETUP","K_HOLD","K_DEFINE","K_DEFINES","K_DEFINEB","K_IF","K_THEN",
"K_ELSE","K_FALSE","K_TRUE","K_EQ","K_NE","K_LE","K_LT","K_GE","K_GT","K_OR",
"K_AND","K_NOT","K_DELAY","K_TABLEDIMENSION","K_TABLEAXIS","K_TABLEENTRIES",
"K_TRANSITIONTIME","K_EXTENSION","K_PROPDEF","K_STRING","K_INTEGER","K_REAL",
"K_RANGE","K_PROPERTY","K_VIRTUAL","K_BUSBITCHARS","K_VERSION","K_BEGINEXT",
"K_ENDEXT","K_UNIVERSALNOISEMARGIN","K_EDGERATETHRESHOLD1","K_CORRECTIONTABLE",
"K_EDGERATESCALEFACTOR","K_EDGERATETHRESHOLD2","K_VICTIMNOISE","K_NOISETABLE",
"K_EDGERATE","K_VICTIMLENGTH","K_CORRECTIONFACTOR","K_OUTPUTPINANTENNASIZE",
"K_INPUTPINANTENNASIZE","K_INOUTPINANTENNASIZE","K_CURRENTDEN","K_PWL","K_ANTENNALENGTHFACTOR",
"K_TAPERRULE","K_DIVIDERCHAR","K_ANTENNASIZE","K_ANTENNAMETALLENGTH","K_ANTENNAMETALAREA",
"K_RISESLEWLIMIT","K_FALLSLEWLIMIT","K_FUNCTION","K_BUFFER","K_INVERTER","K_NAMEMAPSTRING",
"K_NOWIREEXTENSIONATPIN","K_WIREEXTENSION","K_MESSAGE","K_CREATEFILE","K_OPENFILE",
"K_CLOSEFILE","K_WARNING","K_ERROR","K_FATALERROR","K_RECOVERY","K_SKEW","K_ANYEDGE",
"K_POSEDGE","K_NEGEDGE","K_SDFCONDSTART","K_SDFCONDEND","K_SDFCOND","K_MPWH",
"K_MPWL","K_PERIOD","K_ACCURRENTDENSITY","K_DCCURRENTDENSITY","K_AVERAGE","K_PEAK",
"K_RMS","K_FREQUENCY","K_CUTAREA","K_MEGAHERTZ","K_USELENGTHTHRESHOLD","K_LENGTHTHRESHOLD",
"K_ANTENNAINPUTGATEAREA","K_ANTENNAINOUTDIFFAREA","K_ANTENNAOUTPUTDIFFAREA",
"K_ANTENNAAREARATIO","K_ANTENNADIFFAREARATIO","K_ANTENNACUMAREARATIO","K_ANTENNACUMDIFFAREARATIO",
"K_ANTENNAAREAFACTOR","K_ANTENNASIDEAREARATIO","K_ANTENNADIFFSIDEAREARATIO",
"K_ANTENNACUMSIDEAREARATIO","K_ANTENNACUMDIFFSIDEAREARATIO","K_ANTENNASIDEAREAFACTOR",
"K_DIFFUSEONLY","K_MANUFACTURINGGRID","K_ANTENNACELL","K_CLEARANCEMEASURE","K_EUCLIDEAN",
"K_MAXXY","K_USEMINSPACING","K_ROWMINSPACING","K_ROWABUTSPACING","K_FLIP","K_NONE",
"K_ANTENNAPARTIALMETALAREA","K_ANTENNAPARTIALMETALSIDEAREA","K_ANTENNAGATEAREA",
"K_ANTENNADIFFAREA","K_ANTENNAMAXAREACAR","K_ANTENNAMAXSIDEAREACAR","K_ANTENNAPARTIALCUTAREA",
"K_ANTENNAMAXCUTCAR","K_SLOTWIREWIDTH","K_SLOTWIRELENGTH","K_SLOTWIDTH","K_SLOTLENGTH",
"K_MAXADJACENTSLOTSPACING","K_MAXCOAXIALSLOTSPACING","K_MAXEDGESLOTSPACING",
"K_SPLITWIREWIDTH","K_MINIMUMDENSITY","K_MAXIMUMDENSITY","K_DENSITYCHECKWINDOW",
"K_DENSITYCHECKSTEP","K_FILLACTIVESPACING","K_MINIMUMCUT","K_ADJACENTCUTS","K_ANTENNAMODEL",
"K_BUMP","K_ENCLOSURE","K_FROMABOVE","K_FROMBELOW","K_IMPLANT","K_LENGTH","K_MAXVIASTACK",
"K_AREAIO","K_BLACKBOX","K_MAXWIDTH","K_MINENCLOSEDAREA","K_MINSTEP","K_ORIENT",
"K_OXIDE1","K_OXIDE2","K_OXIDE3","K_OXIDE4","K_PARALLELRUNLENGTH","K_MINWIDTH",
"K_PROTRUSIONWIDTH","K_SPACINGTABLE","K_WITHIN","K_ABOVE","K_BELOW","K_CENTERTOCENTER",
"K_CUTSIZE","K_CUTSPACING","K_DENSITY","K_DIAG45","K_DIAG135","K_DIAGMINEDGELENGTH",
"K_DIAGSPACING","K_DIAGPITCH","K_DIAGWIDTH","K_GENERATED","K_GROUNDSENSITIVITY",
"K_HARDSPACING","K_INSIDECORNER","K_LAYERS","K_LENGTHSUM","K_MICRONS","K_MINCUTS",
"K_MINSIZE","K_NETEXPR","K_OUTSIDECORNER","K_PREFERENCLOSURE","K_ROWCOL","K_ROWPATTERN",
"K_SOFT","K_SUPPLYSENSITIVITY","K_USEVIA","K_USEVIARULE","K_WELLTAP","IF","LNOT",
"'-'","'+'","'*'","'/'","UMINUS","';'","'('","')'","'='","'\\n'","'<'","'>'",
"lef_file","version","@1","dividerchar","busbitchars","rules","end_library",
"rule","case_sensitivity","wireextension","manufacturing","useminspacing","clearancemeasure",
"clearance_type","spacing_type","spacing_value","units_section","start_units",
"units_rules","units_rule","layer_rule","start_layer","@2","end_layer","@3",
"layer_options","layer_option","@4","@5","@6","@7","@8","@9","@10","@11","@12",
"@13","@14","@15","@16","@17","@18","@19","@20","@21","@22","@23","@24","sp_options",
"@25","@26","@27","@28","@29","layer_enclosure_type_opt","layer_enclosure_width_opt",
"layer_preferenclosure_width_opt","layer_minimumcut_options","layer_minimumcut_option",
"layer_minstep_options","layer_minstep_option","layer_minstep_type","layer_antenna_pwl",
"@30","layer_diffusion_ratios","layer_diffusion_ratio","layer_antenna_duo","layer_table_type",
"layer_frequency","@31","@32","@33","ac_layer_table_opt","@34","@35","dc_layer_table",
"@36","number_list","layer_prop_list","layer_prop","current_density_pwl_list",
"current_density_pwl","cap_points","cap_point","res_points","res_point","layer_type",
"layer_direction","layer_minen_width","layer_oxide","layer_sp_parallel_widths",
"layer_sp_parallel_width","@37","layer_sp_influence_widths","layer_sp_influence_width",
"maxstack_via","@38","via","@39","via_keyword","start_via","via_viarule","@40",
"@41","@42","via_viarule_options","via_viarule_option","@43","via_option","via_other_options",
"via_more_options","via_other_option","@44","via_prop_list","via_name_value_pair",
"via_foreign","start_foreign","@45","orientation","via_layer_rule","via_layer",
"@46","via_geometries","via_geometry","@47","end_via","@48","viarule_keyword",
"@49","viarule","viarule_generate","@50","viarule_generate_default","viarule_layer_list",
"opt_viarule_props","viarule_props","viarule_prop","@51","viarule_prop_list",
"viarule_layer","via_names","via_name","viarule_layer_name","@52","viarule_layer_options",
"viarule_layer_option","end_viarule","@53","spacing_rule","start_spacing","end_spacing",
"spacings","spacing","samenet_keyword","irdrop","start_irdrop","end_irdrop",
"ir_tables","ir_table","ir_table_values","ir_table_value","ir_tablename","minfeature",
"dielectric","nondefault_rule","@54","@55","@56","end_nd_rule","nd_hardspacing",
"nd_rules","nd_rule","usevia","useviarule","mincuts","nd_prop","@57","nd_prop_list",
"nd_layer","@58","@59","@60","@61","nd_layer_stmts","nd_layer_stmt","site","start_site",
"@62","end_site","@63","site_options","site_option","site_class","site_symmetry_statement",
"site_symmetries","site_symmetry","site_rowpattern_statement","@64","site_rowpatterns",
"site_rowpattern","@65","pt","macro","@66","start_macro","@67","end_macro","@68",
"macro_options","macro_option","@69","macro_prop_list","macro_symmetry_statement",
"macro_symmetries","macro_symmetry","macro_name_value_pair","macro_class","class_type",
"pad_type","core_type","endcap_type","macro_generator","macro_generate","macro_source",
"macro_power","macro_origin","macro_foreign","macro_eeq","@70","macro_leq","@71",
"macro_site","macro_site_word","site_word","macro_size","macro_pin","start_macro_pin",
"@72","end_macro_pin","@73","macro_pin_options","macro_pin_option","@74","@75",
"@76","@77","@78","@79","@80","@81","@82","@83","pin_layer_oxide","pin_prop_list",
"pin_name_value_pair","electrical_direction","start_macro_port","macro_port_class_option",
"macro_pin_use","macro_scan_use","pin_shape","geometries","geometry","@84","@85",
"geometry_options","layer_spacing","firstPt","nextPt","otherPts","via_placement",
"@86","@87","stepPattern","sitePattern","trackPattern","@88","@89","@90","@91",
"trackLayers","layer_name","gcellPattern","macro_obs","start_macro_obs","macro_density",
"density_layers","density_layer","@92","@93","density_layer_rects","density_layer_rect",
"macro_clocktype","@94","timing","start_timing","end_timing","timing_options",
"timing_option","@95","@96","@97","one_pin_trigger","two_pin_trigger","from_pin_trigger",
"to_pin_trigger","delay_or_transition","list_of_table_entries","table_entry",
"list_of_table_axis_numbers","slew_spec","risefall","unateness","list_of_from_strings",
"list_of_to_strings","array","@98","start_array","@99","end_array","@100","array_rules",
"array_rule","@101","@102","@103","@104","@105","floorplan_start","floorplan_list",
"floorplan_element","@106","@107","cap_list","one_cap","msg_statement","@108",
"create_file_statement","@109","def_statement","@110","@111","@112","dtrm","then",
"else","expression","b_expr","s_expr","relop","prop_def_section","@113","prop_stmts",
"prop_stmt","@114","@115","@116","@117","@118","@119","@120","@121","prop_define",
"@122","@123","opt_range_detail","@124","opt_range_second","opt_def_range","opt_def_value",
"opt_spacing_cut","@125","opt_layer_name","@126","req_layer_name","@127","universalnoisemargin",
"edgeratethreshold1","edgeratethreshold2","edgeratescalefactor","noisetable",
"@128","end_noisetable","noise_table_list","noise_table_entry","output_resistance_entry",
"@129","num_list","victim_list","victim","@130","vnoiselist","correctiontable",
"@131","end_correctiontable","correction_table_list","correction_table_item",
"output_list","@132","numo_list","corr_victim_list","corr_victim","@133","corr_list",
"input_antenna","output_antenna","inout_antenna","antenna_input","antenna_inout",
"antenna_output","extension_opt","extension",""
};
#endif

static const short yyr1[] = {     0,
   415,   417,   416,   418,   419,   420,   420,   420,   421,   421,
   422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
   422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
   422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
   422,   422,   422,   422,   422,   422,   422,   422,   423,   423,
   424,   424,   425,   426,   427,   428,   428,   429,   429,   430,
   430,   431,   432,   433,   433,   434,   434,   434,   434,   434,
   434,   434,   434,   435,   437,   436,   439,   438,   440,   440,
   441,   441,   441,   441,   441,   441,   441,   441,   441,   441,
   441,   442,   441,   441,   441,   441,   441,   441,   441,   441,
   441,   441,   441,   441,   441,   441,   441,   441,   443,   441,
   444,   441,   441,   441,   445,   446,   441,   447,   448,   441,
   441,   449,   441,   441,   450,   441,   451,   441,   441,   452,
   441,   441,   453,   441,   454,   441,   455,   441,   441,   441,
   441,   441,   441,   441,   441,   441,   441,   441,   441,   441,
   441,   441,   441,   456,   441,   457,   441,   458,   441,   441,
   459,   441,   460,   441,   461,   441,   441,   441,   462,   441,
   464,   465,   466,   467,   463,   468,   463,   469,   469,   469,
   470,   470,   471,   471,   472,   472,   473,   473,   473,   474,
   474,   475,   475,   476,   476,   476,   477,   478,   477,   479,
   479,   480,   481,   481,   482,   482,   482,   484,   485,   486,
   483,   487,   488,   487,   489,   487,   491,   490,   492,   492,
   493,   493,   494,   494,   494,   495,   495,   496,   497,   497,
   498,   499,   499,   500,   501,   501,   501,   501,   501,   501,
   502,   502,   502,   502,   503,   503,   504,   504,   504,   504,
   505,   505,   507,   506,   508,   508,   509,   510,   511,   510,
   513,   512,   514,   515,   515,   515,   517,   518,   519,   516,
   520,   520,   521,   521,   521,   522,   521,   523,   523,   524,
   525,   525,   526,   526,   526,   527,   526,   526,   528,   528,
   529,   529,   529,   530,   530,   530,   530,   532,   531,   533,
   533,   533,   533,   533,   533,   533,   533,   533,   533,   533,
   533,   533,   533,   533,   533,   534,   536,   535,   537,   537,
   538,   539,   538,   541,   540,   543,   542,   544,   546,   545,
   547,   547,   548,   548,   549,   549,   550,   550,   552,   551,
   553,   553,   551,   551,   551,   554,   555,   555,   556,   558,
   557,   559,   559,   560,   560,   560,   560,   560,   560,   560,
   560,   560,   562,   561,   563,   564,   565,   566,   566,   567,
   567,   568,   569,   570,   571,   572,   572,   573,   574,   574,
   575,   576,   577,   578,   580,   581,   582,   579,   583,   583,
   584,   584,   585,   585,   586,   586,   586,   586,   586,   586,
   586,   587,   588,   589,   591,   590,   592,   592,   590,   590,
   590,   594,   595,   596,   597,   593,   598,   598,   599,   599,
   599,   599,   599,   599,   600,   602,   601,   604,   603,   605,
   605,   606,   606,   606,   606,   607,   607,   607,   608,   609,
   609,   610,   610,   610,   612,   611,   613,   613,   615,   614,
   616,   616,   618,   617,   620,   619,   622,   621,   623,   623,
   624,   624,   624,   624,   624,   624,   624,   624,   624,   624,
   624,   624,   624,   624,   624,   624,   624,   624,   624,   625,
   624,   626,   626,   627,   628,   628,   629,   629,   629,   630,
   630,   630,   631,   632,   632,   632,   632,   632,   632,   632,
   632,   632,   632,   632,   632,   632,   632,   633,   633,   633,
   633,   633,   633,   634,   634,   634,   634,   634,   634,   635,
   635,   635,   635,   635,   635,   636,   637,   638,   638,   638,
   639,   640,   641,   641,   641,   641,   643,   642,   645,   644,
   646,   646,   647,   648,   649,   650,   652,   651,   654,   653,
   655,   655,   656,   656,   656,   656,   656,   656,   657,   656,
   656,   656,   656,   656,   656,   656,   656,   656,   656,   656,
   656,   656,   656,   658,   656,   659,   656,   660,   656,   661,
   656,   656,   656,   656,   656,   656,   656,   656,   656,   656,
   656,   656,   662,   656,   656,   656,   656,   656,   656,   656,
   656,   656,   656,   656,   656,   656,   656,   656,   656,   663,
   656,   664,   656,   665,   656,   666,   656,   667,   667,   667,
   667,   668,   668,   669,   669,   669,   670,   670,   670,   670,
   670,   671,   672,   672,   673,   673,   673,   673,   673,   673,
   674,   674,   674,   674,   675,   675,   675,   675,   676,   678,
   679,   677,   677,   677,   677,   677,   677,   677,   677,   677,
   680,   680,   681,   681,   681,   682,   683,   684,   684,   686,
   685,   687,   685,   688,   689,   689,   691,   692,   690,   693,
   694,   690,   690,   690,   695,   695,   696,   697,   697,   698,
   698,   699,   700,   701,   701,   703,   704,   702,   705,   705,
   706,   708,   707,   709,   710,   711,   712,   712,   714,   713,
   715,   713,   716,   713,   713,   713,   713,   713,   713,   713,
   713,   713,   713,   713,   713,   713,   713,   713,   713,   713,
   713,   713,   713,   717,   717,   717,   718,   718,   718,   718,
   719,   719,   719,   720,   720,   720,   721,   721,   722,   722,
   723,   724,   724,   725,   725,   725,   726,   726,   727,   727,
   727,   728,   728,   729,   729,   731,   730,   733,   732,   735,
   734,   736,   736,   738,   737,   739,   737,   740,   737,   741,
   737,   737,   742,   737,   737,   737,   743,   744,   744,   746,
   745,   747,   745,   748,   748,   749,   751,   750,   753,   752,
   755,   754,   756,   754,   757,   754,   758,   758,   758,   759,
   759,   760,   760,   761,   761,   761,   761,   761,   761,   761,
   761,   762,   762,   762,   762,   762,   762,   762,   762,   762,
   762,   762,   762,   762,   762,   762,   763,   763,   763,   763,
   764,   764,   764,   764,   764,   764,   764,   764,   764,   766,
   765,   767,   767,   769,   768,   770,   768,   771,   768,   772,
   768,   773,   768,   774,   768,   775,   768,   776,   768,   778,
   777,   779,   777,   777,   777,   777,   780,   781,   780,   780,
   780,   782,   782,   782,   782,   782,   783,   783,   784,   784,
   785,   786,   785,   785,   785,   787,   788,   787,   790,   789,
   791,   792,   793,   794,   796,   795,   797,   798,   798,   799,
   799,   801,   800,   802,   802,   803,   803,   805,   804,   806,
   806,   808,   807,   809,   810,   810,   811,   811,   813,   812,
   814,   814,   815,   815,   817,   816,   818,   818,   819,   820,
   821,   822,   823,   824,   825,   825,   826
};

static const short yyr2[] = {     0,
     3,     0,     4,     3,     3,     0,     2,     1,     0,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
     3,     3,     3,     4,     3,     1,     1,     1,     1,     1,
     1,     4,     1,     0,     2,     4,     4,     4,     4,     4,
     4,     4,     4,     3,     0,     3,     0,     3,     0,     2,
     3,     3,     4,     3,     4,     3,     4,     3,     3,     3,
     3,     0,     6,     3,     4,     7,     4,     7,     3,     3,
     3,     3,     3,     3,     3,     3,     6,     6,     0,     4,
     0,     4,     4,     4,     0,     0,     9,     0,     0,     9,
     3,     0,     4,     3,     0,     4,     0,     5,     3,     0,
     4,     3,     0,     4,     0,     5,     0,     4,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     4,     3,
     3,     3,     3,     0,     5,     0,     7,     0,     5,     7,
     0,     4,     0,     7,     0,     7,     3,     3,     0,     5,
     0,     0,     0,     0,    11,     0,     9,     0,     1,     1,
     0,     2,     0,     2,     0,     2,     1,     1,     4,     0,
     2,     1,     2,     1,     1,     1,     1,     0,     7,     0,
     2,     1,     0,     1,     1,     1,     1,     0,     0,     0,
    12,     0,     0,     5,     0,     5,     0,     5,     0,     2,
     1,     2,     2,     2,     2,     1,     2,     4,     1,     2,
     4,     1,     2,     4,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     0,     2,     1,     1,     1,     1,
     0,     2,     0,     4,     0,     2,     6,     3,     0,     7,
     0,     4,     1,     2,     3,     3,     0,     0,     0,    26,
     0,     2,     4,     4,     6,     0,     4,     1,     1,     2,
     0,     2,     1,     1,     3,     0,     4,     1,     1,     2,
     2,     2,     2,     2,     3,     4,     3,     0,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     2,     0,     4,     0,     2,
     4,     0,     7,     0,     3,     0,     3,     5,     0,     7,
     0,     1,     1,     2,     0,     1,     1,     2,     0,     4,
     1,     2,     2,     2,     2,     2,     0,     2,     3,     0,
     4,     0,     2,     3,     3,     4,     5,     4,     5,     3,
     3,     3,     0,     3,     3,     1,     2,     0,     2,     5,
     6,     1,     3,     1,     2,     0,     2,     3,     0,     2,
     2,     2,     4,     3,     0,     0,     0,     8,     1,     2,
     0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
     1,     3,     3,     4,     0,     4,     1,     2,     2,     2,
     2,     0,     0,     0,     0,    12,     0,     2,     3,     3,
     4,     4,     3,     3,     3,     0,     3,     0,     3,     0,
     2,     5,     1,     1,     1,     3,     3,     3,     3,     0,
     2,     1,     1,     1,     0,     4,     0,     2,     0,     3,
     2,     4,     0,     4,     0,     3,     0,     3,     0,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     3,     3,     1,     1,     1,     1,     0,
     4,     1,     2,     3,     0,     2,     1,     1,     1,     2,
     2,     2,     3,     1,     2,     1,     1,     2,     2,     1,
     1,     1,     2,     1,     1,     2,     2,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     3,     4,     3,     3,     3,
     3,     3,     2,     3,     4,     3,     0,     4,     0,     4,
     3,     3,     1,     1,     5,     3,     0,     3,     0,     3,
     0,     2,     2,     3,     4,     3,     4,     5,     0,     4,
     3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     0,     4,     0,     5,     0,     5,     0,
     5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     4,     3,     0,     4,     4,     2,     4,     4,     4,     3,
     3,     4,     4,     4,     4,     4,     4,     4,     4,     0,
     4,     0,     4,     0,     4,     0,     4,     1,     1,     1,
     1,     1,     2,     2,     2,     2,     3,     3,     4,     3,
     3,     1,     0,     3,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     0,     1,     1,     1,     2,     0,
     0,     6,     3,     4,     6,     4,     6,     7,     9,     1,
     0,     2,     0,     2,     2,     1,     1,     0,     2,     0,
     5,     0,     7,     7,    11,     4,     0,     0,    10,     0,
     0,    10,     6,     6,     0,     2,     1,     6,     6,     3,
     2,     1,     4,     0,     2,     0,     0,     7,     0,     2,
     5,     0,     4,     3,     1,     2,     0,     2,     0,     4,
     0,     4,     0,    10,     9,     3,     3,     4,     4,     4,
     4,     4,     4,     4,     4,     3,     7,     8,     6,     3,
     3,     3,     2,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
     5,     1,     2,     0,     4,     7,     1,     1,     1,     1,
     1,     1,     2,     1,     2,     0,     4,     0,     3,     0,
     3,     0,     2,     0,     4,     0,     4,     0,     4,     0,
     4,     4,     0,     4,     5,     1,     2,     0,     2,     0,
     4,     0,     4,     0,     2,     5,     0,     6,     0,     6,
     0,     6,     0,     6,     0,     6,     0,     1,     1,     1,
     2,     1,     2,     3,     3,     3,     3,     2,     3,     6,
     1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     2,     3,     6,     1,     1,     3,     3,     6,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
     5,     0,     2,     0,     5,     0,     5,     0,     5,     0,
     5,     0,     5,     0,     5,     0,     5,     0,     5,     0,
     4,     0,     4,     1,     2,     2,     0,     0,     5,     2,
     5,     0,     1,     2,     5,     3,     0,     3,     0,     1,
     0,     0,     3,     4,     1,     0,     0,     3,     0,     3,
     4,     3,     3,     3,     0,     7,     2,     1,     2,     3,
     1,     0,     5,     1,     2,     1,     2,     0,     7,     1,
     2,     0,     7,     2,     1,     2,     3,     1,     0,     5,
     1,     2,     1,     2,     0,     7,     1,     2,     3,     3,
     3,     3,     3,     3,     0,     1,     1
};

static const short yydefact[] = {     0,
     8,   945,   768,     0,   374,    75,   455,     0,     0,   385,
   426,   366,    63,   263,   326,   801,   803,   805,   850,     0,
     2,   947,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   797,   799,     0,     0,     0,     0,     0,
     0,     0,    11,    19,    12,     7,    13,    20,    44,    45,
    46,    14,    64,    15,    79,    47,    16,     0,   261,     0,
    17,    18,    22,   368,    25,   376,    24,    23,    30,    26,
   430,    27,   459,    28,   772,    21,    48,    29,    31,    32,
    33,    35,    34,    36,    37,    38,    39,    40,    41,    42,
    43,     9,   946,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   852,     0,     0,     0,
     0,     0,     0,     0,   905,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    57,    56,     0,
    58,    59,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     0,     0,     0,     0,     0,     0,   122,     0,   125,     0,
     0,   130,     0,   133,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   137,
   178,     0,     0,     0,     0,     0,   161,     0,     0,     0,
     0,   169,   178,     0,    79,   264,     0,   331,   350,   347,
   333,   352,     0,     0,     0,     0,   702,   537,   298,     0,
     0,   539,   692,     0,   547,     0,   543,     0,     0,   485,
   705,   480,     0,     0,     0,   453,   459,   465,   461,   462,
   463,   464,   467,   466,   468,   469,   470,   472,     0,   471,
   473,   551,   476,     0,   477,   478,   479,   707,   766,     0,
     1,   769,   384,    76,   456,     0,    50,    49,   386,   427,
   327,     0,     0,     0,     0,     5,     0,     0,   902,   922,
   904,   903,     0,   940,   939,   941,     4,    52,    51,     0,
     0,   942,   943,   944,    53,    55,    61,    60,     0,   259,
   258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    65,     0,     0,     0,   241,   242,   243,   244,     0,     0,
     0,     0,     0,     0,     0,     0,    92,     0,   236,   237,
   235,   238,   239,   240,     0,     0,     0,     0,     0,     0,
     0,     0,   206,   205,   207,   111,     0,     0,     0,     0,
     0,   127,     0,     0,     0,     0,   135,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   179,   180,     0,     0,   154,   158,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    77,    74,    80,
   265,   266,   317,     0,   288,   267,   286,   278,     0,   279,
   281,   283,     0,   284,   319,   332,   329,     0,   334,   335,
   346,     0,   372,   365,   369,     0,     0,     0,   373,   377,
   379,     0,   428,     0,   440,   445,   425,   431,   434,   433,
   435,   497,   504,   505,   494,     0,   501,   496,   502,   500,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   696,   694,   309,   300,   302,   303,   301,   304,   306,   307,
   305,   308,   310,   311,   314,   312,   315,   313,   533,     0,
     0,     0,   460,     0,     0,     0,   691,   650,     0,     0,
     0,     0,     0,     0,   661,   660,     0,   544,   776,   778,
   780,     0,   783,     0,   774,     0,   773,   788,   786,    10,
   383,   391,     0,     0,     0,     0,   856,   866,   854,   860,
   868,   858,   862,   864,   853,     3,   901,     0,     0,     0,
     0,    54,     0,     0,     0,     0,    62,     0,     0,     0,
     0,     0,    91,     0,     0,   103,    94,   104,    99,     0,
    86,     0,    82,     0,     0,   167,   102,   877,   101,    81,
    90,     0,     0,   221,   106,     0,     0,   105,   100,     0,
     0,     0,     0,     0,   121,   197,     0,     0,   124,     0,
   203,   129,     0,   132,     0,   203,   139,   140,   141,   142,
   143,   144,   145,   146,   147,   148,     0,   150,   151,     0,
   247,   248,   249,   250,     0,     0,   152,   245,   190,   153,
     0,     0,     0,     0,   168,    89,     0,    84,    88,   666,
   668,     0,     0,     0,     0,     0,     0,   324,   262,   280,
   294,     0,     0,   316,     0,     0,     0,   339,     0,     0,
   336,   337,   348,     0,     0,     0,     0,     0,     0,     0,
     0,   353,   367,     0,   375,   382,     0,     0,     0,     0,
     0,     0,     0,   447,   498,   499,   514,   517,   515,   516,
   518,   519,   506,   495,   524,   525,   521,   520,   522,   523,
   507,   510,   508,   509,   511,   512,   513,   503,   493,     0,
     0,   299,     0,   526,     0,   451,     0,   532,   548,   531,
     0,   530,   529,   528,   487,   488,   489,   484,   486,     0,
     0,   482,   474,   475,     0,     0,   536,   534,     0,   457,
   454,     0,   541,   542,     0,     0,     0,   549,     0,     0,
   580,     0,     0,   559,     0,     0,   574,   576,   578,   632,
     0,     0,     0,     0,     0,     0,     0,     0,   645,     0,
     0,     0,     0,   593,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   610,   616,
   612,   614,     0,   546,   552,   562,   633,     0,     0,   668,
     0,     0,     0,     0,     0,   670,     0,   690,   649,     0,
   758,     0,     0,     0,     0,   709,   757,     0,     0,     0,
     0,   711,     0,     0,   737,   738,     0,     0,     0,   739,
   740,     0,     0,     0,   734,   735,   736,   704,   708,     0,
     0,     0,     0,     0,     0,   787,     0,   794,     0,   770,
   767,     0,     0,   393,   821,     0,     0,     0,   807,   840,
     0,     0,   807,     0,   836,   835,     0,     0,     0,   807,
     0,   851,     0,     0,     0,     0,     0,     0,     0,     0,
   929,     0,     0,   925,   928,   912,     0,     0,   908,   911,
   807,   807,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    97,     0,    87,    83,    95,     0,     0,     0,   891,
   223,   224,   225,   110,   222,     0,     0,   226,     0,   113,
     0,   112,   118,   114,   115,     0,   123,   126,   204,     0,
   131,   134,     0,   149,   156,   138,   163,     0,     0,   196,
   194,     0,   195,     0,   190,   192,     0,     0,   171,   162,
    85,     0,   165,    78,     0,   285,     0,     0,     0,   289,
     0,   282,   297,   295,     0,   322,     0,   320,   335,   351,
   343,   344,   345,     0,     0,   363,   328,   338,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
   380,   437,   436,   438,   429,     0,   442,   443,   444,   439,
   441,     0,   703,   538,   527,   540,     0,     0,   492,   491,
   490,   481,   483,     0,   693,   695,   535,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   641,   642,   643,   644,     0,
   646,   648,   647,     0,     0,   636,   639,   640,   638,   637,
   635,     0,     0,     0,     0,     0,   896,   896,   896,     0,
     0,   896,   896,   896,   896,     0,     0,   896,     0,     0,
     0,     0,     0,     0,   553,     0,     0,   596,     0,   651,
   668,     0,     0,   667,     0,     0,     0,   672,     0,   653,
   662,   706,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   759,   760,   761,     0,     0,   752,     0,     0,
     0,   749,   733,     0,     0,     0,     0,   741,   742,   743,
     0,     0,   747,   748,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   790,   792,
   789,   392,   387,     0,   818,     0,     0,     0,     0,     0,
   808,   809,   802,     0,     0,     0,   804,     0,   832,     0,
     0,     0,   845,   846,   841,   842,   843,   844,     0,     0,
   847,   848,   849,     0,     0,     0,     0,     0,   806,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   807,   926,     0,     0,     0,   807,   909,
   798,   800,     0,    67,    70,    72,    69,    68,    66,    71,
    73,     0,     0,   229,     0,     0,   232,     0,   880,   892,
     0,   895,     0,     0,     0,   227,     0,   208,   219,   219,
     0,   128,   136,   185,   181,   246,   155,   193,   159,   191,
     0,     0,   219,   170,   669,   183,   318,     0,   293,   292,
   291,   287,   290,   325,   296,     0,     0,     0,   341,     0,
   349,     0,   354,   355,   362,   361,     0,   360,     0,     0,
     0,     0,   381,     0,     0,   446,   448,   452,   545,   697,
   458,   676,   582,   587,   588,   631,   630,   627,     0,   628,
   550,   569,   567,     0,     0,   565,     0,   583,   584,     0,
     0,     0,   561,   586,   585,   568,   566,   589,   590,   564,
   573,   572,   563,   571,   570,     0,     0,   622,   592,   897,
     0,     0,     0,   600,   601,     0,     0,     0,     0,   899,
     0,     0,     0,     0,   618,   619,   620,   621,     0,     0,
     0,     0,   556,     0,   554,     0,     0,     0,   663,     0,
   654,     0,     0,     0,   656,     0,     0,     0,     0,     0,
     0,   762,     0,     0,     0,     0,     0,   764,     0,   726,
     0,   753,   716,     0,   717,   750,   730,   731,   732,     0,
   744,   745,   746,     0,     0,     0,   777,   779,     0,     0,
   781,     0,     0,   784,     0,     0,   795,   775,   771,   782,
     0,     0,   412,     0,   405,     0,     0,     0,   396,   397,
     0,   394,   399,   400,   401,   398,   395,   810,     0,     0,
   819,   815,   814,   816,   817,     0,   838,   837,     0,   833,
   824,   823,   822,   828,   829,   831,   830,   827,   826,   825,
   874,   870,   872,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   931,     0,   927,   924,   923,   914,     0,   910,
   907,   906,   260,     0,     0,   230,     0,     0,   233,   878,
     0,     0,     0,    93,     0,   107,   108,   219,     0,     0,
   198,   187,   188,     0,     0,   185,     0,     0,     0,     0,
   172,     0,     0,     0,     0,     0,   330,   340,   342,   364,
   358,     0,     0,   356,     0,   370,   432,   449,     0,     0,
   629,     0,   591,   560,   575,     0,     0,   626,   625,   624,
   594,   623,     0,   597,   599,   598,   602,   603,   606,   605,
     0,   607,   608,   604,   609,   611,   617,   613,   615,   557,
     0,   555,   634,   595,     0,     0,     0,     0,     0,     0,
   668,     0,     0,   671,   721,   725,   723,   719,   763,   710,
   720,   718,   722,   724,   765,   712,     0,     0,     0,     0,
   713,     0,     0,     0,     0,     0,   785,     0,     0,     0,
     0,   409,   410,   411,     0,     0,     0,     0,   389,   388,
   811,     0,     0,     0,     0,     0,   875,   887,   887,   876,
   857,   867,   855,   861,   869,   859,   863,   865,   932,     0,
   915,     0,     0,    98,     0,    96,   882,     0,   893,     0,
   228,     0,   220,   119,   116,   200,     0,   157,   186,   182,
   164,   160,     0,     0,   184,   166,     0,     0,   321,   359,
   357,   371,   450,     0,   699,     0,   581,   577,   579,   898,
   900,   558,   665,   664,   652,     0,   655,   668,     0,   657,
     0,     0,     0,     0,     0,   754,     0,     0,     0,     0,
     0,     0,   791,   793,   413,   407,     0,     0,   402,   403,
   390,   812,     0,     0,     0,     0,     0,   889,   889,     0,
   930,   933,     0,   913,   916,   231,   234,     0,     0,   883,
   879,   881,   894,   209,     0,     0,     0,   200,   202,     0,
     0,     0,     0,   668,     0,   698,     0,     0,     0,   658,
   673,     0,   751,   729,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   406,   408,   404,   813,   820,   839,
   834,     0,   890,   871,   873,     0,   934,     0,   917,   884,
     0,   212,     0,   120,   117,   199,   201,   189,   176,   173,
     0,     0,     0,   700,     0,     0,     0,   727,     0,     0,
     0,     0,   677,   680,   688,   689,   796,     0,   888,   935,
   918,     0,   886,     0,     0,     0,   217,   255,   219,   268,
   323,     0,     0,     0,   659,   728,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   215,   213,     0,   219,   177,
   174,     0,   701,     0,     0,   755,     0,   715,   678,   681,
   414,     0,     0,   885,   219,   219,   210,     0,     0,   256,
   251,     0,   675,   674,     0,   714,   685,   685,   417,   937,
     0,   920,     0,     0,     0,   219,   218,     0,     0,   175,
   251,     0,     0,   679,   682,     0,   938,   936,   921,   919,
   216,   214,     0,     0,   253,   252,     0,   756,   687,   686,
     0,     0,   415,     0,     0,     0,     0,   418,   211,     0,
   219,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   254,     0,     0,   423,   416,     0,   419,   420,   424,   257,
     0,   422,   421,     0,     0,     0,     0,     0,     0,     0,
   269,   271,   270,     0,     0,   276,     0,   272,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   274,   277,   273,
     0,   275,     0,     0,     0
};

static const short yydefgoto[] = {  1913,
    43,   109,    44,    45,     2,   251,    46,    47,    48,    49,
    50,    51,   130,   133,   289,    52,    53,   135,   301,    54,
    55,    96,   379,   613,   194,   195,   548,   327,   561,  1210,
  1686,  1209,  1685,   339,   341,   571,   344,   346,   576,   362,
   598,  1214,   599,   371,  1215,  1226,   376,   604,  1223,  1614,
  1769,  1811,  1768,   365,  1468,  1473,  1465,  1466,   914,   915,
   916,   568,  1606,  1687,  1688,   900,   336,   892,  1458,  1732,
  1826,  1766,  1806,  1805,  1734,  1789,  1459,   553,   554,   887,
   888,  1193,  1194,  1196,  1197,   325,   309,   909,   595,  1830,
  1831,  1861,  1790,  1810,    56,   523,    57,   197,    58,    59,
   388,   616,  1792,  1892,  1893,  1898,  1901,   389,   390,   620,
   391,   617,   929,   930,   392,   225,   434,   470,   394,   395,
   614,   624,   938,  1236,   619,   931,    60,   103,    61,    62,
   625,   397,   200,   630,   631,   632,   944,  1240,   201,   400,
   633,   202,   398,   401,   642,   947,  1242,    63,    64,   404,
   203,   405,   406,    65,    66,   409,   204,   410,   647,   961,
   411,    67,    68,    69,   101,   502,  1391,  1570,   824,  1123,
  1392,  1393,  1394,  1395,  1396,  1565,  1657,  1397,  1561,  1714,
  1819,  1865,  1836,  1858,    70,    71,   102,   417,   651,   205,
   418,   419,   420,   653,   971,   421,   654,   972,  1257,  1623,
  1064,    72,   472,    73,    97,   711,   988,   226,   227,   448,
   701,   228,   447,   699,   702,   229,   431,   678,   663,   671,
   230,   231,   232,   233,   234,   235,   236,   433,   237,   437,
   238,   239,   495,   240,   241,   242,   441,   764,   997,   476,
   765,  1003,  1006,  1007,  1008,  1000,  1035,  1050,  1052,  1053,
  1051,  1319,  1297,  1298,   766,   767,  1059,  1032,  1020,  1024,
   484,   485,   768,  1329,   779,  1527,   611,  1225,   922,   486,
  1069,  1336,  1529,   475,  1111,  1780,  1817,  1781,  1818,  1834,
  1850,  1114,   243,   244,   245,   706,   452,   705,  1489,  1696,
  1625,   246,   432,   247,   248,   808,   487,   809,  1077,  1082,
  1646,   810,   811,  1101,  1364,  1105,  1091,  1092,  1089,  1707,
   812,  1086,  1343,  1349,    74,   496,    75,    94,   821,  1117,
   249,   497,   819,   813,   814,   815,   817,   498,   822,  1121,
  1381,  1382,  1115,  1377,    76,   122,    77,   123,    78,   104,
   105,   106,  1133,  1400,  1664,   839,   840,   841,  1154,    79,
   107,   265,   515,   845,   843,   848,   846,   849,   850,   844,
   847,  1425,  1578,  1579,   880,  1597,  1681,  1668,  1724,  1203,
  1452,  1301,  1503,  1311,  1511,    80,    81,    82,    83,    84,
   273,  1179,   858,   859,   860,  1176,  1439,  1674,  1675,  1784,
  1823,    85,   518,  1174,   853,   854,   855,  1171,  1434,  1671,
  1672,  1783,  1821,    86,    87,    88,    89,    90,    91,    92,
    93
};

static const short yypact[] = {   710,
-32768,   926,-32768,   -96,-32768,-32768,-32768,   -21,   210,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,     5,
-32768,-32768,    52,    88,   145,   178,   206,   250,   255,   267,
   270,   193,   473,-32768,-32768,   287,   307,   313,   331,   336,
   171,   340,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    53,-32768,-32768,    42,-32768,   192,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,    34,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   364,-32768,   242,   121,   370,   378,   402,   148,   174,
   407,   414,   422,   425,   435,   442,-32768,   208,   446,   447,
   216,   226,   238,   243,-32768,   273,   278,   296,   309,   317,
   323,   464,   569,   338,   350,   362,   368,-32768,-32768,   374,
-32768,-32768,   589,  -161,   181,   535,   763,   616,    49,   620,
   630,   634,   636,    26,   638,   642,   647,    15,   659,-32768,
  -103,   665,   669,   246,   557,   689,-32768,   695,-32768,   716,
   722,-32768,   723,-32768,   728,   730,   732,   735,   738,   740,
   743,   750,   751,   753,   755,   756,   757,   761,   762,-32768,
   367,   765,   766,   767,   769,   770,-32768,   771,   773,   778,
   786,-32768,   367,   900,    53,    -6,   749,   932,-32768,   898,
-32768,-32768,    23,    58,     8,   107,-32768,-32768,-32768,   796,
   805,-32768,-32768,   -54,-32768,   804,-32768,   807,   113,-32768,
-32768,-32768,   439,   915,   856,-32768,    34,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   812,-32768,
-32768,-32768,-32768,   679,-32768,-32768,-32768,-32768,   950,   918,
-32768,-32768,-32768,-32768,-32768,   584,-32768,-32768,-32768,-32768,
-32768,   582,   587,   591,   747,-32768,   592,   596,-32768,-32768,
-32768,-32768,   598,-32768,-32768,-32768,-32768,-32768,-32768,   594,
   602,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   600,-32768,
-32768,   895,   936,   611,   867,   954,   961,   978,   888,   785,
-32768,   650,   -85,   651,-32768,-32768,-32768,-32768,   652,   655,
   678,  -121,   -87,    12,   680,   699,-32768,   701,-32768,-32768,
-32768,-32768,-32768,-32768,   717,   718,   955,   720,   724,   957,
   726,   737,-32768,-32768,-32768,   958,    24,   739,    25,   746,
    25,-32768,   748,    25,   760,    25,-32768,   768,   774,   775,
   776,   777,   779,   780,   781,   782,   783,   972,   784,   787,
   987,   335,-32768,-32768,   983,   789,-32768,-32768,   798,   810,
     4,   799,   801,   -58,   806,   -54,  1000,-32768,-32768,-32768,
-32768,-32768,-32768,  1003,-32768,-32768,-32768,-32768,  1141,-32768,
-32768,-32768,   891,-32768,-32768,-32768,-32768,  1021,-32768,   321,
    22,  1059,-32768,-32768,-32768,  1032,  1144,  1040,-32768,-32768,
-32768,    80,-32768,  1039,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  -228,    13,-32768,   868,   977,   105,-32768,-32768,-32768,
   809,  1043,  1046,  1047,  1048,   813,  1050,  1049,  1051,   816,
  1057,   822,  1218,   824,   825,   827,  -106,  1064,   829,   831,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   838,
   491,  1201,-32768,   -40,   840,   426,-32768,-32768,   -32,   -31,
   -30,   -28,  1073,  1212,-32768,-32768,   687,-32768,-32768,-32768,
-32768,  1078,-32768,  1077,-32768,  1215,-32768,-32768,-32768,-32768,
-32768,   869,    -4,  -116,    64,  1022,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -47,   -14,  -116,
  -116,-32768,  1083,  1082,  1084,  1085,-32768,  1086,  1087,  1088,
  1089,  1092,-32768,   849,   858,-32768,-32768,-32768,-32768,   860,
-32768,   861,-32768,   862,   863,-32768,-32768,  -112,-32768,-32768,
-32768,   200,  -123,-32768,-32768,   864,  1097,-32768,-32768,   866,
   980,  1102,   870,  1105,-32768,-32768,   887,   889,-32768,   890,
   988,-32768,   894,-32768,   896,   988,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   897,-32768,-32768,  1129,
-32768,-32768,-32768,-32768,   899,  1132,-32768,  1140,   -70,-32768,
  1135,  1142,  1137,   904,-32768,-32768,   905,-32768,-32768,-32768,
-32768,  1143,  1146,  1147,   906,  1148,  1149,-32768,-32768,   711,
-32768,   907,   700,   228,   898,   908,   484,-32768,  1150,  1286,
   -63,-32768,-32768,    20,  1151,  1152,   -54,  1153,  1154,  1155,
  1156,-32768,-32768,  1159,-32768,-32768,   -38,   924,   925,   927,
  1162,  1323,   -78,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   929,
   930,-32768,   931,-32768,   933,-32768,  1166,-32768,-32768,-32768,
  1168,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   588,
  -114,-32768,-32768,-32768,  1172,   111,-32768,-32768,   937,-32768,
-32768,  1173,-32768,-32768,  1174,    84,   399,-32768,  1175,  1176,
-32768,  1179,  1178,-32768,  1180,  1181,-32768,-32768,-32768,-32768,
  1182,  1183,  1184,  1185,  1186,  1187,  1188,   396,   235,  1189,
   839,  1190,  1191,-32768,  1194,  1193,  1195,  1196,  1197,  1198,
  1199,  1200,  1202,  1203,  1204,  1205,  1206,  1207,-32768,-32768,
-32768,-32768,   -95,-32768,-32768,-32768,   233,  1210,   -54,-32768,
   -54,   -54,   -54,   -54,   -54,-32768,   938,-32768,   828,  1219,
-32768,  1209,  1211,  1213,  1214,-32768,-32768,  1216,  1217,  1220,
  1221,-32768,   397,  1177,-32768,-32768,  1222,   944,   967,-32768,
-32768,  1224,  1225,  1226,-32768,-32768,-32768,-32768,-32768,  1192,
   498,    -3,  1228,  1228,   579,-32768,   597,-32768,  1228,-32768,
-32768,   279,   981,-32768,-32768,    64,    -4,    -4,   207,-32768,
    64,  -116,    78,    64,-32768,-32768,    64,    64,   797,   360,
   830,-32768,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,
-32768,  1237,    36,-32768,-32768,-32768,  1238,   134,-32768,-32768,
    78,    78,  1241,   985,  1001,  1006,  1007,  1008,  1009,  1010,
  1012,-32768,  1013,-32768,-32768,-32768,  1014,  1247,  1250,   -23,
-32768,-32768,-32768,-32768,-32768,  1251,   420,-32768,   976,-32768,
  1252,-32768,-32768,-32768,-32768,   -54,-32768,-32768,-32768,  1019,
-32768,-32768,  1020,-32768,-32768,-32768,-32768,  1255,  1023,-32768,
-32768,  1256,-32768,  1024,   -70,-32768,  1265,  1260,-32768,-32768,
-32768,   -55,-32768,-32768,  1027,-32768,  1028,   626,  -100,-32768,
  1266,-32768,-32768,-32768,  1029,-32768,   -54,-32768,    47,-32768,
-32768,-32768,-32768,   -63,  1031,-32768,-32768,-32768,  1033,  1034,
  1035,  1036,   -54,  1037,  1427,  1296,  1273,  1274,  1275,-32768,
-32768,-32768,-32768,-32768,-32768,  1276,-32768,-32768,-32768,-32768,
-32768,   -75,-32768,-32768,-32768,-32768,  1041,  1044,-32768,-32768,
-32768,-32768,-32768,  1045,-32768,-32768,-32768,  1282,  1106,  1052,
  1053,  1054,  1055,  1056,  1058,  -107,  1283,  1060,  1061,  1284,
  1285,  1062,  1287,  1063,  1065,  1293,  1298,  1301,  1068,  1069,
  1070,  1071,  1072,  1074,  1075,-32768,-32768,-32768,-32768,  1076,
-32768,-32768,-32768,  1079,  1080,-32768,-32768,-32768,-32768,-32768,
-32768,  1081,  1090,  1091,  1295,  1093,  1392,  1392,  1392,  1094,
  1095,  1392,  1392,  1392,  1392,  1421,  1421,  1392,  1421,   358,
  1314,  1317,  1319,    -1,-32768,   916,   107,-32768,   828,-32768,
-32768,    19,   -54,-32768,   -54,   -54,  1096,-32768,  1320,-32768,
-32768,-32768,  1321,  1322,  1326,  1331,  1325,  1332,  1333,  1334,
  1335,  1338,-32768,-32768,-32768,  1103,  1339,-32768,    10,  1340,
   428,-32768,-32768,  1104,  1107,  1108,  1343,-32768,-32768,-32768,
   537,  1344,-32768,-32768,  1336,  1345,  1112,  1113,  1348,  1349,
  1116,  1351,  1352,  1119,    29,  1120,  1357,  1358,-32768,-32768,
-32768,-32768,    17,   328,-32768,   227,    -4,    -4,    -4,    -4,
-32768,-32768,-32768,   328,    62,  -116,-32768,   328,-32768,   421,
    40,   382,-32768,-32768,-32768,-32768,-32768,-32768,    -4,    -4,
-32768,-32768,-32768,    -4,    64,    64,    64,    64,-32768,  -116,
  -116,  -116,   293,   293,   293,   293,   293,   293,   293,   293,
  1359,  1123,  1249,  -287,-32768,  1360,  1124,  1288,  -287,-32768,
-32768,-32768,  1127,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  1362,   452,-32768,  1364,   457,-32768,  1365,  1304,-32768,
  1367,-32768,  1134,  1369,  1136,-32768,  1138,-32768,-32768,-32768,
   -54,-32768,-32768,   -76,  1377,-32768,-32768,-32768,-32768,-32768,
  1373,  1208,-32768,-32768,-32768,  1380,-32768,  1223,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   -54,   -54,  1286,-32768,  -130,
-32768,  1378,-32768,-32768,-32768,-32768,  1145,-32768,  1375,  1381,
  1157,  -101,-32768,  1158,  1106,-32768,-32768,-32768,-32768,-32768,
-32768,  1517,-32768,-32768,-32768,-32768,-32768,-32768,  1160,-32768,
-32768,-32768,-32768,  1382,  1161,-32768,  1163,-32768,-32768,  1164,
  1383,  1384,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   681,   -74,-32768,-32768,-32768,
  1165,  1167,  1170,-32768,-32768,  1171,  1227,  1239,  1240,-32768,
  1242,  1243,  1244,  1245,-32768,-32768,-32768,-32768,  1246,  1248,
  1253,  1254,-32768,   935,-32768,  1257,  1258,  1515,   240,    -5,
-32768,   -54,   -54,  1520,-32768,  1387,  1259,  1261,  1262,  1263,
  1264,-32768,   -73,  1267,  1268,  1269,  1270,-32768,   -57,-32768,
  1353,-32768,-32768,  1386,-32768,-32768,-32768,-32768,-32768,  1388,
-32768,-32768,-32768,  1337,  1389,   397,-32768,-32768,  1533,  1536,
-32768,  1540,  1542,-32768,  1379,  1406,-32768,-32768,-32768,-32768,
  1228,  1228,-32768,   685,-32768,  1409,  1411,  1412,-32768,-32768,
  1548,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1374,    -4,
-32768,   495,   495,-32768,-32768,  -116,-32768,-32768,    64,-32768,
   329,   329,   329,-32768,-32768,   705,   432,  1277,  1277,  1277,
  1413,-32768,-32768,  1416,  1271,  1272,  1278,  1279,  1280,  1281,
  1289,  1290,-32768,    31,-32768,-32768,-32768,-32768,    41,-32768,
-32768,-32768,-32768,  1415,  1291,-32768,  1417,  1292,-32768,-32768,
  1418,  1422,  1294,-32768,  1297,-32768,-32768,-32768,    43,    48,
-32768,-32768,-32768,  1419,  1300,   -76,  1423,  1302,  1303,  1424,
  1425,  1426,  1305,  1428,   -54,  1306,-32768,-32768,-32768,-32768,
-32768,  1307,  1308,-32768,  1309,-32768,-32768,-32768,  1480,  1429,
-32768,  1310,-32768,-32768,-32768,  1311,  1312,-32768,-32768,-32768,
-32768,-32768,  1432,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1433,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1313,-32768,-32768,-32768,  1434,  1435,  1315,  1436,  1316,   -54,
-32768,  1318,  1520,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  1437,  1438,  1439,  1440,
-32768,  1368,  1441,  1442,  1443,  1444,-32768,  1396,  1324,  1327,
  1447,-32768,-32768,-32768,   -46,  1446,  1328,  1329,  1449,-32768,
-32768,  -160,  -153,   289,   357,   301,-32768,  1370,  1370,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1356,
-32768,  1371,  1299,-32768,  1330,-32768,    14,  1450,-32768,  1451,
-32768,    50,-32768,-32768,-32768,   -54,  1361,-32768,-32768,-32768,
-32768,-32768,  1488,  1458,-32768,-32768,  1453,   -54,-32768,-32768,
-32768,-32768,-32768,   -54,-32768,  1615,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,  1616,-32768,-32768,    28,-32768,
  1341,    71,  1342,  1346,  1456,  1457,  1459,  1493,  1495,  1497,
  1498,  1464,-32768,-32768,-32768,-32768,  -125,  1347,-32768,-32768,
-32768,-32768,  1430,    -4,  -116,    64,  1465,  1466,  1466,  1467,
  1356,-32768,  1468,  1371,-32768,-32768,-32768,  1469,  1471,-32768,
-32768,-32768,-32768,-32768,  1420,  1420,  1350,   -54,-32768,  1472,
  1481,  1483,  1354,-32768,   -54,  1480,  1484,  1485,    -5,-32768,
-32768,  1355,-32768,-32768,  1486,  1490,  1508,  1499,  1500,  1509,
  1510,  1511,  1363,  1514,-32768,-32768,-32768,-32768,   329,  1277,
   346,  1516,-32768,-32768,-32768,  1366,-32768,  1372,-32768,  1455,
  1518,   -67,  1519,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1390,    39,  1521,-32768,  1555,  1557,  1376,-32768,  1385,  1527,
  1528,  1529,  1391,  1393,-32768,-32768,-32768,  1530,-32768,-32768,
-32768,  1531,-32768,  1532,  1538,  1496,-32768,-32768,-32768,-32768,
-32768,  1394,  1553,  1554,-32768,-32768,  1559,  1560,  1395,  1658,
  1665,  1397,  1479,  1491,  1564,-32768,-32768,  1567,-32768,  1574,
  1425,  1571,-32768,  1570,  1572,  1573,  1398,-32768,-32768,-32768,
-32768,  1576,  1577,-32768,-32768,-32768,-32768,    54,  1579,-32768,
  1580,  1584,-32768,-32768,  1583,-32768,-32768,-32768,-32768,-32768,
    56,-32768,    57,    60,    61,-32768,-32768,  1399,  1585,-32768,
  1580,  1586,  1587,  1592,  1592,     2,-32768,-32768,-32768,-32768,
-32768,-32768,    67,  1591,-32768,-32768,  1400,-32768,-32768,-32768,
  1720,  1593,-32768,  1641,  1595,  1596,  1598,-32768,-32768,  1638,
-32768,  1401,  1603,  1402,  1606,  1605,  1403,  1404,  1405,  1607,
  1425,  1608,  1407,-32768,-32768,  1408,-32768,-32768,-32768,-32768,
  1609,-32768,-32768,  1410,  1445,  1611,  1612,  1613,  1614,  1414,
-32768,-32768,   -48,  1617,  1618,-32768,  1621,-32768,  1622,  1623,
  1626,  1630,  1633,  1431,  1448,  1452,  1635,-32768,-32768,-32768,
  1454,-32768,  1789,  1790,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,  1619,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  1624,-32768,-32768,   334,-32768,   909,-32768,
-32768,   276,-32768,   131,-32768,  1460,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   135,-32768, -1197,-32768,  1461,-32768,
   939,-32768,   627,-32768,   629,-32768,-32768,-32768,-32768,    -8,
-32768,-32768,-32768,-32768,-32768,-32768,   704,-32768,  1463,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1462,-32768,-32768,   901,-32768,  -185,-32768,  -391,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,  1470,   892,-32768,  -617,-32768,-32768,  -194,-32768,
-32768,-32768,-32768,-32768,-32768,   590,-32768,   706,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768, -1409,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -214,-32768,-32768,-32768,-32768,-32768,-32768,  1610,-32768,-32768,
-32768,-32768,-32768,-32768,  1131,-32768,   788,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   536,-32768,-32768,-32768,-32768,-32768,-32768,
   790,  1067,-32768,-32768,-32768,-32768,  -472,  -767,  -766,-32768,
-32768,-32768, -1317,  -786,-32768,-32768,-32768,-32768,-32768,    16,
-32768,-32768,-32768,-32768,-32768,-32768,  1130,-32768,-32768,-32768,
   139,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   752,-32768,-32768,
   196,   474,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1599,-32768,
-32768,-32768,  -757,  -696,  -986,  -502,  -816,  -501,  -832,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   -26,-32768,-32768,-32768,-32768,-32768,   262,   173,-32768,
-32768,  -544,-32768,  -517,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   986,-32768,-32768,-32768,-32768,   176,-32768,
-32768,-32768,-32768,-32768,-32768,   994,-32768,-32768,-32768,-32768,
   180,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768
};


#define	YYLAST		2095


static const short yytable[] = {   440,
   829,   622,   833,  1062,  1065,   399,   770,   772,  1162,  1124,
   471,   393,  1460,   948,  1134,  1851,  1532,  1138,   861,   862,
  1139,  1141,   381,   412,   602,  1471,  1107,  1108,  1528,   769,
   771,   773,  1116,   775,  1678,  1485,  1852,  1894,  1853,  1895,
  1200,   627,   319,   851,   413,  1054,  1384,  1269,   552,   206,
  1896,   207,   540,  1662,   634,  1102,   830,   700,   657,   402,
  1662,   136,   695,   696,   697,  1375,   137,   138,   910,   208,
   328,   928,  1173,   949,   290,  1137,   856,    95,   438,   709,
  1383,   209,  1159,   210,   211,   139,   542,   140,   534,   991,
   967,   968,   969,   635,   407,   831,  1255,  1296,  1539,   212,
  1764,   648,   305,  1181,  1182,   141,   628,   320,   627,   781,
   199,  1385,  1854,   636,  1545,   607,   422,   213,   438,   438,
  1131,   214,   444,   878,  1132,  1384,   851,   655,   423,   424,
   425,   637,   638,   712,   215,   959,  1855,   321,   142,   414,
   216,   438,   438,   438,   426,   438,   658,   985,   403,   415,
   314,    12,    98,   329,   143,  1656,   639,   659,   660,   672,
   673,   610,   445,   144,   217,   218,   219,   656,   438,   825,
  1178,   535,   438,   628,   451,   220,   649,   108,   623,    14,
   950,   145,   221,  1352,   787,   544,   879,   146,  1384,   640,
  1385,   562,   438,   674,   292,   992,   147,   563,   566,   315,
   408,   438,   852,   427,  1589,   322,   293,   826,   148,   306,
   294,   675,   438,   196,  1591,  1641,  1603,   295,   627,   428,
   149,  1603,  1103,  1603,   856,   110,  1104,  1603,  1765,  1837,
  1839,   935,  1376,  1603,  1603,   857,   830,   825,   676,  1021,
  1603,   198,  1127,  1128,  1129,  1130,   291,  1716,  1057,  1679,
  1136,  1663,   323,  1385,   131,   199,  1155,  1156,  1663,  1525,
  1602,   111,  1157,  1158,   610,   610,   774,   776,   545,  1058,
   222,   132,  1856,  1462,  1463,   834,  1464,  1478,   835,   836,
  1022,   567,  1715,   628,   884,   852,   541,   296,   837,   150,
   763,   297,   832,   982,  1330,  1332,  1061,  1333,  1063,   223,
  1270,   698,   446,    99,   100,   330,  1486,  1232,   151,  1162,
   152,  1680,  1055,   439,   911,  1118,   912,   650,   112,   564,
   543,   913,  1201,   153,  1125,  1126,  1239,   661,   298,   970,
  1135,   936,  1256,  1501,  1540,  1140,  1142,   937,  1414,  1415,
  1416,  1417,   154,   155,   429,  1897,   299,  1023,  1202,   608,
  1546,   113,  1224,   439,   439,   156,   157,   158,   159,   160,
   161,   162,   163,   164,   165,   119,   324,   713,   603,   960,
   641,   881,   882,   883,  1526,   382,   439,   439,   439,   114,
   439,  1747,  1857,   857,   166,   167,   168,   169,   170,   171,
   172,   173,   174,   175,   176,   177,   178,   179,   827,   180,
   250,   181,   416,   439,   828,  1386,  1323,   439,   224,   182,
   183,   184,   662,   252,  1387,  1388,  1437,  1353,   185,   186,
   187,  1442,   953,   115,   307,   308,  1331,   439,   116,   430,
   188,   189,   190,   191,   393,  1700,   439,  1406,  1590,   715,
   117,  1409,   192,   118,   993,   193,  1771,   439,  1592,  1410,
  1604,  1016,   716,   994,   995,  1605,  1083,  1684,   717,   677,
   124,  1827,   718,  1838,  1840,  1136,   827,  1841,  1842,   719,
   720,  1407,   838,   209,  1859,   300,  1119,  1120,  1084,  1085,
   125,  1136,   721,    14,  1017,  1131,   126,   996,   722,  1132,
   723,   724,   627,  1302,  1303,   725,   726,  1306,  1307,  1308,
  1309,   727,  1662,  1313,   127,  1143,  1144,  1145,  1146,  1147,
  1148,  1149,  1150,   134,  1662,   728,   729,  1143,  1144,  1145,
  1146,  1147,  1148,  1160,  1161,  1421,  1422,  1423,   253,  1312,
   730,  1314,   731,  1018,   732,  1019,   733,   333,   334,   335,
  1398,   254,   734,   735,  1155,  1156,   736,   737,  1056,   255,
  1157,  1158,   738,   739,   610,   257,   610,   628,  1066,  1067,
  1068,  1424,  1155,  1156,  1530,  1531,   120,   121,  1157,  1158,
  1662,  1791,   740,  1155,  1156,   256,  1155,  1156,   259,  1157,
  1158,   258,  1157,  1158,   741,   260,  1665,   742,  1666,  1665,
   743,  1808,  1575,   261,  1559,  1560,   262,  1262,  1143,  1144,
  1145,  1146,  1147,  1148,  1160,  1161,   263,  1824,  1825,  1127,
  1128,  1129,  1130,   264,  1131,   266,   570,   267,  1132,   573,
   268,   575,  1479,   269,  1402,  1403,  1404,  1405,  1843,  1127,
  1128,  1129,  1130,   270,  1408,   280,  1401,  1143,  1144,  1145,
  1146,  1147,  1148,  1149,  1150,   271,  1411,  1412,  1155,  1156,
   272,  1413,   128,   129,  1157,   941,   942,   943,  1418,  1419,
  1420,   453,   744,  1871,  1326,   454,   455,   456,   457,   458,
   459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
   274,  1211,   287,   288,   745,   275,   746,   747,   748,   749,
   750,  1127,  1128,  1129,  1130,   591,   592,   593,   594,  1151,
  1663,  1152,  1153,   276,  1136,   449,   450,  1618,   302,    -6,
     1,  1151,  1663,  1152,  1153,   477,   277,    -6,  1315,  1316,
  1317,  1318,  1237,   780,   278,   781,   782,   783,   784,   785,
   279,  1127,  1128,  1129,  1130,   786,   363,   364,  1247,  1399,
   281,    -6,   478,  1162,   399,   282,    -6,  1109,  1110,   751,
   752,   753,   754,   755,   756,   757,   758,   283,   209,   979,
   980,   981,  1638,  1475,  1639,  1112,  1113,  1131,  1663,   284,
    -6,  1132,   759,    -6,   383,   285,   479,    -6,  1098,  1099,
  1100,   286,   480,   506,    -6,  1136,    -6,   303,   481,   304,
    -6,  1407,  1151,   310,  1152,  1153,   209,  1229,  1230,  1231,
   787,   788,   789,   311,   507,   790,   791,   312,   760,   313,
   508,   316,   383,   509,   510,   317,   761,  1361,  1362,  1363,
   318,   384,   762,  1127,  1128,  1129,  1130,   511,   886,  1205,
  1401,  1151,   326,  1152,  1153,  1355,  1090,   792,   331,  1324,
    -6,   482,   332,   793,    -6,  1026,   483,   512,   337,  1721,
  1694,  1334,  1498,  1499,  1500,  1027,  1562,  1563,  1564,   384,
  1192,  1445,   338,  1488,   385,  1195,  1448,    -6,   340,  1028,
   453,  1699,    -6,    -6,   454,   455,   456,   457,   458,   459,
   460,   461,   462,   463,   464,   465,   466,   467,   468,   342,
  1029,   478,   794,   795,   796,   343,   345,  1572,   708,  1129,
  1130,   347,   385,   348,  1573,   349,  1574,  1576,   350,   513,
   514,   351,   386,   352,   797,   798,   353,   799,    -6,    -6,
    -6,  1155,  1156,   354,   355,   479,   356,  1742,   357,   358,
   359,   480,  1521,     3,   360,   361,   378,   481,   366,   367,
   368,    -6,   369,   370,   372,  1030,   373,   387,    -6,    -6,
    -6,   374,    -6,    -6,    -6,    -6,    -6,     4,    -6,   375,
   396,   199,    -6,    -6,    -6,   800,   801,   435,  1031,    -6,
   802,   803,   804,   805,   806,   807,   436,   442,   451,    -6,
   443,    -6,    -6,   474,   500,   387,     5,   665,   666,     6,
   482,   501,   503,     7,   524,   483,  1461,   504,   526,   516,
     8,   505,     9,   517,   520,   519,    10,   522,   525,    -6,
    -6,    -6,   521,  1143,  1144,  1145,  1146,  1147,  1148,  1149,
  1150,   610,  1476,    -6,   527,    -6,   453,   528,    -6,   438,
   454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
   464,   465,   466,   467,   468,   529,  1143,  1144,  1145,  1146,
  1147,  1148,  1160,  1161,   531,   530,    11,   533,   536,   537,
    12,   453,   538,    -6,   438,   454,   455,   456,   457,   458,
   459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
   488,   532,   667,    13,   668,   539,   453,   546,    14,    15,
   454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
   464,   465,   466,   467,   468,   453,   547,   934,   549,   454,
   455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
   465,   466,   467,   468,   550,   551,   552,   555,   669,   670,
   557,   560,   556,   558,    16,    17,    18,  1426,  1427,  1428,
  1429,  1430,  1431,  1432,   559,   587,   565,   489,   490,   491,
   492,   493,   494,   569,   590,   572,   596,    19,    16,    17,
    18,  1719,   601,  1720,    20,    21,    22,   574,    23,    24,
    25,    26,    27,   612,    28,   577,   615,   618,    29,    30,
    31,   578,   579,   580,   581,    32,   582,   583,   584,   585,
   586,   588,   626,   643,   589,    33,   597,    34,    35,  1127,
  1128,  1129,  1130,   644,   645,   600,   605,  1151,   606,  1152,
  1153,   646,   652,   609,   680,   664,   679,   681,   682,   683,
   684,   685,   686,   688,   687,    36,    37,    38,   689,   690,
   691,   692,   693,  1136,   694,   700,   703,   710,   704,    39,
  1151,    40,  1152,  1153,    41,   707,   777,   714,   778,   816,
   818,   820,   823,   842,   863,   864,   872,   865,   866,   867,
   868,   869,   870,   469,   439,   871,   873,   874,   875,   876,
   889,   877,   886,   890,   891,   893,   453,   894,   895,    42,
   454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
   464,   465,   466,   467,   468,   896,   897,   898,   621,   439,
   899,   901,   905,   902,   904,   907,   906,   908,   917,   918,
   919,   920,   921,   926,   933,   940,   923,   924,   925,   927,
   928,   945,   946,  1325,   951,   952,   954,   955,   956,   957,
   958,   962,   963,   965,   964,   966,   973,   974,   975,   977,
   976,   978,  1520,   984,   987,  1070,   989,   990,   998,   999,
  1001,  1002,  1090,  1004,  1005,  1009,  1010,  1011,  1012,  1013,
  1014,  1015,  1025,  1033,  1034,  1036,  1037,  1072,  1038,  1039,
  1040,  1041,  1042,  1043,  1093,  1044,  1045,  1046,  1047,  1048,
  1049,  1060,  1073,  1087,  1074,  1207,  1075,  1076,  1122,  1078,
  1079,  1689,  1184,  1080,  1081,  1088,  1094,  1095,  1096,  1106,
  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1185,  1695,
  1172,  1177,  1183,  1186,  1187,  1188,  1189,  1190,  1097,  1191,
  1198,  1192,  1195,  1199,  1204,  1208,  1212,  1213,  1216,  1218,
  1217,  1219,  1221,  1222,  1227,  1228,  1235,  1234,  1241,  1249,
  1243,  1244,  1245,  1246,  1248,  1250,  1251,  1252,  1253,  1254,
  1258,  1259,  1260,  1261,  1271,  1300,  1275,  1274,  1277,  1263,
  1264,  1265,  1266,  1267,  1280,  1268,  1296,  1272,  1273,  1276,
  1278,  1281,  1279,  1689,  1282,  1283,  1284,  1285,  1286,  1287,
  1743,  1288,  1289,  1290,  1310,  1320,  1291,  1292,  1293,  1321,
  1322,  1337,  1366,  1436,  1338,  1339,  1342,  1294,  1295,  1340,
  1299,  1304,  1305,  1335,  1341,  1344,  1345,  1346,  1347,  1348,
  1350,  1357,  1351,  1354,  1358,  1359,  1360,  1365,   712,  1367,
  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1378,  1379,  1380,
  1435,  1440,  1433,  1438,  1443,  1444,  1441,  1447,  1450,  1451,
  1453,  1454,  1455,  1456,  1467,  1457,  1469,  1472,  1482,  1480,
  1490,  1524,  1481,  1528,  1483,  1492,  1496,  1497,  1533,  1548,
  1547,  1549,  1551,  1550,  1484,  1487,  1553,  1491,  1493,  1554,
  1494,  1495,  1504,  1555,  1505,  1556,  1470,  1506,  1507,  1558,
  1566,  1557,  1567,  1568,  1569,  1577,  1571,  1580,  1593,  1624,
  1595,  1598,  1607,  1599,  1647,  1474,  1610,  1613,  1603,  1615,
  1652,  1617,  1626,  1630,  1631,  1667,  1670,  1633,  1634,  1636,
  1642,  1643,  1644,  1645,  1648,  1649,  1650,  1651,  1655,  1658,
  1661,  1673,  1691,  1682,  1683,  1692,  1693,  1697,  1698,  1705,
  1706,  1709,  1708,  1710,  1508,  1711,  1712,  1713,  1722,  1723,
  1726,  1728,  1730,  1718,  1731,  1738,  1509,  1510,  1733,  1512,
  1513,  1514,  1515,  1516,  1739,  1517,  1740,  1745,  1746,  1749,
  1518,  1519,  1600,  1750,  1522,  1523,  1534,  1751,  1535,  1536,
  1537,  1538,  1752,  1753,  1541,  1542,  1543,  1544,  1581,  1582,
  1136,  1758,  1754,  1755,  1756,  1583,  1584,  1585,  1586,  1759,
  1762,  1763,  1767,  1773,  1772,  1774,  1587,  1588,  1594,  1596,
  1777,  1778,  1779,  1782,  1785,  1786,  1601,  1608,  1676,  1611,
  1612,  1787,  1616,  1619,  1620,  1621,  1622,  1627,  1628,  1629,
  1632,  1799,  1635,  1637,  1788,  1640,  1794,  1795,  1800,  1690,
  1802,  1653,  1796,  1797,  1654,  1659,  1660,  1804,  1803,  1677,
  1807,  1809,  1812,  1813,  1863,  1814,  1815,  1829,  1701,  1820,
  1822,  1703,  1828,  1704,  1717,  1832,  1833,  1847,  1845,  1736,
  1848,  1741,  1748,  1849,  1860,  1866,  1864,  1844,  1867,  1868,
  1757,  1869,  1870,  1760,  1872,  1770,  1873,  1875,  1876,  1761,
  1880,  1881,  1884,  1775,  1887,  1888,  1889,  1890,  1914,  1915,
  1899,  1900,  1776,  1886,  1902,  1903,  1904,  1905,  -683,  1609,
  -684,  1793,  1798,  1906,  1801,  1816,  1907,  1862,  1911,  1874,
  1877,  1878,  1879,   380,  1882,  1883,   377,  1885,  1737,  1446,
  1735,  1891,  1846,  1220,  1449,  1206,  1389,  1477,  1390,  1233,
  1238,   983,  1502,  1835,  1744,   986,   473,  1702,  1908,  1552,
  1669,  1725,  1356,  1180,  1327,  1071,  1175,   499,  1328,  1729,
  1727,     0,     0,     0,     0,  1909,     0,     0,     0,  1910,
     0,  1912,   629,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   885,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   903,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   932,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   939
};

static const short yycheck[] = {   214,
   503,   393,   504,   770,   772,   200,   479,   480,   841,   826,
   225,   197,  1210,   631,   831,    14,  1334,   834,   520,   521,
   837,   838,    29,    16,    21,  1223,   813,   814,    34,    62,
    62,    62,   819,    62,    21,   137,    35,    86,    37,    88,
    64,   172,    28,    91,    37,   141,   172,   155,   172,    16,
    99,    18,   174,   214,    33,    59,   173,   172,    46,    37,
   214,     9,   169,   170,   171,    37,    14,    15,   139,    36,
   174,   172,    37,    54,   236,   833,    91,   174,   174,   471,
    64,    48,   840,    50,    51,    33,   174,    35,   174,     6,
   169,   170,   171,    72,    37,   212,   172,   172,   172,    66,
   168,    22,    54,   861,   862,    53,   237,    93,   172,    39,
    64,   237,   111,    92,   172,   174,    10,    84,   174,   174,
   408,    88,    10,   236,   412,   172,    91,   356,    22,    23,
    24,   110,   111,   174,   101,   174,   135,   123,    86,   132,
   107,   174,   174,   174,    38,   174,   134,    37,   126,   142,
   125,   135,   174,   257,   102,  1565,   135,   145,   146,    55,
    56,   376,    50,   111,   131,   132,   133,   396,   174,   174,
    37,   257,   174,   237,    64,   142,    97,   173,   393,   163,
   161,   129,   149,   174,   114,   174,   299,   135,   172,   168,
   237,   168,   174,    89,    14,   112,   144,   174,   174,   174,
   143,   174,   250,    97,   174,   191,    26,   212,   156,   161,
    30,   107,   174,   172,   174,  1533,   174,    37,   172,   113,
   168,   174,   226,   174,    91,   174,   230,   174,   296,   174,
   174,   623,   204,   174,   174,   250,   173,   174,   134,     5,
   174,    50,   403,   404,   405,   406,   408,  1657,    16,   236,
   404,   412,   238,   237,    84,    64,   217,   218,   412,    20,
  1458,   174,   223,   224,   479,   480,   481,   482,   257,    37,
   237,   101,   271,   350,   351,   212,   353,   408,   215,   216,
    46,   257,   408,   237,   408,   250,   408,   107,   225,   237,
   476,   111,   409,   408,  1061,  1063,   769,  1065,   771,   266,
   408,   408,   190,    94,    95,   409,   408,   408,   256,  1142,
   258,   298,   408,   409,   385,    37,   387,   238,   174,   296,
   408,   392,   346,   271,   827,   828,   944,   315,   148,   408,
   832,   104,   408,   408,   408,   838,   838,   110,  1155,  1156,
  1157,  1158,   290,   291,   238,   394,   166,   113,   372,   408,
   408,   174,   408,   409,   409,   303,   304,   305,   306,   307,
   308,   309,   310,   311,   312,   173,   352,   408,   365,   408,
   349,   172,   173,   174,   135,   382,   409,   409,   409,   174,
   409,  1699,   381,   250,   332,   333,   334,   335,   336,   337,
   338,   339,   340,   341,   342,   343,   344,   345,   403,   347,
    37,   349,   395,   409,   409,   389,   408,   409,   375,   357,
   358,   359,   400,   172,   398,   399,  1174,   408,   366,   367,
   368,  1179,   637,   174,   376,   377,   408,   409,   174,   323,
   378,   379,   380,   381,   620,   408,   409,  1134,   408,    14,
   174,  1138,   390,   174,    46,   393,   408,   409,   408,   410,
   408,    56,    27,    55,    56,   408,    60,   408,    33,   355,
   174,   408,    37,   408,   408,   404,   403,   408,   408,    44,
    45,   410,   409,    48,   408,   295,   198,   199,    82,    83,
   174,   404,    57,   163,    89,   408,   174,    89,    63,   412,
    65,    66,   172,  1038,  1039,    70,    71,  1042,  1043,  1044,
  1045,    76,   214,  1048,   174,   217,   218,   219,   220,   221,
   222,   223,   224,   174,   214,    90,    91,   217,   218,   219,
   220,   221,   222,   223,   224,   233,   234,   235,   408,  1047,
   105,  1049,   107,   138,   109,   140,   111,   292,   293,   294,
   213,   172,   117,   118,   217,   218,   121,   122,   763,   172,
   223,   224,   127,   128,   769,   408,   771,   237,   773,   774,
   775,   269,   217,   218,  1332,  1333,    94,    95,   223,   224,
   214,  1769,   147,   217,   218,   174,   217,   218,   172,   223,
   224,   408,   223,   224,   159,   172,  1573,   162,  1575,  1576,
   165,  1789,  1409,   172,  1381,  1382,   172,   989,   217,   218,
   219,   220,   221,   222,   223,   224,   172,  1805,  1806,   403,
   404,   405,   406,   172,   408,   408,   341,   172,   412,   344,
   174,   346,  1240,   408,  1127,  1128,  1129,  1130,  1826,   403,
   404,   405,   406,   408,  1136,   172,   410,   217,   218,   219,
   220,   221,   222,   223,   224,   408,  1149,  1150,   217,   218,
   408,  1154,   317,   318,   223,   172,   173,   174,  1160,  1161,
  1162,   171,   237,  1861,  1056,   175,   176,   177,   178,   179,
   180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
   408,   896,    94,    95,   259,   408,   261,   262,   263,   264,
   265,   403,   404,   405,   406,   361,   362,   363,   364,   411,
   412,   413,   414,   408,   404,   267,   268,  1475,   174,     0,
     1,   411,   412,   413,   414,    37,   408,     8,   361,   362,
   363,   364,   937,    37,   408,    39,    40,    41,    42,    43,
   408,   403,   404,   405,   406,    49,   370,   371,   953,   412,
   172,    32,    64,  1576,   939,   408,    37,   169,   170,   324,
   325,   326,   327,   328,   329,   330,   331,   408,    48,   172,
   173,   174,  1530,  1236,  1531,   169,   170,   408,   412,   408,
    61,   412,   347,    64,    64,   408,    98,    68,   281,   282,
   283,   408,   104,    37,    75,   404,    77,    25,   110,   174,
    81,   410,   411,   174,   413,   414,    48,   172,   173,   174,
   114,   115,   116,   174,    58,   119,   120,   174,   383,   174,
    64,   174,    64,    67,    68,   174,   391,   281,   282,   283,
   174,   111,   397,   403,   404,   405,   406,    81,   409,   410,
   410,   411,   174,   413,   414,   408,   409,   151,   174,  1054,
   131,   163,   174,   157,   135,     7,   168,   101,   292,  1666,
  1618,  1066,   172,   173,   174,    17,   172,   173,   174,   111,
   409,   410,   174,  1255,   154,   409,   410,   158,   174,    31,
   171,  1638,   163,   164,   175,   176,   177,   178,   179,   180,
   181,   182,   183,   184,   185,   186,   187,   188,   189,   174,
    52,    64,   206,   207,   208,   174,   174,  1400,   408,   405,
   406,   174,   154,   174,  1406,   174,  1409,  1409,   174,   163,
   164,   174,   164,   174,   228,   229,   174,   231,   209,   210,
   211,   217,   218,   174,   174,    98,   174,  1694,   174,   174,
   174,   104,  1324,     8,   174,   174,    37,   110,   174,   174,
   174,   232,   174,   174,   174,   107,   174,   237,   239,   240,
   241,   174,   243,   244,   245,   246,   247,    32,   249,   174,
    29,    64,   253,   254,   255,   279,   280,   172,   130,   260,
   284,   285,   286,   287,   288,   289,   172,   174,    64,   270,
   174,   272,   273,   172,    67,   237,    61,    11,    12,    64,
   163,   408,   411,    68,   100,   168,  1211,   411,   388,   408,
    75,   411,    77,   408,   411,   408,    81,   408,    73,   300,
   301,   302,   411,   217,   218,   219,   220,   221,   222,   223,
   224,  1236,  1237,   314,   158,   316,   171,    74,   319,   174,
   175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
   185,   186,   187,   188,   189,    85,   217,   218,   219,   220,
   221,   222,   223,   224,   167,    78,   131,   408,   408,   408,
   135,   171,   408,   354,   174,   175,   176,   177,   178,   179,
   180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
   131,   297,   106,   158,   108,   408,   171,   408,   163,   164,
   175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
   185,   186,   187,   188,   189,   171,   408,   408,   408,   175,
   176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
   186,   187,   188,   189,   408,   408,   172,   408,   152,   153,
   174,   174,   409,   408,   209,   210,   211,  1164,  1165,  1166,
  1167,  1168,  1169,  1170,   408,   174,   408,   198,   199,   200,
   201,   202,   203,   408,   168,   408,   174,   232,   209,   210,
   211,  1664,   353,  1665,   239,   240,   241,   408,   243,   244,
   245,   246,   247,   174,   249,   408,   174,    37,   253,   254,
   255,   408,   408,   408,   408,   260,   408,   408,   408,   408,
   408,   408,   172,   135,   408,   270,   408,   272,   273,   403,
   404,   405,   406,   172,    61,   408,   408,   411,   408,   413,
   414,   172,   174,   408,   172,   348,   408,   172,   172,   172,
   408,   172,   174,   408,   174,   300,   301,   302,   172,   408,
    13,   408,   408,   404,   408,   172,   408,    37,   408,   314,
   411,   316,   413,   414,   319,   408,   174,   408,    37,   172,
   174,    37,   384,   232,   172,   174,   408,   174,   174,   174,
   174,   174,   174,   408,   409,   174,   409,   408,   408,   408,
   174,   409,   409,   408,   295,   174,   171,   408,   174,   354,
   175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
   185,   186,   187,   188,   189,   409,   408,   408,   408,   409,
   313,   408,   174,   408,   408,   174,   408,   168,   174,   168,
   174,   408,   408,   408,   408,   408,   174,   172,   172,   172,
   172,   172,    37,   408,   174,   174,   174,   174,   174,   174,
   172,   408,   408,   172,   408,    13,   408,   408,   408,   174,
   408,   174,   408,   172,   408,   408,   174,   174,   174,   174,
   172,   174,   409,   174,   174,   174,   174,   174,   174,   174,
   174,   174,   174,   174,   174,   172,   174,   149,   174,   174,
   174,   174,   174,   174,   408,   174,   174,   174,   174,   174,
   174,   172,   174,   207,   174,   410,   174,   174,   408,   174,
   174,  1606,   408,   174,   174,   174,   173,   173,   173,   172,
   172,   172,   172,   172,   172,   172,   172,   172,   408,  1624,
   174,   174,   172,   408,   408,   408,   408,   408,   227,   408,
   174,   409,   409,   174,   174,   174,   408,   408,   174,   174,
   408,   408,   168,   174,   408,   408,   408,   172,   408,    13,
   408,   408,   408,   408,   408,   150,   174,   174,   174,   174,
   410,   408,   408,   172,   172,    64,   172,   174,   172,   408,
   408,   408,   408,   408,   172,   408,   172,   408,   408,   408,
   408,   174,   408,  1688,   174,   408,   408,   408,   408,   408,
  1695,   408,   408,   408,    64,   172,   408,   408,   408,   173,
   172,   172,   157,   245,   174,   174,   172,   408,   408,   174,
   408,   408,   408,   408,   174,   174,   174,   174,   174,   172,
   408,   408,   174,   174,   408,   408,   174,   174,   174,   408,
   408,   174,   174,   408,   174,   174,   408,   408,   172,   172,
   408,   408,   174,   174,   408,   174,   249,   174,   174,   236,
   174,   408,   174,   408,   168,   408,   174,   168,   174,   172,
    34,    37,   408,    34,   174,   174,   174,   174,   172,   174,
   208,   174,   174,   227,   408,   408,    34,   408,   408,    34,
   408,   408,   408,    34,   408,    34,   369,   408,   408,   174,
   172,   203,   172,   172,    37,   173,   213,   172,   174,   110,
   174,   174,   174,   172,   227,   373,   174,   174,   174,   174,
   205,   174,   174,   172,   172,   236,   251,   174,   174,   174,
   174,   174,   174,   174,   174,   174,   174,   174,   172,   174,
   172,   251,   135,   174,   174,   168,   174,    13,    13,   174,
   174,   139,   174,   139,   408,   139,   139,   174,   174,   174,
   174,   174,   174,   214,   174,   174,   408,   408,   229,   408,
   408,   408,   408,   408,   174,   408,   174,   174,   174,   174,
   408,   408,   369,   174,   408,   408,   408,   160,   408,   408,
   408,   408,   174,   174,   408,   408,   408,   408,   408,   408,
   404,   168,   174,   174,   174,   408,   408,   408,   408,   174,
   236,   174,   174,   139,   174,   139,   408,   408,   408,   408,
   174,   174,   174,   174,   174,   174,   410,   408,   410,   408,
   408,   174,   408,   408,   408,   408,   408,   408,   408,   408,
   408,    64,   408,   408,   229,   408,   174,   174,    64,   369,
   252,   408,   174,   174,   408,   408,   408,   174,   248,   410,
   174,   168,   172,   174,    25,   174,   174,   168,   408,   174,
   174,   410,   174,   408,   408,   172,   174,   172,   174,   410,
   174,   408,   408,   172,   174,   125,   174,   369,   174,   174,
   408,   174,   135,   408,   374,   386,   174,   172,   174,   408,
   174,   174,   174,   408,   174,   174,   174,   174,     0,     0,
   174,   174,   408,   349,   174,   174,   174,   172,   408,  1466,
   408,   408,   408,   174,   408,   408,   174,   408,   174,   408,
   408,   408,   408,   195,   408,   408,   193,   408,  1688,  1193,
  1686,   408,  1831,   915,  1196,   887,  1123,  1238,  1123,   929,
   939,   701,  1297,  1818,  1696,   706,   227,  1642,   408,  1366,
  1579,  1669,  1091,   858,  1057,   779,   853,   249,  1059,  1674,
  1671,    -1,    -1,    -1,    -1,   408,    -1,    -1,    -1,   408,
    -1,   408,   400,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   553,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   576,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   620,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   625
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/local/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 433 "lef.y"
{
        // 11/16/2001 - Wanda da Rosa - pcr 408334
        // Return 1 if there are errors
        if (lef_errors)
           return 1;
        if (!hasVer)
           yywarning("VERSION is a required statement.");
        //only pre 5.6, 5.6 it is obsoleted
        if (!hasNameCase && versionNum < 5.6)
           yywarning("NAMESCASESENSITIVE is a required statement.");
        if (!hasBusBit && versionNum < 5.6)
           yywarning("BUSBITCHARS is a required statement.");
        if (!hasDivChar && versionNum < 5.6)
           yywarning("DIVIDERCHAR is a required statement.");
        hasVer = 0;
        hasNameCase = 0;
        hasBusBit = 0;
        hasDivChar = 0;
        hasManufactur = 0;
        antennaInoutWarnings = 0;
        antennaInputWarnings = 0;
        antennaOutputWarnings = 0;
        arrayWarnings = 0;
        caseSensitiveWarnings = 0;
        correctionTableWarnings = 0;
        dielectricWarnings = 0;
        edgeRateThreshold1Warnings = 0;
        edgeRateThreshold2Warnings = 0;
        edgeRateScaleFactorWarnings = 0;
        inoutAntennaWarnings = 0;
        inputAntennaWarnings = 0;
        iRDropWarnings = 0;
        layerWarnings = 0;
        macroWarnings = 0;
        maxStackViaWarnings = 0;
        minFeatureWarnings = 0;
        noiseMarginWarnings = 0;
        noiseTableWarnings = 0;
        nonDefaultWarnings = 0;
        noWireExtensionWarnings = 0;
        outputAntennaWarnings = 0;
        siteWarnings = 0;
        spacingWarnings = 0;
        timingWarnings = 0;
        unitsWarnings = 0;
        useMinSpacingWarnings = 0;
        viaRuleWarnings = 0;
        viaWarnings = 0;
      ;
    break;}
case 2:
#line 483 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 3:
#line 484 "lef.y"
{ 
         versionNum = convert_name2num(yyvsp[-1].string);
         if (versionNum > 5.6) {
           char temp[120];
           sprintf(temp,
           "Lef parser 5.6 does not support lef file with version %s. Parser stops executions",
                   yyvsp[-1].string);
           yyerror(temp);
           return 1;
         }
/*
         versionNum = $3;         Save the version number for future use */
         if (lefrVersionStrCbk) {
            CALLBACK(lefrVersionStrCbk, lefrVersionStrCbkType, yyvsp[-1].string);
         } else {
            if (lefrVersionCbk)
               CALLBACK(lefrVersionCbk, lefrVersionCbkType, versionNum);
         }
         if (versionNum > 5.3 && versionNum < 5.4) {
            ignoreVersion = 1;
         }
         use5_3 = use5_4 = 0;
         lef_errors = 0;
         hasVer = 1;
         if (versionNum < 5.6) {
            doneLib = 0;
            lefNamesCaseSensitive = lefReaderCaseSensitive;
         } else {
            doneLib = 1;
            lefNamesCaseSensitive = 1;
         }
      ;
    break;}
case 4:
#line 519 "lef.y"
{
        if (lefrDividerCharCbk)
          CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, yyvsp[-1].string);
        hasDivChar = 1;
      ;
    break;}
case 5:
#line 526 "lef.y"
{
        if (lefrBusBitCharsCbk)
          CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, yyvsp[-1].string); 
        hasBusBit = 1;
      ;
    break;}
case 8:
#line 535 "lef.y"
{ ;
    break;}
case 9:
#line 538 "lef.y"
{
        if (versionNum >= 5.6) {
           doneLib = 1;
           ge56done = 1;
        }
      ;
    break;}
case 10:
#line 545 "lef.y"
{
        doneLib = 1;
        ge56done = 1;
        if (lefrLibraryEndCbk)
          CALLBACK(lefrLibraryEndCbk, lefrLibraryEndCbkType, 0);
        // 11/16/2001 - Wanda da Rosa - pcr 408334
        // Return 1 if there are errors
/*
        if (lef_errors)
           return 1;
        if (!hasVer)
           yywarning("VERSION is a required statement.");
        if (!hasNameCase)
           yywarning("NAMESCASESENSITIVE is a required statement.");
        if (!hasBusBit && versionNum < 5.6)
           yywarning("BUSBITCHARS is a required statement.");
        if (!hasDivChar && versionNum < 5.6)
           yywarning("DIVIDERCHAR is a required statement.");
        hasVer = 0;
        hasNameCase = 0;
        hasBusBit = 0;
        hasDivChar = 0;
        hasManufactur = 0;
*/
      ;
    break;}
case 49:
#line 586 "lef.y"
{
            if (versionNum < 5.6) {
              lefNamesCaseSensitive = TRUE;
              if (lefrCaseSensitiveCbk)
                CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
			        lefNamesCaseSensitive);
              hasNameCase = 1;
            } else
              if (lefrCaseSensitiveCbk) /* write warning only if cbk is set */
                 if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings)
                   yywarning("NAMESCASESENSITIVE is obsolete in 5.6. It will be ignored.");
	  ;
    break;}
case 50:
#line 599 "lef.y"
{
            if (versionNum < 5.6) {
	      lefNamesCaseSensitive = FALSE;
              if (lefrCaseSensitiveCbk)
	        CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
			       lefNamesCaseSensitive);
              hasNameCase = 1;
            } else {
              if (lefrCaseSensitiveCbk) { /* write error only if cbk is set */
                if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings) {
                  yyerror("NAMESCASESENSITIVE cannot be set to OFF");
                  CHKERR();
                }
              }
            }
	  ;
    break;}
case 51:
#line 617 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrNoWireExtensionCbk)
          CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "ON");
      } else
        if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */
           if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
             yywarning("NOWIREEXTENSIONATPIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 52:
#line 627 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrNoWireExtensionCbk)
          CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "OFF");
      } else
        if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */
           if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
             yywarning("NOWIREEXTENSIONATPIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 53:
#line 638 "lef.y"
{
      if (lefrManufacturingCbk)
        CALLBACK(lefrManufacturingCbk, lefrManufacturingCbkType, yyvsp[-1].dval);
      hasManufactur = 1;
    ;
    break;}
case 54:
#line 645 "lef.y"
{
    if ((strcmp(yyvsp[-2].string, "PIN") == 0) && (versionNum >= 5.6)) {
      if (lefrUseMinSpacingCbk) /* write warning only if cbk is set */
         if (useMinSpacingWarnings++ < lefrUseMinSpacingWarnings)
            yywarning("USEMINSPACING PIN is obsolete in 5.6. It will be ignored.");
    } else {
        if (lefrUseMinSpacingCbk) {
          lefrUseMinSpacing.lefiUseMinSpacing::set(yyvsp[-2].string, yyvsp[-1].integer);
          CALLBACK(lefrUseMinSpacingCbk, lefrUseMinSpacingCbkType,
                   &lefrUseMinSpacing);
      }
    }
  ;
    break;}
case 55:
#line 660 "lef.y"
{ CALLBACK(lefrClearanceMeasureCbk, lefrClearanceMeasureCbkType, yyvsp[-1].string); ;
    break;}
case 56:
#line 663 "lef.y"
{yyval.string = (char*)"MAXXY";;
    break;}
case 57:
#line 664 "lef.y"
{yyval.string = (char*)"EUCLIDEAN";;
    break;}
case 58:
#line 667 "lef.y"
{yyval.string = (char*)"OBS";;
    break;}
case 59:
#line 668 "lef.y"
{yyval.string = (char*)"PIN";;
    break;}
case 60:
#line 671 "lef.y"
{yyval.integer = 1;;
    break;}
case 61:
#line 672 "lef.y"
{yyval.integer = 0;;
    break;}
case 62:
#line 675 "lef.y"
{ 
      if (lefrUnitsCbk)
        CALLBACK(lefrUnitsCbk, lefrUnitsCbkType, &lefrUnits);
    ;
    break;}
case 63:
#line 681 "lef.y"
{
      lefrUnits.lefiUnits::clear();
      if (hasManufactur) {
        if (unitsWarnings++ < lefrUnitsWarnings) {
          yyerror("MANUFACTURINGGRID has defined before UNITS");
          CHKERR();
        }
      }
    ;
    break;}
case 66:
#line 696 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setTime(yyvsp[-1].dval); ;
    break;}
case 67:
#line 698 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setCapacitance(yyvsp[-1].dval); ;
    break;}
case 68:
#line 700 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setResistance(yyvsp[-1].dval); ;
    break;}
case 69:
#line 702 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setPower(yyvsp[-1].dval); ;
    break;}
case 70:
#line 704 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setCurrent(yyvsp[-1].dval); ;
    break;}
case 71:
#line 706 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setVoltage(yyvsp[-1].dval); ;
    break;}
case 72:
#line 708 "lef.y"
{ 
      if(validNum((int)yyvsp[-1].dval)) {
         if (lefrUnitsCbk)
            lefrUnits.lefiUnits::setDatabase("MICRONS", yyvsp[-1].dval);
      }
    ;
    break;}
case 73:
#line 715 "lef.y"
{ if (lefrUnitsCbk) lefrUnits.lefiUnits::setFrequency(yyvsp[-1].dval); ;
    break;}
case 74:
#line 719 "lef.y"
{ 
      if (lefrLayerCbk)
        CALLBACK(lefrLayerCbk, lefrLayerCbkType, &lefrLayer);
    ;
    break;}
case 75:
#line 724 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 76:
#line 725 "lef.y"
{ 
      if (lefrHasMaxVS) {   /* 5.5 */
        if (lefrLayerCbk) { /* write error only if cbk is set */
          if (layerWarnings++ < lefrLayerWarnings) {
            yyerror("A MAXVIASTACK has defined before the LAYER");
            CHKERR();
          }
        }
      }
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setName(yyvsp[0].string);
      useLenThr = 0;
      layerCut = 0;
      layerMastOver = 0;
      layerRout = 0;
      layerDir = 0;
      lefrHasLayer = 1;
      //strcpy(layerName, $3);
      layerName = strdup(yyvsp[0].string);
      hasType = 0;
      hasPitch = 0;
      hasWidth = 0;
      hasDirection = 0;
      hasParallel = 0;
      hasInfluence = 0;
      lefrHasSpacingTbl = 0;
      lefrHasSpacing = 0;
    ;
    break;}
case 77:
#line 754 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 78:
#line 755 "lef.y"
{ 
      if (strcmp(layerName, yyvsp[0].string) != 0) {
        lefFree(layerName);
        if (lefrLayerCbk) { /* write error only if cbk is set */
          if (layerWarnings++ < lefrLayerWarnings) {
            yyerror("END LAYER name is different from LAYER");
            CHKERR(); 
          }
        }
      } else
        lefFree(layerName);
      if (!lefrRelaxMode) {
        if (hasType == 0) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("TYPE in LAYER is required");
              CHKERR(); 
            }
          }
        }
        if ((layerRout == 1) && (hasPitch == 0)) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("PITCH in LAYER with type ROUTING is required");
              CHKERR(); 
            }
          }
        }
        if ((layerRout == 1) && (hasWidth == 0)) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("WIDTH in LAYER with type ROUTING is required");
              CHKERR(); 
            }
          }
        }
        if ((layerRout == 1) && (hasDirection == 0)) {
          if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DIRECTION in LAYER with type ROUTING is required");
              CHKERR(); 
            }
          }
        }
      }
    ;
    break;}
case 79:
#line 804 "lef.y"
{ ;
    break;}
case 80:
#line 809 "lef.y"
{ ;
    break;}
case 81:
#line 813 "lef.y"
{
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::setType(yyvsp[-1].string);
      hasType = 1;
    ;
    break;}
case 82:
#line 819 "lef.y"
{ 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setPitch(yyvsp[-1].dval);
      hasPitch = 1;  
    ;
    break;}
case 83:
#line 824 "lef.y"
{ 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setPitchXY(yyvsp[-2].dval, yyvsp[-1].dval);
      hasPitch = 1;  
    ;
    break;}
case 84:
#line 829 "lef.y"
{ 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagPitch(yyvsp[-1].dval);
    ;
    break;}
case 85:
#line 833 "lef.y"
{ 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagPitchXY(yyvsp[-2].dval, yyvsp[-1].dval);
    ;
    break;}
case 86:
#line 837 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::setOffset(yyvsp[-1].dval);
    ;
    break;}
case 87:
#line 841 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::setOffsetXY(yyvsp[-2].dval, yyvsp[-1].dval);
    ;
    break;}
case 88:
#line 845 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagWidth(yyvsp[-1].dval);
    ;
    break;}
case 89:
#line 849 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDiagSpacing(yyvsp[-1].dval);
    ;
    break;}
case 90:
#line 853 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::setWidth(yyvsp[-1].dval);
      hasWidth = 1;  
    ;
    break;}
case 91:
#line 858 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::setArea(yyvsp[-1].dval);
    ;
    break;}
case 92:
#line 862 "lef.y"
{
      // 11/22/99 - Wanda da Rosa, PCR 283762
      //            Issue an error is this is defined in masterslice
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SPACING can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
              CHKERR();
            }
         }
      }
      // 5.5 either SPACING or SPACINGTABLE, not both for routing layer only
      if (layerRout) {
        if (lefrHasSpacingTbl) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
              }
           }
        }
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setSpacingMin(yyvsp[0].dval);
        lefrHasSpacing = 1;
      } else { 
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setSpacingMin(yyvsp[0].dval);
      }
    ;
    break;}
case 94:
#line 892 "lef.y"
{
      layerDir = 1;
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DIRECTION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDirection(yyvsp[-1].string);
      hasDirection = 1;  
    ;
    break;}
case 95:
#line 906 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setResistance(yyvsp[-1].dval);
    ;
    break;}
case 96:
#line 918 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
    ;
    break;}
case 97:
#line 929 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCapacitance(yyvsp[-1].dval);
    ;
    break;}
case 98:
#line 941 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
    ;
    break;}
case 99:
#line 952 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("HEIGHT can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setHeight(yyvsp[-1].dval);
    ;
    break;}
case 100:
#line 964 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("WIREEXTENSION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setWireExtension(yyvsp[-1].dval);
    ;
    break;}
case 101:
#line 976 "lef.y"
{
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("THICKNESS can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setThickness(yyvsp[-1].dval);
    ;
    break;}
case 102:
#line 988 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SHRINKAGE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setShrinkage(yyvsp[-1].dval);
    ;
    break;}
case 103:
#line 1000 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CAPMULTIPLIER can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCapMultiplier(yyvsp[-1].dval);
    ;
    break;}
case 104:
#line 1012 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("EDGECAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setEdgeCap(yyvsp[-1].dval);
    ;
    break;}
case 105:
#line 1037 "lef.y"
{ /* 5.3 syntax */
      use5_3 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNALENGTHFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                yyerror("ANTENNALENGTHFACTOR is a 5.3 or earlier syntax. Your lef file has both old and new syntax, which is illegal.");
                CHKERR();
              }
           }
        }
      }

      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaLength(yyvsp[-1].dval);
    ;
    break;}
case 106:
#line 1060 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentDensity(yyvsp[-1].dval);
    ;
    break;}
case 107:
#line 1072 "lef.y"
{ 
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
    ;
    break;}
case 108:
#line 1083 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentPoint(yyvsp[-3].dval, yyvsp[-2].dval);
    ;
    break;}
case 109:
#line 1094 "lef.y"
{ lefDumbMode = 10000000; lefRealNum = 1; ;
    break;}
case 110:
#line 1095 "lef.y"
{
      lefDumbMode = 0;
      lefRealNum = 0;
    ;
    break;}
case 111:
#line 1100 "lef.y"
{
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addAccurrentDensity(yyvsp[0].string);
    ;
    break;}
case 113:
#line 1113 "lef.y"
{
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
           lefrLayer.lefiLayer::addAccurrentDensity(yyvsp[-2].string);
           lefrLayer.lefiLayer::setAcOneEntry(yyvsp[-1].dval);
      }
    ;
    break;}
case 114:
#line 1128 "lef.y"
{
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::setDcOneEntry(yyvsp[-1].dval);
      }
    ;
    break;}
case 115:
#line 1143 "lef.y"
{
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (!layerCut) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CUTAREA can only be defined in LAYER with type CUT");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::addNumber(yyvsp[0].dval);
      }
    ;
    break;}
case 116:
#line 1166 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addDcCutarea(); ;
    break;}
case 118:
#line 1169 "lef.y"
{
      if (layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("WIDTH can only be defined in LAYER with type ROUTING");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::addNumber(yyvsp[0].dval);
      }
    ;
    break;}
case 119:
#line 1192 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addDcWidth(); ;
    break;}
case 121:
#line 1197 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaRatio(yyvsp[-1].dval);
    ;
    break;}
case 122:
#line 1227 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNADIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
              CHKERR();
            }
         }
      }
      antennaType = lefiAntennaDAR; 
    ;
    break;}
case 124:
#line 1258 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumAreaRatio(yyvsp[-1].dval);
    ;
    break;}
case 125:
#line 1288 "lef.y"
{  /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMDIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
              CHKERR();
            }
         }
      }
      antennaType = lefiAntennaCDAR;
    ;
    break;}
case 127:
#line 1319 "lef.y"
{ /* both 5.3  & 5.4 syntax */
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
              CHKERR();
            }
         }
      }
      /* this does not need to check, since syntax is in both 5.3 & 5.4 */
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaFactor(yyvsp[0].dval);
      antennaType = lefiAntennaAF;
    ;
    break;}
case 129:
#line 1334 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNASIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaSideAreaRatio(yyvsp[-1].dval);
    ;
    break;}
case 130:
#line 1364 "lef.y"
{  /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNADIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      antennaType = lefiAntennaDSAR;
    ;
    break;}
case 132:
#line 1395 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumSideAreaRatio(yyvsp[-1].dval);
    ;
    break;}
case 133:
#line 1425 "lef.y"
{  /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMDIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      antennaType = lefiAntennaCDSAR;
    ;
    break;}
case 135:
#line 1456 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNASIDEAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaSideAreaFactor(yyvsp[0].dval);
      antennaType = lefiAntennaSAF;
    ;
    break;}
case 137:
#line 1488 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && !layerCut && layerMastOver) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAMODEL can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      } else if (use5_3) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ANTENNAMODEL is a 5.5 syntax.  Your lef file has both old and new syntax, which is illegal."); 
              CHKERR();
            }
         }
      }
      antennaType = lefiAntennaO;
    ;
    break;}
case 139:
#line 1519 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SLOTWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireWidth(yyvsp[-1].dval);
    ;
    break;}
case 140:
#line 1533 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SLOTWIRELENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireLength(yyvsp[-1].dval);
    ;
    break;}
case 141:
#line 1547 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SLOTWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWidth(yyvsp[-1].dval);
    ;
    break;}
case 142:
#line 1561 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SLOTLENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotLength(yyvsp[-1].dval);
    ;
    break;}
case 143:
#line 1575 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MAXADJACENTSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxAdjacentSlotSpacing(yyvsp[-1].dval);
    ;
    break;}
case 144:
#line 1589 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MAXCOAXIALSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxCoaxialSlotSpacing(yyvsp[-1].dval);
    ;
    break;}
case 145:
#line 1603 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MAXEDGESLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxEdgeSlotSpacing(yyvsp[-1].dval);
    ;
    break;}
case 146:
#line 1617 "lef.y"
{ /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SPLITWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSplitWireWidth(yyvsp[-1].dval);
    ;
    break;}
case 147:
#line 1631 "lef.y"
{ /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MINIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinimumDensity(yyvsp[-1].dval);
    ;
    break;}
case 148:
#line 1645 "lef.y"
{ /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MAXIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaximumDensity(yyvsp[-1].dval);
    ;
    break;}
case 149:
#line 1659 "lef.y"
{ /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DENSITYCHECKWINDOW is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDensityCheckWindow(yyvsp[-2].dval, yyvsp[-1].dval);
    ;
    break;}
case 150:
#line 1673 "lef.y"
{ /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DENSITYCHECKSTEP is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDensityCheckStep(yyvsp[-1].dval);
    ;
    break;}
case 151:
#line 1687 "lef.y"
{ /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("FILLACTIVESPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setFillActiveSpacing(yyvsp[-1].dval);
    ;
    break;}
case 152:
#line 1701 "lef.y"
{
      // 5.5 MAXWIDTH, is for routing layer only
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MAXWIDTH can only be defined in LAYER with TYPE ROUTING.  Parser stops executions");
              CHKERR();
            }
         }
      }
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MAXWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxwidth(yyvsp[-1].dval);
    ;
    break;}
case 153:
#line 1722 "lef.y"
{
      // 5.5 MINWIDTH, is for routing layer only
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MINWIDTH can only be defined in LAYER with TYPE ROUTING.  Parser stops executions");
              CHKERR();
            }
         }
      }
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MINWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinwidth(yyvsp[-1].dval);
    ;
    break;}
case 154:
#line 1743 "lef.y"
{
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("MINENCLOSEDAREA is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinenclosedarea(yyvsp[0].dval);
    ;
    break;}
case 156:
#line 1756 "lef.y"
{ /* pcr 409334 */
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcut((int)yyvsp[-2].dval, yyvsp[0].dval); 
      hasLayerMincut = 0;
    ;
    break;}
case 157:
#line 1762 "lef.y"
{
      if (!hasLayerMincut) {   /* FROMABOVE nor FROMBELOW is set */
         if (lefrLayerCbk)
             lefrLayer.lefiLayer::addMinimumcutConnect((char*)"");
      }
    ;
    break;}
case 158:
#line 1769 "lef.y"
{
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstep(yyvsp[0].dval);
    ;
    break;}
case 159:
#line 1773 "lef.y"
{
    ;
    break;}
case 160:
#line 1776 "lef.y"
{
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("PROTRUSION RULE is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setProtrusion(yyvsp[-5].dval, yyvsp[-3].dval, yyvsp[-1].dval);
    ;
    break;}
case 161:
#line 1788 "lef.y"
{
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("SPACINGTABLE is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      // 5.5 either SPACING or SPACINGTABLE in a layer, not both
      if (lefrHasSpacing && layerRout) {
         if (lefrLayerCbk)  /* write warning only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
            }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpacingTable();
      lefrHasSpacingTbl = 1;
    ;
    break;}
case 163:
#line 1810 "lef.y"
{
      if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ENCLOSURE is a 5.6 syntax.  Your lef file is not defined as 5.6");
              CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::addEnclosure(yyvsp[-2].string, yyvsp[-1].dval, yyvsp[0].dval);
      }
    ;
    break;}
case 165:
#line 1826 "lef.y"
{
      if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("ENCLOSURE is a 5.6 syntax.  Your lef file is not defined as 5.6");
              CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::addPreferEnclosure(yyvsp[-2].string, yyvsp[-1].dval, yyvsp[0].dval);
      }
    ;
    break;}
case 167:
#line 1841 "lef.y"
{
      if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("RESISTANCE is a 5.6 syntax.  Your lef file is not defined as 5.6");
              CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::setResPerCut(yyvsp[-1].dval);
      }
    ;
    break;}
case 168:
#line 1855 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DIAGMINEDGELENGTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
              CHKERR();
            }
         }
      } else if (versionNum < 5.6) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("DIAGMINEDGELENGTH is a 5.6 syntax.  Your lef file is not defined as 5.6");
              CHKERR();
            }
         }
      } else {
         if (lefrLayerCbk)
            lefrLayer.lefiLayer::setDiagMinEdgeLength(yyvsp[-1].dval);
      }
    ;
    break;}
case 169:
#line 1876 "lef.y"
{
      // Use the polygon code to retrieve the points for MINSIZE
      lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
      lefrGeometriesPtr->lefiGeometries::Init();
      lefrDoGeometries = 1;
    ;
    break;}
case 170:
#line 1883 "lef.y"
{
      if (lefrLayerCbk) {
         lefrGeometriesPtr->lefiGeometries::addPolygon();
         lefrLayer.lefiLayer::setMinSize(lefrGeometriesPtr);
      }
      lefrDoGeometries = 0;
      lefrGeometriesPtr->lefiGeometries::Destroy();
                                       // Don't need the object anymore
      lefFree(lefrGeometriesPtr);
    ;
    break;}
case 171:
#line 1896 "lef.y"
{ 
      if (hasInfluence) {  // 5.5 - INFLUENCE table must follow a PARALLEL
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
              CHKERR();
            }
         }
      }
      if (hasParallel) { // 5.5 - Only one PARALLEL table is allowed per layer
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("Only one PARALLELRUNLENGTH table is allowed per layer");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval);
      hasParallel = 1;
    ;
    break;}
case 172:
#line 1917 "lef.y"
{
      spParallelLength = lefrLayer.lefiLayer::getNumber();
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelLength();
    ;
    break;}
case 173:
#line 1922 "lef.y"
{ 
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addSpParallelWidth(yyvsp[0].dval);
      }
    ;
    break;}
case 174:
#line 1928 "lef.y"
{ 
      if (lefrLayer.lefiLayer::getNumber() != spParallelLength) {
         if (lefrLayerCbk) {
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("The number of length in PARALLELRUNLENGTH is not the same as the number of spacing in WIDTH");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelWidthSpacing();
    ;
    break;}
case 176:
#line 1941 "lef.y"
{
      if (hasInfluence) {  // 5.5 - INFLUENCE table must follow a PARALLEL
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("Only one INFLUENCE table is allowed per layer");
              CHKERR();
            }
         }
      }
      if (!hasParallel) {  // 5.5 - INFLUENCE must follow a PARALLEL
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::setInfluence();
         lefrLayer.lefiLayer::addSpInfluence(yyvsp[-4].dval, yyvsp[-2].dval, yyvsp[0].dval);
      }
    ;
    break;}
case 178:
#line 1966 "lef.y"
{yyval.string = (char*)"NULL";;
    break;}
case 179:
#line 1967 "lef.y"
{yyval.string = (char*)"ABOVE";;
    break;}
case 180:
#line 1968 "lef.y"
{yyval.string = (char*)"BELOW";;
    break;}
case 182:
#line 1972 "lef.y"
{
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addEnclosureWidth(yyvsp[0].dval);
      }
    ;
    break;}
case 184:
#line 1980 "lef.y"
{
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addPreferEnclosureWidth(yyvsp[0].dval);
      }
    ;
    break;}
case 187:
#line 1994 "lef.y"
{
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("FROMABOVE is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      hasLayerMincut = 1;
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutConnect((char*)"FROMABOVE");

    ;
    break;}
case 188:
#line 2009 "lef.y"
{
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("FROMBELOW is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      hasLayerMincut = 1;
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutConnect((char*)"FROMBELOW");
    ;
    break;}
case 189:
#line 2023 "lef.y"
{   
      if (versionNum < 5.5) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("LENGTH WITHIN is a 5.5 syntax. Your lef file is not defined as 5.5");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutLengDis(yyvsp[-2].dval, yyvsp[0].dval);
    ;
    break;}
case 192:
#line 2044 "lef.y"
{
    if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstepType(yyvsp[0].string);
  ;
    break;}
case 193:
#line 2048 "lef.y"
{
    if (lefrLayerCbk) lefrLayer.lefiLayer::addMinstepLengthsum(yyvsp[0].dval);
  ;
    break;}
case 194:
#line 2053 "lef.y"
{yyval.string = (char*)"INSIDECORNER";;
    break;}
case 195:
#line 2054 "lef.y"
{yyval.string = (char*)"OUTSIDECORNER";;
    break;}
case 196:
#line 2055 "lef.y"
{yyval.string = (char*)"STEP";;
    break;}
case 197:
#line 2059 "lef.y"
{ if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaValue(antennaType, yyvsp[0].dval); ;
    break;}
case 198:
#line 2062 "lef.y"
{ if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */
          lefrAntennaPWLPtr = (lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
          lefrAntennaPWLPtr->lefiAntennaPWL::Init();
          lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL(yyvsp[-1].pt.x, yyvsp[-1].pt.y);
          lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL(yyvsp[0].pt.x, yyvsp[0].pt.y);
        }
      ;
    break;}
case 199:
#line 2070 "lef.y"
{ if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaPWL(antennaType, lefrAntennaPWLPtr);
      ;
    break;}
case 202:
#line 2083 "lef.y"
{ if (lefrLayerCbk)
      lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL(yyvsp[0].pt.x, yyvsp[0].pt.y);
  ;
    break;}
case 204:
#line 2089 "lef.y"
{ 
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        }
        else if ((antennaType == lefiAntennaAF) && (versionNum <= 5.3)) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file is not defined as 5.4");
                CHKERR();
              }
           }
        } else if (use5_3) {
           if (lefrLayerCbk) { /* write error only if cbk is set */
              if (layerWarnings++ < lefrLayerWarnings) {
                yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
                CHKERR();
              }
           }
        }
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaDUO(antennaType);
      ;
    break;}
case 205:
#line 2114 "lef.y"
{yyval.string = (char*)"PEAK";;
    break;}
case 206:
#line 2115 "lef.y"
{yyval.string = (char*)"AVERAGE";;
    break;}
case 207:
#line 2116 "lef.y"
{yyval.string = (char*)"RMS";;
    break;}
case 208:
#line 2120 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval); ;
    break;}
case 209:
#line 2122 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addAcFrequency(); ;
    break;}
case 210:
#line 2125 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval); ;
    break;}
case 211:
#line 2127 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addAcTableEntry(); ;
    break;}
case 213:
#line 2131 "lef.y"
{
      if (!layerCut) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("CUTAREA can only be defined in LAYER with type CUT");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval);
    ;
    break;}
case 214:
#line 2143 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addAcCutarea(); ;
    break;}
case 215:
#line 2145 "lef.y"
{
      if (!layerRout) {
         if (lefrLayerCbk) { /* write error only if cbk is set */
            if (layerWarnings++ < lefrLayerWarnings) {
              yyerror("WIDTH can only be defined in LAYER with type ROUTING");
              CHKERR();
            }
         }
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval);
    ;
    break;}
case 216:
#line 2157 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addAcWidth(); ;
    break;}
case 217:
#line 2161 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval); ;
    break;}
case 218:
#line 2163 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addDcTableEntry(); ;
    break;}
case 220:
#line 2167 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber(yyvsp[0].dval); ;
    break;}
case 223:
#line 2176 "lef.y"
{
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType(yyvsp[-1].string);
        lefrLayer.lefiLayer::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 224:
#line 2184 "lef.y"
{
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType(yyvsp[-1].string);
        lefrLayer.lefiLayer::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 225:
#line 2192 "lef.y"
{
      char temp[32];
      sprintf(temp, "%.11g", yyvsp[0].dval);
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType(yyvsp[-1].string);
        lefrLayer.lefiLayer::addNumProp(yyvsp[-1].string, yyvsp[0].dval, temp, propTp);
      }
    ;
    break;}
case 226:
#line 2204 "lef.y"
{ ;
    break;}
case 227:
#line 2206 "lef.y"
{ ;
    break;}
case 228:
#line 2209 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentPoint(yyvsp[-2].dval, yyvsp[-1].dval); ;
    break;}
case 231:
#line 2217 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::setCapacitancePoint(yyvsp[-2].dval, yyvsp[-1].dval); ;
    break;}
case 233:
#line 2222 "lef.y"
{ ;
    break;}
case 234:
#line 2225 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::setResistancePoint(yyvsp[-2].dval, yyvsp[-1].dval); ;
    break;}
case 235:
#line 2228 "lef.y"
{yyval.string = (char*)"ROUTING"; layerRout = 1;;
    break;}
case 236:
#line 2229 "lef.y"
{yyval.string = (char*)"CUT"; layerCut = 1;;
    break;}
case 237:
#line 2230 "lef.y"
{yyval.string = (char*)"OVERLAP"; layerMastOver = 1;;
    break;}
case 238:
#line 2231 "lef.y"
{yyval.string = (char*)"MASTERSLICE"; layerMastOver = 1;;
    break;}
case 239:
#line 2232 "lef.y"
{yyval.string = (char*)"VIRTUAL";;
    break;}
case 240:
#line 2233 "lef.y"
{yyval.string = (char*)"IMPLANT";;
    break;}
case 241:
#line 2236 "lef.y"
{yyval.string = (char*)"HORIZONTAL";;
    break;}
case 242:
#line 2237 "lef.y"
{yyval.string = (char*)"VERTICAL";;
    break;}
case 243:
#line 2238 "lef.y"
{yyval.string = (char*)"DIAG45";;
    break;}
case 244:
#line 2239 "lef.y"
{yyval.string = (char*)"DIAG135";;
    break;}
case 246:
#line 2243 "lef.y"
{
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addMinenclosedareaWidth(yyvsp[0].dval);
    ;
    break;}
case 247:
#line 2250 "lef.y"
{
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(1);
    ;
    break;}
case 248:
#line 2255 "lef.y"
{
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(2);
    ;
    break;}
case 249:
#line 2260 "lef.y"
{
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(3);
    ;
    break;}
case 250:
#line 2265 "lef.y"
{
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(4);
    ;
    break;}
case 251:
#line 2271 "lef.y"
{ ;
    break;}
case 252:
#line 2276 "lef.y"
{ ;
    break;}
case 253:
#line 2279 "lef.y"
{ 
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addSpParallelWidth(yyvsp[0].dval);
      }
    ;
    break;}
case 254:
#line 2285 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelWidthSpacing(); ;
    break;}
case 255:
#line 2288 "lef.y"
{ ;
    break;}
case 256:
#line 2290 "lef.y"
{ ;
    break;}
case 257:
#line 2293 "lef.y"
{ if (lefrLayerCbk) lefrLayer.lefiLayer::addSpInfluence(yyvsp[-4].dval, yyvsp[-2].dval, yyvsp[0].dval); ;
    break;}
case 258:
#line 2296 "lef.y"
{
      if (!lefrHasLayer) {  /* 5.5 */
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             yyerror("MAXVIASTACK has to define after the LAYER");
             CHKERR();
           }
        }
      } else if (lefrHasMaxVS) {
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             yyerror("Only one MAXVIASTACK is allowed in a lef file");
             CHKERR();
           }
        }
      } else {
        if (lefrMaxStackViaCbk) {
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackVia((int)yyvsp[-1].dval);
           CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
        }
      }
      if (versionNum < 5.5) {
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             yyerror("MAXVIASTACK is a 5.5 syntax. Your lef file is not defined as 5.5");
             CHKERR();
           }
        }
      }
      lefrHasMaxVS = 1;
    ;
    break;}
case 259:
#line 2327 "lef.y"
{lefDumbMode = 2; lefNoNum= 2;;
    break;}
case 260:
#line 2329 "lef.y"
{
      if (!lefrHasLayer) {  /* 5.5 */
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             yyerror("MAXVIASTACK has to define after the LAYER");
             CHKERR();
           }
        }
      } else if (lefrHasMaxVS) {
        if (lefrMaxStackViaCbk) { /* write error only if cbk is set */
           if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
             yyerror("Only one MAXVIASTACK is allowed in a lef file");
             CHKERR();
           }
        }
      } else {
        if (lefrMaxStackViaCbk) {
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackVia((int)yyvsp[-5].dval);
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackViaRange(yyvsp[-2].string, yyvsp[-1].string);
           CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
        }
      }
      lefrHasMaxVS = 1;
    ;
    break;}
case 261:
#line 2354 "lef.y"
{ hasViaRule_layer = 0; ;
    break;}
case 262:
#line 2355 "lef.y"
{ 
      if (lefrViaCbk)
        CALLBACK(lefrViaCbk, lefrViaCbkType, &lefrVia);
    ;
    break;}
case 263:
#line 2361 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 264:
#line 2364 "lef.y"
{
      /* 0 is nodefault */
      if (lefrViaCbk) lefrVia.lefiVia::setName(yyvsp[0].string, 0);
      viaLayer = 0;
      numVia++;
      //strcpy(viaName, $2);
      viaName = strdup(yyvsp[0].string);
    ;
    break;}
case 265:
#line 2373 "lef.y"
{
      /* 1 is default */
      if (lefrViaCbk) lefrVia.lefiVia::setName(yyvsp[-1].string, 1);
      viaLayer = 0;
      //strcpy(viaName, $2);
      viaName = strdup(yyvsp[-1].string);
    ;
    break;}
case 266:
#line 2381 "lef.y"
{
      /* 2 is generated */
      if (lefrViaCbk) lefrVia.lefiVia::setName(yyvsp[-1].string, 2);
      viaLayer = 0;
      //strcpy(viaName, $2);
      viaName = strdup(yyvsp[-1].string);
    ;
    break;}
case 267:
#line 2389 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 268:
#line 2391 "lef.y"
{lefDumbMode = 3; lefNoNum = 1; ;
    break;}
case 269:
#line 2394 "lef.y"
{
       if (versionNum < 5.6) {
         if (lefrViaCbk) { /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("VIARULE is a 5.6 syntax.  Your lef file is not defined as 5.6");
              CHKERR();
            }
         }
       } else {
         if (lefrViaCbk) lefrVia.lefiVia::setViaRule(yyvsp[-21].string, yyvsp[-18].dval, yyvsp[-17].dval, yyvsp[-13].string, yyvsp[-12].string, yyvsp[-11].string,
                         yyvsp[-8].dval, yyvsp[-7].dval, yyvsp[-4].dval, yyvsp[-3].dval, yyvsp[-2].dval, yyvsp[-1].dval);
       }
       viaLayer++;
       hasViaRule_layer = 1;
    ;
    break;}
case 273:
#line 2417 "lef.y"
{
       if (lefrViaCbk) lefrVia.lefiVia::setRowCol((int)yyvsp[-2].dval, (int)yyvsp[-1].dval);
    ;
    break;}
case 274:
#line 2421 "lef.y"
{
       if (lefrViaCbk) lefrVia.lefiVia::setOrigin(yyvsp[-2].dval, yyvsp[-1].dval);
    ;
    break;}
case 275:
#line 2425 "lef.y"
{
       if (lefrViaCbk) lefrVia.lefiVia::setOffset(yyvsp[-4].dval, yyvsp[-3].dval, yyvsp[-2].dval, yyvsp[-1].dval);
    ;
    break;}
case 276:
#line 2428 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 277:
#line 2429 "lef.y"
{
       if (lefrViaCbk) lefrVia.lefiVia::setPattern(yyvsp[-1].string);
    ;
    break;}
case 283:
#line 2446 "lef.y"
{ ;
    break;}
case 284:
#line 2448 "lef.y"
{ ;
    break;}
case 285:
#line 2450 "lef.y"
{ if (lefrViaCbk) lefrVia.lefiVia::setResistance(yyvsp[-1].dval); ;
    break;}
case 286:
#line 2451 "lef.y"
{ lefDumbMode = 1000000; lefRealNum = 1; ;
    break;}
case 287:
#line 2452 "lef.y"
{ lefDumbMode = 0;
      lefRealNum = 0;
    ;
    break;}
case 288:
#line 2456 "lef.y"
{ 
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setTopOfStack();
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
              yywarning("TOPOFSTACKONLY is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 291:
#line 2472 "lef.y"
{ 
      char temp[32];
      sprintf(temp, "%.11g", yyvsp[0].dval);
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType(yyvsp[-1].string);
         lefrVia.lefiVia::addNumProp(yyvsp[-1].string, yyvsp[0].dval, temp, propTp);
      }
    ;
    break;}
case 292:
#line 2482 "lef.y"
{
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType(yyvsp[-1].string);
         lefrVia.lefiVia::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 293:
#line 2490 "lef.y"
{
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType(yyvsp[-1].string);
         lefrVia.lefiVia::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 294:
#line 2500 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign(yyvsp[-1].string, 0, 0.0, 0.0, -1);
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
    ;
    break;}
case 295:
#line 2509 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign(yyvsp[-2].string, 1, yyvsp[-1].pt.x, yyvsp[-1].pt.y, -1);
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
    ;
    break;}
case 296:
#line 2518 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign(yyvsp[-3].string, 1, yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].integer);
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
    ;
    break;}
case 297:
#line 2527 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrViaCbk) lefrVia.lefiVia::setForeign(yyvsp[-2].string, 0, 0.0, 0.0, yyvsp[-1].integer);
      } else
        if (lefrViaCbk)  /* write warning only if cbk is set */
           if (viaWarnings++ < lefrViaWarnings)
             yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
    ;
    break;}
case 298:
#line 2536 "lef.y"
{lefDumbMode = 1; lefNoNum= 1;;
    break;}
case 299:
#line 2537 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 300:
#line 2540 "lef.y"
{yyval.integer = 0;;
    break;}
case 301:
#line 2541 "lef.y"
{yyval.integer = 1;;
    break;}
case 302:
#line 2542 "lef.y"
{yyval.integer = 2;;
    break;}
case 303:
#line 2543 "lef.y"
{yyval.integer = 3;;
    break;}
case 304:
#line 2544 "lef.y"
{yyval.integer = 4;;
    break;}
case 305:
#line 2545 "lef.y"
{yyval.integer = 5;;
    break;}
case 306:
#line 2546 "lef.y"
{yyval.integer = 6;;
    break;}
case 307:
#line 2547 "lef.y"
{yyval.integer = 7;;
    break;}
case 308:
#line 2548 "lef.y"
{yyval.integer = 0;;
    break;}
case 309:
#line 2549 "lef.y"
{yyval.integer = 1;;
    break;}
case 310:
#line 2550 "lef.y"
{yyval.integer = 2;;
    break;}
case 311:
#line 2551 "lef.y"
{yyval.integer = 3;;
    break;}
case 312:
#line 2552 "lef.y"
{yyval.integer = 4;;
    break;}
case 313:
#line 2553 "lef.y"
{yyval.integer = 5;;
    break;}
case 314:
#line 2554 "lef.y"
{yyval.integer = 6;;
    break;}
case 315:
#line 2555 "lef.y"
{yyval.integer = 7;;
    break;}
case 316:
#line 2558 "lef.y"
{ ;
    break;}
case 317:
#line 2560 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 318:
#line 2561 "lef.y"
{
      if (lefrViaCbk) lefrVia.lefiVia::addLayer(yyvsp[-1].string);
      viaLayer++;
      hasViaRule_layer = 1;
    ;
    break;}
case 321:
#line 2574 "lef.y"
{ if (lefrViaCbk)
        lefrVia.lefiVia::addRectToLayer(yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].pt.x, yyvsp[-1].pt.y); ;
    break;}
case 322:
#line 2577 "lef.y"
{
      lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
      lefrGeometriesPtr->lefiGeometries::Init();
      lefrDoGeometries = 1;
    ;
    break;}
case 323:
#line 2583 "lef.y"
{ 
      lefrGeometriesPtr->lefiGeometries::addPolygon();
      if (lefrViaCbk)
        lefrVia.lefiVia::addPolyToLayer(lefrGeometriesPtr);   // 5.6
      lefrGeometriesPtr->lefiGeometries::clearPolyItems();  // free items fields
      lefFree((char*)(lefrGeometriesPtr)); // Don't need anymore, poly data has
      lefrDoGeometries = 0;                // copied
    ;
    break;}
case 324:
#line 2592 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 325:
#line 2593 "lef.y"
{ 
      // 10/17/2001 - Wanda da Rosa, PCR 404149
      //              Error if no layer in via
      if (!viaLayer) {
         if (lefrViaCbk) {  /* write error only if cbk is set */
            if (viaWarnings++ < lefrViaWarnings) {
              yyerror("VIA requires at least one layer");
              CHKERR();
            }
         }
      }
      if (strcmp(viaName, yyvsp[0].string) != 0) {
         lefFree(viaName);
         if (lefrViaCbk) { /* write error only if cbk is set */
            if (viaWarnings++ < lefrViaWarnings) {
              yyerror("END VIA name is different from VIA");
              CHKERR();
            }
         }
      } else
         lefFree(viaName);
    ;
    break;}
case 326:
#line 2616 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 327:
#line 2617 "lef.y"
{ 
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setName(yyvsp[0].string);
      viaRuleLayer = 0;
      //strcpy(viaRuleName, $3);
      viaRuleName = strdup(yyvsp[0].string);
      isGenerate = 0;
    ;
    break;}
case 328:
#line 2627 "lef.y"
{
      // 10/17/2001 - Wanda da Rosa, PCR 404163
      //              Error if layer number is not equal 2.
      // 11/14/2001 - Wanda da Rosa,
      //              Commented out for pcr 411781
      //if (viaRuleLayer != 2) {
         //yyerror("VIARULE requires two layers");
         //CHKERR();
      //}
      if (viaRuleLayer == 0 || viaRuleLayer > 2) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("VIARULE requires two layers");
              CHKERR();
            }
         }
      }
      if (lefrViaRuleCbk)
        CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
      // 2/19/2004 - reset the ENCLOSURE overhang values which may be
      // set by the old syntax OVERHANG -- Not necessary, but just incase
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::clearLayerOverhang();
    ;
    break;}
case 329:
#line 2653 "lef.y"
{
      isGenerate = 1;
    ;
    break;}
case 330:
#line 2657 "lef.y"
{
      // 10/17/2001 - Wanda da Rosa, PCR 404181
      //              Error if layer number is not equal 3.
      // 11/14/2001 - Wanda da Rosa,
      //              Commented out for pcr 411781
      //if (viaRuleLayer != 3) {
         //yyerror("VIARULE requires three layers");
         //CHKERR();
      //}
      if (viaRuleLayer == 0) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("VIARULE requires three layers");
              CHKERR();
            }
         }
      } else if ((viaRuleLayer < 3) && (versionNum >= 5.6)) {
         if (lefrViaRuleCbk)  /* write warning only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings)
              yywarning("turn-via is obsolete in 5.6. It will be ignored.");
      } else {
         if (lefrViaRuleCbk) {
            lefrViaRule.lefiViaRule::setGenerate();
            CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
         }
      }
      // 2/19/2004 - reset the ENCLOSURE overhang values which may be
      // set by the old syntax OVERHANG
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::clearLayerOverhang();
    ;
    break;}
case 332:
#line 2690 "lef.y"
{
      if (versionNum < 5.6) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("DEFAULT is a 5.6 syntax. Your lef file is not defined as 5.6");
              CHKERR();
            }
         }
      } else
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setDefault();
    ;
    break;}
case 339:
#line 2717 "lef.y"
{ lefDumbMode = 10000000; lefRealNum = 1; ;
    break;}
case 340:
#line 2718 "lef.y"
{ lefDumbMode = 0;
      lefRealNum = 0;
    ;
    break;}
case 343:
#line 2729 "lef.y"
{
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType(yyvsp[-1].string);
         lefrViaRule.lefiViaRule::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 344:
#line 2737 "lef.y"
{
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType(yyvsp[-1].string);
         lefrViaRule.lefiViaRule::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 345:
#line 2745 "lef.y"
{
      char temp[32];
      sprintf(temp, "%.11g", yyvsp[0].dval);
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType(yyvsp[-1].string);
         lefrViaRule.lefiViaRule::addNumProp(yyvsp[-1].string, yyvsp[0].dval, temp, propTp);
      }
    ;
    break;}
case 346:
#line 2756 "lef.y"
{
      // 10/18/2001 - Wanda da Rosa PCR 404181
      //              Make sure the 1st 2 layers in viarule has direction
      // 04/28/2004 - PCR 704072 - DIRECTION in viarule generate is
      //              obsoleted in 5.6
      if (versionNum >= 5.6) {
         if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
             !isGenerate) {
            if (lefrViaRuleCbk) {  /* write error only if cbk is set */
               if (viaRuleWarnings++ < lefrViaRuleWarnings) {
                 yyerror("VIARULE requires the DIRECTION construct in LAYER");
                 CHKERR(); 
               }
            }
         }
      } else {
         if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
             isGenerate) {
            if (lefrViaRuleCbk) {  /* write error only if cbk is set */
               if (viaRuleWarnings++ < lefrViaRuleWarnings) {
                 yyerror("VIARULE requires the DIRECTION construct in LAYER");
                 CHKERR(); 
               }
            }
         }
      }
      viaRuleLayer++;
    ;
    break;}
case 349:
#line 2792 "lef.y"
{ if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::addViaName(yyvsp[-1].string); ;
    break;}
case 350:
#line 2794 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 351:
#line 2795 "lef.y"
{ if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setLayer(yyvsp[-1].string);
      viaRuleHasDir = 0;
      viaRuleHasEnc = 0;
    ;
    break;}
case 354:
#line 2807 "lef.y"
{
      if (viaRuleHasEnc) {
        if (lefrViaRuleCbk) {  /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
             yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
             CHKERR();
           }
        }
      } else {
        if ((versionNum < 5.6) || (!isGenerate)) {
          if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setHorizontal();
        } else
          if (lefrViaRuleCbk)  /* write warning only if cbk is set */
             if (viaRuleWarnings++ < lefrViaRuleWarnings)
               yywarning("DIRECTION in VIARULE is obsolete in 5.6. It will be ignored.");
      }
      viaRuleHasDir = 1;
    ;
    break;}
case 355:
#line 2826 "lef.y"
{ 
      if (viaRuleHasEnc) {
        if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
             yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
             CHKERR();
           }
        }
      } else {
        if ((versionNum < 5.6) || (!isGenerate)) {
          if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setVertical();
        } else
          if (lefrViaRuleCbk) /* write warning only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings)
              yywarning("DIRECTION in VIARULE is obsolete in 5.6. It will be ignored.");
      }
      viaRuleHasDir = 1;
    ;
    break;}
case 356:
#line 2845 "lef.y"
{
      if (versionNum < 5.5) {
         if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
             yyerror("ENCLOSURE is a 5.5 syntax. Your lef file is not defined as 5.5");
             CHKERR();
           }
         }
      }
      // 2/19/2004 - Enforced the rule that ENCLOSURE can only be defined
      // in VIARULE GENERATE
      if (!isGenerate) {
         if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
             yyerror("ENCLOSURE can only be defined in VIARULE GENERATE");
             CHKERR();
           }
         }
      }
      if (viaRuleHasDir) {
         if (lefrViaRuleCbk) { /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
             yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
             CHKERR();
           }
         }
      } else {
         if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setEnclosure(yyvsp[-2].dval, yyvsp[-1].dval);
      }
      viaRuleHasEnc = 1;
    ;
    break;}
case 357:
#line 2877 "lef.y"
{ if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setWidth(yyvsp[-3].dval,yyvsp[-1].dval); ;
    break;}
case 358:
#line 2879 "lef.y"
{ if (lefrViaRuleCbk)
	lefrViaRule.lefiViaRule::setRect(yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].pt.x, yyvsp[-1].pt.y); ;
    break;}
case 359:
#line 2882 "lef.y"
{ if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setSpacing(yyvsp[-3].dval,yyvsp[-1].dval); ;
    break;}
case 360:
#line 2884 "lef.y"
{ if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setResistance(yyvsp[-1].dval); ;
    break;}
case 361:
#line 2886 "lef.y"
{
      if (!viaRuleHasDir) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("OVERHANG can only be defined with DIRECTION");
              CHKERR();
            }
         }
      }
      // 2/19/2004 - Enforced the rule that OVERHANG can only be defined
      // in VIARULE GENERATE after 5.3
      if ((versionNum > 5.3) && (!isGenerate)) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("OVERHANG can only be defined in VIARULE GENERATE");
              CHKERR();
            }
         }
      }
      if (versionNum < 5.6) {
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setOverhang(yyvsp[-1].dval);
      } else {
        if (lefrViaRuleCbk)  /* write warning only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings)
              yywarning("OVERHANG translated into similar ENCLOSURE rule");
        // In 5.6 & later, set it to either ENCLOSURE overhang1 or overhang2
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setOverhangToEnclosure(yyvsp[-1].dval);
      }
    ;
    break;}
case 362:
#line 2916 "lef.y"
{
      // 2/19/2004 - Enforced the rule that METALOVERHANG can only be defined
      // in VIARULE GENERATE
      if ((versionNum > 5.3) && (!isGenerate)) {
         if (lefrViaRuleCbk) {  /* write error only if cbk is set */
            if (viaRuleWarnings++ < lefrViaRuleWarnings) {
              yyerror("METALOVERHANG can only be defined in VIARULE GENERATE");
              CHKERR();
            }
         }
      }
      if (versionNum < 5.6) {
        if (!viaRuleHasDir) {
           if (lefrViaRuleCbk) {  /* write error only if cbk is set */
             if (viaRuleWarnings++ < lefrViaRuleWarnings) {
               yyerror("METALOVERHANG can only be defined with DIRECTION");
               CHKERR();
             } 
           }
        }
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setMetalOverhang(yyvsp[-1].dval);
      } else
        if (lefrViaRuleCbk)  /* write warning only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings)
             yywarning("METALOVERHANG is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 363:
#line 2943 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 364:
#line 2944 "lef.y"
{
      if (strcmp(viaRuleName, yyvsp[0].string) != 0) {
        lefFree(viaRuleName);
        if (lefrViaRuleCbk) {  /* write error only if cbk is set */
           if (viaRuleWarnings++ < lefrViaRuleWarnings) {
             yyerror("END VIARULE name is different from VIARULE");
             CHKERR();
           }
        }
      } else
        lefFree(viaRuleName);
    ;
    break;}
case 365:
#line 2958 "lef.y"
{ ;
    break;}
case 366:
#line 2961 "lef.y"
{
      hasSamenet = 0;
      if ((versionNum < 5.6) || (!ndRule)) {
        if (lefrSpacingBeginCbk)
	  CALLBACK(lefrSpacingBeginCbk, lefrSpacingBeginCbkType, 0);
      } else
        if (lefrSpacingBeginCbk)  /* write warning only if cbk is set */
           if (spacingWarnings++ < lefrSpacingWarnings)
             yywarning("SAMENET in NONDEFAULTRULE is obsolete in 5.6. It will be ignored");
    ;
    break;}
case 367:
#line 2973 "lef.y"
{
      if ((versionNum < 5.6) || (!ndRule)) {
        if ((versionNum <= 5.4) && (!hasSamenet)) {
           yyerror("SAMENET is required inside SPACING for any lef file with version 5.4 and earlier, but is not defined in the parsed lef file");
           CHKERR();
        }
        if (lefrSpacingEndCbk)
           CALLBACK(lefrSpacingEndCbk, lefrSpacingEndCbkType, 0);
      }
    ;
    break;}
case 370:
#line 2990 "lef.y"
{
      if ((versionNum < 5.6) || (!ndRule)) {
        if (lefrSpacingCbk) {
          lefrSpacing.lefiSpacing::set(yyvsp[-3].string, yyvsp[-2].string, yyvsp[-1].dval, 0);
          CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
        }
      }
    ;
    break;}
case 371:
#line 2999 "lef.y"
{
      if ((versionNum < 5.6) || (!ndRule)) {
        if (lefrSpacingCbk) {
	  lefrSpacing.lefiSpacing::set(yyvsp[-4].string, yyvsp[-3].string, yyvsp[-2].dval, 1);
	  CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
        }
      }
    ;
    break;}
case 372:
#line 3010 "lef.y"
{ lefDumbMode = 2; lefNoNum = 2; hasSamenet = 1; ;
    break;}
case 373:
#line 3013 "lef.y"
{ ;
    break;}
case 374:
#line 3016 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrIRDropBeginCbk) 
	  CALLBACK(lefrIRDropBeginCbk, lefrIRDropBeginCbkType, 0);
      } else
        if (lefrIRDropBeginCbk) /* write warning only if cbk is set */
          if (iRDropWarnings++ < lefrIRDropWarnings)
            yywarning("IRDROP is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 375:
#line 3027 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrIRDropEndCbk)
	  CALLBACK(lefrIRDropEndCbk, lefrIRDropEndCbkType, 0);
      }
    ;
    break;}
case 378:
#line 3041 "lef.y"
{ 
      if (versionNum < 5.4) {
        if (lefrIRDropCbk)
          CALLBACK(lefrIRDropCbk, lefrIRDropCbkType, &lefrIRDrop);
      }
    ;
    break;}
case 381:
#line 3054 "lef.y"
{ if (lefrIRDropCbk) lefrIRDrop.lefiIRDrop::setValues(yyvsp[-1].dval, yyvsp[0].dval); ;
    break;}
case 382:
#line 3057 "lef.y"
{ if (lefrIRDropCbk) lefrIRDrop.lefiIRDrop::setTableName(yyvsp[0].string); ;
    break;}
case 383:
#line 3060 "lef.y"
{
    if (versionNum < 5.4) {
       if (lefrMinFeatureCbk) {
         lefrMinFeature.lefiMinFeature::set(yyvsp[-2].dval, yyvsp[-1].dval);
         CALLBACK(lefrMinFeatureCbk, lefrMinFeatureCbkType, &lefrMinFeature);
       }
    } else
       if (lefrMinFeatureCbk) /* write warning only if cbk is set */
          if (minFeatureWarnings++ < lefrMinFeatureWarnings)
            yywarning("MINFEATURE is obsolete in 5.4. It will be ignored.");
  ;
    break;}
case 384:
#line 3073 "lef.y"
{
    if (versionNum < 5.4) {
       if (lefrDielectricCbk)
         CALLBACK(lefrDielectricCbk, lefrDielectricCbkType, yyvsp[-1].dval);
    } else
       if (lefrDielectricCbk) /* write warning only if cbk is set */
         if (dielectricWarnings++ < lefrDielectricWarnings)
           yywarning("DIELECTRIC is obsolete in 5.4. It will be ignored.");
  ;
    break;}
case 385:
#line 3083 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 386:
#line 3084 "lef.y"
{
    (void)lefSetNonDefault(yyvsp[0].string);
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::setName(yyvsp[0].string);
    ndLayer = 0;
    ndRule = 1;
    numVia = 0;
    //strcpy(nonDefaultRuleName, $3);
    nonDefaultRuleName = strdup(yyvsp[0].string);
  ;
    break;}
case 387:
#line 3094 "lef.y"
{lefNdRule = 1;;
    break;}
case 388:
#line 3095 "lef.y"
{
    // 10/18/2001 - Wanda da Rosa, PCR 404189
    //              At least 1 layer is required
    if ((!ndLayer) && (!lefrRelaxMode)) {
       if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
           yyerror("NONDEFAULTRULE requires at least one layer");
           CHKERR();
         }
       }
    }
    if ((!numVia) && (!lefrRelaxMode) && (versionNum < 5.6)) {
       // VIA is no longer a required statement in 5.6
       if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
           yyerror("NONDEFAULTRULE requires at least one via");
           CHKERR();
         }
       }
    }
    if (lefrNonDefaultCbk) {
      lefrNonDefault.lefiNonDefault::end();
      CALLBACK(lefrNonDefaultCbk, lefrNonDefaultCbkType, &lefrNonDefault);
    }
    ndRule = 0;
    lefDumbMode = 0;
    (void)lefUnsetNonDefault();
  ;
    break;}
case 389:
#line 3125 "lef.y"
{
      if ((nonDefaultRuleName) && (*nonDefaultRuleName != '\0'))
        lefFree(nonDefaultRuleName);
    ;
    break;}
case 390:
#line 3130 "lef.y"
{
      if (strcmp(nonDefaultRuleName, yyvsp[0].string) != 0) {
        lefFree(nonDefaultRuleName);
        if (lefrNonDefaultCbk) { /* write error only if cbk is set */
          if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
            yyerror("END NONDEFAULTRULE name is different from NONDEFAULTRULE");
            CHKERR();
          }
        }
      } else
        lefFree(nonDefaultRuleName);
    ;
    break;}
case 392:
#line 3147 "lef.y"
{
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
              yyerror("HARDSPACING is a 5.6 syntax, Your lef file is not defined as 5.6");
              CHKERR();
            }
          }
       } else 
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::setHardspacing();
    ;
    break;}
case 402:
#line 3176 "lef.y"
{
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
             yyerror("USEVIA is a 5.6 syntax, Your lef file is not defined as 5.6");
             CHKERR();
          }
       } else {
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::addUseVia(yyvsp[-1].string);
       }
    ;
    break;}
case 403:
#line 3189 "lef.y"
{
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
             if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               yyerror("USEVIARULE is a 5.6 syntax, Your lef file is not defined as 5.6");
               CHKERR();
             }
          }
       } else {
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::addUseViaRule(yyvsp[-1].string);
       }
    ;
    break;}
case 404:
#line 3204 "lef.y"
{
       if (versionNum < 5.6) {
          if (lefrNonDefaultCbk) { /* write error only if cbk is set */
             if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
               yyerror("MINCUTS is a 5.6 syntax, Your lef file is not defined as 5.6");
               CHKERR();
             }
          }
       } else {
          if (lefrNonDefaultCbk)
             lefrNonDefault.lefiNonDefault::addMinCuts(yyvsp[-2].string, (int)yyvsp[-1].dval);
       }
    ;
    break;}
case 405:
#line 3218 "lef.y"
{ lefDumbMode = 10000000; lefRealNum = 1; ;
    break;}
case 406:
#line 3219 "lef.y"
{ lefDumbMode = 0;
      lefRealNum = 0;
    ;
    break;}
case 409:
#line 3230 "lef.y"
{
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType(yyvsp[-1].string);
         lefrNonDefault.lefiNonDefault::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 410:
#line 3238 "lef.y"
{
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType(yyvsp[-1].string);
         lefrNonDefault.lefiNonDefault::addProp(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 411:
#line 3246 "lef.y"
{
      if (lefrNonDefaultCbk) {
         char temp[32];
         char propTp;
         sprintf(temp, "%.11g", yyvsp[0].dval);
         propTp = lefrNondefProp.lefiPropType::propType(yyvsp[-1].string);
         lefrNonDefault.lefiNonDefault::addNumProp(yyvsp[-1].string, yyvsp[0].dval, temp, propTp);
      }
    ;
    break;}
case 412:
#line 3256 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 413:
#line 3257 "lef.y"
{
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addLayer(yyvsp[0].string);
    ndLayer++;
    //strcpy(layerName, $3);
    layerName = strdup(yyvsp[0].string);
    ndLayerWidth = 0;
    ndLayerSpace = 0;
  ;
    break;}
case 414:
#line 3266 "lef.y"
{ 
    ndLayerWidth = 1;
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addWidth(yyvsp[-1].dval);
  ;
    break;}
case 415:
#line 3270 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 416:
#line 3271 "lef.y"
{
    if (strcmp(layerName, yyvsp[0].string) != 0) {
      lefFree(layerName);
      if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
           yyerror("END LAYER name is different from LAYER");
           CHKERR();
         }
      }
    } else
      lefFree(layerName);
    if (!ndLayerWidth) {
      if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
           yyerror("WIDTH is required in LAYER inside NONDEFULTRULE");
           CHKERR();
         }
      }
    }
    if (!ndLayerSpace && versionNum < 5.6) {   // 5.6, SPACING is optional
      if (lefrNonDefaultCbk) { /* write error only if cbk is set */
         if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
           yyerror("SPACING is required in LAYER inside NONDEFULTRULE");
           CHKERR();
         }
      }
    }
  ;
    break;}
case 419:
#line 3308 "lef.y"
{
      ndLayerSpace = 1;
      if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addSpacing(yyvsp[-1].dval);
    ;
    break;}
case 420:
#line 3313 "lef.y"
{ if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addWireExtension(yyvsp[-1].dval); ;
    break;}
case 421:
#line 3316 "lef.y"
{
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addResistance(yyvsp[-1].dval);
      } else if (versionNum < 5.4) {
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
              yyerror("RESISTANCE RPERSQ is a 5.4 syntax.  Your lef file is not defined as version 5.4");
              CHKERR();
            }
         }
      } else if (versionNum > 5.5) {  // obsolete in 5.6
         if (lefrNonDefaultCbk) /* write warning only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
              yywarning("RESISTANCE RPERSQ is obsolete in 5.6. It will be ignored.");
      } else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addResistance(yyvsp[-1].dval);
    ;
    break;}
case 422:
#line 3336 "lef.y"
{
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addCapacitance(yyvsp[-1].dval);
      } else if (versionNum < 5.4) {
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
              yyerror("CAPACITANCE CPERSQDIST is a 5.4 syntax.  Your lef file is not defined as version 5.4");
              CHKERR();
            }
         }
      } else if (versionNum > 5.5) { // obsolete in 5.6
         if (lefrNonDefaultCbk) /* write warning only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
              yywarning("CAPACITANCE CPERSQDIST is obsolete in 5.6. It will be ignored.");
      } else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addCapacitance(yyvsp[-1].dval);
    ;
    break;}
case 423:
#line 3355 "lef.y"
{
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addEdgeCap(yyvsp[-1].dval);
      } else if (versionNum < 5.4) {
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
              yyerror("EDGECAPACITANCE is a 5.4 syntax.  Your lef file is not defined as version 5.4");
              CHKERR();
            }
         }
      } else if (versionNum > 5.5) {  // obsolete in 5.6
         if (lefrNonDefaultCbk) /* write warning only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
              yywarning("EDGECAPACITANCE is obsolete in 5.6. It will be ignored.");
      } else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addEdgeCap(yyvsp[-1].dval);
    ;
    break;}
case 424:
#line 3374 "lef.y"
{
      if (versionNum < 5.6) {  // 5.6 syntax
         if (lefrNonDefaultCbk) { /* write error only if cbk is set */
            if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
              yyerror("DIAGWIDTH is a 5.6 syntax. Your lef file is not defined as 5.6");
              CHKERR(); 
            }
         }
      } else {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addDiagWidth(yyvsp[-1].dval);
      }
    ;
    break;}
case 425:
#line 3389 "lef.y"
{ 
      if (lefrSiteCbk)
        CALLBACK(lefrSiteCbk, lefrSiteCbkType, &lefrSite);
    ;
    break;}
case 426:
#line 3394 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 427:
#line 3395 "lef.y"
{ 
      if (lefrSiteCbk) lefrSite.lefiSite::setName(yyvsp[0].string);
      //strcpy(siteName, $3);
      siteName = strdup(yyvsp[0].string);
      hasSiteClass = 0;
      hasSiteSize = 0;
    ;
    break;}
case 428:
#line 3403 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 429:
#line 3404 "lef.y"
{
      if (strcmp(siteName, yyvsp[0].string) != 0) {
        lefFree(siteName);
        if (lefrSiteCbk) { /* write error only if cbk is set */
           if (siteWarnings++ < lefrSiteWarnings) {
             yyerror("END SITE name is different from SITE");
             CHKERR();
           }
        }
      } else {
        lefFree(siteName);
        if (lefrSiteCbk) { /* write error only if cbk is set */
          if (hasSiteClass == 0) {
            yyerror("CLASS in SITE is required");
            CHKERR();
          }
          if (hasSiteSize == 0) {
            yyerror("SIZE in SITE is required");
            CHKERR();
          }
        }
      }
    ;
    break;}
case 432:
#line 3435 "lef.y"
{
/* Workaround for pcr 640902
*/
      if (lefrSiteCbk) lefrSite.lefiSite::setSize(yyvsp[-3].dval,yyvsp[-1].dval);
      hasSiteSize = 1;
    ;
    break;}
case 433:
#line 3442 "lef.y"
{ ;
    break;}
case 434:
#line 3444 "lef.y"
{ 
      if (lefrSiteCbk) lefrSite.lefiSite::setClass(yyvsp[0].string);
      hasSiteClass = 1;
    ;
    break;}
case 435:
#line 3449 "lef.y"
{ ;
    break;}
case 436:
#line 3452 "lef.y"
{yyval.string = (char*)"PAD"; ;
    break;}
case 437:
#line 3453 "lef.y"
{yyval.string = (char*)"CORE"; ;
    break;}
case 438:
#line 3454 "lef.y"
{yyval.string = (char*)"VIRTUAL"; ;
    break;}
case 439:
#line 3457 "lef.y"
{ ;
    break;}
case 442:
#line 3466 "lef.y"
{ if (lefrSiteCbk) lefrSite.lefiSite::setXSymmetry(); ;
    break;}
case 443:
#line 3468 "lef.y"
{ if (lefrSiteCbk) lefrSite.lefiSite::setYSymmetry(); ;
    break;}
case 444:
#line 3470 "lef.y"
{ if (lefrSiteCbk) lefrSite.lefiSite::set90Symmetry(); ;
    break;}
case 445:
#line 3472 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 446:
#line 3474 "lef.y"
{ ;
    break;}
case 449:
#line 3481 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 450:
#line 3482 "lef.y"
{ if (lefrSiteCbk) lefrSite.lefiSite::addRowPattern(yyvsp[-2].string, yyvsp[-1].integer); ;
    break;}
case 451:
#line 3486 "lef.y"
{ yyval.pt.x = yyvsp[-1].dval; yyval.pt.y = yyvsp[0].dval; ;
    break;}
case 452:
#line 3488 "lef.y"
{ yyval.pt.x = yyvsp[-2].dval; yyval.pt.y = yyvsp[-1].dval; ;
    break;}
case 453:
#line 3491 "lef.y"
{ 
      if (lefrMacroCbk)
        CALLBACK(lefrMacroCbk, lefrMacroCbkType, &lefrMacro);
      lefrDoSite = 0;
    ;
    break;}
case 455:
#line 3498 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 456:
#line 3499 "lef.y"
{
      siteDef = 0;
      symDef = 0;
      sizeDef = 0; 
      pinDef = 0; 
      obsDef = 0; 
      origDef = 0;
      lefrMacro.lefiMacro::clear();      
      if (lefrMacroBeginCbk || lefrMacroCbk) {
        // some reader may not have MacroBeginCB, but has MacroCB set
        lefrMacro.lefiMacro::setName(yyvsp[0].string);
        CALLBACK(lefrMacroBeginCbk, lefrMacroBeginCbkType, yyvsp[0].string);
      }
      //strcpy(macroName, $3);
      macroName = strdup(yyvsp[0].string);
    ;
    break;}
case 457:
#line 3516 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 458:
#line 3517 "lef.y"
{
      if (strcmp(macroName, yyvsp[0].string) != 0) {
        lefFree(macroName);
        if (lefrMacroEndCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("END MACRO name is different from MACRO");
             CHKERR();
           }
        }
      } else
        lefFree(macroName);
      if (lefrMacroEndCbk)
        CALLBACK(lefrMacroEndCbk, lefrMacroEndCbkType, yyvsp[0].string);
    ;
    break;}
case 466:
#line 3547 "lef.y"
{ ;
    break;}
case 467:
#line 3549 "lef.y"
{ ;
    break;}
case 468:
#line 3551 "lef.y"
{ ;
    break;}
case 471:
#line 3555 "lef.y"
{ ;
    break;}
case 472:
#line 3557 "lef.y"
{ ;
    break;}
case 473:
#line 3559 "lef.y"
{ ;
    break;}
case 474:
#line 3561 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setBuffer(); ;
    break;}
case 475:
#line 3563 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setInverter(); ;
    break;}
case 476:
#line 3565 "lef.y"
{ ;
    break;}
case 477:
#line 3567 "lef.y"
{ ;
    break;}
case 478:
#line 3569 "lef.y"
{ ;
    break;}
case 479:
#line 3571 "lef.y"
{ ;
    break;}
case 480:
#line 3572 "lef.y"
{lefDumbMode = 1000000; lefRealNum = 1;;
    break;}
case 481:
#line 3573 "lef.y"
{ lefDumbMode = 0;
        lefRealNum = 0;
      ;
    break;}
case 484:
#line 3583 "lef.y"
{
      if (siteDef) { /* SITE is defined before SYMMETRY */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               yywarning("SITE is defined before SYMMETRY.");
      }
      symDef = 1;
    ;
    break;}
case 487:
#line 3600 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setXSymmetry(); ;
    break;}
case 488:
#line 3602 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setYSymmetry(); ;
    break;}
case 489:
#line 3604 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::set90Symmetry(); ;
    break;}
case 490:
#line 3608 "lef.y"
{
      char temp[32];
      sprintf(temp, "%.11g", yyvsp[0].dval);
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType(yyvsp[-1].string);
         lefrMacro.lefiMacro::setNumProperty(yyvsp[-1].string, yyvsp[0].dval, temp,  propTp);
      }
    ;
    break;}
case 491:
#line 3618 "lef.y"
{
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType(yyvsp[-1].string);
         lefrMacro.lefiMacro::setProperty(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 492:
#line 3626 "lef.y"
{
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType(yyvsp[-1].string);
         lefrMacro.lefiMacro::setProperty(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 493:
#line 3635 "lef.y"
{
       if (lefrMacroCbk) lefrMacro.lefiMacro::setClass(yyvsp[-1].string);
       if (lefrMacroClassTypeCbk)
          CALLBACK(lefrMacroClassTypeCbk, lefrMacroClassTypeCbkType, yyvsp[-1].string);
    ;
    break;}
case 494:
#line 3642 "lef.y"
{yyval.string = (char*)"COVER"; ;
    break;}
case 495:
#line 3644 "lef.y"
{ yyval.string = (char*)"COVER BUMP";
      if (versionNum < 5.5) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             if (lefrRelaxMode)
                yywarning("The statement COVER BUMP is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
              else
                yyerror("COVER BUMP is a 5.5 syntax.  Your lef file is not defined as 5.5 or later");
             CHKERR();
           }
        }
      }
    ;
    break;}
case 496:
#line 3657 "lef.y"
{yyval.string = (char*)"RING"; ;
    break;}
case 497:
#line 3658 "lef.y"
{yyval.string = (char*)"BLOCK"; ;
    break;}
case 498:
#line 3660 "lef.y"
{ yyval.string = (char*)"BLOCK BLACKBOX";
      if (versionNum < 5.5) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             if (lefrRelaxMode)
                yywarning("The statement BLOCK BLACKBOX is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
              else
                yyerror("BLOCK BLACKBOX is a 5.5 syntax.  Your lef file is not defined as 5.5 or later");
             CHKERR();
           }
        }
      }
    ;
    break;}
case 499:
#line 3674 "lef.y"
{
      if (ignoreVersion) {
        yyval.string = (char*)"BLOCK SOFT";
      } else if (versionNum < 5.6) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("BLOCK SOFT is a 5.6 syntax.  Your lef file is not defined as version 5.6");
             CHKERR();
           }
        }
      }
      else
        yyval.string = (char*)"BLOCK SOFT";
    ;
    break;}
case 500:
#line 3688 "lef.y"
{yyval.string = (char*)"NONE"; ;
    break;}
case 501:
#line 3689 "lef.y"
{yyval.string = (char*)"PAD"; ;
    break;}
case 502:
#line 3690 "lef.y"
{yyval.string = (char*)"VIRTUAL"; ;
    break;}
case 503:
#line 3692 "lef.y"
{  sprintf(temp_name, "PAD %s", yyvsp[0].string);
        yyval.string = temp_name; 
        if (versionNum < 5.5) {
           if (strcmp("AREAIO", yyvsp[0].string) != 0) {
             sprintf(temp_name, "PAD %s", yyvsp[0].string);
             yyval.string = temp_name; 
           } else if (lefrMacroCbk) { 
             if (macroWarnings++ < lefrMacroWarnings) {
               if (lefrRelaxMode)
                 yywarning("The statement PAD AREAIO is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
               else
                 yyerror("PAD AREAIO is a 5.5 syntax.  Your lef file is not defined as 5.5 or later");
              CHKERR();
            }
          }
        }
      ;
    break;}
case 504:
#line 3709 "lef.y"
{yyval.string = (char*)"CORE"; ;
    break;}
case 505:
#line 3711 "lef.y"
{yyval.string = (char*)"CORNER";
      /* This token is NOT in the spec but has shown up in 
       * some lef files.  This exception came from LEFOUT
       * in 'frameworks'
       */
      ;
    break;}
case 506:
#line 3718 "lef.y"
{sprintf(temp_name, "CORE %s", yyvsp[0].string);
      yyval.string = temp_name;;
    break;}
case 507:
#line 3721 "lef.y"
{sprintf(temp_name, "ENDCAP %s", yyvsp[0].string);
      yyval.string = temp_name;;
    break;}
case 508:
#line 3726 "lef.y"
{yyval.string = (char*)"INPUT";;
    break;}
case 509:
#line 3727 "lef.y"
{yyval.string = (char*)"OUTPUT";;
    break;}
case 510:
#line 3728 "lef.y"
{yyval.string = (char*)"INOUT";;
    break;}
case 511:
#line 3729 "lef.y"
{yyval.string = (char*)"POWER";;
    break;}
case 512:
#line 3730 "lef.y"
{yyval.string = (char*)"SPACER";;
    break;}
case 513:
#line 3731 "lef.y"
{yyval.string = (char*)"AREAIO";;
    break;}
case 514:
#line 3734 "lef.y"
{yyval.string = (char*)"FEEDTHRU";;
    break;}
case 515:
#line 3735 "lef.y"
{yyval.string = (char*)"TIEHIGH";;
    break;}
case 516:
#line 3736 "lef.y"
{yyval.string = (char*)"TIELOW";;
    break;}
case 517:
#line 3738 "lef.y"
{ 
      if (ignoreVersion) {
        yyval.string = (char*)"SPACER";
      } else if (versionNum < 5.4) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("SPACER is a 5.4 syntax.  Your lef file is not defined as version 5.4"); 
             CHKERR();
           }
        }
      }
      else
        yyval.string = (char*)"SPACER";
    ;
    break;}
case 518:
#line 3753 "lef.y"
{ 
      if (ignoreVersion) {
        yyval.string = (char*)"ANTENNACELL";
      } else if (versionNum < 5.4) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("ANTENNACELL is a 5.4 syntax.  Your lef file is not defined as version 5.4"); 
             CHKERR();
           }
        }
      }
      else
        yyval.string = (char*)"ANTENNACELL";
    ;
    break;}
case 519:
#line 3768 "lef.y"
{ 
      if (ignoreVersion) {
        yyval.string = (char*)"WELLTAP";
      } else if (versionNum < 5.6) {
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("WELLTAP is a 5.6 syntax.  Your lef file is not defined as version 5.6"); 
             CHKERR();
           }
        }
      }
      else
        yyval.string = (char*)"WELLTAP";
    ;
    break;}
case 520:
#line 3784 "lef.y"
{yyval.string = (char*)"PRE";;
    break;}
case 521:
#line 3785 "lef.y"
{yyval.string = (char*)"POST";;
    break;}
case 522:
#line 3786 "lef.y"
{yyval.string = (char*)"TOPLEFT";;
    break;}
case 523:
#line 3787 "lef.y"
{yyval.string = (char*)"TOPRIGHT";;
    break;}
case 524:
#line 3788 "lef.y"
{yyval.string = (char*)"BOTTOMLEFT";;
    break;}
case 525:
#line 3789 "lef.y"
{yyval.string = (char*)"BOTTOMRIGHT";;
    break;}
case 526:
#line 3792 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setGenerator(yyvsp[-1].string); ;
    break;}
case 527:
#line 3795 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setGenerate(yyvsp[-2].string, yyvsp[-1].string); ;
    break;}
case 528:
#line 3799 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("USER");
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             yywarning("SOURCE is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 529:
#line 3808 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("GENERATE");
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             yywarning("SOURCE is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 530:
#line 3817 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("BLOCK");
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             yywarning("SOURCE is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 531:
#line 3827 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setPower(yyvsp[-1].dval);
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 532:
#line 3837 "lef.y"
{ 
       if (origDef) { /* Has multiple ORIGIN defined in a macro, stop parsing*/
          if (lefrMacroCbk) { /* write error only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings) {
               yyerror("ORIGIN statement is defined more than once.  Parser stops executions.");
               CHKERR();
             }
          }
       }
       origDef = 1;
       if (siteDef) { /* SITE is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               yywarning("SITE is defined before ORIGIN.");
       }
       if (pinDef) { /* PIN is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               yywarning("PIN is defined before ORIGIN.");
       }
       if (obsDef) { /* OBS is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          if (lefrMacroCbk) /* write warning only if cbk is set */
             if (macroWarnings++ < lefrMacroWarnings)
               yywarning("OBS is defined before ORIGIN.");
       }
       /* 11/22/99 - Wanda da Rosa. PCR 283846 
                     can be defined any order.
       if (symDef)  * SYMMETRY is defined before ORIGIN *
          yywarning("SYMMETRY is defined before ORIGIN.");
       */
       /* Add back it back in per Nora request on PCR 283846 */
       /* 1/14/2000 - Wanda da Rosa, PCR 288770
       if (sizeDef)  * SIZE is defined before ORIGIN *
          yywarning("SIZE is defined before ORIGIN.");
       */
      
       /* Workaround for pcr 640902 */
       if (lefrMacroCbk) lefrMacro.lefiMacro::setOrigin(yyvsp[-1].pt.x, yyvsp[-1].pt.y);
       if (lefrMacroOriginCbk) {
          macroNum.x = yyvsp[-1].pt.x; 
          macroNum.y = yyvsp[-1].pt.y; 
          CALLBACK(lefrMacroOriginCbk, lefrMacroOriginCbkType, macroNum);
       }
    ;
    break;}
case 533:
#line 3887 "lef.y"
{ if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign(yyvsp[-1].string, 0, 0.0, 0.0, -1);
    ;
    break;}
case 534:
#line 3891 "lef.y"
{ if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign(yyvsp[-2].string, 1, yyvsp[-1].pt.x, yyvsp[-1].pt.y, -1);
    ;
    break;}
case 535:
#line 3895 "lef.y"
{ if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign(yyvsp[-3].string, 1, yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].integer);
    ;
    break;}
case 536:
#line 3899 "lef.y"
{ if (lefrMacroCbk)
      lefrMacro.lefiMacro::addForeign(yyvsp[-2].string, 0, 0.0, 0.0, yyvsp[-1].integer);
    ;
    break;}
case 537:
#line 3903 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 538:
#line 3904 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setEEQ(yyvsp[-1].string); ;
    break;}
case 539:
#line 3906 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 540:
#line 3907 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setLEQ(yyvsp[-1].string);
      } else
        if (lefrMacroCbk) /* write warning only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings)
             yywarning("LEQ in MACRO is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 541:
#line 3918 "lef.y"
{
      if (lefrMacroCbk) {
        lefrMacro.lefiMacro::setSiteName(yyvsp[-1].string);
/* For later than 5.6 release
        lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
                              sizeof(lefiSitePattern));
        lefrSitePatternPtr->lefiSitePattern::Init();
        lefrSitePatternPtr->lefiSitePattern::setSiteName($2);
*/
      }
    ;
    break;}
case 542:
#line 3930 "lef.y"
{
      if (lefrMacroCbk) {
        /* also set site name in the variable siteName_ in lefiMacro */
        /* this, if user wants to use method siteName will get the name also */
        /* Does not work, it will only set with the last sitename, if multiple
           SITEs are defined.  Therefore, data will not be correct
        lefrMacro.lefiMacro::setSitePatternName(
            lefrSitePatternPtr->lefiSitePattern::name());
        */
	lefrMacro.lefiMacro::setSitePattern(lefrSitePatternPtr);
	lefrSitePatternPtr = 0;
      }
    ;
    break;}
case 543:
#line 3945 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; siteDef = 1;
        if (lefrMacroCbk) lefrDoSite = 1; ;
    break;}
case 544:
#line 3949 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 545:
#line 3952 "lef.y"
{ 
      if (siteDef) { /* SITE is defined before SIZE */
         /* pcr 283846 suppress warning
         if (siteWarnings++ < lefrSiteWarnings)
           yywarning("SITE is defined before SIZE.");
         return 1; 
         */
      }
      sizeDef = 1;
      if (lefrMacroCbk) lefrMacro.lefiMacro::setSize(yyvsp[-3].dval, yyvsp[-1].dval);
      if (lefrMacroSizeCbk) {
         macroNum.x = yyvsp[-3].dval; 
         macroNum.y = yyvsp[-1].dval; 
         CALLBACK(lefrMacroSizeCbk, lefrMacroSizeCbkType, macroNum);
      }
    ;
    break;}
case 546:
#line 3973 "lef.y"
{ 
      if (lefrPinCbk)
        CALLBACK(lefrPinCbk, lefrPinCbkType, &lefrPin);
      lefrPin.lefiPin::clear();
    ;
    break;}
case 547:
#line 3979 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; pinDef = 1;;
    break;}
case 548:
#line 3980 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setName(yyvsp[0].string);
      //strcpy(pinName, $3);
      pinName = strdup(yyvsp[0].string);
    ;
    break;}
case 549:
#line 3985 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 550:
#line 3986 "lef.y"
{
      if (strcmp(pinName, yyvsp[0].string) != 0) {
        lefFree(pinName);
        if (lefrMacroCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("END PIN name is different from PIN");
             CHKERR();
           }
        }
      } else
        lefFree(pinName);
    ;
    break;}
case 551:
#line 4001 "lef.y"
{ ;
    break;}
case 552:
#line 4003 "lef.y"
{ ;
    break;}
case 553:
#line 4007 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign(yyvsp[-1].string, 0, 0.0, 0.0, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 554:
#line 4016 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign(yyvsp[-2].string, 1, yyvsp[-1].pt.x, yyvsp[-1].pt.y, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 555:
#line 4025 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign(yyvsp[-3].string, 1, yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].integer);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 556:
#line 4034 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign(yyvsp[-2].string, 0, 0.0, 0.0, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 557:
#line 4043 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign(yyvsp[-3].string, 1, yyvsp[-1].pt.x, yyvsp[-1].pt.y, -1);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 558:
#line 4052 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::addForeign(yyvsp[-4].string, 1, yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].integer);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 559:
#line 4060 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 560:
#line 4061 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) lefrPin.lefiPin::setLEQ(yyvsp[-1].string);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("LEQ in MACRO PIN is obsolete in 5.6. It will be ignored.");
   ;
    break;}
case 561:
#line 4070 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setPower(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 562:
#line 4079 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setDirection(yyvsp[0].string); ;
    break;}
case 563:
#line 4081 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setUse(yyvsp[-1].string); ;
    break;}
case 564:
#line 4083 "lef.y"
{ ;
    break;}
case 565:
#line 4085 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setLeakage(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO LEAKAGE is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 566:
#line 4094 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseThresh(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO RISETHRESH is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 567:
#line 4103 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallThresh(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO FALLTHRESH is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 568:
#line 4112 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseSatcur(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO RISESATCUR is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 569:
#line 4121 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallSatcur(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO FALLSATCUR is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 570:
#line 4130 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setVLO(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO VLO is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 571:
#line 4139 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setVHI(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO VHI is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 572:
#line 4148 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setTieoffr(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO TIEOFFR is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 573:
#line 4157 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setShape(yyvsp[-1].string); ;
    break;}
case 574:
#line 4158 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 575:
#line 4159 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setMustjoin(yyvsp[-1].string); ;
    break;}
case 576:
#line 4160 "lef.y"
{lefDumbMode = 1;;
    break;}
case 577:
#line 4161 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setOutMargin(yyvsp[-2].dval, yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO OUTPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 578:
#line 4170 "lef.y"
{lefDumbMode = 1;;
    break;}
case 579:
#line 4171 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setOutResistance(yyvsp[-2].dval, yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO OUTPUTRESISTANCE is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 580:
#line 4180 "lef.y"
{lefDumbMode = 1;;
    break;}
case 581:
#line 4181 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setInMargin(yyvsp[-2].dval, yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO INPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 582:
#line 4191 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCapacitance(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO CAPACITANCE is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 583:
#line 4200 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setMaxdelay(yyvsp[-1].dval); ;
    break;}
case 584:
#line 4202 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setMaxload(yyvsp[-1].dval); ;
    break;}
case 585:
#line 4204 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setResistance(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO RESISTANCE is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 586:
#line 4213 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setPulldownres(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO PULLDOWNRES is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 587:
#line 4222 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCurrentSource("ACTIVE");
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 588:
#line 4232 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCurrentSource("RESISTIVE");
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 589:
#line 4242 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseVoltage(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO RISEVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 590:
#line 4252 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallVoltage(yyvsp[-1].dval);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning(
             "MACRO FALLVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 591:
#line 4262 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setTables(yyvsp[-2].string, yyvsp[-1].string);
      } else
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("MACRO IV_TABLES is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 592:
#line 4271 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setTaperRule(yyvsp[-1].string); ;
    break;}
case 593:
#line 4272 "lef.y"
{lefDumbMode = 1000000; lefRealNum = 1;;
    break;}
case 594:
#line 4273 "lef.y"
{ lefDumbMode = 0;
      lefRealNum = 0;
    ;
    break;}
case 595:
#line 4277 "lef.y"
{
      lefDumbMode = 0;
      if (lefrPinCbk) {
	lefrPin.lefiPin::addPort(lefrGeometriesPtr);
	lefrGeometriesPtr = 0;
	lefrDoGeometries = 0;
      }
      if ((needGeometry) && (needGeometry != 2))  // if the last LAYER in PORT
        if (lefrPinCbk) /* write warning only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings)
             yywarning("Either PATH, RECT or POLYGON is a required in MACRO/PIN/PORT.");
    ;
    break;}
case 596:
#line 4293 "lef.y"
{
      // Since in start_macro_port it has call the Init method, here
      // we need to call the Destroy method.
      // Still add a null pointer to set the number of port
      if (lefrPinCbk) {
        lefrPin.lefiPin::addPort(lefrGeometriesPtr);
        lefrGeometriesPtr = 0;
        lefrDoGeometries = 0;
      }
    ;
    break;}
case 597:
#line 4304 "lef.y"
{  /* a pre 5.4 syntax */
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrPinCbk) { /* write error only if cbk is set */
             if (pinWarnings++ < lefrPinWarnings) {
               yyerror("ANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
               CHKERR();
             }
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaSize(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 598:
#line 4321 "lef.y"
{  /* a pre 5.4 syntax */
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrPinCbk) { /* write error only if cbk is set */
              if (pinWarnings++ < lefrPinWarnings) {
                yyerror("ANTENNAMETALAREA is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
                CHKERR();
              }
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMetalArea(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 599:
#line 4338 "lef.y"
{ /* a pre 5.4 syntax */ 
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           if (lefrPinCbk) { /* write error only if cbk is set */
              if (pinWarnings++ < lefrPinWarnings) {
                yyerror("ANTENNAMETALLENGTH is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
                CHKERR();
              }
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMetalLength(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 600:
#line 4355 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setRiseSlewLimit(yyvsp[-1].dval); ;
    break;}
case 601:
#line 4357 "lef.y"
{ if (lefrPinCbk) lefrPin.lefiPin::setFallSlewLimit(yyvsp[-1].dval); ;
    break;}
case 602:
#line 4359 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialMetalArea(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 603:
#line 4381 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialMetalSideArea(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 604:
#line 4403 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialCutArea(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 605:
#line 4425 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaDiffArea(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 606:
#line 4447 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaGateArea(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 607:
#line 4469 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxAreaCar(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 608:
#line 4491 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxSideAreaCar(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 609:
#line 4513 "lef.y"
{ /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file is not defined as 5.4");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxCutCar(yyvsp[-2].dval, yyvsp[-1].string);
    ;
    break;}
case 610:
#line 4535 "lef.y"
{ /* 5.5 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.5) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
             CHKERR();
           }
        }
      } else if (use5_3) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file has both old and new syntax, which is illegal.");
             CHKERR();
           }
        }
      }
    ;
    break;}
case 612:
#line 4556 "lef.y"
{lefDumbMode = 2; lefNoNum = 2; ;
    break;}
case 613:
#line 4557 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("NETEXPR is a 5.6 syntax.  Your lef file is not defined as 5.6") ;
             CHKERR();
           }
        }
      } else
        if (lefrPinCbk) lefrPin.lefiPin::setNetExpr(yyvsp[-1].string);
    ;
    break;}
case 614:
#line 4568 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 615:
#line 4569 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("SUPPLYSENSITIVITY is a 5.6 syntax.  Your lef file is not defined as 5.6") ;
             CHKERR();
           }
        }
      } else
        if (lefrPinCbk) lefrPin.lefiPin::setSupplySensitivity(yyvsp[-1].string);
    ;
    break;}
case 616:
#line 4580 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 617:
#line 4581 "lef.y"
{
      if (versionNum < 5.6) {
        if (lefrPinCbk) { /* write error only if cbk is set */
           if (pinWarnings++ < lefrPinWarnings) {
             yyerror("GROUNDSENSITIVITY is a 5.6 syntax.  Your lef file is not defined as 5.6") ;
             CHKERR();
           }
        }
      } else
        if (lefrPinCbk) lefrPin.lefiPin::setGroundSensitivity(yyvsp[-1].string);
    ;
    break;}
case 618:
#line 4595 "lef.y"
{
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(1);
    ;
    break;}
case 619:
#line 4600 "lef.y"
{
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(2);
    ;
    break;}
case 620:
#line 4605 "lef.y"
{
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(3);
    ;
    break;}
case 621:
#line 4610 "lef.y"
{
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(4);
    ;
    break;}
case 624:
#line 4622 "lef.y"
{ 
      char temp[32];
      sprintf(temp, "%.11g", yyvsp[0].dval);
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType(yyvsp[-1].string);
         lefrPin.lefiPin::setNumProperty(yyvsp[-1].string, yyvsp[0].dval, temp, propTp);
      }
    ;
    break;}
case 625:
#line 4632 "lef.y"
{
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType(yyvsp[-1].string);
         lefrPin.lefiPin::setProperty(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 626:
#line 4640 "lef.y"
{
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType(yyvsp[-1].string);
         lefrPin.lefiPin::setProperty(yyvsp[-1].string, yyvsp[0].string, propTp);
      }
    ;
    break;}
case 627:
#line 4649 "lef.y"
{yyval.string = (char*)"INPUT";;
    break;}
case 628:
#line 4650 "lef.y"
{yyval.string = (char*)"OUTPUT";;
    break;}
case 629:
#line 4651 "lef.y"
{yyval.string = (char*)"OUTPUT TRISTATE";;
    break;}
case 630:
#line 4652 "lef.y"
{yyval.string = (char*)"INOUT";;
    break;}
case 631:
#line 4653 "lef.y"
{yyval.string = (char*)"FEEDTHRU";;
    break;}
case 632:
#line 4656 "lef.y"
{
      if (lefrPinCbk) {
	lefrDoGeometries = 1;
        hasPRP = 0;
	lefrGeometriesPtr = (lefiGeometries*)lefMalloc( sizeof(lefiGeometries));
	lefrGeometriesPtr->lefiGeometries::Init();
      }
      needGeometry = 0;  // don't need rect/path/poly define yet
    ;
    break;}
case 634:
#line 4668 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addClass(yyvsp[-1].string); ;
    break;}
case 635:
#line 4672 "lef.y"
{yyval.string = (char*)"SIGNAL";;
    break;}
case 636:
#line 4673 "lef.y"
{yyval.string = (char*)"ANALOG";;
    break;}
case 637:
#line 4674 "lef.y"
{yyval.string = (char*)"POWER";;
    break;}
case 638:
#line 4675 "lef.y"
{yyval.string = (char*)"GROUND";;
    break;}
case 639:
#line 4676 "lef.y"
{yyval.string = (char*)"CLOCK";;
    break;}
case 640:
#line 4677 "lef.y"
{yyval.string = (char*)"DATA";;
    break;}
case 641:
#line 4680 "lef.y"
{yyval.string = (char*)"INPUT";;
    break;}
case 642:
#line 4681 "lef.y"
{yyval.string = (char*)"OUTPUT";;
    break;}
case 643:
#line 4682 "lef.y"
{yyval.string = (char*)"START";;
    break;}
case 644:
#line 4683 "lef.y"
{yyval.string = (char*)"STOP";;
    break;}
case 645:
#line 4686 "lef.y"
{yyval.string = (char*)""; ;
    break;}
case 646:
#line 4687 "lef.y"
{yyval.string = (char*)"ABUTMENT";;
    break;}
case 647:
#line 4688 "lef.y"
{yyval.string = (char*)"RING";;
    break;}
case 648:
#line 4689 "lef.y"
{yyval.string = (char*)"FEEDTHRU";;
    break;}
case 650:
#line 4694 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 651:
#line 4695 "lef.y"
{
      if ((needGeometry) && (needGeometry != 2)) // 1 LAYER follow after another
        if (layerWarnings++ < lefrLayerWarnings)
          yywarning("Either PATH, RECT or POLYGON is a required in MACRO/PIN/PORT.");
      if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addLayer(yyvsp[0].string);
      needGeometry = 1;    // within LAYER it requires either path, rect, poly
    ;
    break;}
case 653:
#line 4705 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addWidth(yyvsp[-1].dval); ;
    break;}
case 654:
#line 4708 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPath();
      hasPRP = 1;
      needGeometry = 2;
    ;
    break;}
case 655:
#line 4714 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPathIter();
      hasPRP = 1;
      needGeometry = 2;
    ;
    break;}
case 656:
#line 4720 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addRect(yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].pt.x, yyvsp[-1].pt.y);
      needGeometry = 2;
    ;
    break;}
case 657:
#line 4725 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addRectIter(yyvsp[-3].pt.x, yyvsp[-3].pt.y, yyvsp[-2].pt.x, yyvsp[-2].pt.y);
      needGeometry = 2;
    ;
    break;}
case 658:
#line 4730 "lef.y"
{
      if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPolygon();
      hasPRP = 1;
      needGeometry = 2;
    ;
    break;}
case 659:
#line 4737 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPolygonIter();
      hasPRP = 1;
      needGeometry = 2;
    ;
    break;}
case 660:
#line 4743 "lef.y"
{ ;
    break;}
case 664:
#line 4750 "lef.y"
{ if (lefrDoGeometries) {
        if (zeroOrGt(yyvsp[0].dval))
           lefrGeometriesPtr->lefiGeometries::addLayerMinSpacing(yyvsp[0].dval);
        else {
           yyerror("SPACING value in MACRO OBS has to be 0 or greater");
           CHKERR();
        }
      }
    ;
    break;}
case 665:
#line 4760 "lef.y"
{ if (lefrDoGeometries) {
        if (zeroOrGt(yyvsp[0].dval))
           lefrGeometriesPtr->lefiGeometries::addLayerRuleWidth(yyvsp[0].dval);
        else {
           yyerror("DESIGNRULEWIDTH value in MACRO OBS has to be 0 or greater");
           CHKERR();
        }
      }
    ;
    break;}
case 666:
#line 4771 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::startList(yyvsp[0].pt.x, yyvsp[0].pt.y); ;
    break;}
case 667:
#line 4775 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addToList(yyvsp[0].pt.x, yyvsp[0].pt.y); ;
    break;}
case 670:
#line 4787 "lef.y"
{lefDumbMode = 1;;
    break;}
case 671:
#line 4788 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addVia(yyvsp[-3].pt.x, yyvsp[-3].pt.y, yyvsp[-1].string); ;
    break;}
case 672:
#line 4790 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 673:
#line 4792 "lef.y"
{ if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addViaIter(yyvsp[-4].pt.x, yyvsp[-4].pt.y, yyvsp[-2].string); ;
    break;}
case 674:
#line 4797 "lef.y"
{ if (lefrDoGeometries)
         lefrGeometriesPtr->lefiGeometries::addStepPattern(yyvsp[-5].dval, yyvsp[-3].dval, yyvsp[-1].dval, yyvsp[0].dval); ;
    break;}
case 675:
#line 4802 "lef.y"
{
      if (lefrDoSite) {
	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
				   sizeof(lefiSitePattern));
	lefrSitePatternPtr->lefiSitePattern::Init();
	lefrSitePatternPtr->lefiSitePattern::set(yyvsp[-10].string, yyvsp[-9].dval, yyvsp[-8].dval, yyvsp[-7].integer, yyvsp[-5].dval, yyvsp[-3].dval,
	  yyvsp[-1].dval, yyvsp[0].dval);
	}
    ;
    break;}
case 676:
#line 4812 "lef.y"
{
      if (lefrDoSite) {
	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
				   sizeof(lefiSitePattern));
	lefrSitePatternPtr->lefiSitePattern::Init();
	lefrSitePatternPtr->lefiSitePattern::set(yyvsp[-3].string, yyvsp[-2].dval, yyvsp[-1].dval, yyvsp[0].integer, -1, -1,
	  -1, -1);
	}
    ;
    break;}
case 677:
#line 4824 "lef.y"
{ 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
				sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("X", yyvsp[-4].dval, (int)yyvsp[-2].dval, yyvsp[0].dval);
      }    
    ;
    break;}
case 678:
#line 4832 "lef.y"
{lefDumbMode = 1000000000;;
    break;}
case 679:
#line 4833 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 680:
#line 4835 "lef.y"
{ 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("Y", yyvsp[-4].dval, (int)yyvsp[-2].dval, yyvsp[0].dval);
      }    
    ;
    break;}
case 681:
#line 4843 "lef.y"
{lefDumbMode = 1000000000;;
    break;}
case 682:
#line 4844 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 683:
#line 4846 "lef.y"
{ 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("X", yyvsp[-4].dval, (int)yyvsp[-2].dval, yyvsp[0].dval);
      }    
    ;
    break;}
case 684:
#line 4855 "lef.y"
{ 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("Y", yyvsp[-4].dval, (int)yyvsp[-2].dval, yyvsp[0].dval);
      }    
    ;
    break;}
case 687:
#line 4870 "lef.y"
{ if (lefrDoTrack) lefrTrackPatternPtr->lefiTrackPattern::addLayer(yyvsp[0].string); ;
    break;}
case 688:
#line 4873 "lef.y"
{
      if (lefrDoGcell) {
	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
                                    sizeof(lefiGcellPattern));
	lefrGcellPatternPtr->lefiGcellPattern::Init();
	lefrGcellPatternPtr->lefiGcellPattern::set("X", yyvsp[-4].dval, (int)yyvsp[-2].dval, yyvsp[0].dval);
      }    
    ;
    break;}
case 689:
#line 4882 "lef.y"
{
      if (lefrDoGcell) {
	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
                                    sizeof(lefiGcellPattern));
	lefrGcellPatternPtr->lefiGcellPattern::Init();
	lefrGcellPatternPtr->lefiGcellPattern::set("Y", yyvsp[-4].dval, (int)yyvsp[-2].dval, yyvsp[0].dval);
      }    
    ;
    break;}
case 690:
#line 4892 "lef.y"
{ 
      if (lefrObstructionCbk) {
	lefrObstruction.lefiObstruction::setGeometries(lefrGeometriesPtr);
	lefrGeometriesPtr = 0;
	lefrDoGeometries = 0;
        CALLBACK(lefrObstructionCbk, lefrObstructionCbkType, &lefrObstruction);
      }
      lefDumbMode = 0;
    ;
    break;}
case 691:
#line 4909 "lef.y"
{
       /* The pointer has malloced in start, need to free manually */
       if (lefrGeometriesPtr) {
          lefrGeometriesPtr->lefiGeometries::Destroy();
          lefFree(lefrGeometriesPtr);
       }
    ;
    break;}
case 692:
#line 4918 "lef.y"
{
      obsDef = 1;
      if (lefrObstructionCbk) {
	lefrDoGeometries = 1;
	lefrGeometriesPtr = (lefiGeometries*)lefMalloc(
	    sizeof(lefiGeometries));
	lefrGeometriesPtr->lefiGeometries::Init();
	}
    ;
    break;}
case 693:
#line 4929 "lef.y"
{ 
      if (versionNum < 5.6) {
        if (lefrDensityCbk) { /* write error only if cbk is set */
           if (macroWarnings++ < lefrMacroWarnings) {
             yyerror("DENSITY is a 5.6 syntax.  Your lef file is not defined as 5.6");
             CHKERR();
           }
        }
      } else
        if (lefrDensityCbk) {
          CALLBACK(lefrDensityCbk, lefrDensityCbkType, &lefrDensity);
          lefrDensity.lefiDensity::clear();
        }
      lefDumbMode = 0;
    ;
    break;}
case 696:
#line 4949 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 697:
#line 4950 "lef.y"
{
      if (lefrDensityCbk)
        lefrDensity.lefiDensity::addLayer(yyvsp[-1].string);
    ;
    break;}
case 701:
#line 4961 "lef.y"
{
      if (lefrDensityCbk)
        lefrDensity.lefiDensity::addRect(yyvsp[-3].pt.x, yyvsp[-3].pt.y, yyvsp[-2].pt.x, yyvsp[-2].pt.y, yyvsp[-1].dval); 
    ;
    break;}
case 702:
#line 4966 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 703:
#line 4967 "lef.y"
{ if (lefrMacroCbk) lefrMacro.lefiMacro::setClockType(yyvsp[-1].string); ;
    break;}
case 704:
#line 4970 "lef.y"
{ ;
    break;}
case 705:
#line 4973 "lef.y"
{ /* XXXXX for macros */ ;
    break;}
case 706:
#line 4976 "lef.y"
{
    if (versionNum < 5.4) {
      if (lefrTimingCbk && lefrTiming.lefiTiming::hasData())
        CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming);
      lefrTiming.lefiTiming::clear();
    } else {
      if (lefrTimingCbk) /* write warning only if cbk is set */
        if (timingWarnings++ < lefrTimingWarnings)
          yywarning("MACRO TIMING is obsolete in 5.4. It will be ignored.");
      lefrTiming.lefiTiming::clear();
    }
  ;
    break;}
case 709:
#line 4996 "lef.y"
{
    if (versionNum < 5.4) {
      if (lefrTimingCbk && lefrTiming.lefiTiming::hasData())
        CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming);
    }
    lefDumbMode = 1000000000;
    lefrTiming.lefiTiming::clear();
    ;
    break;}
case 710:
#line 5005 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 711:
#line 5006 "lef.y"
{lefDumbMode = 1000000000;;
    break;}
case 712:
#line 5007 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 713:
#line 5009 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFall(yyvsp[-3].string,yyvsp[-1].dval,yyvsp[0].dval); ;
    break;}
case 714:
#line 5011 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallVariable(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 715:
#line 5014 "lef.y"
{ if (lefrTimingCbk) {
	if (yyvsp[-7].string[0] == 'D' || yyvsp[-7].string[0] == 'd') /* delay */
	  lefrTiming.lefiTiming::addDelay(yyvsp[-8].string, yyvsp[-5].string, yyvsp[-3].dval, yyvsp[-2].dval, yyvsp[-1].dval);
	else
	  lefrTiming.lefiTiming::addTransition(yyvsp[-8].string, yyvsp[-5].string, yyvsp[-3].dval, yyvsp[-2].dval, yyvsp[-1].dval);
      }
    ;
    break;}
case 716:
#line 5022 "lef.y"
{ ;
    break;}
case 717:
#line 5024 "lef.y"
{ ;
    break;}
case 718:
#line 5026 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseRS(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 719:
#line 5028 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setFallRS(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 720:
#line 5030 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseCS(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 721:
#line 5032 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setFallCS(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 722:
#line 5034 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseAtt1(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 723:
#line 5036 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setFallAtt1(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 724:
#line 5038 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseTo(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 725:
#line 5040 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setFallTo(yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 726:
#line 5042 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addUnateness(yyvsp[-1].string); ;
    break;}
case 727:
#line 5044 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setStable(yyvsp[-4].dval,yyvsp[-2].dval,yyvsp[-1].string); ;
    break;}
case 728:
#line 5046 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addSDF2Pins(yyvsp[-7].string,yyvsp[-6].string,yyvsp[-5].string,yyvsp[-3].dval,yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 729:
#line 5048 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addSDF1Pin(yyvsp[-5].string,yyvsp[-3].dval,yyvsp[-2].dval,yyvsp[-2].dval); ;
    break;}
case 730:
#line 5050 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcondStart(yyvsp[-1].string); ;
    break;}
case 731:
#line 5052 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcondEnd(yyvsp[-1].string); ;
    break;}
case 732:
#line 5054 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcond(yyvsp[-1].string); ;
    break;}
case 733:
#line 5056 "lef.y"
{ /* XXXXX */ ;
    break;}
case 734:
#line 5060 "lef.y"
{ yyval.string = (char*)"MPWH";;
    break;}
case 735:
#line 5062 "lef.y"
{ yyval.string = (char*)"MPWL";;
    break;}
case 736:
#line 5064 "lef.y"
{ yyval.string = (char*)"PERIOD";;
    break;}
case 737:
#line 5068 "lef.y"
{ yyval.string = (char*)"SETUP";;
    break;}
case 738:
#line 5070 "lef.y"
{ yyval.string = (char*)"HOLD";;
    break;}
case 739:
#line 5072 "lef.y"
{ yyval.string = (char*)"RECOVERY";;
    break;}
case 740:
#line 5074 "lef.y"
{ yyval.string = (char*)"SKEW";;
    break;}
case 741:
#line 5078 "lef.y"
{ yyval.string = (char*)"ANYEDGE";;
    break;}
case 742:
#line 5080 "lef.y"
{ yyval.string = (char*)"POSEDGE";;
    break;}
case 743:
#line 5082 "lef.y"
{ yyval.string = (char*)"NEGEDGE";;
    break;}
case 744:
#line 5086 "lef.y"
{ yyval.string = (char*)"ANYEDGE";;
    break;}
case 745:
#line 5088 "lef.y"
{ yyval.string = (char*)"POSEDGE";;
    break;}
case 746:
#line 5090 "lef.y"
{ yyval.string = (char*)"NEGEDGE";;
    break;}
case 747:
#line 5094 "lef.y"
{ yyval.string = (char*)"DELAY"; ;
    break;}
case 748:
#line 5096 "lef.y"
{ yyval.string = (char*)"TRANSITION"; ;
    break;}
case 749:
#line 5100 "lef.y"
{ ;
    break;}
case 750:
#line 5102 "lef.y"
{ ;
    break;}
case 751:
#line 5105 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addTableEntry(yyvsp[-3].dval,yyvsp[-2].dval,yyvsp[-1].dval); ;
    break;}
case 752:
#line 5109 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addTableAxisNumber(yyvsp[0].dval); ;
    break;}
case 753:
#line 5111 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addTableAxisNumber(yyvsp[0].dval); ;
    break;}
case 754:
#line 5115 "lef.y"
{ ;
    break;}
case 755:
#line 5117 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew(yyvsp[-3].dval,yyvsp[-2].dval,yyvsp[-1].dval,yyvsp[0].dval); ;
    break;}
case 756:
#line 5119 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew(yyvsp[-6].dval,yyvsp[-5].dval,yyvsp[-4].dval,yyvsp[-3].dval);
      if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew2(yyvsp[-2].dval,yyvsp[-1].dval,yyvsp[0].dval); ;
    break;}
case 757:
#line 5124 "lef.y"
{ yyval.string = (char*)"RISE"; ;
    break;}
case 758:
#line 5126 "lef.y"
{ yyval.string = (char*)"FALL"; ;
    break;}
case 759:
#line 5130 "lef.y"
{ yyval.string = (char*)"INVERT"; ;
    break;}
case 760:
#line 5132 "lef.y"
{ yyval.string = (char*)"NONINVERT"; ;
    break;}
case 761:
#line 5134 "lef.y"
{ yyval.string = (char*)"NONUNATE"; ;
    break;}
case 762:
#line 5138 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addFromPin(yyvsp[0].string); ;
    break;}
case 763:
#line 5140 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addFromPin(yyvsp[0].string); ;
    break;}
case 764:
#line 5144 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addToPin(yyvsp[0].string); ;
    break;}
case 765:
#line 5146 "lef.y"
{ if (lefrTimingCbk) lefrTiming.lefiTiming::addToPin(yyvsp[0].string); ;
    break;}
case 766:
#line 5149 "lef.y"
{
      if (lefrArrayCbk)
	CALLBACK(lefrArrayCbk, lefrArrayCbkType, &lefrArray);
      lefrArray.lefiArray::clear();
      lefrSitePatternPtr = 0;
      lefrDoSite = 0;
   ;
    break;}
case 768:
#line 5158 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 769:
#line 5159 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::setName(yyvsp[0].string);
	CALLBACK(lefrArrayBeginCbk, lefrArrayBeginCbkType, yyvsp[0].string);
      }
      //strcpy(arrayName, $3);
      arrayName = strdup(yyvsp[0].string);
    ;
    break;}
case 770:
#line 5168 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 771:
#line 5169 "lef.y"
{
      if (lefrArrayCbk && lefrArrayEndCbk)
	CALLBACK(lefrArrayEndCbk, lefrArrayEndCbkType, yyvsp[0].string);
      if (strcmp(arrayName, yyvsp[0].string) != 0) {
        lefFree(arrayName);
        if (lefrArrayCbk) { /* write error only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings) {
             yyerror("END ARRAY name is different from ARRAY");
             CHKERR();
           }
        }
      } else
        lefFree(arrayName);
    ;
    break;}
case 772:
#line 5186 "lef.y"
{ ;
    break;}
case 773:
#line 5188 "lef.y"
{ ;
    break;}
case 774:
#line 5191 "lef.y"
{ if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;
    break;}
case 775:
#line 5193 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addSitePattern(lefrSitePatternPtr);
      }
    ;
    break;}
case 776:
#line 5198 "lef.y"
{lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;
    break;}
case 777:
#line 5200 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addCanPlace(lefrSitePatternPtr);
      }
    ;
    break;}
case 778:
#line 5205 "lef.y"
{lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;
    break;}
case 779:
#line 5207 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addCannotOccupy(lefrSitePatternPtr);
      }
    ;
    break;}
case 780:
#line 5212 "lef.y"
{ if (lefrArrayCbk) lefrDoTrack = 1; ;
    break;}
case 781:
#line 5213 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addTrack(lefrTrackPatternPtr);
      }
    ;
    break;}
case 782:
#line 5219 "lef.y"
{
    ;
    break;}
case 783:
#line 5221 "lef.y"
{ if (lefrArrayCbk) lefrDoGcell = 1; ;
    break;}
case 784:
#line 5222 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addGcell(lefrGcellPatternPtr);
      }
    ;
    break;}
case 785:
#line 5228 "lef.y"
{
      if (lefrArrayCbk) {
	lefrArray.lefiArray::setTableSize((int)yyvsp[-3].dval);
      }
    ;
    break;}
case 786:
#line 5234 "lef.y"
{ ;
    break;}
case 787:
#line 5237 "lef.y"
{ if (lefrArrayCbk) lefrArray.lefiArray::addFloorPlan(yyvsp[0].string); ;
    break;}
case 788:
#line 5241 "lef.y"
{ ;
    break;}
case 789:
#line 5243 "lef.y"
{ ;
    break;}
case 790:
#line 5246 "lef.y"
{ lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;
    break;}
case 791:
#line 5248 "lef.y"
{
      if (lefrArrayCbk)
	lefrArray.lefiArray::addSiteToFloorPlan("CANPLACE",
	lefrSitePatternPtr);
    ;
    break;}
case 792:
#line 5253 "lef.y"
{ if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;
    break;}
case 793:
#line 5255 "lef.y"
{
      if (lefrArrayCbk)
	lefrArray.lefiArray::addSiteToFloorPlan("CANNOTOCCUPY",
	lefrSitePatternPtr);
     ;
    break;}
case 794:
#line 5263 "lef.y"
{ ;
    break;}
case 795:
#line 5265 "lef.y"
{ ;
    break;}
case 796:
#line 5268 "lef.y"
{ if (lefrArrayCbk) lefrArray.lefiArray::addDefaultCap((int)yyvsp[-3].dval, yyvsp[-1].dval); ;
    break;}
case 797:
#line 5271 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 798:
#line 5272 "lef.y"
{ lefAddStringMessage(yyvsp[-3].string, yyvsp[-1].string); ;
    break;}
case 799:
#line 5275 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 800:
#line 5276 "lef.y"
{ ;
    break;}
case 801:
#line 5279 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 802:
#line 5280 "lef.y"
{
      if (versionNum < 5.6)
        lefAddNumDefine(yyvsp[-3].string, yyvsp[-1].dval);
      else
        if (lefrArrayCbk) /* write warning only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings)
             yywarning("DEFINE is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 803:
#line 5288 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 804:
#line 5289 "lef.y"
{
      if (versionNum < 5.6)
        lefAddStringDefine(yyvsp[-3].string, yyvsp[-1].string);
      else
        if (lefrArrayCbk) /* write warning only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings)
             yywarning("DEFINES is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 805:
#line 5297 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 806:
#line 5298 "lef.y"
{
      if (versionNum < 5.6)
        lefAddBooleanDefine(yyvsp[-3].string, yyvsp[-1].integer);
      else
        if (lefrArrayCbk) /* write warning only if cbk is set */
           if (arrayWarnings++ < lefrArrayWarnings)
             yywarning("DEFINEB is obsolete in 5.6. It will be ignored.");
    ;
    break;}
case 808:
#line 5309 "lef.y"
{lefNlToken = FALSE;;
    break;}
case 809:
#line 5310 "lef.y"
{lefNlToken = FALSE;;
    break;}
case 814:
#line 5323 "lef.y"
{yyval.dval = yyvsp[-2].dval + yyvsp[0].dval; ;
    break;}
case 815:
#line 5324 "lef.y"
{yyval.dval = yyvsp[-2].dval - yyvsp[0].dval; ;
    break;}
case 816:
#line 5325 "lef.y"
{yyval.dval = yyvsp[-2].dval * yyvsp[0].dval; ;
    break;}
case 817:
#line 5326 "lef.y"
{yyval.dval = yyvsp[-2].dval / yyvsp[0].dval; ;
    break;}
case 818:
#line 5327 "lef.y"
{yyval.dval = -yyvsp[0].dval;;
    break;}
case 819:
#line 5328 "lef.y"
{yyval.dval = yyvsp[-1].dval;;
    break;}
case 820:
#line 5330 "lef.y"
{yyval.dval = (yyvsp[-4].integer != 0) ? yyvsp[-2].dval : yyvsp[0].dval;;
    break;}
case 821:
#line 5331 "lef.y"
{yyval.dval = yyvsp[0].dval;;
    break;}
case 822:
#line 5334 "lef.y"
{yyval.integer = comp_num(yyvsp[-2].dval,yyvsp[-1].integer,yyvsp[0].dval);;
    break;}
case 823:
#line 5335 "lef.y"
{yyval.integer = yyvsp[-2].dval != 0 && yyvsp[0].dval != 0;;
    break;}
case 824:
#line 5336 "lef.y"
{yyval.integer = yyvsp[-2].dval != 0 || yyvsp[0].dval != 0;;
    break;}
case 825:
#line 5337 "lef.y"
{yyval.integer = comp_str(yyvsp[-2].string,yyvsp[-1].integer,yyvsp[0].string);;
    break;}
case 826:
#line 5338 "lef.y"
{yyval.integer = yyvsp[-2].string[0] != 0 && yyvsp[0].string[0] != 0;;
    break;}
case 827:
#line 5339 "lef.y"
{yyval.integer = yyvsp[-2].string[0] != 0 || yyvsp[0].string[0] != 0;;
    break;}
case 828:
#line 5340 "lef.y"
{yyval.integer = yyvsp[-2].integer == yyvsp[0].integer;;
    break;}
case 829:
#line 5341 "lef.y"
{yyval.integer = yyvsp[-2].integer != yyvsp[0].integer;;
    break;}
case 830:
#line 5342 "lef.y"
{yyval.integer = yyvsp[-2].integer && yyvsp[0].integer;;
    break;}
case 831:
#line 5343 "lef.y"
{yyval.integer = yyvsp[-2].integer || yyvsp[0].integer;;
    break;}
case 832:
#line 5344 "lef.y"
{yyval.integer = !yyval.integer;;
    break;}
case 833:
#line 5345 "lef.y"
{yyval.integer = yyvsp[-1].integer;;
    break;}
case 834:
#line 5347 "lef.y"
{yyval.integer = (yyvsp[-4].integer != 0) ? yyvsp[-2].integer : yyvsp[0].integer;;
    break;}
case 835:
#line 5348 "lef.y"
{yyval.integer = 1;;
    break;}
case 836:
#line 5349 "lef.y"
{yyval.integer = 0;;
    break;}
case 837:
#line 5353 "lef.y"
{
      yyval.string = (char*)lefMalloc(strlen(yyvsp[-2].string)+strlen(yyvsp[0].string)+1);
      strcpy(yyval.string,yyvsp[-2].string);
      strcat(yyval.string,yyvsp[0].string);
    ;
    break;}
case 838:
#line 5359 "lef.y"
{ yyval.string = yyvsp[-1].string; ;
    break;}
case 839:
#line 5361 "lef.y"
{
      lefDefIf = TRUE;
      if (yyvsp[-4].integer != 0) {
	yyval.string = yyvsp[-2].string;	
      } else {
	yyval.string = yyvsp[0].string;
      }
    ;
    break;}
case 840:
#line 5370 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 841:
#line 5373 "lef.y"
{yyval.integer = C_LE;;
    break;}
case 842:
#line 5374 "lef.y"
{yyval.integer = C_LT;;
    break;}
case 843:
#line 5375 "lef.y"
{yyval.integer = C_GE;;
    break;}
case 844:
#line 5376 "lef.y"
{yyval.integer = C_GT;;
    break;}
case 845:
#line 5377 "lef.y"
{yyval.integer = C_EQ;;
    break;}
case 846:
#line 5378 "lef.y"
{yyval.integer = C_NE;;
    break;}
case 847:
#line 5379 "lef.y"
{yyval.integer = C_EQ;;
    break;}
case 848:
#line 5380 "lef.y"
{yyval.integer = C_LT;;
    break;}
case 849:
#line 5381 "lef.y"
{yyval.integer = C_GT;;
    break;}
case 850:
#line 5385 "lef.y"
{ 
      if (lefrPropBeginCbk)
        CALLBACK(lefrPropBeginCbk, lefrPropBeginCbkType, 0);
    ;
    break;}
case 851:
#line 5390 "lef.y"
{ 
      if (lefrPropEndCbk)
        CALLBACK(lefrPropEndCbk, lefrPropEndCbkType, 0);
      lefRealNum = 0;     /* just want to make sure it is reset */
    ;
    break;}
case 852:
#line 5398 "lef.y"
{ ;
    break;}
case 853:
#line 5400 "lef.y"
{ ;
    break;}
case 854:
#line 5403 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 855:
#line 5405 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("library", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrLibProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 856:
#line 5412 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 857:
#line 5414 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("componentpin", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrCompProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 858:
#line 5421 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 859:
#line 5423 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("pin", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrPinProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
      
    ;
    break;}
case 860:
#line 5431 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 861:
#line 5433 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("macro", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrMacroProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 862:
#line 5440 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 863:
#line 5442 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("via", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrViaProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 864:
#line 5449 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 865:
#line 5451 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("viarule", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrViaRuleProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 866:
#line 5458 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 867:
#line 5460 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("layer", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrLayerProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 868:
#line 5467 "lef.y"
{lefDumbMode = 1; lefrProp.lefiProp::clear(); ;
    break;}
case 869:
#line 5469 "lef.y"
{ 
      if (lefrPropCbk) {
        lefrProp.lefiProp::setPropType("nondefaultrule", yyvsp[-2].string);
        CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      }
      lefrNondefProp.lefiPropType::setPropType(yyvsp[-2].string, lefPropDefType);
    ;
    break;}
case 870:
#line 5478 "lef.y"
{ lefRealNum = 0 ;
    break;}
case 871:
#line 5479 "lef.y"
{ 
      if (lefrPropCbk) lefrProp.lefiProp::setPropInteger();
      lefPropDefType = 'I'
    ;
    break;}
case 872:
#line 5483 "lef.y"
{ lefRealNum = 1 ;
    break;}
case 873:
#line 5484 "lef.y"
{ 
      if (lefrPropCbk) lefrProp.lefiProp::setPropReal();
      lefPropDefType = 'R';
      lefRealNum = 0;
    ;
    break;}
case 874:
#line 5490 "lef.y"
{
      if (lefrPropCbk) lefrProp.lefiProp::setPropString();
      lefPropDefType = 'S'
    ;
    break;}
case 875:
#line 5495 "lef.y"
{
      if (lefrPropCbk) lefrProp.lefiProp::setPropQString(yyvsp[0].string);
      lefPropDefType = 'Q'
    ;
    break;}
case 876:
#line 5500 "lef.y"
{
      if (lefrPropCbk) lefrProp.lefiProp::setPropNameMapString(yyvsp[0].string);
      lefPropDefType = 'S'
    ;
    break;}
case 877:
#line 5508 "lef.y"
{ 
    ;
    break;}
case 878:
#line 5511 "lef.y"
{ 
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRange(yyvsp[-1].dval, yyvsp[0].dval);
    ;
    break;}
case 880:
#line 5517 "lef.y"
{
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingLength(yyvsp[0].dval);
      }
    ;
    break;}
case 881:
#line 5523 "lef.y"
{
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingLength(yyvsp[-3].dval);
        lefrLayer.lefiLayer::setSpacingLengthRange(yyvsp[-1].dval, yyvsp[0].dval);
      }
    ;
    break;}
case 882:
#line 5532 "lef.y"
{ ;
    break;}
case 883:
#line 5534 "lef.y"
{
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRangeUseLength();
    ;
    break;}
case 884:
#line 5539 "lef.y"
{
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingRangeInfluence(yyvsp[0].dval);
        lefrLayer.lefiLayer::setSpacingRangeInfluenceRange(-1, -1);
      }
    ;
    break;}
case 885:
#line 5546 "lef.y"
{
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingRangeInfluence(yyvsp[-3].dval);
        lefrLayer.lefiLayer::setSpacingRangeInfluenceRange(yyvsp[-1].dval, yyvsp[0].dval);
      }
    ;
    break;}
case 886:
#line 5553 "lef.y"
{
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRangeRange(yyvsp[-1].dval, yyvsp[0].dval);
    ;
    break;}
case 887:
#line 5560 "lef.y"
{ ;
    break;}
case 888:
#line 5562 "lef.y"
{  if (lefrPropCbk) lefrProp.lefiProp::setRange(yyvsp[-1].dval, yyvsp[0].dval); ;
    break;}
case 889:
#line 5566 "lef.y"
{ ;
    break;}
case 890:
#line 5568 "lef.y"
{ if (lefrPropCbk) lefrProp.lefiProp::setNumber(yyvsp[0].dval); ;
    break;}
case 892:
#line 5572 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 893:
#line 5573 "lef.y"
{
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingName(yyvsp[0].string);
    ;
    break;}
case 894:
#line 5578 "lef.y"
{
      if (versionNum < 5.5) {
         if (layerWarnings++ < lefrLayerWarnings) {
           yyerror("ADJACENTCUTS is a 5.5 syntax. Your lef file is not defined as 5.5");
           CHKERR();
         }
      }
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingAdjacent((int)yyvsp[-2].dval, yyvsp[0].dval);
    ;
    break;}
case 895:
#line 5589 "lef.y"
{
      if (versionNum < 5.6) {
         if (layerWarnings++ < lefrLayerWarnings) {
           yyerror("CENTERTOCENTER is a 5.6 syntax. Your lef file is not defined as 5.6");
           CHKERR();
         }
      }
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingCenterToCenter();
    ;
    break;}
case 896:
#line 5602 "lef.y"
{ yyval.string = 0; ;
    break;}
case 897:
#line 5603 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 898:
#line 5604 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 899:
#line 5608 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 900:
#line 5609 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 901:
#line 5613 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrNoiseMarginCbk) {
          lefrNoiseMargin.low = yyvsp[-2].dval;
          lefrNoiseMargin.high = yyvsp[-1].dval;
          CALLBACK(lefrNoiseMarginCbk, lefrNoiseMarginCbkType, &lefrNoiseMargin);
        }
      } else
        if (lefrNoiseMarginCbk) /* write warning only if cbk is set */
          if (noiseMarginWarnings++ < lefrNoiseMarginWarnings)
            yywarning(
            "UNIVERSALNOISEMARGIN is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 902:
#line 5628 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrEdgeRateThreshold1Cbk) {
          CALLBACK(lefrEdgeRateThreshold1Cbk,
          lefrEdgeRateThreshold1CbkType, yyvsp[-1].dval);
        }
      } else
        if (lefrEdgeRateThreshold1Cbk) /* write warning only if cbk is set */
          if (edgeRateThreshold1Warnings++ < lefrEdgeRateThreshold1Warnings)
            yywarning("EDGERATETHRESHOLD1 is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 903:
#line 5641 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrEdgeRateThreshold2Cbk) {
	  CALLBACK(lefrEdgeRateThreshold2Cbk,
	  lefrEdgeRateThreshold2CbkType, yyvsp[-1].dval);
        }
      } else
        if (lefrEdgeRateThreshold2Cbk) /* write warning only if cbk is set */
          if (edgeRateThreshold2Warnings++ < lefrEdgeRateThreshold2Warnings)
            yywarning("EDGERATETHRESHOLD2 is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 904:
#line 5654 "lef.y"
{
      if (versionNum < 5.4) {
        if (lefrEdgeRateScaleFactorCbk) {
	  CALLBACK(lefrEdgeRateScaleFactorCbk,
	  lefrEdgeRateScaleFactorCbkType, yyvsp[-1].dval);
        }
      } else
        if (lefrEdgeRateScaleFactorCbk) /* write warning only if cbk is set */
          if (edgeRateScaleFactorWarnings++ < lefrEdgeRateScaleFactorWarnings)
            yywarning(
            "EDGERATESCALEFACTOR is obsolete in 5.4. It will be ignored.");
    ;
    break;}
case 905:
#line 5668 "lef.y"
{ if (lefrNoiseTableCbk) lefrNoiseTable.lefiNoiseTable::setup((int)yyvsp[0].dval); ;
    break;}
case 906:
#line 5670 "lef.y"
{ ;
    break;}
case 907:
#line 5674 "lef.y"
{
    if (versionNum < 5.4) {
      if (lefrNoiseTableCbk)
        CALLBACK(lefrNoiseTableCbk, lefrNoiseTableCbkType, &lefrNoiseTable);
    } else
      if (lefrNoiseTableCbk) /* write warning only if cbk is set */
        if (noiseTableWarnings++ < lefrNoiseTableWarnings)
          yywarning("NOISETABLE is obsolete in 5.4. It will be ignored.");
  ;
    break;}
case 910:
#line 5692 "lef.y"
{ if (lefrNoiseTableCbk)
         {
            lefrNoiseTable.lefiNoiseTable::newEdge();
            lefrNoiseTable.lefiNoiseTable::addEdge(yyvsp[-1].dval);
         }
    ;
    break;}
case 911:
#line 5699 "lef.y"
{ ;
    break;}
case 912:
#line 5702 "lef.y"
{ if (lefrNoiseTableCbk) lefrNoiseTable.lefiNoiseTable::addResistance(); ;
    break;}
case 914:
#line 5708 "lef.y"
{ if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addResistanceNumber(yyvsp[0].dval); ;
    break;}
case 915:
#line 5711 "lef.y"
{ if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addResistanceNumber(yyvsp[0].dval); ;
    break;}
case 918:
#line 5720 "lef.y"
{ if (lefrNoiseTableCbk)
	lefrNoiseTable.lefiNoiseTable::addVictimLength(yyvsp[-1].dval); ;
    break;}
case 919:
#line 5723 "lef.y"
{ ;
    break;}
case 920:
#line 5727 "lef.y"
{ if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addVictimNoise(yyvsp[0].dval); ;
    break;}
case 921:
#line 5730 "lef.y"
{ if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addVictimNoise(yyvsp[0].dval); ;
    break;}
case 922:
#line 5734 "lef.y"
{ if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::setup((int)yyvsp[-1].dval); ;
    break;}
case 923:
#line 5737 "lef.y"
{ ;
    break;}
case 924:
#line 5741 "lef.y"
{
    if (versionNum < 5.4) {
      if (lefrCorrectionTableCbk)
        CALLBACK(lefrCorrectionTableCbk, lefrCorrectionTableCbkType,
               &lefrCorrectionTable);
    } else
      if (lefrCorrectionTableCbk) /* write warning only if cbk is set */
        if (correctionTableWarnings++ < lefrCorrectionTableWarnings)
          yywarning("CORRECTIONTABLE is obsolete in 5.4. It will be ignored.");
  ;
    break;}
case 927:
#line 5759 "lef.y"
{ if (lefrCorrectionTableCbk)
         {
            lefrCorrectionTable.lefiCorrectionTable::newEdge();
            lefrCorrectionTable.lefiCorrectionTable::addEdge(yyvsp[-1].dval);
         }
    ;
    break;}
case 928:
#line 5766 "lef.y"
{ ;
    break;}
case 929:
#line 5769 "lef.y"
{ if (lefrCorrectionTableCbk)
  lefrCorrectionTable.lefiCorrectionTable::addResistance(); ;
    break;}
case 930:
#line 5772 "lef.y"
{ ;
    break;}
case 931:
#line 5776 "lef.y"
{ if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::addResistanceNumber(yyvsp[0].dval); ;
    break;}
case 932:
#line 5779 "lef.y"
{ if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::addResistanceNumber(yyvsp[0].dval); ;
    break;}
case 935:
#line 5789 "lef.y"
{ if (lefrCorrectionTableCbk)
     lefrCorrectionTable.lefiCorrectionTable::addVictimLength(yyvsp[-1].dval); ;
    break;}
case 936:
#line 5792 "lef.y"
{ ;
    break;}
case 937:
#line 5796 "lef.y"
{ if (lefrCorrectionTableCbk)
	lefrCorrectionTable.lefiCorrectionTable::addVictimCorrection(yyvsp[0].dval); ;
    break;}
case 938:
#line 5799 "lef.y"
{ if (lefrCorrectionTableCbk)
	lefrCorrectionTable.lefiCorrectionTable::addVictimCorrection(yyvsp[0].dval); ;
    break;}
case 939:
#line 5805 "lef.y"
{ /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              if (lefrInputAntennaCbk) { /* write warning only if cbk is set */
                if (inputAntennaWarnings++ < lefrInputAntennaWarnings) {
                  yyerror("INPUTPINANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
                  CHKERR();
                }
              }
           }
        }
        if (lefrInputAntennaCbk)
          CALLBACK(lefrInputAntennaCbk, lefrInputAntennaCbkType, yyvsp[-1].dval);
    ;
    break;}
case 940:
#line 5825 "lef.y"
{ /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              if (lefrOutputAntennaCbk) { /* write warning only if cbk is set */
                if (outputAntennaWarnings++ < lefrOutputAntennaWarnings) {
                  yyerror("OUTPUTPINANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
                  CHKERR();
                }
              }
           }
        }
        if (lefrOutputAntennaCbk)
          CALLBACK(lefrOutputAntennaCbk, lefrOutputAntennaCbkType, yyvsp[-1].dval);
    ;
    break;}
case 941:
#line 5845 "lef.y"
{ /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              if (lefrInoutAntennaCbk) { /* write warning only if cbk is set */
                if (inoutAntennaWarnings++ < lefrInoutAntennaWarnings) {
                  yyerror("INOUTPINANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
                  CHKERR();
                }
              }
           }
        }
        if (lefrInoutAntennaCbk)
          CALLBACK(lefrInoutAntennaCbk, lefrInoutAntennaCbkType, yyvsp[-1].dval);
    ;
    break;}
case 942:
#line 5865 "lef.y"
{ /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, suppose should be ingored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           if (lefrAntennaInputCbk) { /* write warning only if cbk is set */
             if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
               yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
               CHKERR();
             }
           }
        } else if (use5_3) {
           if (lefrAntennaInputCbk) { /* write warning only if cbk is set */
             if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
               yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal.");
               CHKERR();
             }
           }
        }
        if (lefrAntennaInputCbk)
          CALLBACK(lefrAntennaInputCbk, lefrAntennaInputCbkType, yyvsp[-1].dval);
    ;
    break;}
case 943:
#line 5891 "lef.y"
{ /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */
              if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
                yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax.  Please either update your lef VERSION number or use the 5.3 syntax.");
                CHKERR();
              }
           }
        } else if (use5_3) {
           if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */
              if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
                yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal.");
                CHKERR();
              }
           }
        }
        if (lefrAntennaInoutCbk)
          CALLBACK(lefrAntennaInoutCbk, lefrAntennaInoutCbkType, yyvsp[-1].dval);
    ;
    break;}
case 944:
#line 5917 "lef.y"
{ /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */
              if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
                yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax.  Please either update your lef VERSION number or use the 5.3 syntax.");
                CHKERR();
              }
           }
        } else if (use5_3) {
           if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */
              if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
                yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal.");
                CHKERR();
              }
           }
        }
        if (lefrAntennaOutputCbk)
          CALLBACK(lefrAntennaOutputCbk, lefrAntennaOutputCbkType, yyvsp[-1].dval);
    ;
    break;}
case 947:
#line 5946 "lef.y"
{ 
        if (lefrExtensionCbk)
          CALLBACK(lefrExtensionCbk, lefrExtensionCbkType, Hist_text);
        if (versionNum >= 5.6)
           ge56almostDone = 1;
    ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/local/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 5953 "lef.y"

