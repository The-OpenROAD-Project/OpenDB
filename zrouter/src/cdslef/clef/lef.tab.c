/* A Bison parser, made from lef.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse lefyyparse
#define yylex lefyylex
#define yyerror lefyyerror
#define yylval lefyylval
#define yychar lefyychar
#define yydebug lefyydebug
#define yynerrs lefyynerrs
# define	K_HISTORY	257
# define	K_ABUT	258
# define	K_ABUTMENT	259
# define	K_ACTIVE	260
# define	K_ANALOG	261
# define	K_ARRAY	262
# define	K_AREA	263
# define	K_BLOCK	264
# define	K_BOTTOMLEFT	265
# define	K_BOTTOMRIGHT	266
# define	K_BY	267
# define	K_CAPACITANCE	268
# define	K_CAPMULTIPLIER	269
# define	K_CLASS	270
# define	K_CLOCK	271
# define	K_CLOCKTYPE	272
# define	K_COLUMNMAJOR	273
# define	K_DESIGNRULEWIDTH	274
# define	K_INFLUENCE	275
# define	K_COMPONENTS	276
# define	K_CORE	277
# define	K_CORNER	278
# define	K_COVER	279
# define	K_CPERSQDIST	280
# define	K_CURRENT	281
# define	K_CURRENTSOURCE	282
# define	K_CUT	283
# define	K_DEFAULT	284
# define	K_DATABASE	285
# define	K_DATA	286
# define	K_DIELECTRIC	287
# define	K_DIRECTION	288
# define	K_DO	289
# define	K_EDGECAPACITANCE	290
# define	K_EEQ	291
# define	K_END	292
# define	K_ENDCAP	293
# define	K_FALL	294
# define	K_FALLCS	295
# define	K_FALLT0	296
# define	K_FALLSATT1	297
# define	K_FALLRS	298
# define	K_FALLSATCUR	299
# define	K_FALLTHRESH	300
# define	K_FEEDTHRU	301
# define	K_FIXED	302
# define	K_FOREIGN	303
# define	K_FROMPIN	304
# define	K_GENERATE	305
# define	K_GENERATOR	306
# define	K_GROUND	307
# define	K_HEIGHT	308
# define	K_HORIZONTAL	309
# define	K_INOUT	310
# define	K_INPUT	311
# define	K_INPUTNOISEMARGIN	312
# define	K_COMPONENTPIN	313
# define	K_INTRINSIC	314
# define	K_INVERT	315
# define	K_IRDROP	316
# define	K_ITERATE	317
# define	K_IV_TABLES	318
# define	K_LAYER	319
# define	K_LEAKAGE	320
# define	K_LEQ	321
# define	K_LIBRARY	322
# define	K_MACRO	323
# define	K_MATCH	324
# define	K_MAXDELAY	325
# define	K_MAXLOAD	326
# define	K_METALOVERHANG	327
# define	K_MILLIAMPS	328
# define	K_MILLIWATTS	329
# define	K_MINFEATURE	330
# define	K_MUSTJOIN	331
# define	K_NAMESCASESENSITIVE	332
# define	K_NANOSECONDS	333
# define	K_NETS	334
# define	K_NEW	335
# define	K_NONDEFAULTRULE	336
# define	K_NONINVERT	337
# define	K_NONUNATE	338
# define	K_OBS	339
# define	K_OHMS	340
# define	K_OFFSET	341
# define	K_ORIENTATION	342
# define	K_ORIGIN	343
# define	K_OUTPUT	344
# define	K_OUTPUTNOISEMARGIN	345
# define	K_OUTPUTRESISTANCE	346
# define	K_OVERHANG	347
# define	K_OVERLAP	348
# define	K_OFF	349
# define	K_ON	350
# define	K_OVERLAPS	351
# define	K_PAD	352
# define	K_PATH	353
# define	K_PATTERN	354
# define	K_PICOFARADS	355
# define	K_PIN	356
# define	K_PITCH	357
# define	K_PLACED	358
# define	K_POLYGON	359
# define	K_PORT	360
# define	K_POST	361
# define	K_POWER	362
# define	K_PRE	363
# define	K_PULLDOWNRES	364
# define	K_RECT	365
# define	K_RESISTANCE	366
# define	K_RESISTIVE	367
# define	K_RING	368
# define	K_RISE	369
# define	K_RISECS	370
# define	K_RISERS	371
# define	K_RISESATCUR	372
# define	K_RISETHRESH	373
# define	K_RISESATT1	374
# define	K_RISET0	375
# define	K_RISEVOLTAGETHRESHOLD	376
# define	K_FALLVOLTAGETHRESHOLD	377
# define	K_ROUTING	378
# define	K_ROWMAJOR	379
# define	K_RPERSQ	380
# define	K_SAMENET	381
# define	K_SCANUSE	382
# define	K_SHAPE	383
# define	K_SHRINKAGE	384
# define	K_SIGNAL	385
# define	K_SITE	386
# define	K_SIZE	387
# define	K_SOURCE	388
# define	K_SPACER	389
# define	K_SPACING	390
# define	K_SPECIALNETS	391
# define	K_STACK	392
# define	K_START	393
# define	K_STEP	394
# define	K_STOP	395
# define	K_STRUCTURE	396
# define	K_SYMMETRY	397
# define	K_TABLE	398
# define	K_THICKNESS	399
# define	K_TIEHIGH	400
# define	K_TIELOW	401
# define	K_TIEOFFR	402
# define	K_TIME	403
# define	K_TIMING	404
# define	K_TO	405
# define	K_TOPIN	406
# define	K_TOPLEFT	407
# define	K_TOPRIGHT	408
# define	K_TOPOFSTACKONLY	409
# define	K_TRISTATE	410
# define	K_TYPE	411
# define	K_UNATENESS	412
# define	K_UNITS	413
# define	K_USE	414
# define	K_VARIABLE	415
# define	K_VERTICAL	416
# define	K_VHI	417
# define	K_VIA	418
# define	K_VIARULE	419
# define	K_VLO	420
# define	K_VOLTAGE	421
# define	K_VOLTS	422
# define	K_WIDTH	423
# define	K_X	424
# define	K_Y	425
# define	K_R90	426
# define	T_STRING	427
# define	QSTRING	428
# define	NUMBER	429
# define	K_N	430
# define	K_S	431
# define	K_E	432
# define	K_W	433
# define	K_FN	434
# define	K_FS	435
# define	K_FE	436
# define	K_FW	437
# define	K_R0	438
# define	K_R180	439
# define	K_R270	440
# define	K_MX	441
# define	K_MY	442
# define	K_MXR90	443
# define	K_MYR90	444
# define	K_USER	445
# define	K_MASTERSLICE	446
# define	K_ENDMACRO	447
# define	K_ENDMACROPIN	448
# define	K_ENDVIARULE	449
# define	K_ENDVIA	450
# define	K_ENDLAYER	451
# define	K_ENDSITE	452
# define	K_CANPLACE	453
# define	K_CANNOTOCCUPY	454
# define	K_TRACKS	455
# define	K_FLOORPLAN	456
# define	K_GCELLGRID	457
# define	K_DEFAULTCAP	458
# define	K_MINPINS	459
# define	K_WIRECAP	460
# define	K_STABLE	461
# define	K_SETUP	462
# define	K_HOLD	463
# define	K_DEFINE	464
# define	K_DEFINES	465
# define	K_DEFINEB	466
# define	K_IF	467
# define	K_THEN	468
# define	K_ELSE	469
# define	K_FALSE	470
# define	K_TRUE	471
# define	K_EQ	472
# define	K_NE	473
# define	K_LE	474
# define	K_LT	475
# define	K_GE	476
# define	K_GT	477
# define	K_OR	478
# define	K_AND	479
# define	K_NOT	480
# define	K_DELAY	481
# define	K_TABLEDIMENSION	482
# define	K_TABLEAXIS	483
# define	K_TABLEENTRIES	484
# define	K_TRANSITIONTIME	485
# define	K_EXTENSION	486
# define	K_PROPDEF	487
# define	K_STRING	488
# define	K_INTEGER	489
# define	K_REAL	490
# define	K_RANGE	491
# define	K_PROPERTY	492
# define	K_VIRTUAL	493
# define	K_BUSBITCHARS	494
# define	K_VERSION	495
# define	K_BEGINEXT	496
# define	K_ENDEXT	497
# define	K_UNIVERSALNOISEMARGIN	498
# define	K_EDGERATETHRESHOLD1	499
# define	K_CORRECTIONTABLE	500
# define	K_EDGERATESCALEFACTOR	501
# define	K_EDGERATETHRESHOLD2	502
# define	K_VICTIMNOISE	503
# define	K_NOISETABLE	504
# define	K_EDGERATE	505
# define	K_VICTIMLENGTH	506
# define	K_CORRECTIONFACTOR	507
# define	K_OUTPUTPINANTENNASIZE	508
# define	K_INPUTPINANTENNASIZE	509
# define	K_INOUTPINANTENNASIZE	510
# define	K_CURRENTDEN	511
# define	K_PWL	512
# define	K_ANTENNALENGTHFACTOR	513
# define	K_TAPERRULE	514
# define	K_DIVIDERCHAR	515
# define	K_ANTENNASIZE	516
# define	K_ANTENNAMETALLENGTH	517
# define	K_ANTENNAMETALAREA	518
# define	K_RISESLEWLIMIT	519
# define	K_FALLSLEWLIMIT	520
# define	K_FUNCTION	521
# define	K_BUFFER	522
# define	K_INVERTER	523
# define	K_NAMEMAPSTRING	524
# define	K_NOWIREEXTENSIONATPIN	525
# define	K_WIREEXTENSION	526
# define	K_MESSAGE	527
# define	K_CREATEFILE	528
# define	K_OPENFILE	529
# define	K_CLOSEFILE	530
# define	K_WARNING	531
# define	K_ERROR	532
# define	K_FATALERROR	533
# define	K_RECOVERY	534
# define	K_SKEW	535
# define	K_ANYEDGE	536
# define	K_POSEDGE	537
# define	K_NEGEDGE	538
# define	K_SDFCONDSTART	539
# define	K_SDFCONDEND	540
# define	K_SDFCOND	541
# define	K_MPWH	542
# define	K_MPWL	543
# define	K_PERIOD	544
# define	K_ACCURRENTDENSITY	545
# define	K_DCCURRENTDENSITY	546
# define	K_AVERAGE	547
# define	K_PEAK	548
# define	K_RMS	549
# define	K_FREQUENCY	550
# define	K_CUTAREA	551
# define	K_MEGAHERTZ	552
# define	K_USELENGTHTHRESHOLD	553
# define	K_LENGTHTHRESHOLD	554
# define	K_ANTENNAINPUTGATEAREA	555
# define	K_ANTENNAINOUTDIFFAREA	556
# define	K_ANTENNAOUTPUTDIFFAREA	557
# define	K_ANTENNAAREARATIO	558
# define	K_ANTENNADIFFAREARATIO	559
# define	K_ANTENNACUMAREARATIO	560
# define	K_ANTENNACUMDIFFAREARATIO	561
# define	K_ANTENNAAREAFACTOR	562
# define	K_ANTENNASIDEAREARATIO	563
# define	K_ANTENNADIFFSIDEAREARATIO	564
# define	K_ANTENNACUMSIDEAREARATIO	565
# define	K_ANTENNACUMDIFFSIDEAREARATIO	566
# define	K_ANTENNASIDEAREAFACTOR	567
# define	K_DIFFUSEONLY	568
# define	K_MANUFACTURINGGRID	569
# define	K_ANTENNACELL	570
# define	K_CLEARANCEMEASURE	571
# define	K_EUCLIDEAN	572
# define	K_MAXXY	573
# define	K_USEMINSPACING	574
# define	K_ROWMINSPACING	575
# define	K_ROWABUTSPACING	576
# define	K_FLIP	577
# define	K_NONE	578
# define	K_ANTENNAPARTIALMETALAREA	579
# define	K_ANTENNAPARTIALMETALSIDEAREA	580
# define	K_ANTENNAGATEAREA	581
# define	K_ANTENNADIFFAREA	582
# define	K_ANTENNAMAXAREACAR	583
# define	K_ANTENNAMAXSIDEAREACAR	584
# define	K_ANTENNAPARTIALCUTAREA	585
# define	K_ANTENNAMAXCUTCAR	586
# define	K_SLOTWIREWIDTH	587
# define	K_SLOTWIRELENGTH	588
# define	K_SLOTWIDTH	589
# define	K_SLOTLENGTH	590
# define	K_MAXADJACENTSLOTSPACING	591
# define	K_MAXCOAXIALSLOTSPACING	592
# define	K_MAXEDGESLOTSPACING	593
# define	K_SPLITWIREWIDTH	594
# define	K_MINIMUMDENSITY	595
# define	K_MAXIMUMDENSITY	596
# define	K_DENSITYCHECKWINDOW	597
# define	K_DENSITYCHECKSTEP	598
# define	K_FILLACTIVESPACING	599
# define	K_MINIMUMCUT	600
# define	K_ADJACENTCUTS	601
# define	K_ANTENNAMODEL	602
# define	K_BUMP	603
# define	K_ENCLOSURE	604
# define	K_FROMABOVE	605
# define	K_FROMBELOW	606
# define	K_IMPLANT	607
# define	K_LENGTH	608
# define	K_MAXVIASTACK	609
# define	K_AREAIO	610
# define	K_BLACKBOX	611
# define	K_MAXWIDTH	612
# define	K_MINENCLOSEDAREA	613
# define	K_MINSTEP	614
# define	K_ORIENT	615
# define	K_OXIDE1	616
# define	K_OXIDE2	617
# define	K_OXIDE3	618
# define	K_OXIDE4	619
# define	K_PARALLELRUNLENGTH	620
# define	K_MINWIDTH	621
# define	K_PROTRUSIONWIDTH	622
# define	K_SPACINGTABLE	623
# define	K_WITHIN	624
# define	IF	625
# define	LNOT	626
# define	UMINUS	627

#line 16 "lef.y"

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
 #include "lefiDefs.h"
 #include "lefiUser.h"
 #include "lefiUtil.h"
 #include "lefrCallBacks.h"
 #ifdef WIN32
 #include <malloc.h>
 #endif
  
 #define LYPROP_ECAP "EDGE_CAPACITANCE"
 
 int lefRetVal;
 static char lefPropDefType; /* save the current type of the property */

 
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
 
 /* *********************************************************************/
/* *********************************************************************/

 #define C_EQ 0
 #define C_NE 1
 #define C_LT 2
 #define C_LE 3
 #define C_GT 4
 #define C_GE 5
 
 extern int lefDumbMode; /* for communicating with parser
 */
extern int lefNoNum; /* likewise, says no numbers for the next N tokens
 */
extern int lefRealNum; /* Next token will be a real number
 */
extern int lefNlToken; /* likewise
  */
extern int lefDefIf; /* likewise
  */

 extern int lef_errors; /* from lex.cpph
 */

 static int ignoreVersion = 0; /* ignore checking version number
 */

 int lefNamesCaseSensitive = FALSE; /* are names case sensitive?
 */

 /* XXXXXXXXXXXXX check out these variables */

 double lef_save_x, lef_save_y; /* for interpreting (*) notation of LEF/DEF
 */

 /* #define STRING_LIST_SIZE 1024 */
 /* char string_list[STRING_LIST_SIZE]; */

 
 static char temp_name[256];
 static char layerName[128];
 static char viaName[128];
 static char viaRuleName[128];
 static char nonDefaultRuleName[128];
 static char siteName[128];
 static char arrayName[128];
 static char macroName[128];
 static char pinName[128];
 
 static int siteDef, symDef, sizeDef, pinDef, obsDef, origDef;
 static int useLenThr;
 static int layerCut, layerMastOver, layerRout, layerDir;
 static lefiAntennaEnum antennaType = lefiAntennaAR; /* 5.4 - antenna type */

 static int viaRuleLayer; /* keep track number of layer in a viarule */

 static int viaLayer; /* keep track number of layer in a via */

 static int ndLayer; /* keep track number of layer in a nondefault */

 static int numVia; /* keep track number of via */

 static int viaRuleHasDir; /* viarule layer has direction construct */

 static int viaRuleHasEnc; /* viarule layer has enclosure construct */

 static int ndLayerWidth; /* keep track if width is set at ndLayer */

 static int ndLayerSpace; /* keep track if spacing is set at ndLayer */

 static int isGenerate; /* keep track if viarule has generate */

 static int hasPitch; /* keep track of pitch in layer */

 static int hasWidth; /* keep track of width in layer */

 static int hasDirection; /* keep track of direction in layer */

 static int hasParallel; /* keep track of parallelrunlength */

 static int hasInfluence; /* keep track of influence */

 
 int spParallelLength; /* the number of layer parallelrunlength */

 
 lefiNum macroNum; /* for origin & size callback */

 
 int comp_str(char *s1, int op, char *s2)
 {
 int k = strcmp(s1, s2);
 switch (op) {
 	case C_EQ: return k == 0;
 	case C_NE: return k != 0;
 	case C_GT: return k > 0;
 	case C_GE: return k >= 0;
 	case C_LT: return k < 0;
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
 	case C_GT: return k > 0;
 	case C_GE: return k >= 0;
 	case C_LT: return k < 0;
 	case C_LE: return k <= 0;
 	}
 return 0;
 }
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
 
#line 228 "lef.y"
#ifndef YYSTYPE
typedef union {
 	double dval ;
 	int integer ;
 	char * string ;
 	lefPOINT pt;
 } yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		1727
#define	YYFLAG		-32768
#define	YYNTBASE	385

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 627 ? yytranslate[x] : 751)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     382,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     379,   380,   375,   374,     2,   373,     2,   376,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   378,
     383,   381,   384,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
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
     366,   367,   368,   369,   370,   371,   372,   377
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     3,     4,     9,    13,    17,    18,    21,    23,
      26,    28,    30,    32,    34,    36,    38,    40,    42,    44,
      46,    48,    50,    52,    54,    56,    58,    60,    62,    64,
      66,    68,    70,    72,    74,    76,    78,    80,    82,    84,
      86,    88,    90,    92,    94,    96,    98,   100,   102,   104,
     108,   112,   116,   120,   124,   129,   133,   135,   137,   139,
     141,   143,   145,   150,   152,   153,   156,   161,   166,   171,
     176,   181,   186,   187,   193,   198,   202,   203,   207,   208,
     212,   213,   216,   220,   224,   228,   232,   236,   237,   244,
     248,   253,   261,   266,   274,   278,   282,   286,   290,   294,
     298,   302,   306,   313,   320,   321,   326,   327,   333,   338,
     339,   345,   350,   354,   355,   360,   364,   365,   370,   371,
     377,   381,   382,   387,   391,   392,   397,   398,   404,   405,
     410,   414,   418,   422,   426,   430,   434,   438,   442,   446,
     450,   455,   459,   463,   467,   471,   472,   478,   479,   480,
     490,   494,   502,   503,   508,   509,   510,   511,   512,   524,
     525,   535,   537,   538,   546,   547,   550,   552,   553,   555,
     557,   559,   561,   562,   565,   566,   572,   573,   579,   580,
     585,   586,   592,   593,   596,   597,   603,   604,   609,   610,
     616,   617,   620,   622,   625,   628,   631,   634,   636,   639,
     644,   646,   649,   654,   656,   659,   664,   666,   668,   670,
     672,   674,   676,   678,   680,   681,   683,   685,   686,   691,
     692,   695,   697,   699,   701,   703,   704,   707,   708,   713,
     714,   717,   724,   728,   729,   737,   741,   743,   746,   750,
     751,   754,   756,   758,   762,   763,   768,   770,   772,   775,
     778,   781,   784,   787,   791,   796,   800,   801,   805,   807,
     809,   811,   813,   815,   817,   819,   821,   823,   825,   827,
     829,   831,   833,   835,   837,   840,   841,   846,   847,   850,
     855,   856,   860,   861,   865,   871,   872,   879,   881,   884,
     885,   887,   889,   892,   893,   898,   900,   903,   906,   909,
     912,   915,   916,   919,   923,   924,   929,   930,   933,   937,
     941,   946,   952,   957,   963,   967,   971,   975,   976,   980,
     984,   986,   989,   991,   994,  1000,  1007,  1009,  1013,  1015,
    1018,  1019,  1022,  1026,  1027,  1030,  1033,  1036,  1041,  1045,
    1046,  1052,  1054,  1057,  1058,  1061,  1063,  1065,  1067,  1069,
    1070,  1075,  1077,  1080,  1083,  1086,  1089,  1090,  1097,  1098,
    1101,  1105,  1109,  1113,  1118,  1123,  1127,  1131,  1132,  1136,
    1137,  1141,  1142,  1145,  1151,  1153,  1155,  1159,  1163,  1167,
    1171,  1172,  1175,  1177,  1179,  1181,  1184,  1189,  1190,  1195,
    1196,  1200,  1201,  1205,  1206,  1209,  1211,  1213,  1215,  1217,
    1219,  1221,  1223,  1225,  1227,  1229,  1231,  1233,  1235,  1239,
    1243,  1245,  1247,  1249,  1250,  1255,  1257,  1260,  1264,  1265,
    1268,  1270,  1272,  1274,  1277,  1280,  1283,  1287,  1289,  1292,
    1294,  1296,  1299,  1301,  1303,  1305,  1308,  1310,  1312,  1315,
    1318,  1320,  1322,  1324,  1326,  1328,  1330,  1332,  1334,  1336,
    1338,  1340,  1342,  1344,  1346,  1348,  1350,  1352,  1356,  1361,
    1365,  1369,  1373,  1377,  1381,  1384,  1388,  1393,  1397,  1398,
    1403,  1404,  1409,  1413,  1417,  1419,  1421,  1427,  1431,  1432,
    1436,  1437,  1441,  1442,  1445,  1448,  1452,  1457,  1461,  1466,
    1472,  1473,  1478,  1482,  1484,  1488,  1492,  1496,  1500,  1504,
    1508,  1512,  1516,  1520,  1524,  1528,  1529,  1534,  1535,  1541,
    1542,  1548,  1549,  1555,  1559,  1563,  1567,  1571,  1575,  1579,
    1583,  1587,  1591,  1596,  1600,  1601,  1606,  1611,  1614,  1619,
    1624,  1629,  1633,  1637,  1642,  1647,  1652,  1657,  1662,  1667,
    1672,  1677,  1678,  1683,  1685,  1687,  1689,  1691,  1693,  1696,
    1699,  1702,  1705,  1709,  1713,  1718,  1722,  1726,  1728,  1729,
    1733,  1735,  1737,  1739,  1741,  1743,  1745,  1747,  1749,  1751,
    1753,  1754,  1756,  1758,  1760,  1763,  1764,  1765,  1772,  1776,
    1781,  1788,  1793,  1800,  1808,  1818,  1820,  1821,  1824,  1825,
    1828,  1831,  1833,  1835,  1836,  1839,  1840,  1846,  1847,  1855,
    1863,  1875,  1876,  1877,  1888,  1889,  1890,  1901,  1908,  1915,
    1916,  1919,  1921,  1928,  1935,  1939,  1942,  1944,  1945,  1950,
    1954,  1956,  1959,  1960,  1963,  1964,  1969,  1970,  1975,  1976,
    1987,  1997,  2001,  2005,  2010,  2015,  2020,  2025,  2030,  2035,
    2040,  2045,  2049,  2057,  2066,  2073,  2077,  2081,  2085,  2088,
    2090,  2092,  2094,  2096,  2098,  2100,  2102,  2104,  2106,  2108,
    2110,  2112,  2114,  2116,  2118,  2120,  2123,  2129,  2131,  2134,
    2135,  2140,  2148,  2150,  2152,  2154,  2156,  2158,  2160,  2163,
    2165,  2168,  2169,  2174,  2175,  2179,  2180,  2184,  2185,  2188,
    2189,  2194,  2195,  2200,  2201,  2206,  2207,  2212,  2217,  2218,
    2223,  2229,  2231,  2234,  2235,  2238,  2239,  2244,  2245,  2250,
    2251,  2254,  2260,  2261,  2268,  2269,  2276,  2277,  2284,  2285,
    2292,  2293,  2300,  2301,  2303,  2305,  2307,  2310,  2312,  2315,
    2319,  2323,  2327,  2331,  2334,  2338,  2345,  2347,  2351,  2355,
    2359,  2363,  2367,  2371,  2375,  2379,  2383,  2387,  2390,  2394,
    2401,  2403,  2405,  2409,  2413,  2420,  2422,  2424,  2426,  2428,
    2430,  2432,  2434,  2436,  2438,  2440,  2441,  2447,  2448,  2451,
    2452,  2458,  2459,  2465,  2466,  2472,  2473,  2479,  2480,  2486,
    2487,  2493,  2494,  2500,  2501,  2507,  2508,  2513,  2514,  2519,
    2521,  2524,  2527,  2528,  2529,  2535,  2538,  2544,  2545,  2547,
    2550,  2556,  2560,  2561,  2565,  2566,  2568,  2569,  2570,  2574,
    2579,  2580,  2581,  2585,  2586,  2590,  2595,  2599,  2603,  2607,
    2608,  2616,  2619,  2621,  2624,  2628,  2630,  2631,  2637,  2639,
    2642,  2644,  2647,  2648,  2656,  2658,  2661,  2662,  2670,  2673,
    2675,  2678,  2682,  2684,  2685,  2691,  2693,  2696,  2698,  2701,
    2702,  2710,  2712,  2715,  2719,  2723,  2727,  2731,  2735,  2739
};
static const short yyrhs[] =
{
     390,   391,     0,     0,   241,   387,   173,   378,     0,   261,
     174,   378,     0,   240,   174,   378,     0,     0,   390,   392,
       0,     1,     0,    38,    68,     0,   386,     0,   389,     0,
     393,     0,   401,     0,   406,     0,   473,     0,   494,     0,
     495,     0,   388,     0,   394,     0,   675,     0,   512,     0,
     527,     0,   526,     0,   518,     0,   540,     0,   552,     0,
     655,     0,   679,     0,   528,     0,   690,     0,   716,     0,
     717,     0,   719,     0,   718,     0,   720,     0,   732,     0,
     744,     0,   745,     0,   746,     0,   750,     0,   747,     0,
     748,     0,   749,     0,   395,     0,   396,     0,   397,     0,
     471,     0,   677,     0,    78,    96,   378,     0,    78,    95,
     378,     0,   271,    96,   378,     0,   271,    95,   378,     0,
     315,   175,   378,     0,   320,   399,   400,   378,     0,   317,
     398,   378,     0,   319,     0,   318,     0,    85,     0,   102,
       0,    96,     0,    95,     0,   402,   403,    38,   159,     0,
     159,     0,     0,   403,   404,     0,   149,    79,   175,   378,
       0,    14,   101,   175,   378,     0,   112,    86,   175,   378,
       0,   108,    75,   175,   378,     0,    27,    74,   175,   378,
       0,   167,   168,   175,   378,     0,     0,    31,   405,   173,
     175,   378,     0,   296,   298,   175,   378,     0,   407,   411,
     409,     0,     0,    65,   408,   173,     0,     0,    38,   410,
     173,     0,     0,   411,   412,     0,   157,   460,   378,     0,
     103,   175,   378,     0,    87,   175,   378,     0,   169,   175,
     378,     0,     9,   175,   378,     0,     0,   136,   175,   413,
     705,   710,   378,     0,    34,   461,   378,     0,   112,   126,
     175,   378,     0,   112,   126,   258,   379,   458,   380,   378,
       0,    14,    26,   175,   378,     0,    14,    26,   258,   379,
     456,   380,   378,     0,    54,   175,   378,     0,   272,   175,
     378,     0,   145,   175,   378,     0,   130,   175,   378,     0,
      15,   175,   378,     0,    36,   175,   378,     0,   259,   175,
     378,     0,   257,   175,   378,     0,   257,   258,   379,   454,
     380,   378,     0,   257,   379,   175,   175,   380,   378,     0,
       0,   238,   414,   452,   378,     0,     0,   291,   439,   415,
     440,   378,     0,   291,   439,   175,   378,     0,     0,   292,
     293,   416,   446,   378,     0,   292,   293,   175,   378,     0,
     304,   175,   378,     0,     0,   305,   417,   434,   378,     0,
     306,   175,   378,     0,     0,   307,   418,   434,   378,     0,
       0,   308,   175,   419,   438,   378,     0,   309,   175,   378,
       0,     0,   310,   420,   434,   378,     0,   311,   175,   378,
       0,     0,   312,   421,   434,   378,     0,     0,   313,   175,
     422,   438,   378,     0,     0,   348,   423,   465,   378,     0,
     333,   175,   378,     0,   334,   175,   378,     0,   335,   175,
     378,     0,   336,   175,   378,     0,   337,   175,   378,     0,
     338,   175,   378,     0,   339,   175,   378,     0,   340,   175,
     378,     0,   341,   175,   378,     0,   342,   175,   378,     0,
     343,   175,   175,   378,     0,   344,   175,   378,     0,   345,
     175,   378,     0,   358,   175,   378,     0,   367,   175,   378,
       0,     0,   359,   175,   424,   464,   378,     0,     0,     0,
     346,   175,   169,   175,   425,   462,   426,   463,   378,     0,
     360,   175,   378,     0,   368,   175,   354,   175,   169,   175,
     378,     0,     0,   369,   427,   428,   378,     0,     0,     0,
       0,     0,   366,   175,   429,   451,   430,   169,   175,   431,
     451,   432,   466,     0,     0,    21,   169,   175,   370,   175,
     136,   175,   433,   469,     0,   175,     0,     0,   258,   379,
     551,   551,   435,   436,   380,     0,     0,   436,   437,     0,
     551,     0,     0,   314,     0,   294,     0,   293,     0,   295,
       0,     0,   440,   441,     0,     0,   296,   175,   442,   451,
     378,     0,     0,   297,   175,   443,   451,   378,     0,     0,
     230,   175,   444,   451,     0,     0,   169,   175,   445,   451,
     378,     0,     0,   446,   447,     0,     0,   297,   175,   448,
     451,   378,     0,     0,   230,   175,   449,   451,     0,     0,
     169,   175,   450,   451,   378,     0,     0,   451,   175,     0,
     453,     0,   452,   453,     0,   173,   173,     0,   173,   174,
       0,   173,   175,     0,   455,     0,   454,   455,     0,   379,
     175,   175,   380,     0,   457,     0,   456,   457,     0,   379,
     175,   175,   380,     0,   459,     0,   458,   459,     0,   379,
     175,   175,   380,     0,   124,     0,    29,     0,    94,     0,
     192,     0,   239,     0,   353,     0,    55,     0,   162,     0,
       0,   351,     0,   352,     0,     0,   354,   175,   370,   175,
       0,     0,   169,   175,     0,   362,     0,   363,     0,   364,
       0,   365,     0,     0,   466,   467,     0,     0,   169,   175,
     468,   451,     0,     0,   469,   470,     0,   169,   175,   370,
     175,   136,   175,     0,   355,   175,   378,     0,     0,   355,
     175,   237,   472,   173,   173,   378,     0,   475,   476,   490,
       0,   164,     0,   474,   173,     0,   474,   173,    30,     0,
       0,   476,   477,     0,   481,     0,   485,     0,   112,   175,
     378,     0,     0,   238,   478,   479,   378,     0,   155,     0,
     480,     0,   479,   480,     0,   173,   175,     0,   173,   174,
       0,   173,   173,     0,   482,   378,     0,   482,   551,   378,
       0,   482,   551,   484,   378,     0,   482,   484,   378,     0,
       0,    49,   483,   173,     0,   176,     0,   179,     0,   177,
       0,   178,     0,   180,     0,   183,     0,   181,     0,   182,
       0,   184,     0,   172,     0,   185,     0,   186,     0,   188,
       0,   190,     0,   187,     0,   189,     0,   486,   488,     0,
       0,    65,   487,   173,   378,     0,     0,   488,   489,     0,
     111,   551,   551,   378,     0,     0,    38,   491,   173,     0,
       0,   165,   493,   173,     0,   492,   497,   504,   498,   510,
       0,     0,   492,    51,   496,   497,   498,   510,     0,   503,
       0,   497,   503,     0,     0,   499,     0,   500,     0,   499,
     500,     0,     0,   238,   501,   502,   378,     0,   500,     0,
     502,   500,     0,   173,   173,     0,   173,   174,     0,   173,
     175,     0,   506,   508,     0,     0,   504,   505,     0,   474,
     173,   378,     0,     0,    65,   507,   173,   378,     0,     0,
     508,   509,     0,    34,    55,   378,     0,    34,   162,   378,
       0,   350,   175,   175,   378,     0,   169,   175,   151,   175,
     378,     0,   111,   551,   551,   378,     0,   136,   175,    13,
     175,   378,     0,   112,   175,   378,     0,    93,   175,   378,
       0,    73,   175,   378,     0,     0,    38,   511,   173,     0,
     513,   515,   514,     0,   136,     0,    38,   136,     0,   516,
       0,   515,   516,     0,   517,   173,   173,   175,   378,     0,
     517,   173,   173,   175,   138,   378,     0,   127,     0,   519,
     521,   520,     0,    62,     0,    38,    62,     0,     0,   521,
     522,     0,   525,   523,   378,     0,     0,   523,   524,     0,
     175,   175,     0,   144,   173,     0,    76,   175,   175,   378,
       0,    33,   175,   378,     0,     0,    82,   173,   529,   531,
     530,     0,    38,     0,    38,   173,     0,     0,   531,   532,
       0,   536,     0,   473,     0,   512,     0,   533,     0,     0,
     238,   534,   535,   378,     0,   533,     0,   535,   533,     0,
     173,   173,     0,   173,   174,     0,   173,   175,     0,     0,
      65,   173,   537,   538,    38,   173,     0,     0,   538,   539,
       0,   169,   175,   378,     0,   136,   175,   378,     0,   272,
     175,   378,     0,   112,   126,   175,   378,     0,    14,    26,
     175,   378,     0,    36,   175,   378,     0,   541,   545,   543,
       0,     0,   132,   542,   173,     0,     0,    38,   544,   173,
       0,     0,   545,   546,     0,   133,   175,    13,   175,   378,
       0,   548,     0,   547,     0,    16,    98,   378,     0,    16,
      23,   378,     0,    16,   239,   378,     0,   143,   549,   378,
       0,     0,   549,   550,     0,   170,     0,   171,     0,   172,
       0,   175,   175,     0,   379,   175,   175,   380,     0,     0,
     554,   558,   553,   556,     0,     0,    69,   555,   173,     0,
       0,    38,   557,   173,     0,     0,   558,   559,     0,   566,
       0,   571,     0,   572,     0,   573,     0,   562,     0,   575,
       0,   574,     0,   576,     0,   577,     0,   579,     0,   584,
       0,   581,     0,   585,     0,   267,   268,   378,     0,   267,
     269,   378,     0,   630,     0,   632,     0,   634,     0,     0,
     238,   560,   561,   378,     0,   565,     0,   561,   565,     0,
     143,   563,   378,     0,     0,   563,   564,     0,   170,     0,
     171,     0,   172,     0,   173,   175,     0,   173,   174,     0,
     173,   173,     0,    16,   567,   378,     0,    25,     0,    25,
     349,     0,   114,     0,    10,     0,    10,   357,     0,   324,
       0,    98,     0,   239,     0,    98,   568,     0,    23,     0,
      24,     0,    23,   569,     0,    39,   570,     0,    57,     0,
      90,     0,    56,     0,   108,     0,   135,     0,   356,     0,
      47,     0,   146,     0,   147,     0,   135,     0,   316,     0,
     109,     0,   107,     0,   153,     0,   154,     0,    11,     0,
      12,     0,    52,   173,   378,     0,    51,   173,   173,   378,
       0,   134,   191,   378,     0,   134,    51,   378,     0,   134,
      10,   378,     0,   108,   175,   378,     0,    89,   551,   378,
       0,   482,   378,     0,   482,   551,   378,     0,   482,   551,
     484,   378,     0,   482,   484,   378,     0,     0,    37,   578,
     173,   378,     0,     0,    67,   580,   173,   378,     0,   582,
     173,   378,     0,   582,   621,   378,     0,   132,     0,   132,
       0,   133,   175,    13,   175,   378,     0,   586,   590,   588,
       0,     0,   102,   587,   173,     0,     0,    38,   589,   173,
       0,     0,   590,   591,     0,   482,   378,     0,   482,   551,
     378,     0,   482,   551,   484,   378,     0,   482,   142,   378,
       0,   482,   142,   551,   378,     0,   482,   142,   551,   484,
     378,     0,     0,    67,   592,   173,   378,     0,   108,   175,
     378,     0,   602,     0,   160,   605,   378,     0,   128,   606,
     378,     0,    66,   175,   378,     0,   119,   175,   378,     0,
      46,   175,   378,     0,   118,   175,   378,     0,    45,   175,
     378,     0,   166,   175,   378,     0,   163,   175,   378,     0,
     148,   175,   378,     0,   129,   607,   378,     0,     0,    77,
     593,   173,   378,     0,     0,    91,   594,   175,   175,   378,
       0,     0,    92,   595,   175,   175,   378,     0,     0,    58,
     596,   175,   175,   378,     0,    14,   175,   378,     0,    71,
     175,   378,     0,    72,   175,   378,     0,   112,   175,   378,
       0,   110,   175,   378,     0,    28,     6,   378,     0,    28,
     113,   378,     0,   122,   175,   378,     0,   123,   175,   378,
       0,    64,   173,   173,   378,     0,   260,   173,   378,     0,
       0,   238,   597,   600,   378,     0,   603,   604,   608,    38,
       0,   603,    38,     0,   262,   175,   712,   378,     0,   264,
     175,   712,   378,     0,   263,   175,   712,   378,     0,   265,
     175,   378,     0,   266,   175,   378,     0,   325,   175,   712,
     378,     0,   326,   175,   712,   378,     0,   331,   175,   712,
     378,     0,   328,   175,   712,   378,     0,   327,   175,   712,
     378,     0,   329,   175,   714,   378,     0,   330,   175,   714,
     378,     0,   332,   175,   714,   378,     0,     0,   348,   598,
     599,   378,     0,   362,     0,   363,     0,   364,     0,   365,
       0,   601,     0,   600,   601,     0,   173,   175,     0,   173,
     174,     0,   173,   173,     0,    34,    57,   378,     0,    34,
      90,   378,     0,    34,    90,   156,   378,     0,    34,    56,
     378,     0,    34,    47,   378,     0,   106,     0,     0,    16,
     567,   378,     0,   131,     0,     7,     0,   108,     0,    53,
       0,    17,     0,    32,     0,    57,     0,    90,     0,   139,
       0,   141,     0,     0,     5,     0,   114,     0,    47,     0,
     609,   612,     0,     0,     0,    65,   610,   173,   611,   613,
     378,     0,   169,   175,   378,     0,    99,   614,   616,   378,
       0,    99,    63,   614,   616,   620,   378,     0,   111,   551,
     551,   378,     0,   111,    63,   551,   551,   620,   378,     0,
     105,   614,   615,   615,   615,   616,   378,     0,   105,    63,
     614,   615,   615,   615,   616,   620,   378,     0,   617,     0,
       0,   612,   609,     0,     0,   136,   175,     0,    20,   175,
       0,   551,     0,   551,     0,     0,   615,   616,     0,     0,
     164,   551,   618,   173,   378,     0,     0,   164,    63,   551,
     619,   173,   620,   378,     0,    35,   175,    13,   175,   140,
     175,   175,     0,   173,   175,   175,   484,    35,   175,    13,
     175,   140,   175,   175,     0,     0,     0,   170,   175,    35,
     175,   140,   175,   623,    65,   624,   627,     0,     0,     0,
     171,   175,    35,   175,   140,   175,   625,    65,   626,   627,
       0,   170,   175,    35,   175,   140,   175,     0,   171,   175,
      35,   175,   140,   175,     0,     0,   627,   628,     0,   173,
       0,   170,   175,    35,   175,   140,   175,     0,   171,   175,
      35,   175,   140,   175,     0,   631,   608,    38,     0,   631,
      38,     0,    85,     0,     0,    18,   633,   173,   378,     0,
     635,   637,   636,     0,   150,     0,    38,   150,     0,     0,
     637,   638,     0,     0,    50,   639,   653,   378,     0,     0,
     152,   640,   654,   378,     0,     0,   651,    60,   175,   175,
     641,   650,   161,   175,   175,   378,     0,   651,   646,   158,
     652,   228,   175,   175,   175,   378,     0,   229,   649,   378,
       0,   230,   647,   378,     0,   117,   175,   175,   378,     0,
      44,   175,   175,   378,     0,   116,   175,   175,   378,     0,
      41,   175,   175,   378,     0,   120,   175,   175,   378,     0,
      43,   175,   175,   378,     0,   121,   175,   175,   378,     0,
      42,   175,   175,   378,     0,   158,   652,   378,     0,   207,
     208,   175,   209,   175,   651,   378,     0,   643,   644,   645,
     228,   175,   175,   175,   378,     0,   642,   228,   175,   175,
     175,   378,     0,   285,   174,   378,     0,   286,   174,   378,
       0,   287,   174,   378,     0,   232,   378,     0,   288,     0,
     289,     0,   290,     0,   208,     0,   209,     0,   280,     0,
     281,     0,   282,     0,   283,     0,   284,     0,   282,     0,
     283,     0,   284,     0,   227,     0,   231,     0,   648,     0,
     647,   648,     0,   379,   175,   175,   175,   380,     0,   175,
       0,   649,   175,     0,     0,   175,   175,   175,   175,     0,
     175,   175,   175,   175,   175,   175,   175,     0,   115,     0,
      40,     0,    61,     0,    83,     0,    84,     0,   173,     0,
     653,   173,     0,   173,     0,   654,   173,     0,     0,   657,
     661,   656,   659,     0,     0,     8,   658,   173,     0,     0,
      38,   660,   173,     0,     0,   661,   662,     0,     0,   583,
     663,   621,   378,     0,     0,   199,   664,   621,   378,     0,
       0,   200,   665,   621,   378,     0,     0,   201,   666,   622,
     378,     0,   668,   669,    38,   173,     0,     0,   203,   667,
     629,   378,     0,   204,   175,   673,    38,   204,     0,   679,
       0,   202,   173,     0,     0,   669,   670,     0,     0,   199,
     671,   621,   378,     0,     0,   200,   672,   621,   378,     0,
       0,   673,   674,     0,   205,   175,   206,   175,   378,     0,
       0,   273,   676,   173,   381,   688,   683,     0,     0,   274,
     678,   173,   381,   688,   683,     0,     0,   210,   680,   173,
     381,   686,   683,     0,     0,   211,   681,   173,   381,   688,
     683,     0,     0,   212,   682,   173,   381,   687,   683,     0,
       0,   378,     0,   382,     0,   214,     0,   382,   214,     0,
     215,     0,   382,   215,     0,   686,   374,   686,     0,   686,
     373,   686,     0,   686,   375,   686,     0,   686,   376,   686,
       0,   373,   686,     0,   379,   686,   380,     0,   213,   687,
     684,   686,   685,   686,     0,   175,     0,   686,   689,   686,
       0,   686,   225,   686,     0,   686,   224,   686,     0,   688,
     689,   688,     0,   688,   225,   688,     0,   688,   224,   688,
       0,   687,   218,   687,     0,   687,   219,   687,     0,   687,
     225,   687,     0,   687,   224,   687,     0,   226,   687,     0,
     379,   687,   380,     0,   213,   687,   684,   687,   685,   687,
       0,   217,     0,   216,     0,   688,   374,   688,     0,   379,
     688,   380,     0,   213,   687,   684,   688,   685,   688,     0,
     174,     0,   220,     0,   221,     0,   222,     0,   223,     0,
     218,     0,   219,     0,   381,     0,   383,     0,   384,     0,
       0,   233,   691,   692,    38,   233,     0,     0,   692,   693,
       0,     0,    68,   694,   173,   702,   378,     0,     0,    59,
     695,   173,   702,   378,     0,     0,   102,   696,   173,   702,
     378,     0,     0,    69,   697,   173,   702,   378,     0,     0,
     164,   698,   173,   702,   378,     0,     0,   165,   699,   173,
     702,   378,     0,     0,    65,   700,   173,   702,   378,     0,
       0,    82,   701,   173,   702,   378,     0,     0,   235,   703,
     708,   709,     0,     0,   236,   704,   708,   709,     0,   234,
       0,   234,   174,     0,   270,   173,     0,     0,     0,   237,
     175,   175,   706,   707,     0,   300,   175,     0,   300,   175,
     237,   175,   175,     0,     0,   299,     0,    21,   175,     0,
      21,   175,   237,   175,   175,     0,   237,   175,   175,     0,
       0,   237,   175,   175,     0,     0,   175,     0,     0,     0,
      65,   711,   173,     0,   347,   175,   370,   175,     0,     0,
       0,    65,   713,   173,     0,     0,    65,   715,   173,     0,
     244,   175,   175,   378,     0,   245,   175,   378,     0,   248,
     175,   378,     0,   247,   175,   378,     0,     0,   250,   175,
     721,   378,   723,   722,   683,     0,    38,   250,     0,   724,
       0,   723,   724,     0,   251,   175,   378,     0,   725,     0,
       0,    92,   726,   727,   378,   728,     0,   175,     0,   727,
     175,     0,   729,     0,   728,   729,     0,     0,   252,   175,
     378,   730,   249,   731,   378,     0,   175,     0,   731,   175,
       0,     0,   246,   175,   378,   733,   735,   734,   683,     0,
      38,   246,     0,   736,     0,   735,   736,     0,   251,   175,
     378,     0,   737,     0,     0,    92,   738,   739,   378,   740,
       0,   175,     0,   739,   175,     0,   741,     0,   740,   741,
       0,     0,   252,   175,   378,   742,   253,   743,   378,     0,
     175,     0,   743,   175,     0,   255,   175,   378,     0,   254,
     175,   378,     0,   256,   175,   378,     0,   301,   175,   378,
       0,   302,   175,   378,     0,   303,   175,   378,     0,   242,
       0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   351,   354,   354,   374,   380,   386,   388,   389,   392,
     415,   415,   415,   415,   416,   416,   416,   416,   416,   417,
     417,   418,   418,   418,   418,   418,   418,   418,   419,   419,
     419,   420,   420,   421,   421,   422,   422,   422,   423,   423,
     423,   424,   424,   424,   424,   425,   425,   425,   426,   429,
     436,   444,   446,   449,   452,   462,   465,   467,   469,   471,
     473,   475,   477,   480,   484,   486,   489,   493,   497,   501,
     505,   509,   513,   513,   518,   523,   526,   526,   550,   550,
     570,   574,   577,   582,   588,   593,   599,   604,   604,   631,
     642,   651,   658,   667,   674,   683,   692,   701,   710,   719,
     741,   758,   767,   774,   784,   784,   788,   788,   798,   811,
     811,   821,   836,   857,   857,   878,   899,   899,   920,   920,
     934,   955,   955,   976,   997,   997,  1018,  1018,  1041,  1041,
    1063,  1076,  1089,  1102,  1115,  1128,  1141,  1154,  1167,  1180,
    1193,  1207,  1220,  1233,  1249,  1265,  1265,  1276,  1276,  1276,
    1290,  1296,  1308,  1308,  1325,  1325,  1325,  1325,  1325,  1362,
    1362,  1384,  1391,  1391,  1411,  1413,  1416,  1424,  1426,  1445,
    1447,  1448,  1450,  1451,  1453,  1453,  1461,  1461,  1468,  1468,
    1475,  1475,  1483,  1484,  1486,  1486,  1494,  1494,  1501,  1501,
    1509,  1510,  1515,  1517,  1520,  1533,  1545,  1561,  1564,  1567,
    1573,  1575,  1578,  1584,  1586,  1589,  1595,  1597,  1598,  1599,
    1600,  1601,  1603,  1605,  1607,  1608,  1617,  1626,  1627,  1639,
    1640,  1647,  1654,  1660,  1666,  1673,  1676,  1679,  1679,  1690,
    1693,  1696,  1703,  1724,  1724,  1747,  1751,  1755,  1766,  1777,
    1780,  1783,  1786,  1788,  1792,  1792,  1794,  1798,  1800,  1803,
    1819,  1831,  1844,  1853,  1861,  1869,  1878,  1878,  1881,  1883,
    1884,  1885,  1886,  1887,  1888,  1889,  1890,  1891,  1892,  1893,
    1894,  1895,  1896,  1897,  1899,  1902,  1902,  1909,  1912,  1915,
    1924,  1924,  1940,  1940,  1949,  1971,  1971,  1998,  2000,  2003,
    2006,  2009,  2011,  2014,  2014,  2017,  2019,  2022,  2035,  2047,
    2063,  2077,  2080,  2083,  2088,  2088,  2095,  2098,  2101,  2112,
    2122,  2139,  2144,  2152,  2157,  2161,  2170,  2180,  2180,  2188,
    2191,  2195,  2199,  2201,  2204,  2215,  2227,  2231,  2234,  2243,
    2252,  2255,  2258,  2265,  2268,  2271,  2277,  2282,  2294,  2302,
    2302,  2328,  2329,  2338,  2341,  2344,  2346,  2347,  2348,  2351,
    2351,  2354,  2356,  2359,  2372,  2384,  2400,  2400,  2426,  2429,
    2432,  2439,  2445,  2450,  2465,  2479,  2494,  2497,  2497,  2504,
    2504,  2512,  2515,  2518,  2524,  2526,  2531,  2533,  2534,  2536,
    2539,  2542,  2545,  2549,  2552,  2557,  2560,  2563,  2563,  2568,
    2568,  2583,  2583,  2592,  2595,  2598,  2600,  2601,  2602,  2603,
    2604,  2606,  2608,  2610,  2611,  2612,  2614,  2616,  2618,  2621,
    2624,  2626,  2628,  2630,  2630,  2633,  2635,  2638,  2649,  2652,
    2655,  2659,  2662,  2666,  2682,  2694,  2707,  2715,  2717,  2718,
    2719,  2720,  2721,  2722,  2723,  2724,  2727,  2728,  2736,  2739,
    2743,  2745,  2746,  2747,  2748,  2749,  2751,  2753,  2754,  2755,
    2766,  2778,  2780,  2781,  2782,  2783,  2784,  2786,  2791,  2797,
    2802,  2806,  2811,  2820,  2870,  2880,  2889,  2898,  2908,  2908,
    2913,  2913,  2918,  2923,  2932,  2936,  2939,  2964,  2969,  2969,
    2975,  2975,  2983,  2987,  2990,  2999,  3007,  3015,  3023,  3031,
    3039,  3039,  3043,  3051,  3055,  3059,  3061,  3069,  3077,  3085,
    3093,  3101,  3109,  3117,  3125,  3129,  3129,  3133,  3133,  3143,
    3143,  3153,  3153,  3163,  3171,  3175,  3179,  3187,  3195,  3204,
    3213,  3222,  3231,  3240,  3244,  3244,  3246,  3256,  3273,  3290,
    3307,  3324,  3328,  3332,  3350,  3368,  3386,  3404,  3422,  3440,
    3458,  3476,  3476,  3493,  3500,  3506,  3512,  3519,  3521,  3524,
    3540,  3552,  3565,  3567,  3568,  3569,  3570,  3572,  3581,  3583,
    3589,  3591,  3592,  3593,  3594,  3595,  3597,  3599,  3600,  3601,
    3603,  3607,  3608,  3609,  3611,  3613,  3613,  3613,  3620,  3625,
    3628,  3632,  3640,  3648,  3652,  3656,  3659,  3661,  3663,  3665,
    3670,  3676,  3683,  3690,  3693,  3696,  3696,  3704,  3704,  3714,
    3723,  3742,  3742,  3742,  3757,  3757,  3757,  3771,  3783,  3796,
    3799,  3802,  3807,  3819,  3832,  3843,  3853,  3864,  3864,  3869,
    3872,  3876,  3887,  3890,  3893,  3893,  3904,  3904,  3906,  3906,
    3917,  3937,  3939,  3941,  3946,  3951,  3956,  3961,  3966,  3971,
    3976,  3981,  3985,  3991,  4000,  4007,  4011,  4015,  4019,  4023,
    4026,  4028,  4031,  4034,  4036,  4038,  4041,  4044,  4046,  4049,
    4052,  4054,  4057,  4060,  4063,  4066,  4069,  4076,  4081,  4086,
    4090,  4097,  4109,  4112,  4115,  4118,  4120,  4123,  4128,  4133,
    4138,  4143,  4143,  4152,  4152,  4162,  4162,  4172,  4176,  4179,
    4179,  4188,  4188,  4196,  4196,  4204,  4204,  4211,  4214,  4214,
    4221,  4228,  4231,  4236,  4240,  4243,  4243,  4252,  4252,  4261,
    4265,  4268,  4273,  4273,  4277,  4277,  4281,  4281,  4284,  4284,
    4286,  4286,  4291,  4292,  4293,  4295,  4297,  4300,  4302,  4305,
    4307,  4308,  4309,  4310,  4311,  4312,  4314,  4316,  4318,  4319,
    4320,  4321,  4322,  4323,  4324,  4325,  4326,  4327,  4328,  4329,
    4331,  4332,  4334,  4341,  4343,  4352,  4355,  4357,  4358,  4359,
    4360,  4361,  4362,  4363,  4364,  4367,  4367,  4376,  4380,  4383,
    4383,  4396,  4396,  4408,  4408,  4421,  4421,  4433,  4433,  4445,
    4445,  4457,  4457,  4469,  4469,  4482,  4482,  4488,  4488,  4494,
    4499,  4505,  4513,  4518,  4518,  4526,  4533,  4544,  4548,  4553,
    4563,  4573,  4581,  4585,  4591,  4595,  4600,  4603,  4603,  4609,
    4621,  4625,  4625,  4628,  4628,  4636,  4649,  4660,  4671,  4683,
    4683,  4690,  4700,  4702,  4705,  4714,  4717,  4717,  4723,  4729,
    4735,  4737,  4740,  4740,  4748,  4754,  4760,  4760,  4768,  4778,
    4780,  4783,  4792,  4795,  4795,  4802,  4808,  4814,  4816,  4819,
    4819,  4828,  4834,  4843,  4861,  4879,  4897,  4918,  4939,  4974
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "K_HISTORY", "K_ABUT", "K_ABUTMENT", 
  "K_ACTIVE", "K_ANALOG", "K_ARRAY", "K_AREA", "K_BLOCK", "K_BOTTOMLEFT", 
  "K_BOTTOMRIGHT", "K_BY", "K_CAPACITANCE", "K_CAPMULTIPLIER", "K_CLASS", 
  "K_CLOCK", "K_CLOCKTYPE", "K_COLUMNMAJOR", "K_DESIGNRULEWIDTH", 
  "K_INFLUENCE", "K_COMPONENTS", "K_CORE", "K_CORNER", "K_COVER", 
  "K_CPERSQDIST", "K_CURRENT", "K_CURRENTSOURCE", "K_CUT", "K_DEFAULT", 
  "K_DATABASE", "K_DATA", "K_DIELECTRIC", "K_DIRECTION", "K_DO", 
  "K_EDGECAPACITANCE", "K_EEQ", "K_END", "K_ENDCAP", "K_FALL", "K_FALLCS", 
  "K_FALLT0", "K_FALLSATT1", "K_FALLRS", "K_FALLSATCUR", "K_FALLTHRESH", 
  "K_FEEDTHRU", "K_FIXED", "K_FOREIGN", "K_FROMPIN", "K_GENERATE", 
  "K_GENERATOR", "K_GROUND", "K_HEIGHT", "K_HORIZONTAL", "K_INOUT", 
  "K_INPUT", "K_INPUTNOISEMARGIN", "K_COMPONENTPIN", "K_INTRINSIC", 
  "K_INVERT", "K_IRDROP", "K_ITERATE", "K_IV_TABLES", "K_LAYER", 
  "K_LEAKAGE", "K_LEQ", "K_LIBRARY", "K_MACRO", "K_MATCH", "K_MAXDELAY", 
  "K_MAXLOAD", "K_METALOVERHANG", "K_MILLIAMPS", "K_MILLIWATTS", 
  "K_MINFEATURE", "K_MUSTJOIN", "K_NAMESCASESENSITIVE", "K_NANOSECONDS", 
  "K_NETS", "K_NEW", "K_NONDEFAULTRULE", "K_NONINVERT", "K_NONUNATE", 
  "K_OBS", "K_OHMS", "K_OFFSET", "K_ORIENTATION", "K_ORIGIN", "K_OUTPUT", 
  "K_OUTPUTNOISEMARGIN", "K_OUTPUTRESISTANCE", "K_OVERHANG", "K_OVERLAP", 
  "K_OFF", "K_ON", "K_OVERLAPS", "K_PAD", "K_PATH", "K_PATTERN", 
  "K_PICOFARADS", "K_PIN", "K_PITCH", "K_PLACED", "K_POLYGON", "K_PORT", 
  "K_POST", "K_POWER", "K_PRE", "K_PULLDOWNRES", "K_RECT", "K_RESISTANCE", 
  "K_RESISTIVE", "K_RING", "K_RISE", "K_RISECS", "K_RISERS", 
  "K_RISESATCUR", "K_RISETHRESH", "K_RISESATT1", "K_RISET0", 
  "K_RISEVOLTAGETHRESHOLD", "K_FALLVOLTAGETHRESHOLD", "K_ROUTING", 
  "K_ROWMAJOR", "K_RPERSQ", "K_SAMENET", "K_SCANUSE", "K_SHAPE", 
  "K_SHRINKAGE", "K_SIGNAL", "K_SITE", "K_SIZE", "K_SOURCE", "K_SPACER", 
  "K_SPACING", "K_SPECIALNETS", "K_STACK", "K_START", "K_STEP", "K_STOP", 
  "K_STRUCTURE", "K_SYMMETRY", "K_TABLE", "K_THICKNESS", "K_TIEHIGH", 
  "K_TIELOW", "K_TIEOFFR", "K_TIME", "K_TIMING", "K_TO", "K_TOPIN", 
  "K_TOPLEFT", "K_TOPRIGHT", "K_TOPOFSTACKONLY", "K_TRISTATE", "K_TYPE", 
  "K_UNATENESS", "K_UNITS", "K_USE", "K_VARIABLE", "K_VERTICAL", "K_VHI", 
  "K_VIA", "K_VIARULE", "K_VLO", "K_VOLTAGE", "K_VOLTS", "K_WIDTH", "K_X", 
  "K_Y", "K_R90", "T_STRING", "QSTRING", "NUMBER", "K_N", "K_S", "K_E", 
  "K_W", "K_FN", "K_FS", "K_FE", "K_FW", "K_R0", "K_R180", "K_R270", 
  "K_MX", "K_MY", "K_MXR90", "K_MYR90", "K_USER", "K_MASTERSLICE", 
  "K_ENDMACRO", "K_ENDMACROPIN", "K_ENDVIARULE", "K_ENDVIA", "K_ENDLAYER", 
  "K_ENDSITE", "K_CANPLACE", "K_CANNOTOCCUPY", "K_TRACKS", "K_FLOORPLAN", 
  "K_GCELLGRID", "K_DEFAULTCAP", "K_MINPINS", "K_WIRECAP", "K_STABLE", 
  "K_SETUP", "K_HOLD", "K_DEFINE", "K_DEFINES", "K_DEFINEB", "K_IF", 
  "K_THEN", "K_ELSE", "K_FALSE", "K_TRUE", "K_EQ", "K_NE", "K_LE", "K_LT", 
  "K_GE", "K_GT", "K_OR", "K_AND", "K_NOT", "K_DELAY", "K_TABLEDIMENSION", 
  "K_TABLEAXIS", "K_TABLEENTRIES", "K_TRANSITIONTIME", "K_EXTENSION", 
  "K_PROPDEF", "K_STRING", "K_INTEGER", "K_REAL", "K_RANGE", "K_PROPERTY", 
  "K_VIRTUAL", "K_BUSBITCHARS", "K_VERSION", "K_BEGINEXT", "K_ENDEXT", 
  "K_UNIVERSALNOISEMARGIN", "K_EDGERATETHRESHOLD1", "K_CORRECTIONTABLE", 
  "K_EDGERATESCALEFACTOR", "K_EDGERATETHRESHOLD2", "K_VICTIMNOISE", 
  "K_NOISETABLE", "K_EDGERATE", "K_VICTIMLENGTH", "K_CORRECTIONFACTOR", 
  "K_OUTPUTPINANTENNASIZE", "K_INPUTPINANTENNASIZE", 
  "K_INOUTPINANTENNASIZE", "K_CURRENTDEN", "K_PWL", 
  "K_ANTENNALENGTHFACTOR", "K_TAPERRULE", "K_DIVIDERCHAR", 
  "K_ANTENNASIZE", "K_ANTENNAMETALLENGTH", "K_ANTENNAMETALAREA", 
  "K_RISESLEWLIMIT", "K_FALLSLEWLIMIT", "K_FUNCTION", "K_BUFFER", 
  "K_INVERTER", "K_NAMEMAPSTRING", "K_NOWIREEXTENSIONATPIN", 
  "K_WIREEXTENSION", "K_MESSAGE", "K_CREATEFILE", "K_OPENFILE", 
  "K_CLOSEFILE", "K_WARNING", "K_ERROR", "K_FATALERROR", "K_RECOVERY", 
  "K_SKEW", "K_ANYEDGE", "K_POSEDGE", "K_NEGEDGE", "K_SDFCONDSTART", 
  "K_SDFCONDEND", "K_SDFCOND", "K_MPWH", "K_MPWL", "K_PERIOD", 
  "K_ACCURRENTDENSITY", "K_DCCURRENTDENSITY", "K_AVERAGE", "K_PEAK", 
  "K_RMS", "K_FREQUENCY", "K_CUTAREA", "K_MEGAHERTZ", 
  "K_USELENGTHTHRESHOLD", "K_LENGTHTHRESHOLD", "K_ANTENNAINPUTGATEAREA", 
  "K_ANTENNAINOUTDIFFAREA", "K_ANTENNAOUTPUTDIFFAREA", 
  "K_ANTENNAAREARATIO", "K_ANTENNADIFFAREARATIO", "K_ANTENNACUMAREARATIO", 
  "K_ANTENNACUMDIFFAREARATIO", "K_ANTENNAAREAFACTOR", 
  "K_ANTENNASIDEAREARATIO", "K_ANTENNADIFFSIDEAREARATIO", 
  "K_ANTENNACUMSIDEAREARATIO", "K_ANTENNACUMDIFFSIDEAREARATIO", 
  "K_ANTENNASIDEAREAFACTOR", "K_DIFFUSEONLY", "K_MANUFACTURINGGRID", 
  "K_ANTENNACELL", "K_CLEARANCEMEASURE", "K_EUCLIDEAN", "K_MAXXY", 
  "K_USEMINSPACING", "K_ROWMINSPACING", "K_ROWABUTSPACING", "K_FLIP", 
  "K_NONE", "K_ANTENNAPARTIALMETALAREA", "K_ANTENNAPARTIALMETALSIDEAREA", 
  "K_ANTENNAGATEAREA", "K_ANTENNADIFFAREA", "K_ANTENNAMAXAREACAR", 
  "K_ANTENNAMAXSIDEAREACAR", "K_ANTENNAPARTIALCUTAREA", 
  "K_ANTENNAMAXCUTCAR", "K_SLOTWIREWIDTH", "K_SLOTWIRELENGTH", 
  "K_SLOTWIDTH", "K_SLOTLENGTH", "K_MAXADJACENTSLOTSPACING", 
  "K_MAXCOAXIALSLOTSPACING", "K_MAXEDGESLOTSPACING", "K_SPLITWIREWIDTH", 
  "K_MINIMUMDENSITY", "K_MAXIMUMDENSITY", "K_DENSITYCHECKWINDOW", 
  "K_DENSITYCHECKSTEP", "K_FILLACTIVESPACING", "K_MINIMUMCUT", 
  "K_ADJACENTCUTS", "K_ANTENNAMODEL", "K_BUMP", "K_ENCLOSURE", 
  "K_FROMABOVE", "K_FROMBELOW", "K_IMPLANT", "K_LENGTH", "K_MAXVIASTACK", 
  "K_AREAIO", "K_BLACKBOX", "K_MAXWIDTH", "K_MINENCLOSEDAREA", 
  "K_MINSTEP", "K_ORIENT", "K_OXIDE1", "K_OXIDE2", "K_OXIDE3", "K_OXIDE4", 
  "K_PARALLELRUNLENGTH", "K_MINWIDTH", "K_PROTRUSIONWIDTH", 
  "K_SPACINGTABLE", "K_WITHIN", "IF", "LNOT", "'-'", "'+'", "'*'", "'/'", 
  "UMINUS", "';'", "'('", "')'", "'='", "'\\n'", "'<'", "'>'", "lef_file", 
  "version", "@1", "dividerchar", "busbitchars", "rules", "end_library", 
  "rule", "case_sensitivity", "wireextension", "manufacturing", 
  "useminspacing", "clearancemeasure", "clearance_type", "spacing_type", 
  "spacing_value", "units_section", "start_units", "units_rules", 
  "units_rule", "@2", "layer_rule", "start_layer", "@3", "end_layer", 
  "@4", "layer_options", "layer_option", "@5", "@6", "@7", "@8", "@9", 
  "@10", "@11", "@12", "@13", "@14", "@15", "@16", "@17", "@18", "@19", 
  "sp_options", "@20", "@21", "@22", "@23", "@24", "layer_antenna_pwl", 
  "@25", "layer_diffusion_ratios", "layer_diffusion_ratio", 
  "layer_antenna_duo", "layer_table_type", "ac_layer_table_opt_list", 
  "ac_layer_table_opt", "@26", "@27", "@28", "@29", 
  "dc_layer_table_opt_list", "dc_layer_table_opt", "@30", "@31", "@32", 
  "number_list", "layer_prop_list", "layer_prop", 
  "current_density_pwl_list", "current_density_pwl", "cap_points", 
  "cap_point", "res_points", "res_point", "layer_type", "layer_direction", 
  "layer_connect_type", "layer_cuts_required", "layer_minen_width", 
  "layer_oxide", "layer_sp_parallel_widths", "layer_sp_parallel_width", 
  "@33", "layer_sp_influence_widths", "layer_sp_influence_width", 
  "maxstack_via", "@34", "via", "via_keyword", "start_via", "via_options", 
  "via_option", "@35", "via_prop_list", "via_name_value_pair", 
  "via_foreign", "start_foreign", "@36", "orientation", "via_layer_rule", 
  "via_layer", "@37", "via_rects", "via_rect", "end_via", "@38", 
  "viarule_keyword", "@39", "viarule", "viarule_generate", "@40", 
  "viarule_layer_list", "opt_viarule_props", "viarule_props", 
  "viarule_prop", "@41", "viarule_prop_list", "viarule_layer", 
  "via_names", "via_name", "viarule_layer_name", "@42", 
  "viarule_layer_options", "viarule_layer_option", "end_viarule", "@43", 
  "spacing_rule", "start_spacing", "end_spacing", "spacings", "spacing", 
  "samenet_keyword", "irdrop", "start_irdrop", "end_irdrop", "ir_tables", 
  "ir_table", "ir_table_values", "ir_table_value", "ir_tablename", 
  "minfeature", "dielectric", "nondefault_rule", "@44", "end_nd_rule", 
  "nd_rules", "nd_rule", "nd_prop", "@45", "nd_prop_list", "nd_layer", 
  "@46", "nd_layer_stmts", "nd_layer_stmt", "site", "start_site", "@47", 
  "end_site", "@48", "site_options", "site_option", "site_class", 
  "site_symmetry_statement", "site_symmetries", "site_symmetry", "pt", 
  "macro", "@49", "start_macro", "@50", "end_macro", "@51", 
  "macro_options", "macro_option", "@52", "macro_prop_list", 
  "macro_symmetry_statement", "macro_symmetries", "macro_symmetry", 
  "macro_name_value_pair", "macro_class", "class_type", "pad_type", 
  "core_type", "endcap_type", "macro_generator", "macro_generate", 
  "macro_source", "macro_power", "macro_origin", "macro_foreign", 
  "macro_eeq", "@53", "macro_leq", "@54", "macro_site", "macro_site_word", 
  "site_word", "macro_size", "macro_pin", "start_macro_pin", "@55", 
  "end_macro_pin", "@56", "macro_pin_options", "macro_pin_option", "@57", 
  "@58", "@59", "@60", "@61", "@62", "@63", "pin_layer_oxide", 
  "pin_prop_list", "pin_name_value_pair", "electrical_direction", 
  "start_macro_port", "macro_port_class_option", "macro_pin_use", 
  "macro_scan_use", "pin_shape", "geometries", "geometry", "@64", "@65", 
  "geometry_options", "layer_spacing", "firstPt", "nextPt", "otherPts", 
  "via_placement", "@66", "@67", "stepPattern", "sitePattern", 
  "trackPattern", "@68", "@69", "@70", "@71", "trackLayers", "layer_name", 
  "gcellPattern", "macro_obs", "start_macro_obs", "macro_clocktype", 
  "@72", "timing", "start_timing", "end_timing", "timing_options", 
  "timing_option", "@73", "@74", "@75", "one_pin_trigger", 
  "two_pin_trigger", "from_pin_trigger", "to_pin_trigger", 
  "delay_or_transition", "list_of_table_entries", "table_entry", 
  "list_of_table_axis_numbers", "slew_spec", "risefall", "unateness", 
  "list_of_from_strings", "list_of_to_strings", "array", "@76", 
  "start_array", "@77", "end_array", "@78", "array_rules", "array_rule", 
  "@79", "@80", "@81", "@82", "@83", "floorplan_start", "floorplan_list", 
  "floorplan_element", "@84", "@85", "cap_list", "one_cap", 
  "msg_statement", "@86", "create_file_statement", "@87", "def_statement", 
  "@88", "@89", "@90", "dtrm", "then", "else", "expression", "b_expr", 
  "s_expr", "relop", "prop_def_section", "@91", "prop_stmts", "prop_stmt", 
  "@92", "@93", "@94", "@95", "@96", "@97", "@98", "@99", "prop_define", 
  "@100", "@101", "opt_range_detail", "@102", "opt_range_second", 
  "opt_def_range", "opt_def_value", "opt_spacing_cut", "@103", 
  "opt_layer_name", "@104", "req_layer_name", "@105", 
  "universalnoisemargin", "edgeratethreshold1", "edgeratethreshold2", 
  "edgeratescalefactor", "noisetable", "@106", "end_noisetable", 
  "noise_table_list", "noise_table_entry", "output_resistance_entry", 
  "@107", "num_list", "victim_list", "victim", "@108", "vnoiselist", 
  "correctiontable", "@109", "end_correctiontable", 
  "correction_table_list", "correction_table_item", "output_list", "@110", 
  "numo_list", "corr_victim_list", "corr_victim", "@111", "corr_list", 
  "input_antenna", "output_antenna", "inout_antenna", "antenna_input", 
  "antenna_inout", "antenna_output", "extension", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   385,   387,   386,   388,   389,   390,   390,   390,   391,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   393,
     393,   394,   394,   395,   396,   397,   398,   398,   399,   399,
     400,   400,   401,   402,   403,   403,   404,   404,   404,   404,
     404,   404,   405,   404,   404,   406,   408,   407,   410,   409,
     411,   411,   412,   412,   412,   412,   412,   413,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   414,   412,   415,   412,   412,   416,
     412,   412,   412,   417,   412,   412,   418,   412,   419,   412,
     412,   420,   412,   412,   421,   412,   422,   412,   423,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   424,   412,   425,   426,   412,
     412,   412,   427,   412,   429,   430,   431,   432,   428,   433,
     428,   434,   435,   434,   436,   436,   437,   438,   438,   439,
     439,   439,   440,   440,   442,   441,   443,   441,   444,   441,
     445,   441,   446,   446,   448,   447,   449,   447,   450,   447,
     451,   451,   452,   452,   453,   453,   453,   454,   454,   455,
     456,   456,   457,   458,   458,   459,   460,   460,   460,   460,
     460,   460,   461,   461,   462,   462,   462,   463,   463,   464,
     464,   465,   465,   465,   465,   466,   466,   468,   467,   469,
     469,   470,   471,   472,   471,   473,   474,   475,   475,   476,
     476,   477,   477,   477,   478,   477,   477,   479,   479,   480,
     480,   480,   481,   481,   481,   481,   483,   482,   484,   484,
     484,   484,   484,   484,   484,   484,   484,   484,   484,   484,
     484,   484,   484,   484,   485,   487,   486,   488,   488,   489,
     491,   490,   493,   492,   494,   496,   495,   497,   497,   498,
     498,   499,   499,   501,   500,   502,   502,   500,   500,   500,
     503,   504,   504,   505,   507,   506,   508,   508,   509,   509,
     509,   509,   509,   509,   509,   509,   509,   511,   510,   512,
     513,   514,   515,   515,   516,   516,   517,   518,   519,   520,
     521,   521,   522,   523,   523,   524,   525,   526,   527,   529,
     528,   530,   530,   531,   531,   532,   532,   532,   532,   534,
     533,   535,   535,   533,   533,   533,   537,   536,   538,   538,
     539,   539,   539,   539,   539,   539,   540,   542,   541,   544,
     543,   545,   545,   546,   546,   546,   547,   547,   547,   548,
     549,   549,   550,   550,   550,   551,   551,   553,   552,   555,
     554,   557,   556,   558,   558,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   560,   559,   561,   561,   562,   563,   563,
     564,   564,   564,   565,   565,   565,   566,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     568,   568,   568,   568,   568,   568,   569,   569,   569,   569,
     569,   570,   570,   570,   570,   570,   570,   571,   572,   573,
     573,   573,   574,   575,   576,   576,   576,   576,   578,   577,
     580,   579,   581,   581,   582,   583,   584,   585,   587,   586,
     589,   588,   590,   590,   591,   591,   591,   591,   591,   591,
     592,   591,   591,   591,   591,   591,   591,   591,   591,   591,
     591,   591,   591,   591,   591,   593,   591,   594,   591,   595,
     591,   596,   591,   591,   591,   591,   591,   591,   591,   591,
     591,   591,   591,   591,   597,   591,   591,   591,   591,   591,
     591,   591,   591,   591,   591,   591,   591,   591,   591,   591,
     591,   598,   591,   599,   599,   599,   599,   600,   600,   601,
     601,   601,   602,   602,   602,   602,   602,   603,   604,   604,
     605,   605,   605,   605,   605,   605,   606,   606,   606,   606,
     607,   607,   607,   607,   608,   610,   611,   609,   609,   609,
     609,   609,   609,   609,   609,   609,   612,   612,   613,   613,
     613,   614,   615,   616,   616,   618,   617,   619,   617,   620,
     621,   623,   624,   622,   625,   626,   622,   622,   622,   627,
     627,   628,   629,   629,   630,   630,   631,   633,   632,   634,
     635,   636,   637,   637,   639,   638,   640,   638,   641,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   638,   642,
     642,   642,   643,   643,   643,   643,   644,   644,   644,   645,
     645,   645,   646,   646,   647,   647,   648,   649,   649,   650,
     650,   650,   651,   651,   652,   652,   652,   653,   653,   654,
     654,   656,   655,   658,   657,   660,   659,   661,   661,   663,
     662,   664,   662,   665,   662,   666,   662,   662,   667,   662,
     662,   662,   668,   669,   669,   671,   670,   672,   670,   673,
     673,   674,   676,   675,   678,   677,   680,   679,   681,   679,
     682,   679,   683,   683,   683,   684,   684,   685,   685,   686,
     686,   686,   686,   686,   686,   686,   686,   687,   687,   687,
     687,   687,   687,   687,   687,   687,   687,   687,   687,   687,
     687,   687,   688,   688,   688,   688,   689,   689,   689,   689,
     689,   689,   689,   689,   689,   691,   690,   692,   692,   694,
     693,   695,   693,   696,   693,   697,   693,   698,   693,   699,
     693,   700,   693,   701,   693,   703,   702,   704,   702,   702,
     702,   702,   705,   706,   705,   705,   705,   707,   707,   707,
     707,   707,   708,   708,   709,   709,   710,   711,   710,   710,
     712,   713,   712,   715,   714,   716,   717,   718,   719,   721,
     720,   722,   723,   723,   724,   724,   726,   725,   727,   727,
     728,   728,   730,   729,   731,   731,   733,   732,   734,   735,
     735,   736,   736,   738,   737,   739,   739,   740,   740,   742,
     741,   743,   743,   744,   745,   746,   747,   748,   749,   750
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     2,     0,     4,     3,     3,     0,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     4,     3,     1,     1,     1,     1,
       1,     1,     4,     1,     0,     2,     4,     4,     4,     4,
       4,     4,     0,     5,     4,     3,     0,     3,     0,     3,
       0,     2,     3,     3,     3,     3,     3,     0,     6,     3,
       4,     7,     4,     7,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     6,     0,     4,     0,     5,     4,     0,
       5,     4,     3,     0,     4,     3,     0,     4,     0,     5,
       3,     0,     4,     3,     0,     4,     0,     5,     0,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     3,     3,     3,     0,     5,     0,     0,     9,
       3,     7,     0,     4,     0,     0,     0,     0,    11,     0,
       9,     1,     0,     7,     0,     2,     1,     0,     1,     1,
       1,     1,     0,     2,     0,     5,     0,     5,     0,     4,
       0,     5,     0,     2,     0,     5,     0,     4,     0,     5,
       0,     2,     1,     2,     2,     2,     2,     1,     2,     4,
       1,     2,     4,     1,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     0,     4,     0,
       2,     1,     1,     1,     1,     0,     2,     0,     4,     0,
       2,     6,     3,     0,     7,     3,     1,     2,     3,     0,
       2,     1,     1,     3,     0,     4,     1,     1,     2,     2,
       2,     2,     2,     3,     4,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     4,     0,     2,     4,
       0,     3,     0,     3,     5,     0,     6,     1,     2,     0,
       1,     1,     2,     0,     4,     1,     2,     2,     2,     2,
       2,     0,     2,     3,     0,     4,     0,     2,     3,     3,
       4,     5,     4,     5,     3,     3,     3,     0,     3,     3,
       1,     2,     1,     2,     5,     6,     1,     3,     1,     2,
       0,     2,     3,     0,     2,     2,     2,     4,     3,     0,
       5,     1,     2,     0,     2,     1,     1,     1,     1,     0,
       4,     1,     2,     2,     2,     2,     0,     6,     0,     2,
       3,     3,     3,     4,     4,     3,     3,     0,     3,     0,
       3,     0,     2,     5,     1,     1,     3,     3,     3,     3,
       0,     2,     1,     1,     1,     2,     4,     0,     4,     0,
       3,     0,     3,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       1,     1,     1,     0,     4,     1,     2,     3,     0,     2,
       1,     1,     1,     2,     2,     2,     3,     1,     2,     1,
       1,     2,     1,     1,     1,     2,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     3,
       3,     3,     3,     3,     2,     3,     4,     3,     0,     4,
       0,     4,     3,     3,     1,     1,     5,     3,     0,     3,
       0,     3,     0,     2,     2,     3,     4,     3,     4,     5,
       0,     4,     3,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     0,     4,     0,     5,     0,
       5,     0,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     3,     0,     4,     4,     2,     4,     4,
       4,     3,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     0,     4,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     4,     3,     3,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     1,     2,     0,     0,     6,     3,     4,
       6,     4,     6,     7,     9,     1,     0,     2,     0,     2,
       2,     1,     1,     0,     2,     0,     5,     0,     7,     7,
      11,     0,     0,    10,     0,     0,    10,     6,     6,     0,
       2,     1,     6,     6,     3,     2,     1,     0,     4,     3,
       1,     2,     0,     2,     0,     4,     0,     4,     0,    10,
       9,     3,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     7,     8,     6,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     5,     1,     2,     0,
       4,     7,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     0,     4,     0,     3,     0,     3,     0,     2,     0,
       4,     0,     4,     0,     4,     0,     4,     4,     0,     4,
       5,     1,     2,     0,     2,     0,     4,     0,     4,     0,
       2,     5,     0,     6,     0,     6,     0,     6,     0,     6,
       0,     6,     0,     1,     1,     1,     2,     1,     2,     3,
       3,     3,     3,     2,     3,     6,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     6,
       1,     1,     3,     3,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     5,     0,     2,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     5,     0,
       5,     0,     5,     0,     5,     0,     4,     0,     4,     1,
       2,     2,     0,     0,     5,     2,     5,     0,     1,     2,
       5,     3,     0,     3,     0,     1,     0,     0,     3,     4,
       0,     0,     3,     0,     3,     4,     3,     3,     3,     0,
       7,     2,     1,     2,     3,     1,     0,     5,     1,     2,
       1,     2,     0,     7,     1,     2,     0,     7,     2,     1,
       2,     3,     1,     0,     5,     1,     2,     1,     2,     0,
       7,     1,     2,     3,     3,     3,     3,     3,     3,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     8,     0,   683,     0,     0,   328,    76,   389,     0,
       0,     0,   367,   320,    63,   236,   282,   716,   718,   720,
     765,     0,     2,   859,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   712,   714,     0,     0,     0,
       0,     0,     0,     0,    10,    18,    11,     1,     7,    12,
      19,    44,    45,    46,    13,    64,    14,    80,    47,    15,
       0,   239,     0,    16,    17,    21,     0,    24,   330,    23,
      22,    29,    25,   371,    26,   393,    27,   687,    20,    48,
      28,    30,    31,    32,    34,    33,    35,    36,    37,    38,
      39,    41,    42,    43,    40,     0,     0,     9,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,   767,
       0,     0,     0,     0,     0,     0,     0,   819,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    56,     0,    58,    59,     0,     0,     0,     0,   237,
       0,   285,   304,   301,   287,   306,   326,     0,   322,     0,
       0,     0,   387,   681,   684,   338,    77,   390,     0,    50,
      49,   343,   368,   283,     0,     0,     0,     0,     5,     0,
       0,   816,   836,   818,   817,     0,   854,   853,   855,     4,
      52,    51,     0,     0,   856,   857,   858,    53,    55,    61,
      60,     0,   233,   232,     0,     0,    72,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,   113,     0,   116,     0,
       0,   121,     0,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   128,
       0,     0,     0,     0,     0,   152,    75,    81,   238,   280,
     256,   275,     0,   246,   244,   240,   241,     0,   242,   277,
     235,     0,     0,   288,   289,   300,     0,   319,   323,     0,
       0,     0,   327,   331,   333,     0,   369,     0,   380,   366,
     372,   375,   374,     0,   617,   468,     0,     0,   470,   616,
       0,   478,     0,   474,     0,     0,   418,   620,   413,     0,
       0,     0,   394,   399,   395,   396,   397,   398,   401,   400,
     402,   403,   404,   406,     0,   405,   407,   482,   410,     0,
     411,   412,   622,   475,   691,   693,   695,     0,   698,     0,
     689,     0,   688,   703,   701,   337,     0,     0,     0,     0,
       0,   771,   781,   769,   775,   783,   773,   777,   779,   768,
       3,   815,     0,     0,     0,     0,    54,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,   207,   208,   206,   209,   210,   211,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   169,   171,   106,
     109,     0,     0,     0,     0,   118,     0,     0,     0,     0,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,   258,
     260,   261,   259,   262,   264,   265,   263,   266,   268,   269,
     272,   270,   273,   271,   252,     0,     0,     0,   274,   289,
       0,     0,   293,     0,     0,   290,   291,   302,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   321,     0,   329,
     336,     0,     0,     0,     0,     0,     0,     0,   430,   436,
     437,   427,     0,   433,   429,   434,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   464,     0,     0,   391,   388,     0,
       0,     0,   615,   575,     0,     0,     0,     0,     0,     0,
     586,   585,     0,     0,     0,     0,   702,     0,   709,     0,
     685,   682,     0,   341,     0,     0,   349,   346,   347,   340,
     344,   348,   345,   736,     0,     0,     0,   722,   755,     0,
       0,   722,     0,   751,   750,     0,     0,     0,   722,     0,
     766,     0,     0,     0,     0,     0,     0,     0,     0,   843,
       0,     0,   839,   842,   826,     0,     0,   822,   825,   722,
     722,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,    98,    89,    99,    79,    94,    84,    83,
       0,     0,    97,   792,    96,    82,    85,     0,     0,   192,
     101,     0,     0,   100,    95,     0,   172,     0,   182,   112,
     161,     0,     0,   115,     0,   167,   120,     0,   123,     0,
     167,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,     0,   141,   142,     0,   221,   222,   223,   224,     0,
     143,   219,   150,   144,     0,     0,     0,     0,   281,   257,
       0,   243,     0,     0,   247,   385,     0,   255,   253,     0,
       0,   278,     0,   305,   297,   298,   299,     0,     0,   317,
     284,   292,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,   334,   377,   376,   378,   370,     0,
     382,   383,   384,   379,   381,   431,   446,   449,   447,   448,
     450,   438,   428,   455,   456,   452,   451,   453,   454,   439,
     442,   440,   441,   443,   444,   445,   435,   426,     0,     0,
       0,   457,     0,   463,   479,   462,     0,   461,   460,   459,
     420,   421,   422,   417,   419,     0,     0,   415,   408,   409,
     467,   465,     0,     0,     0,   472,   473,     0,     0,     0,
     480,     0,     0,   511,     0,     0,   490,     0,     0,   505,
     507,   509,   557,     0,     0,     0,     0,     0,     0,     0,
       0,   570,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,     0,   477,   483,   493,   558,     0,     0,   591,
     593,     0,     0,     0,     0,     0,   595,     0,   614,   574,
       0,   673,     0,     0,     0,     0,   624,   672,     0,     0,
       0,     0,   626,     0,     0,   652,   653,     0,     0,     0,
     654,   655,     0,     0,     0,   649,   650,   651,   619,   623,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   705,   707,   704,   342,
     356,   353,   354,   355,     0,     0,   733,     0,     0,     0,
       0,     0,   723,   724,   717,     0,     0,     0,   719,     0,
     747,     0,     0,     0,   760,   761,   756,   757,   758,   759,
       0,     0,   762,   763,   764,     0,     0,     0,     0,     0,
     721,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   722,   840,     0,     0,     0,
     722,   823,   713,   715,     0,    67,    70,     0,    69,    68,
      66,    71,    74,    92,     0,    90,     0,     0,     0,   806,
     194,   195,   196,   105,   193,     0,     0,   197,     0,   108,
       0,   111,     0,     0,   114,   117,   168,     0,   122,   125,
       0,   140,   147,   129,     0,     0,     0,     0,   154,   153,
     276,   251,   250,   249,   245,   248,     0,   254,     0,   286,
     295,     0,   303,     0,   308,   309,   316,   315,     0,   314,
       0,     0,     0,     0,   324,   335,     0,   618,   469,   458,
     471,     0,   425,   424,   423,   414,   416,   466,   392,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   566,   567,   568,   569,
       0,   571,   573,   572,     0,     0,   561,   564,   565,   563,
     562,   560,     0,     0,     0,     0,     0,   810,   810,   810,
       0,     0,   810,   810,   810,   810,     0,     0,   810,     0,
       0,     0,   484,     0,     0,   527,     0,   576,   593,   592,
     593,     0,     0,     0,     0,     0,   597,     0,   578,   587,
     621,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   674,   675,   676,     0,     0,   667,     0,     0,     0,
     664,   648,     0,     0,     0,     0,   656,   657,   658,     0,
       0,   662,   663,     0,   692,   694,     0,     0,   696,     0,
       0,   699,     0,     0,   710,   690,   686,   697,     0,     0,
     358,   351,     0,   725,     0,     0,   734,   730,   729,   731,
     732,     0,   753,   752,     0,   748,   739,   738,   737,   743,
     744,   746,   745,   742,   741,   740,   789,   785,   787,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   845,     0,
     841,   838,   837,   828,     0,   824,   821,   820,   234,    73,
       0,     0,   200,     0,     0,   203,     0,   795,   807,     0,
       0,     0,     0,   198,     0,     0,     0,     0,     0,   107,
     173,     0,     0,     0,   110,   183,     0,   119,   127,   214,
     220,   146,     0,     0,   190,   386,     0,   294,   296,   318,
     312,     0,     0,   310,   325,   373,   476,     0,   513,   518,
     519,   556,   555,   552,     0,   553,   481,   500,   498,     0,
       0,   496,     0,   514,   515,     0,     0,     0,   492,   517,
     516,   499,   497,   520,   521,   495,   504,   503,   494,   502,
     501,     0,     0,   547,   523,   811,     0,     0,     0,   531,
     532,     0,     0,     0,     0,   813,     0,     0,     0,     0,
     543,   544,   545,   546,     0,   487,     0,   485,     0,     0,
       0,   588,     0,   594,   579,     0,     0,     0,   581,     0,
       0,     0,     0,     0,     0,   677,     0,     0,     0,     0,
       0,   679,     0,   641,     0,   668,   631,     0,   632,   665,
     645,   646,   647,     0,   659,   660,   661,     0,     0,     0,
       0,     0,     0,     0,   700,     0,     0,     0,     0,   350,
     352,   726,     0,     0,     0,     0,     0,   790,   802,   802,
     791,   772,   782,   770,   776,   784,   774,   778,   780,   846,
       0,   829,     0,     0,     0,   201,     0,     0,   204,   793,
       0,     0,     0,    88,     0,   102,   103,   180,   178,   174,
     176,   188,   186,   184,   162,   215,   216,   148,     0,     0,
     155,   279,   313,   311,     0,   554,     0,   522,   491,   506,
       0,     0,   551,   550,   549,   525,   548,     0,   528,   530,
     529,   533,   534,   537,   536,     0,   538,   539,   535,   540,
     542,   488,     0,   486,   559,   526,     0,     0,     0,     0,
       0,     0,   593,     0,     0,   596,   636,   640,   638,   634,
     678,   625,   635,   633,   637,   639,   680,   627,     0,     0,
       0,     0,   628,     0,     0,     0,     0,     0,     0,   706,
     708,     0,     0,     0,     0,     0,     0,     0,   359,   727,
       0,     0,     0,     0,     0,   804,   804,     0,   844,   847,
       0,   827,   830,     0,    93,     0,    91,   797,     0,   808,
       0,   199,   190,   190,   190,   190,   190,   190,   190,   164,
     217,   151,     0,   191,     0,     0,   512,   508,   510,   812,
     814,   489,   590,   589,   577,     0,   580,   593,     0,   582,
       0,     0,     0,     0,     0,   669,     0,     0,     0,     0,
       0,     0,     0,     0,   357,     0,     0,     0,     0,   728,
     735,   754,   749,     0,   805,   786,   788,     0,   848,     0,
     831,   202,   205,     0,     0,   798,   794,   796,   809,     0,
     179,     0,     0,     0,   187,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   583,   598,     0,   666,   644,     0,
       0,     0,     0,   601,   604,   612,   613,   711,     0,   365,
       0,   361,   360,   362,   803,   849,   832,   799,     0,   181,
     175,   177,   189,   185,   163,   165,   166,     0,   149,   159,
     156,     0,     0,     0,   642,     0,     0,     0,     0,     0,
       0,   364,   363,     0,     0,     0,   801,     0,   229,   190,
       0,     0,   584,   643,     0,     0,     0,   602,   605,     0,
       0,     0,   218,   160,   157,     0,     0,   670,     0,   630,
     609,   609,   851,     0,   834,     0,   800,     0,   230,   225,
     600,   599,     0,   629,   603,   606,   852,   850,   835,   833,
       0,   158,     0,   611,   610,     0,     0,   226,   671,     0,
     227,     0,   190,   231,   228,     0,     0,     0
};

static const short yydefgoto[] =
{
    1725,    44,   111,    45,    46,     2,    47,    48,    49,    50,
      51,    52,    53,   132,   135,   191,    54,    55,   137,   203,
     370,    56,    57,    98,   256,   384,   138,   257,   623,   400,
     636,   638,   412,   414,   645,   417,   419,   650,   435,   671,
    1249,  1540,   441,   677,  1254,  1544,  1669,  1699,  1668,   642,
    1539,  1606,  1645,   987,   409,   980,  1240,  1534,  1535,  1533,
    1532,   982,  1245,  1538,  1537,  1536,  1430,   628,   629,   976,
     977,  1221,  1222,  1224,  1225,   398,   382,  1427,  1608,   995,
     669,  1711,  1717,  1722,  1683,  1698,    58,   367,    59,    60,
      61,   140,   265,   446,   683,   684,   266,   267,   443,   466,
     268,   269,   444,   468,   691,   270,   442,    62,   105,    63,
      64,   271,   143,   474,   475,   476,   697,  1011,   144,   274,
     477,   145,   272,   275,   486,   700,  1013,    65,    66,   277,
     147,   148,   149,    67,    68,   282,   150,   283,   491,   714,
     284,    69,    70,    71,   161,   559,   346,   560,   561,   894,
    1172,   562,  1170,  1378,  1508,    72,    73,   104,   289,   495,
     151,   290,   291,   292,   497,   724,  1109,    74,   311,    75,
      99,   528,   773,   152,   312,   521,   766,   313,   520,   764,
     767,   314,   507,   746,   731,   739,   315,   316,   317,   318,
     319,   320,   321,   509,   322,   512,   323,   324,   340,   325,
     326,   327,   514,   823,  1047,   531,   824,  1053,  1056,  1057,
    1058,  1050,  1085,  1100,  1324,  1302,  1303,   825,   826,  1106,
    1082,  1070,  1074,   539,   540,   827,  1331,   839,  1468,   830,
    1110,  1111,   541,  1117,  1339,  1470,   530,   878,  1659,  1690,
    1660,  1691,  1704,  1714,   881,   328,   329,   330,   508,   331,
     332,   868,   542,   869,  1125,  1130,  1565,   870,   871,  1149,
    1367,  1153,  1139,  1140,  1137,  1621,   872,  1134,  1346,  1352,
      76,   341,    77,    95,   551,   884,   153,   342,   549,   543,
     544,   545,   547,   343,   552,   888,  1168,  1169,   882,  1164,
      78,   124,    79,   125,    80,   106,   107,   108,   904,  1175,
    1511,   577,   578,   579,   925,    81,   109,   167,   359,   583,
     581,   586,   584,   587,   588,   582,   585,  1200,  1388,  1389,
     969,  1527,  1596,  1515,  1585,  1230,  1411,  1306,  1447,  1316,
    1455,    82,    83,    84,    85,    86,   175,   950,   596,   597,
     598,   947,  1214,  1521,  1522,  1664,  1695,    87,   362,   945,
     591,   592,   593,   942,  1209,  1518,  1519,  1663,  1693,    88,
      89,    90,    91,    92,    93,    94
};

static const short yypact[] =
{
     515,-32768,   655,-32768,   -23,    17,-32768,-32768,-32768,    15,
     254,    64,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    21,-32768,-32768,    35,    43,    51,    70,    76,    87,
     116,   158,   166,   170,   368,-32768,-32768,   194,   206,   209,
     215,   220,   131,   217,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     221,-32768,   138,-32768,-32768,-32768,   284,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   252,    57,-32768,   265,   275,
     302,   119,   130,-32768,   342,   353,   355,   357,   359,-32768,
     144,   372,   374,   182,   187,   192,   196,-32768,   226,   229,
     231,   237,   239,   260,   399,   425,   268,   274,   280,   282,
  -32768,-32768,   303,-32768,-32768,   486,  -172,    20,   145,   517,
     347,-32768,-32768,   535,-32768,-32768,-32768,   123,-32768,   489,
      60,   363,   888,   720,-32768,-32768,-32768,-32768,   311,-32768,
  -32768,-32768,-32768,-32768,   240,   334,   340,  1089,-32768,   351,
     354,-32768,-32768,-32768,-32768,   369,-32768,-32768,-32768,-32768,
  -32768,-32768,   373,   377,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   371,-32768,-32768,   663,   701,-32768,   624,   710,   709,
     722,   642,   525,-32768,   640,   798,   650,    53,   651,-32768,
     652,   653,   654,   705,   662,   665,   666,     2,   668,-32768,
     -78,   669,   670,    68,   545,   671,-32768,   673,-32768,   678,
     686,-32768,   687,-32768,   688,   689,   694,   712,   714,   715,
     716,   717,   718,   719,   732,   733,   737,   738,   739,-32768,
     740,   742,   743,   752,   758,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   759,-32768,-32768,-32768,-32768,   696,-32768,-32768,
  -32768,   535,   762,-32768,   159,    -7,   800,-32768,-32768,   765,
     879,   769,-32768,-32768,-32768,    22,-32768,   768,-32768,-32768,
  -32768,-32768,-32768,    32,-32768,-32768,   771,   772,-32768,-32768,
    -103,-32768,   773,-32768,   774,   137,-32768,-32768,-32768,   283,
     864,   908,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   777,-32768,-32768,-32768,-32768,   554,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   780,-32768,   792,
  -32768,   909,-32768,-32768,-32768,-32768,   148,   -73,   -86,    26,
     721,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   -29,    79,   -86,   -86,-32768,   795,   794,   796,
     801,-32768,   814,   816,   817,   818,   819,   598,    27,   602,
  -32768,-32768,   617,   627,   833,   629,   630,   631,    34,   633,
  -32768,   634,-32768,-32768,-32768,-32768,-32768,-32768,   635,   636,
     842,   638,   573,   843,   641,   656,-32768,-32768,-32768,   857,
     858,   657,    37,   659,    37,-32768,   679,    37,   680,    37,
  -32768,   699,   700,   745,   750,   751,   753,   754,   755,   756,
     757,   881,   760,   761,   910,   306,   764,-32768,   778,   781,
     744,    -6,   924,   945,   946,   783,   947,-32768,   921,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   955,   786,   520,  1025,    -3,
     787,   456,-32768,   967,  1103,    91,-32768,-32768,    84,   968,
     974,  -103,   975,   976,   977,   991,-32768,-32768,   993,-32768,
  -32768,   -82,   797,   799,   804,   997,  1159,   -90,   826,    31,
  -32768,   824,   422,   -21,-32768,-32768,-32768,   806,  1001,  1003,
    1012,   809,  1019,   815,  1036,   832,  1198,   834,   835,   836,
     -27,  1042,   838,   839,-32768,   840,   883,-32768,-32768,   -74,
     841,    45,-32768,-32768,   -40,   -37,   -34,   -33,  1045,  1183,
  -32768,-32768,   570,  1049,  1049,   419,-32768,   424,-32768,  1049,
  -32768,-32768,    29,  1050,  1051,   502,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,    26,   -73,   -73,   266,-32768,    26,
     -86,  -138,    26,-32768,-32768,    26,    26,   805,   300,   779,
  -32768,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,-32768,
    1060,     8,-32768,-32768,-32768,  1061,    14,-32768,-32768,  -138,
    -138,  1064,   855,   856,  1063,   861,   862,   863,   866,   867,
  -32768,   868,   869,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     871,   872,-32768,   -62,-32768,-32768,-32768,   561,  -129,-32768,
  -32768,   873,  1072,-32768,-32768,   877,-32768,   878,-32768,-32768,
  -32768,   880,   882,-32768,   884,   936,-32768,   885,-32768,   886,
     936,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   887,-32768,-32768,  1082,-32768,-32768,-32768,-32768,   889,
  -32768,  1097,-32768,-32768,  1083,  1099,  1094,   892,-32768,-32768,
     893,-32768,   565,  -112,-32768,-32768,  1098,-32768,-32768,   894,
    -103,-32768,  1103,-32768,-32768,-32768,-32768,    91,   896,-32768,
  -32768,-32768,   897,   898,   899,   900,  -103,   901,  1267,  1130,
    1108,  -106,  1109,-32768,-32768,-32768,-32768,-32768,-32768,  1110,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   911,   912,
     913,-32768,   914,-32768,-32768,-32768,  1111,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   578,   -96,-32768,-32768,-32768,
  -32768,-32768,   915,  1114,  1113,-32768,-32768,  1119,   142,   464,
  -32768,  1120,  1121,-32768,  1124,  1123,-32768,  1125,  1127,-32768,
  -32768,-32768,-32768,  1128,  1129,  1131,  1132,  1133,  1134,  1135,
     460,   413,  1136,   611,  1137,  1138,-32768,  1126,  1139,  1140,
    1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,-32768,  -101,-32768,-32768,-32768,   169,  1154,  -103,-32768,
    -103,  -103,  -103,  -103,  -103,  -103,-32768,   927,-32768,   685,
    1178,-32768,  1155,  1156,  1157,  1158,-32768,-32768,  1160,  1161,
    1162,  1163,-32768,   337,  1152,-32768,-32768,  1165,   950,   956,
  -32768,-32768,  1167,  1168,  1169,-32768,-32768,-32768,-32768,-32768,
    1116,   484,   160,  1170,   969,   970,  1171,  1174,   972,  1176,
    1177,   978,    69,   979,  1180,  1181,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   151,   189,-32768,   281,   -73,   -73,
     -73,   -73,-32768,-32768,-32768,   189,  -304,   -86,-32768,   189,
  -32768,   741,   243,   222,-32768,-32768,-32768,-32768,-32768,-32768,
     -73,   -73,-32768,-32768,-32768,   -73,    26,    26,    26,    26,
  -32768,   -86,   -86,   -86,   299,   299,   299,   299,   299,   299,
     299,   299,  1184,   980,  1115,   -24,-32768,  1187,   985,  1105,
     -24,-32768,-32768,-32768,   986,-32768,-32768,   987,-32768,-32768,
  -32768,-32768,-32768,-32768,   988,-32768,   989,  1191,  1194,   -28,
  -32768,-32768,-32768,-32768,-32768,  1195,   270,-32768,   992,-32768,
    -105,-32768,  -115,  -103,-32768,-32768,-32768,   995,-32768,-32768,
     996,-32768,-32768,-32768,  1196,   998,  1206,  1202,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   999,-32768,  -103,-32768,
  -32768,  -125,-32768,  1205,-32768,-32768,-32768,-32768,  1002,-32768,
    1207,  1208,  1006,  1007,-32768,-32768,  1008,-32768,-32768,-32768,
  -32768,  1009,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1018,
    1010,  1011,  1013,  1014,  1015,  1016,  -118,  1217,  1017,  1020,
    1221,  1224,  1021,  1227,  1023,  1024,  1230,  1229,  1231,  1027,
    1029,  1030,  1031,  1032,  1033,  1034,-32768,-32768,-32768,-32768,
    1035,-32768,-32768,-32768,  1037,  1038,-32768,-32768,-32768,-32768,
  -32768,-32768,  1039,  1040,  1041,  1241,  1043,  1316,  1316,  1316,
    1044,  1046,  1316,  1316,  1316,  1316,  1355,  1355,  1316,  1355,
     349,   -80,-32768,   904,    32,-32768,   685,-32768,  -103,-32768,
    -103,  1047,  -103,  -103,  -103,  1048,-32768,  1250,-32768,-32768,
  -32768,  1252,  1253,  1254,  1255,  1258,  1257,  1259,  1260,  1261,
    1264,-32768,-32768,-32768,  1062,  1263,-32768,   -65,  1266,   305,
  -32768,-32768,  1065,  1066,  1067,  1271,-32768,-32768,-32768,   490,
    1272,-32768,-32768,  1275,-32768,-32768,  1404,  1407,-32768,  1413,
    1414,-32768,  1246,  1276,-32768,-32768,-32768,-32768,  1049,  1049,
  -32768,-32768,  -120,-32768,  1238,   -73,-32768,   319,   319,-32768,
  -32768,   -86,-32768,-32768,    26,-32768,   370,   370,   370,-32768,
  -32768,   574,   107,  1079,  1079,  1079,  1280,-32768,-32768,  1282,
    1078,  1080,  1081,  1084,  1085,  1086,  1087,  1088,-32768,   -56,
  -32768,-32768,-32768,-32768,   -51,-32768,-32768,-32768,-32768,-32768,
    1285,   418,-32768,  1286,   433,-32768,  1292,  1220,-32768,  1293,
    1091,  1295,  1093,-32768,  1095,  1297,  1299,  1300,  1301,-32768,
  -32768,  1302,  1303,  1304,-32768,-32768,  -103,-32768,-32768,   470,
  -32768,-32768,  1305,  1112,-32768,-32768,  1106,-32768,-32768,-32768,
  -32768,  1107,  1117,-32768,-32768,-32768,-32768,  1446,-32768,-32768,
  -32768,-32768,-32768,-32768,  1118,-32768,-32768,-32768,-32768,  1308,
    1122,-32768,  1153,-32768,-32768,  1164,  1311,  1312,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   607,   -89,-32768,-32768,-32768,  1166,  1172,  1173,-32768,
  -32768,  1175,  1179,  1182,  1185,-32768,  1186,  1188,  1189,  1190,
  -32768,-32768,-32768,-32768,  1192,-32768,   923,-32768,  1193,  1197,
    1450,    48,  1454,-32768,-32768,  -103,  -103,  1454,-32768,  1317,
    1199,  1200,  1201,  1203,  1204,-32768,   -84,  1209,  1210,  1211,
    1212,-32768,   -81,-32768,  1283,-32768,-32768,  1318,-32768,-32768,
  -32768,-32768,-32768,  1319,-32768,-32768,-32768,  1269,  1323,   337,
    1324,  1326,  1327,  1328,-32768,  1298,  1213,  1214,    85,-32768,
  -32768,-32768,   125,  -155,   251,   191,   763,-32768,  1268,  1268,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1239,-32768,  1256,  1331,  1215,-32768,  1332,  1216,-32768,-32768,
    1334,  1337,  1225,-32768,  1218,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1219,  1336,
    1338,-32768,-32768,-32768,  1339,-32768,  1222,-32768,-32768,-32768,
    1223,  1226,-32768,-32768,-32768,-32768,-32768,  1342,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1343,-32768,-32768,-32768,-32768,
  -32768,-32768,  1228,-32768,-32768,-32768,  1344,  1345,  1232,  1346,
    1233,  -103,  -103,  1234,  1454,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1347,  1348,
    1349,  1350,-32768,  1284,  1377,  1378,  1386,  1387,  1353,-32768,
  -32768,  1503,  1357,  1360,  1408,  1361,  1362,  1363,-32768,-32768,
    1315,   -73,   -86,    26,  1364,  1365,  1365,  1366,  1239,-32768,
    1368,  1256,-32768,  1235,-32768,  1236,-32768,    19,  1370,-32768,
    1371,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1242,-32768,  1399,-32768,  1379,  1534,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1536,-32768,  -103,  1240,-32768,
    1243,   132,  1237,  1244,  1380,  1381,  1383,  1384,  1390,  1394,
    1397,  1245,  1398,  1247,-32768,  1401,  1248,  1249,  1251,-32768,
     370,  1079,   448,  1405,-32768,-32768,-32768,  1262,-32768,  1265,
  -32768,-32768,-32768,  1409,  1410,-32768,-32768,-32768,-32768,   -44,
    1338,   -42,   -41,   -26,  1338,   -25,  -100,  1411,  1270,  1424,
    1427,  1428,  1430,  1454,-32768,-32768,  1273,-32768,-32768,  1432,
    1433,  1391,  1434,  1274,  1277,-32768,-32768,-32768,  1278,-32768,
    1279,-32768,-32768,-32768,-32768,-32768,-32768,  1325,  1438,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1288,-32768,-32768,
  -32768,  1421,  1443,  1281,-32768,  1287,  1439,  1444,  1445,  1489,
    1509,-32768,-32768,  1375,  1382,  1449,-32768,  1455,-32768,-32768,
    1457,  1458,-32768,-32768,  1459,  1460,  1289,-32768,-32768,  1461,
    1462,  1463,-32768,  1470,  1338,  1466,  1467,  1469,  1290,-32768,
  -32768,-32768,-32768,   -19,-32768,   -10,-32768,  1471,-32768,-32768,
  -32768,-32768,  1472,-32768,  1476,  1476,-32768,-32768,-32768,-32768,
    1291,  1481,  1478,-32768,-32768,  1479,  1485,-32768,-32768,  1526,
  -32768,  1488,-32768,-32768,  1338,  1645,  1664,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,     5,
  -32768,-32768,-32768,  1022,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768, -1106,-32768,  1068,-32768,
     690,-32768,   449,-32768,   445,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1329,  1400,
  -32768,-32768,-32768,-32768,-32768,   990,-32768,  -143,-32768,  -309,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1406,  1294,-32768,  -467,-32768,-32768,  -121,-32768,
  -32768,-32768,-32768,-32768,-32768,   984,-32768,  1333,-32768,-32768,
  -32768,  1524,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -869,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  -267,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     916,-32768,   576,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   376,-32768,-32768,-32768,
  -32768,-32768,-32768,   575,   844,-32768,-32768,-32768,-32768,  -511,
    -826, -1092,-32768,-32768,-32768, -1298,  -532,-32768,-32768,-32768,
  -32768,-32768,    -5,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   546,-32768,-32768,   126,   315,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1535,-32768,-32768,-32768,  -550,  -527,
    -891,  -342,  -562,  -345,  -575,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -132,-32768,-32768,
  -32768,-32768,-32768,   301,   173,-32768,-32768,  -552,-32768,  -901,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1096,
  -32768,-32768,-32768,-32768,   172,-32768,-32768,-32768,-32768,-32768,
  -32768,  1100,-32768,-32768,-32768,-32768,   176,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768
};


#define	YYLAST		1763


static const short yytable[] =
{
     467,   525,   895,   571,   933,   567,  1113,   905,   701,   310,
     909,   874,   875,   910,   912,   675,  1332,   883,  1333,   599,
     600,   908,   273,   828,   832,  1171,   831,   478,   930,   833,
     835,   392,  1023,   513,   194,   740,   741,  1228,  1274,  1473,
    1593,  1101,   498,   526,   627,   492,   944,   195,   471,   952,
     953,   196,   949,   555,  1241,   499,   500,   501,   197,   777,
    1509,   682,   142,   589,  1235,   192,   479,   885,  1466,   742,
     907,   502,   448,   778,   448,   448,  1182,   765,   726,   779,
     720,   721,   722,   780,  1301,    97,   480,   743,   568,  1480,
     781,   782,  1486,   712,   260,   448,   393,   401,   280,  1501,
     589,   774,   563,   783,   481,   482,   594,  1162,   380,   784,
    1355,   785,   786,   472,   744,  1242,   787,   788,   556,  1399,
     493,  1502,   789,  1503,  1401,  1236,   394,   569,   198,   483,
     503,  1543,   199,  1543,  1543,   448,   790,   791,   448,   702,
     564,   448,   448,   760,   761,   762,   504,   517,  1041,  1543,
    1543,   792,    96,   793,   204,   794,  1706,   795,   689,   205,
     206,   276,   484,   796,   797,  1708,   727,   798,   799,   200,
     471,   594,   841,   800,   801,   967,  1560,   728,   729,   207,
     402,   208,  1243,   209,  1467,  1104,   553,   201,   518,   141,
     100,  1237,  1238,   802,   395,   110,  1317,  1504,  1319,   210,
     568,   563,   611,   142,   281,   803,   193,  1105,   804,   620,
     112,   805,   640,   554,   706,   381,   133,   772,   113,   907,
    1150,  1505,   590,   896,   897,   906,   114,  1510,   886,   887,
    1010,   913,   211,   134,   911,   472,   907,   103,   968,   572,
     902,   396,   573,   574,   903,   115,   703,   847,   212,   973,
     146,   116,   575,  1257,  1506,  1042,  1594,   213,  1379,   590,
    1275,   494,   117,  1244,   471,   595,  1004,   829,   829,   834,
     836,   505,  1024,  1239,  1163,   214,   465,  1102,   465,   465,
    1644,   215,  1035,   806,    13,   612,  1335,  1336,   723,  1445,
     216,   118,   621,   570,  1481,   641,   713,  1487,  1325,   465,
     565,   403,   217,  1380,   775,   807,   566,   808,   809,   810,
     811,   812,    15,  1356,   218,  1653,   202,  1108,  1595,  1229,
    1112,   555,  1400,    15,   555,   926,   927,  1402,   519,   472,
     595,   928,   471,   119,  1639,   745,  1640,  1641,   933,   465,
    1509,   120,   465,   485,   121,   465,   465,   730,   273,   101,
     102,   763,  1642,  1643,   902,   397,   506,  1507,   903,  1707,
     676,   406,   407,   408,  1189,  1190,  1191,  1192,  1709,   126,
     813,   814,   815,   816,   817,   818,   819,   820,  1181,   285,
    1558,   127,  1184,   219,   128,   259,   556,  1151,   822,   556,
     129,  1152,   136,   821,   139,  1212,   260,   472,  1131,   565,
    1217,   286,   220,  1173,   221,   576,  1509,   926,   927,   926,
     927,   146,   261,   928,   929,   928,   929,   222,  1071,   644,
    1132,  1133,   647,  1008,   649,   154,  1599,  1600,  1601,  1602,
    1603,  1604,  1605,   733,   734,   155,   223,   224,   156,  1018,
     914,   915,   916,   917,   918,   919,   931,   932,   157,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   262,
    1072,   926,   927,   122,   123,  1613,  1509,   928,   929,   914,
     915,   916,   917,   918,   919,   920,   921,   158,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,  1512,   249,  1513,  1512,   287,   159,   898,   899,
     900,   901,   263,   250,   251,   252,   288,  1510,   160,  1471,
    1472,  1043,   253,   254,   255,   162,     1,  1066,   926,   927,
    1044,  1045,   168,    -6,   928,   929,   163,  1073,   164,   735,
     165,   736,   166,  1196,  1197,  1198,  1307,  1308,   130,   131,
    1311,  1312,  1313,  1314,  1258,   169,  1318,   258,    -6,   170,
    1067,   522,   523,    -6,  1046,  1103,  1177,  1178,  1179,  1180,
     171,   829,  1183,  1684,   829,   172,  1114,  1115,  1116,  1199,
     173,  1174,   182,  1510,   174,   737,   738,    -6,  1186,  1187,
      -6,   189,   190,  1188,    -6,   264,  1193,  1194,  1195,   876,
     877,    -6,   532,    -6,   879,   880,   907,    -6,   183,  1068,
     142,  1069,  1182,   922,   176,   923,   924,   177,   840,   178,
     841,   842,   843,   844,   845,   179,  1724,   180,  1076,   533,
     846,   347,  1385,  1185,   898,   899,   900,   901,  1077,   694,
     695,   696,   922,  1510,   923,   924,  1376,  1377,   181,   898,
     899,   900,   901,  1078,   902,  1557,   184,    -6,   903,   975,
    1232,    -6,   185,   534,   898,   899,   900,   901,   186,   535,
     187,  1176,   279,     3,  1079,   536,   926,   927,   665,   666,
     667,   668,   928,   929,    -6,   891,   892,   893,   902,    -6,
      -6,   188,   903,  1358,  1138,   847,   848,   849,     4,   345,
     850,   851,   447,     5,   900,   901,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,  1320,  1321,  1322,  1323,   348,  1246,     6,   537,  1080,
       7,   349,   852,   538,     8,    -6,    -6,    -6,   853,   360,
    1267,     9,   361,    10,   970,   971,   972,    11,  1001,  1002,
    1003,  1256,  1081,   898,   899,   900,   901,   363,    -6,   366,
     533,  1032,  1033,  1034,   364,    -6,    -6,    -6,   365,    -6,
      -6,    -6,    -6,    -6,   368,    -6,  1146,  1147,  1148,    -6,
      -6,    -6,  1364,  1365,  1366,   369,    -6,   854,   855,   856,
    1442,  1443,  1444,   371,   534,   372,    -6,    12,    -6,    -6,
     535,    13,   926,   927,  1328,   373,   536,  1220,  1404,   857,
     858,   374,   859,  1201,  1202,  1203,  1204,  1205,  1206,  1207,
     375,   933,  1223,  1407,    14,   377,    -6,    -6,    -6,    15,
      16,  1425,  1426,   376,   378,   379,   383,   385,   386,   387,
      -6,   388,    -6,  1382,  1326,    -6,  1383,   389,   410,  1386,
     390,   391,  1384,   399,   404,   405,   411,  1337,   413,   537,
     860,   861,   333,   415,   538,   862,   863,   864,   865,   866,
     867,   416,   418,   420,   421,    17,    18,    19,   447,   422,
      -6,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   423,    20,   424,
     425,   426,   427,   428,   429,    21,    22,    23,   688,    24,
      25,    26,    27,    28,   293,    29,   294,   430,   431,    30,
      31,    32,   432,   433,   434,   436,    33,   437,   438,   334,
     335,   336,   337,   338,   339,   295,    34,   439,    35,    36,
      17,    18,    19,   440,   445,   470,   487,   260,   488,   296,
     297,   489,   490,   496,   510,   511,   527,   550,   515,   516,
     529,  1582,   631,   546,   580,   298,    37,    38,    39,   914,
     915,   916,   917,   918,   919,   920,   921,   548,   601,   602,
      40,   603,    41,   299,   604,    42,   610,   300,  1509,  1424,
     613,   914,   915,   916,   917,   918,   919,   931,   932,   605,
     301,   606,   607,   608,   609,   614,   302,   914,   915,   916,
     917,   918,   919,   931,   932,   615,   616,   617,   618,   619,
      43,   622,   624,   625,   626,   627,   630,  1462,   632,   633,
     303,   304,   305,   914,   915,   916,   917,   918,   919,   920,
     921,   306,   635,   637,   634,   639,   447,   643,   307,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   447,   661,   646,   648,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   447,   651,   652,   664,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   447,   685,   678,   674,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   898,   899,   900,   901,   679,   680,
     682,  1176,   922,   653,   923,   924,   308,   350,   654,   655,
     686,   656,   657,   658,   659,   660,   690,   907,   662,   663,
     698,   699,   670,   704,   922,  1510,   923,   924,   351,   705,
     707,   708,   709,   907,   352,   309,   672,   353,   354,   673,
     922,   681,   923,   924,   687,   693,   710,  1581,   711,  1580,
     718,   355,   719,   732,   748,   715,   749,   716,   898,   899,
     900,   901,   717,   725,   747,   750,   922,   751,   923,   924,
     447,   356,   752,   753,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   754,
     755,   756,   757,   758,   759,   765,   768,   769,   770,   776,
     837,   838,   873,   889,   890,   934,   935,   936,   937,   938,
     939,   940,   941,   955,   956,   943,   948,   954,   957,   958,
     959,   960,   524,   465,   961,   962,   963,   978,   964,   965,
     986,   966,   975,   357,   358,   979,   981,   992,   996,   983,
     984,   771,   985,   988,   989,   991,   994,   993,   997,   998,
     999,  1000,  1007,  1006,  1012,  1014,  1015,  1016,  1017,  1019,
    1020,  1021,  1327,  1022,  1025,  1026,  1031,  1038,  1039,  1027,
    1028,  1029,  1030,  1037,  1040,  1048,  1049,  1051,  1052,  1086,
    1054,  1461,  1055,  1059,  1060,  1118,  1061,  1062,  1063,  1064,
    1065,  1075,  1083,  1084,  1087,  1088,  1089,  1090,  1091,  1092,
    1093,  1094,  1095,  1096,  1097,  1098,  1099,  1107,  1120,  1138,
    1121,  1122,  1123,  1124,  1141,  1126,  1127,  1128,  1129,  1646,
    1136,  1142,  1143,  1144,  1145,   774,  1156,  1154,  1155,  1157,
    1158,  1159,  1160,  1166,  1167,  1216,  1161,  1165,  1210,  1208,
    1135,  1211,  1213,  1215,  1218,  1219,  1226,  1220,  1223,  1227,
    1231,  1250,  1234,  1247,  1248,  1252,  1251,  1253,  1259,  1255,
    1260,  1305,  1261,  1262,  1263,  1264,  1265,  1266,  1268,  1269,
    1276,  1270,  1271,  1272,  1273,  1277,  1279,  1280,  1278,  1281,
    1282,  1283,  1284,  1285,  1286,  1288,  1287,  1289,  1290,  1291,
    1292,  1293,  1294,  1295,  1301,  1296,  1297,  1298,  1299,  1300,
    1315,  1304,  1309,  1340,  1310,  1334,  1338,  1341,  1342,  1343,
    1344,  1345,  1347,  1369,  1348,  1349,  1350,  1351,  1354,  1370,
    1353,  1357,  1371,  1360,  1361,  1362,  1363,  1368,  1372,  1373,
    1374,  1375,  1381,   907,  1387,  1390,  1391,  1410,  1392,  1393,
    1403,  1406,  1394,  1395,  1396,  1397,  1398,  1409,  1412,  1413,
    1414,  1415,  1417,  1416,  1418,  1419,  1420,  1421,  1422,  1423,
    1428,  1434,  1429,  1436,  1431,  1432,  1440,  1441,  1465,  1469,
    1474,  1517,  1488,  1489,  1490,  1433,  1435,  1491,  1492,  1494,
    1437,  1495,  1496,  1497,  1498,  1514,  1523,  1525,  1520,  1528,
    1529,  1542,  1566,  1543,  1545,  1549,  1550,  1567,  1568,  1552,
    1553,  1555,  1561,  1562,  1563,  1564,  1569,  1570,  1571,  1572,
    1579,  1438,  1573,  1574,  1575,  1609,  1576,  1577,  1578,  1583,
    1584,  1587,  1439,  1589,  1448,  1597,  1598,  1611,  1610,  1612,
    1449,  1450,  1657,  1451,  1677,  1619,  1620,  1452,  1622,  1623,
    1453,  1670,  1665,  1454,  1456,  1624,  1457,  1458,  1459,  1625,
    1460,  1463,  1626,  1628,  1678,  1464,  1630,  1475,  1476,  1477,
    1634,  1478,  1479,  1671,  1637,  1638,  1647,  1482,  1483,  1484,
    1485,  1499,  1500,  1524,  1526,  1530,  1607,  1541,  1531,  1649,
    1546,  1547,  1650,  1651,  1548,  1652,  1551,  1655,  1656,  1658,
    1554,  1556,  1559,  1666,  1674,  1591,  1592,  1617,  1614,  1675,
    1676,  1615,  1618,  1627,  1681,  1629,  1631,  1632,  1679,  1633,
    1682,  1680,  1685,  1686,  1687,  1688,  1692,  1694,  1696,  1697,
    1635,  1700,  1701,  1636,  1702,  1726,  1710,  1712,  1648,  1713,
    1716,  1654,  -607,  1718,  1719,  -608,  1661,  1662,  1667,  1672,
    1720,  1715,  1721,  1723,  1727,  1673,  1233,  1689,  1703,  1408,
    1405,   278,   990,  1005,   473,   557,  1009,   469,  1446,   558,
    1329,  1330,  1036,  1119,  1493,  1359,  1705,  1616,   344,  1586,
    1516,   946,   951,  1590,  1588,     0,   974,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   692
};

static const short yycheck[] =
{
     267,   310,   564,   348,   579,   347,   832,   569,   475,   152,
     572,   543,   544,   575,   576,    21,  1108,   549,  1110,   364,
     365,   571,   143,    63,   535,   894,    63,    34,   578,    63,
      63,    29,   138,   300,    14,    56,    57,    65,   156,  1337,
      21,   142,    10,   310,   173,    23,    38,    27,   173,   599,
     600,    31,    38,   173,   169,    23,    24,    25,    38,    14,
     215,   173,    65,    92,   169,   237,    73,    38,    20,    90,
     374,    39,   175,    28,   175,   175,   380,   173,    47,    34,
     170,   171,   172,    38,   173,    68,    93,   108,   174,   173,
      45,    46,   173,   175,    49,   175,    94,   175,    38,    14,
      92,   175,   175,    58,   111,   112,    92,    38,    55,    64,
     175,    66,    67,   238,   135,   230,    71,    72,   238,   175,
      98,    36,    77,    38,   175,   230,   124,   213,   108,   136,
      98,   175,   112,   175,   175,   175,    91,    92,   175,    55,
     213,   175,   175,   170,   171,   172,   114,    10,     6,   175,
     175,   106,   175,   108,     9,   110,   175,   112,   467,    14,
      15,    38,   169,   118,   119,   175,   135,   122,   123,   149,
     173,    92,    40,   128,   129,   237,  1474,   146,   147,    34,
     258,    36,   297,    38,   136,    16,    38,   167,    51,    51,
     175,   296,   297,   148,   192,   174,  1097,   112,  1099,    54,
     174,   175,   175,    65,   144,   160,   378,    38,   163,   175,
     175,   166,   175,    65,   481,   162,    85,   526,   175,   374,
      60,   136,   251,   565,   566,   570,   175,   382,   199,   200,
     697,   576,    87,   102,   576,   238,   374,   173,   300,   213,
     378,   239,   216,   217,   382,   175,   162,   115,   103,   378,
     127,   175,   226,   378,   169,   113,   237,   112,   378,   251,
     378,   239,   175,   378,   173,   251,   378,   534,   535,   536,
     537,   239,   378,   378,   205,   130,   379,   378,   379,   379,
     380,   136,   378,   238,   136,   258,  1112,  1113,   378,   378,
     145,   175,   258,   379,   378,   258,   378,   378,   378,   379,
     373,   379,   157,  1172,   378,   260,   379,   262,   263,   264,
     265,   266,   164,   378,   169,  1613,   296,   828,   299,   347,
     831,   173,   378,   164,   173,   218,   219,   378,   191,   238,
     251,   224,   173,   175,   378,   356,   378,   378,   913,   379,
     215,   175,   379,   350,   174,   379,   379,   316,   469,    95,
      96,   378,   378,   378,   378,   353,   324,   272,   382,   378,
     366,   293,   294,   295,   926,   927,   928,   929,   378,   175,
     325,   326,   327,   328,   329,   330,   331,   332,   905,    16,
    1472,   175,   909,   238,   175,    38,   238,   227,   531,   238,
     175,   231,   175,   348,   173,   945,    49,   238,    61,   373,
     950,    38,   257,   214,   259,   379,   215,   218,   219,   218,
     219,   127,    65,   224,   225,   224,   225,   272,     5,   414,
      83,    84,   417,   690,   419,   173,  1532,  1533,  1534,  1535,
    1536,  1537,  1538,    11,    12,   378,   291,   292,   173,   706,
     218,   219,   220,   221,   222,   223,   224,   225,   173,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   112,
      47,   218,   219,    95,    96,  1557,   215,   224,   225,   218,
     219,   220,   221,   222,   223,   224,   225,   175,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,  1383,   348,  1385,  1386,   133,   378,   373,   374,
     375,   376,   155,   358,   359,   360,   143,   382,   378,  1335,
    1336,    47,   367,   368,   369,   173,     1,    57,   218,   219,
      56,    57,   378,     8,   224,   225,   173,   114,   173,   107,
     173,   109,   173,   234,   235,   236,  1088,  1089,   318,   319,
    1092,  1093,  1094,  1095,  1011,   173,  1098,    30,    33,   175,
      90,   268,   269,    38,    90,   822,   898,   899,   900,   901,
     378,   828,   907,  1669,   831,   378,   833,   834,   835,   270,
     378,   382,   173,   382,   378,   153,   154,    62,   920,   921,
      65,    95,    96,   925,    69,   238,   931,   932,   933,   170,
     171,    76,    38,    78,   170,   171,   374,    82,   173,   139,
      65,   141,   380,   381,   378,   383,   384,   378,    38,   378,
      40,    41,    42,    43,    44,   378,  1722,   378,     7,    65,
      50,   381,  1184,   380,   373,   374,   375,   376,    17,   173,
     174,   175,   381,   382,   383,   384,  1168,  1169,   378,   373,
     374,   375,   376,    32,   378,  1471,   378,   132,   382,   379,
     380,   136,   378,    99,   373,   374,   375,   376,   378,   105,
     378,   380,   173,     8,    53,   111,   218,   219,   362,   363,
     364,   365,   224,   225,   159,   173,   174,   175,   378,   164,
     165,   378,   382,   378,   379,   115,   116,   117,    33,   378,
     120,   121,   172,    38,   375,   376,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   362,   363,   364,   365,   381,   983,    62,   164,   108,
      65,   381,   152,   169,    69,   210,   211,   212,   158,   378,
    1039,    76,   378,    78,   173,   174,   175,    82,   173,   174,
     175,  1008,   131,   373,   374,   375,   376,   378,   233,   378,
      65,   173,   174,   175,   381,   240,   241,   242,   381,   244,
     245,   246,   247,   248,   101,   250,   282,   283,   284,   254,
     255,   256,   282,   283,   284,    74,   261,   207,   208,   209,
     173,   174,   175,   159,    99,    75,   271,   132,   273,   274,
     105,   136,   218,   219,  1103,    86,   111,   379,   380,   229,
     230,    79,   232,   935,   936,   937,   938,   939,   940,   941,
     168,  1386,   379,   380,   159,   175,   301,   302,   303,   164,
     165,   351,   352,   298,    26,   175,   175,   175,   175,   175,
     315,   126,   317,  1175,  1101,   320,  1181,   175,   293,  1184,
     175,   175,  1184,   175,   175,   175,   175,  1114,   175,   164,
     280,   281,   132,   175,   169,   285,   286,   287,   288,   289,
     290,   175,   175,   175,   175,   210,   211,   212,   172,   175,
     355,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   175,   233,   175,
     175,   175,   175,   175,   175,   240,   241,   242,   378,   244,
     245,   246,   247,   248,    16,   250,    18,   175,   175,   254,
     255,   256,   175,   175,   175,   175,   261,   175,   175,   199,
     200,   201,   202,   203,   204,    37,   271,   175,   273,   274,
     210,   211,   212,   175,   175,   173,   136,    49,   173,    51,
      52,    62,   173,   175,   173,   173,    38,    38,   175,   175,
     173,  1513,   379,   173,   233,    67,   301,   302,   303,   218,
     219,   220,   221,   222,   223,   224,   225,   175,   173,   175,
     315,   175,   317,    85,   173,   320,   378,    89,   215,  1246,
     378,   218,   219,   220,   221,   222,   223,   224,   225,   175,
     102,   175,   175,   175,   175,   378,   108,   218,   219,   220,
     221,   222,   223,   224,   225,   378,   173,   378,   378,   378,
     355,   378,   378,   378,   378,   173,   378,  1326,   175,   378,
     132,   133,   134,   218,   219,   220,   221,   222,   223,   224,
     225,   143,   175,   175,   378,   378,   172,   378,   150,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   172,   175,   378,   378,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   378,   379,   172,   378,   378,   169,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   172,   175,   173,   354,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   373,   374,   375,   376,   173,   173,
     173,   380,   381,   378,   383,   384,   238,    38,   378,   378,
     175,   378,   378,   378,   378,   378,   111,   374,   378,   378,
     173,    38,   378,   175,   381,   382,   383,   384,    59,   175,
     175,   175,   175,   374,    65,   267,   378,    68,    69,   378,
     381,   378,   383,   384,   378,   378,   175,  1512,   175,  1511,
     173,    82,    13,   349,   173,   378,   173,   378,   373,   374,
     375,   376,   378,   357,   378,   173,   381,   378,   383,   384,
     172,   102,   173,   378,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   173,
     378,    13,   378,   378,   378,   173,   378,   378,   378,   378,
     175,    38,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   378,   378,   175,   175,   173,   175,   378,
     378,   378,   378,   379,   378,   378,   378,   175,   379,   378,
     314,   379,   379,   164,   165,   378,   378,   175,   175,   379,
     378,   378,   378,   378,   378,   378,   169,   378,   169,   175,
     378,   378,   378,   175,   378,   378,   378,   378,   378,   378,
      13,   151,   378,   175,   175,   175,   175,   173,   175,   378,
     378,   378,   378,   378,   175,   175,   175,   173,   175,   173,
     175,   378,   175,   175,   175,   378,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   173,   150,   379,
     175,   175,   175,   175,   378,   175,   175,   175,   175,  1606,
     175,   174,   174,   174,   228,   175,   175,   378,   378,   175,
     378,   175,   175,   173,   173,   250,   378,   378,   378,   175,
     208,   246,   175,   378,   378,   378,   175,   379,   379,   175,
     175,   175,   380,   378,   378,   169,   378,   175,   173,   380,
     378,    65,   175,   175,   378,   378,   378,   378,   378,   378,
     173,   378,   378,   378,   378,   378,   175,   173,   378,   378,
     173,   378,   378,   173,   175,   378,   175,   378,   378,   378,
     378,   378,   378,   378,   173,   378,   378,   378,   378,   378,
      65,   378,   378,   173,   378,   378,   378,   175,   175,   175,
     175,   173,   175,   158,   175,   175,   175,   173,   175,    35,
     378,   175,    35,   378,   378,   378,   175,   175,    35,    35,
     204,   175,   214,   374,   174,   173,   378,   237,   378,   378,
     175,   175,   378,   378,   378,   378,   378,   175,   175,   378,
     175,   378,   175,   378,   175,   175,   175,   175,   175,   175,
     175,    35,   370,   175,   378,   378,   175,   175,    38,    35,
     173,   252,   209,   175,   175,   378,   378,   228,   175,   175,
     378,   175,   175,   175,   206,   237,   175,   175,   252,   175,
     173,   175,   228,   175,   175,   173,   173,   140,   140,   175,
     175,   175,   175,   175,   175,   175,   140,   140,   175,    26,
     215,   378,   175,   173,   126,   136,   175,   175,   175,   175,
     175,   175,   378,   175,   378,   175,   175,    13,   169,    13,
     378,   378,   161,   378,    65,   175,   175,   378,   175,   175,
     378,   140,   237,   378,   378,   175,   378,   378,   378,   175,
     378,   378,   175,   175,    65,   378,   175,   378,   378,   378,
     175,   378,   378,   140,   175,   175,   175,   378,   378,   378,
     378,   378,   378,   378,   378,   370,   354,   378,   380,   175,
     378,   378,   175,   175,   378,   175,   378,   175,   175,   175,
     378,   378,   378,   175,   175,   380,   380,   380,   378,   175,
     175,   378,   378,   378,   175,   378,   378,   378,   253,   378,
     175,   249,   175,   175,   175,   175,   175,   175,   175,   169,
     378,   175,   175,   378,   175,     0,   175,   175,   378,   173,
     169,   378,   378,   175,   175,   378,   378,   378,   370,   378,
     175,   370,   136,   175,     0,   378,   976,   378,   378,  1224,
    1221,   147,   650,   683,   274,   346,   692,   271,  1302,   346,
    1104,  1106,   766,   839,  1369,  1139,  1691,  1561,   153,  1516,
    1389,   591,   596,  1521,  1518,    -1,   628,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   469
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
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
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
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
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 2:
#line 354 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 3:
#line 355 "lef.y"
{
 versionNum = convert_name2num(yyvsp[-1].string);
 /* 
versionNum = $3; Save the version number for future use */

 if (lefrVersionStrCbk) {
 CALLBACK(lefrVersionStrCbk, lefrVersionStrCbkType, yyvsp[-1].string);
 } else {
 CALLBACK(lefrVersionCbk, lefrVersionCbkType, versionNum);
 }
 if (versionNum > 5.3 && versionNum < 5.4) {
 ignoreVersion = 1;
 }
 use5_3 = use5_4 = 0;
 lef_errors = 0;
 hasVer = 1;
 ;
    break;}
case 4:
#line 375 "lef.y"
{
 CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, yyvsp[-1].string);
 hasDivChar = 1;
 ;
    break;}
case 5:
#line 381 "lef.y"
{
 CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, yyvsp[-1].string);
 hasBusBit = 1;
 ;
    break;}
case 8:
#line 390 "lef.y"
{ ;
    break;}
case 9:
#line 393 "lef.y"
{
 CALLBACK(lefrLibraryEndCbk, lefrLibraryEndCbkType, 0);
 /* 11/16/2001 - Wanda da Rosa - pcr 408334
 */
/* Return 1 if there are errors
 */
if (lef_errors)
 return 1;
 if (!hasVer)
 yywarning("VERSION is a required statement.");
 if (!hasNameCase)
 yywarning("NAMESCASESENSITIVE is a required statement.");
 if (!hasBusBit)
 yywarning("BUSBITCHARS is a required statement.");
 if (!hasDivChar)
 yywarning("DIVIDERCHAR is a required statement.");
 hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 ;
    break;}
case 49:
#line 430 "lef.y"
{
 	 lefNamesCaseSensitive = TRUE;
 	 CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
 			 lefNamesCaseSensitive);
 hasNameCase = 1;
 	 ;
    break;}
case 50:
#line 437 "lef.y"
{
 	 lefNamesCaseSensitive = FALSE;
 	 CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
 			 lefNamesCaseSensitive);
 hasNameCase = 1;
 	 ;
    break;}
case 51:
#line 445 "lef.y"
{ CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "ON"); ;
    break;}
case 52:
#line 447 "lef.y"
{ CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "OFF"); ;
    break;}
case 53:
#line 450 "lef.y"
{ CALLBACK(lefrManufacturingCbk, lefrManufacturingCbkType, yyvsp[-1].dval); ;
    break;}
case 54:
#line 453 "lef.y"
{
 if (lefrUseMinSpacingCbk)
 lefiUseMinSpacing_set( &(lefrUseMinSpacing),
 yyvsp[-2].string,
yyvsp[-1].integer) ;
 CALLBACK(lefrUseMinSpacingCbk, lefrUseMinSpacingCbkType,
 &lefrUseMinSpacing);
 ;
    break;}
case 55:
#line 463 "lef.y"
{ CALLBACK(lefrClearanceMeasureCbk, lefrClearanceMeasureCbkType, yyvsp[-1].string); ;
    break;}
case 56:
#line 466 "lef.y"
{yyval.string = (char*)"MAXXY";;
    break;}
case 57:
#line 467 "lef.y"
{yyval.string = (char*)"EUCLIDEAN";;
    break;}
case 58:
#line 470 "lef.y"
{yyval.string = (char*)"OBS";;
    break;}
case 59:
#line 471 "lef.y"
{yyval.string = (char*)"PIN";;
    break;}
case 60:
#line 474 "lef.y"
{yyval.integer = 1;;
    break;}
case 61:
#line 475 "lef.y"
{yyval.integer = 0;;
    break;}
case 62:
#line 478 "lef.y"
{ CALLBACK(lefrUnitsCbk, lefrUnitsCbkType, &lefrUnits); ;
    break;}
case 63:
#line 481 "lef.y"
{ lefiUnits_clear( &(lefrUnits) ) ;
 ;
    break;}
case 66:
#line 490 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setTime( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 67:
#line 494 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setCapacitance( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 68:
#line 498 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setResistance( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 69:
#line 502 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setPower( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 70:
#line 506 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setCurrent( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 71:
#line 510 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setVoltage( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 72:
#line 513 "lef.y"
{ lefDumbMode = 1; ;
    break;}
case 73:
#line 514 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setDatabase( &(lefrUnits),
 yyvsp[-2].string,
yyvsp[-1].dval) ;
 ;
    break;}
case 74:
#line 519 "lef.y"
{ if (lefrUnitsCbk) lefiUnits_setFrequency( &(lefrUnits),
 yyvsp[-1].dval) ;
 ;
    break;}
case 75:
#line 524 "lef.y"
{ CALLBACK(lefrLayerCbk, lefrLayerCbkType, &lefrLayer); ;
    break;}
case 76:
#line 526 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 77:
#line 527 "lef.y"
{
 if (lefrHasMaxVS) { /* 5.5 */

 yyerror("A MAXVIASTACK has defined before the LAYER");
 CHKERR();
 }
 if (lefrLayerCbk)
 lefiLayer_setName( &(lefrLayer),
 yyvsp[0].string) ;
 useLenThr = 0;
 layerCut = 0;
 layerMastOver = 0;
 layerRout = 0;
 layerDir = 0;
 lefrHasLayer = 1;
 strcpy(layerName, yyvsp[0].string);
 hasPitch = 0;
 hasWidth = 0;
 hasDirection = 0;
 hasParallel = 0;
 hasInfluence = 0;
 ;
    break;}
case 78:
#line 550 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 79:
#line 551 "lef.y"
{
 if (strcmp(layerName, yyvsp[0].string) != 0) {
 yyerror("END LAYER name is different from LAYER");
 CHKERR();
 }
 if ((layerRout == 1) && (hasPitch == 0)) {
 yyerror("PITCH in LAYER with type ROUTING is required");
 CHKERR();
 }
 if ((layerRout == 1) && (hasWidth == 0)) {
 yyerror("WIDTH in LAYER with type ROUTING is required");
 CHKERR();
 }
 if ((layerRout == 1) && (hasDirection == 0)) {
 yyerror("DIRECTION in LAYER with type ROUTING is required");
 CHKERR();
 }
 ;
    break;}
case 80:
#line 573 "lef.y"
{ ;
    break;}
case 81:
#line 575 "lef.y"
{ ;
    break;}
case 82:
#line 579 "lef.y"
{ if (lefrLayerCbk) lefiLayer_setType( &(lefrLayer),
 yyvsp[-1].string) ;
 ;
    break;}
case 83:
#line 583 "lef.y"
{
 if (lefrLayerCbk) lefiLayer_setPitch( &(lefrLayer),
 yyvsp[-1].dval) ;
 hasPitch = 1;
 ;
    break;}
case 84:
#line 589 "lef.y"
{
 if (lefrLayerCbk) lefiLayer_setOffset( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 85:
#line 594 "lef.y"
{
 if (lefrLayerCbk) lefiLayer_setWidth( &(lefrLayer),
 yyvsp[-1].dval) ;
 hasWidth = 1;
 ;
    break;}
case 86:
#line 600 "lef.y"
{
 if (lefrLayerCbk) lefiLayer_setArea( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 87:
#line 605 "lef.y"
{
 /* 11/22/99 - Wanda da Rosa, PCR 283762
 */
/*            Issue an error is this is defined in masterslice
 */
if (layerMastOver) {
 yyerror("SPACING can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 /* 5.5 either SPACING or SPACINGTABLE, not both for routing layer only
 */
if (layerRout) {
 if (lefrHasSpacingTbl) {
 yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingMin( &(lefrLayer),
 yyvsp[0].dval) ;
 lefrHasSpacing = 1;
 } else {
 if (lefrLayerCbk)
 lefiLayer_setSpacingMin( &(lefrLayer),
 yyvsp[0].dval) ;
 }
 ;
    break;}
case 89:
#line 632 "lef.y"
{
 layerDir = 1;
 if (!layerRout) {
 yyerror("DIRECTION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setDirection( &(lefrLayer),
 yyvsp[-1].string) ;
 hasDirection = 1;
 ;
    break;}
case 90:
#line 643 "lef.y"
{
 if (!layerRout) {
 yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setResistance( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 91:
#line 652 "lef.y"
{
 if (!layerRout) {
 yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 ;
    break;}
case 92:
#line 659 "lef.y"
{
 if (!layerRout) {
 yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setCapacitance( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 93:
#line 668 "lef.y"
{
 if (!layerRout) {
 yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 ;
    break;}
case 94:
#line 675 "lef.y"
{
 if (!layerRout) {
 yyerror("HEIGHT can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setHeight( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 95:
#line 684 "lef.y"
{
 if (!layerRout) {
 yyerror("WIREEXTENSION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setWireExtension( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 96:
#line 693 "lef.y"
{
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("THICKNESS can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setThickness( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 97:
#line 702 "lef.y"
{
 if (!layerRout) {
 yyerror("SHRINKAGE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setShrinkage( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 98:
#line 711 "lef.y"
{
 if (!layerRout) {
 yyerror("CAPMULTIPLIER can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setCapMultiplier( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 99:
#line 720 "lef.y"
{
 if (!layerRout) {
 yyerror("EDGECAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setEdgeCap( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 100:
#line 742 "lef.y"
{ /* 5.3 syntax */

 use5_3 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("ANTENNALENGTHFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 } else if (versionNum >= 5.4) {
 if (use5_4) {
 yyerror("ANTENNALENGTHFACTOR is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 
 if (lefrLayerCbk) lefiLayer_setAntennaLength( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 101:
#line 759 "lef.y"
{
 if (!layerRout) {
 yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setCurrentDensity( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 102:
#line 768 "lef.y"
{
 if (!layerRout) {
 yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 ;
    break;}
case 103:
#line 775 "lef.y"
{
 if (!layerRout) {
 yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setCurrentPoint( &(lefrLayer),
 yyvsp[-3].dval,
yyvsp[-2].dval) ;
 ;
    break;}
case 104:
#line 784 "lef.y"
{ lefDumbMode = 10000000; ;
    break;}
case 105:
#line 785 "lef.y"
{
 lefDumbMode = 0;
 ;
    break;}
case 106:
#line 789 "lef.y"
{
 if (layerMastOver) {
 yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_addAccurrentDensity( &(lefrLayer),
 yyvsp[0].string) ;
 ;
    break;}
case 108:
#line 799 "lef.y"
{
 if (layerMastOver) {
 yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) {
 lefiLayer_addAccurrentDensity( &(lefrLayer),
 yyvsp[-2].string) ;
 lefiLayer_setAcOneEntry( &(lefrLayer),
 yyvsp[-1].dval) ;
 }
 ;
    break;}
case 109:
#line 812 "lef.y"
{
 if (layerMastOver) {
 yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 ;
    break;}
case 111:
#line 822 "lef.y"
{
 if (layerMastOver) {
 yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_setDcOneEntry( &(lefrLayer),
 yyvsp[-1].dval) ;
 }
 ;
    break;}
case 112:
#line 837 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (!layerRout && !layerCut && layerMastOver) {
 yyerror("ANTENNAAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setAntennaAreaRatio( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 113:
#line 858 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (!layerRout && !layerCut && layerMastOver) {
 yyerror("ANTENNADIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 antennaType = lefiAntennaDAR;
 ;
    break;}
case 115:
#line 879 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNACUMAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNACUMAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (!layerRout && !layerCut && layerMastOver) {
 yyerror("ANTENNACUMAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumAreaRatio( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 116:
#line 900 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (!layerRout && !layerCut && layerMastOver) {
 yyerror("ANTENNACUMDIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 antennaType = lefiAntennaCDAR;
 ;
    break;}
case 118:
#line 921 "lef.y"
{ /* both 5.3 & 5.4 syntax */

 if (!layerRout && !layerCut && layerMastOver) {
 yyerror("ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 /* this does not need to check, since syntax is in both 5.3 & 5.4 */

 if (lefrLayerCbk) lefiLayer_setAntennaAreaFactor( &(lefrLayer),
 yyvsp[0].dval) ;
 antennaType = lefiAntennaAF;
 ;
    break;}
case 120:
#line 935 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("ANTENNASIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setAntennaSideAreaRatio( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 121:
#line 956 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("ANTENNADIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 antennaType = lefiAntennaDSAR;
 ;
    break;}
case 123:
#line 977 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("ANTENNACUMSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumSideAreaRatio( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 124:
#line 998 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("ANTENNACUMDIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 antennaType = lefiAntennaCDSAR;
 ;
    break;}
case 126:
#line 1019 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 yyerror("ANTENNASIDEAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setAntennaSideAreaFactor( &(lefrLayer),
 yyvsp[0].dval) ;
 antennaType = lefiAntennaSAF;
 ;
    break;}
case 128:
#line 1043 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && !layerCut && layerMastOver) {
 yyerror("ANTENNAMODEL can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.5) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 antennaType = lefiAntennaO;
 ;
    break;}
case 130:
#line 1064 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("SLOTWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setSlotWireWidth( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 131:
#line 1077 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("SLOTWIRELENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setSlotWireLength( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 132:
#line 1090 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("SLOTWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setSlotWidth( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 133:
#line 1103 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("SLOTLENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setSlotLength( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 134:
#line 1116 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("MAXADJACENTSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMaxAdjacentSlotSpacing( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 135:
#line 1129 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("MAXCOAXIALSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMaxCoaxialSlotSpacing( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 136:
#line 1142 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("MAXEDGESLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMaxEdgeSlotSpacing( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 137:
#line 1155 "lef.y"
{ /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("SPLITWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setSplitWireWidth( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 138:
#line 1168 "lef.y"
{ /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("MINIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMinimumDensity( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 139:
#line 1181 "lef.y"
{ /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("MAXIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMaximumDensity( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 140:
#line 1194 "lef.y"
{ /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("DENSITYCHECKWINDOW is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setDensityCheckWindow( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 141:
#line 1208 "lef.y"
{ /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("DENSITYCHECKSTEP is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setDensityCheckStep( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 142:
#line 1221 "lef.y"
{ /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("FILLACTIVESPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setFillActiveSpacing( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 143:
#line 1235 "lef.y"
{
 /* 5.5 MAXWIDTH, is for routing layer only
 */
if (!layerRout) {
 yyerror("MAXWIDTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (versionNum < 5.5) {
 yyerror("MAXWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMaxwidth( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 144:
#line 1251 "lef.y"
{
 /* 5.5 MINWIDTH, is for routing layer only
 */
if (!layerRout) {
 yyerror("MINWIDTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 if (versionNum < 5.5) {
 yyerror("MINWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setMinwidth( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 145:
#line 1267 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("MINENCLOSEDAREA is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_addMinenclosedarea( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 147:
#line 1278 "lef.y"
{ /* pcr 409334 */

 if (lefrLayerCbk) lefiLayer_addMinimumcut( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 148:
#line 1285 "lef.y"
{
 if (lefrLayerCbk) lefiLayer_addMinimumcutConnect( &(lefrLayer),
 yyvsp[0].string) ;
 ;
    break;}
case 150:
#line 1292 "lef.y"
{
 if (lefrLayerCbk) lefiLayer_setMinstep( &(lefrLayer),
 yyvsp[-1].dval) ;
 ;
    break;}
case 151:
#line 1298 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("PROTRUSION RULE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_setProtrusion( &(lefrLayer),
 yyvsp[-5].dval,
yyvsp[-3].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 152:
#line 1310 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("SPACINGTABLE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 /* 5.5 either SPACING or SPACINGTABLE in a layer, not both
 */
if (lefrHasSpacing && layerRout) {
 yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
 }
 if (lefrLayerCbk) lefiLayer_addSpacingTable( &(lefrLayer) ) ;
 lefrHasSpacingTbl = 1;
 ;
    break;}
case 154:
#line 1327 "lef.y"
{
 if (hasInfluence) { /* 5.5 - INFLUENCE table must follow a PARALLEL
 */
yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
 CHKERR();
 }
 if (hasParallel) { /* 5.5 - Only one PARALLEL table is allowed per layer
 */
yyerror("Only one PARALLELRUNLENGTH table is allowed per layer");
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 hasParallel = 1;
 ;
    break;}
case 155:
#line 1343 "lef.y"
{
 spParallelLength = lefiLayer_getNumber( &(lefrLayer) ) ;
 if (lefrLayerCbk) lefiLayer_addSpParallelLength( &(lefrLayer) ) ;
 ;
    break;}
case 156:
#line 1348 "lef.y"
{
 if (lefrLayerCbk) {
 lefiLayer_addSpParallelWidth( &(lefrLayer),
 yyvsp[0].dval) ;
 }
 ;
    break;}
case 157:
#line 1355 "lef.y"
{
 if (lefiLayer_getNumber( &(lefrLayer) )!=spParallelLength) { yyerror("The number of length in PARALLELRUNLENGTH is not the same as the number of spacing in WIDTH") ;
 CHKERR();
 }
 if (lefrLayerCbk) lefiLayer_addSpParallelWidthSpacing( &(lefrLayer) ) ;
 ;
    break;}
case 159:
#line 1363 "lef.y"
{
 if (hasInfluence) { /* 5.5 - INFLUENCE table must follow a PARALLEL
 */
yyerror("Only one INFLUENCE table is allowed per layer");
 CHKERR();
 }
 if (!hasParallel) { /* 5.5 - INFLUENCE must follow a PARALLEL
 */
yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
 CHKERR();
 }
 if (lefrLayerCbk) {
 lefiLayer_setInfluence( &(lefrLayer) ) ;
 lefiLayer_addSpInfluence( &(lefrLayer),
 yyvsp[-4].dval,
yyvsp[-2].dval,
yyvsp[0].dval) ;
 }
 ;
    break;}
case 161:
#line 1386 "lef.y"
{ if (lefrLayerCbk)
 lefiLayer_setAntennaValue( &(lefrLayer),
 antennaType,
yyvsp[0].dval) ;
 ;
    break;}
case 162:
#line 1392 "lef.y"
{ if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */

 lefrAntennaPWLPtr = (lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
 lefiAntennaPWL_Init( lefrAntennaPWLPtr ) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 yyvsp[-1].pt.x,
yyvsp[-1].pt.y) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 yyvsp[0].pt.x,
yyvsp[0].pt.y) ;
 }
 ;
    break;}
case 163:
#line 1405 "lef.y"
{ if (lefrLayerCbk)
 lefiLayer_setAntennaPWL( &(lefrLayer),
 antennaType,
lefrAntennaPWLPtr) ;
 ;
    break;}
case 166:
#line 1418 "lef.y"
{ if (lefrLayerCbk)
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 yyvsp[0].pt.x,
yyvsp[0].pt.y) ;
 ;
    break;}
case 168:
#line 1427 "lef.y"
{
 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 }
 else if ((antennaType == lefiAntennaAF) && (versionNum <= 5.3)) {
 yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrLayerCbk)
 lefiLayer_setAntennaDUO( &(lefrLayer),
 antennaType) ;
 ;
    break;}
case 169:
#line 1446 "lef.y"
{yyval.string = (char*)"PEAK";;
    break;}
case 170:
#line 1447 "lef.y"
{yyval.string = (char*)"AVERAGE";;
    break;}
case 171:
#line 1448 "lef.y"
{yyval.string = (char*)"RMS";;
    break;}
case 174:
#line 1455 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 175:
#line 1459 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addAcFrequency( &(lefrLayer) ) ;
 ;
    break;}
case 176:
#line 1462 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 177:
#line 1466 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addAcCutarea( &(lefrLayer) ) ;
 ;
    break;}
case 178:
#line 1469 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 179:
#line 1473 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addAcTableEntry( &(lefrLayer) ) ;
 ;
    break;}
case 180:
#line 1476 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 181:
#line 1480 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addAcWidth( &(lefrLayer) ) ;
 ;
    break;}
case 184:
#line 1488 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 185:
#line 1492 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addDcCutarea( &(lefrLayer) ) ;
 ;
    break;}
case 186:
#line 1495 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 187:
#line 1499 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addDcTableEntry( &(lefrLayer) ) ;
 ;
    break;}
case 188:
#line 1502 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 189:
#line 1506 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addDcWidth( &(lefrLayer) ) ;
 ;
    break;}
case 191:
#line 1511 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 194:
#line 1522 "lef.y"
{
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 yyvsp[-1].string) ;
 lefiLayer_addProp( &(lefrLayer),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 195:
#line 1534 "lef.y"
{
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 yyvsp[-1].string) ;
 lefiLayer_addProp( &(lefrLayer),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 196:
#line 1546 "lef.y"
{
 char temp[32];
 sprintf(temp, "%g", yyvsp[0].dval);
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 yyvsp[-1].string) ;
 lefiLayer_addNumProp( &(lefrLayer),
 yyvsp[-1].string,
yyvsp[0].dval,
temp,
propTp) ;
 }
 ;
    break;}
case 197:
#line 1563 "lef.y"
{ ;
    break;}
case 198:
#line 1565 "lef.y"
{ ;
    break;}
case 199:
#line 1568 "lef.y"
{ if (lefrLayerCbk) lefiLayer_setCurrentPoint( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 202:
#line 1579 "lef.y"
{ if (lefrLayerCbk) lefiLayer_setCapacitancePoint( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 204:
#line 1587 "lef.y"
{ ;
    break;}
case 205:
#line 1590 "lef.y"
{ if (lefrLayerCbk) lefiLayer_setResistancePoint( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 206:
#line 1596 "lef.y"
{yyval.string = (char*)"ROUTING"; layerRout = 1;;
    break;}
case 207:
#line 1597 "lef.y"
{yyval.string = (char*)"CUT"; layerCut = 1;;
    break;}
case 208:
#line 1598 "lef.y"
{yyval.string = (char*)"OVERLAP"; layerMastOver = 1;;
    break;}
case 209:
#line 1599 "lef.y"
{yyval.string = (char*)"MASTERSLICE"; layerMastOver = 1;;
    break;}
case 210:
#line 1600 "lef.y"
{yyval.string = (char*)"VIRTUAL";;
    break;}
case 211:
#line 1601 "lef.y"
{yyval.string = (char*)"IMPLANT";;
    break;}
case 212:
#line 1604 "lef.y"
{yyval.string = (char*)"HORIZONTAL";;
    break;}
case 213:
#line 1605 "lef.y"
{yyval.string = (char*)"VERTICAL";;
    break;}
case 214:
#line 1607 "lef.y"
{yyval.string = (char*)"";;
    break;}
case 215:
#line 1609 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("FROMABOVE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 yyval.string = (char*)"FROMABOVE";
 
 ;
    break;}
case 216:
#line 1618 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("FROMBELOW is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 yyval.string = (char*)"FROMBELOW";
 ;
    break;}
case 218:
#line 1628 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("LENGTH WITHIN is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutLengDis( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 220:
#line 1641 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_addMinenclosedareaWidth( &(lefrLayer),
 yyvsp[0].dval) ;
 ;
    break;}
case 221:
#line 1649 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 1) ;
 ;
    break;}
case 222:
#line 1655 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 2) ;
 ;
    break;}
case 223:
#line 1661 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 3) ;
 ;
    break;}
case 224:
#line 1667 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 4) ;
 ;
    break;}
case 225:
#line 1675 "lef.y"
{ ;
    break;}
case 226:
#line 1677 "lef.y"
{ ;
    break;}
case 227:
#line 1680 "lef.y"
{
 if (lefrLayerCbk) {
 lefiLayer_addSpParallelWidth( &(lefrLayer),
 yyvsp[0].dval) ;
 }
 ;
    break;}
case 228:
#line 1687 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addSpParallelWidthSpacing( &(lefrLayer) ) ;
 ;
    break;}
case 229:
#line 1692 "lef.y"
{ ;
    break;}
case 230:
#line 1694 "lef.y"
{ ;
    break;}
case 231:
#line 1697 "lef.y"
{ if (lefrLayerCbk) lefiLayer_addSpInfluence( &(lefrLayer),
 yyvsp[-4].dval,
yyvsp[-2].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 232:
#line 1704 "lef.y"
{
 if (!lefrHasLayer) { /* 5.5 */

 yyerror("MAXVIASTACK has to define after the LAYER");
 CHKERR();
 } else if (lefrHasMaxVS) {
 yyerror("Only one MAXVIASTACK is allowed in a lef file");
 CHKERR();
 } else {
 if (lefrMaxStackViaCbk)
 lefiMaxStackVia_setMaxStackVia( &(lefrMaxStackVia),
 yyvsp[-1].dval) ;
 CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
 }
 if (versionNum < 5.5) {
 yyerror("MAXVIASTACK is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 lefrHasMaxVS = 1;
 ;
    break;}
case 233:
#line 1724 "lef.y"
{lefDumbMode = 2; lefNoNum= 2;;
    break;}
case 234:
#line 1726 "lef.y"
{
 if (!lefrHasLayer) { /* 5.5 */

 yyerror("MAXVIASTACK has to define after the LAYER");
 CHKERR();
 } else if (lefrHasMaxVS) {
 yyerror("Only one MAXVIASTACK is allowed in a lef file");
 CHKERR();
 } else {
 if (lefrMaxStackViaCbk) {
 lefiMaxStackVia_setMaxStackVia( &(lefrMaxStackVia),
 yyvsp[-5].dval) ;
 lefiMaxStackVia_setMaxStackViaRange( &(lefrMaxStackVia),
 yyvsp[-2].string,
yyvsp[-1].string) ;
 }
 CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
 }
 lefrHasMaxVS = 1;
 ;
    break;}
case 235:
#line 1748 "lef.y"
{ CALLBACK(lefrViaCbk, lefrViaCbkType, &lefrVia); ;
    break;}
case 236:
#line 1753 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 237:
#line 1756 "lef.y"
{
 /* 0 is nodefault */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 yyvsp[0].string,
0) ;
 viaLayer = 0;
 numVia++;
 strcpy(viaName, yyvsp[0].string);
 ;
    break;}
case 238:
#line 1767 "lef.y"
{
 /* 1 is default */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 yyvsp[-1].string,
1) ;
 viaLayer = 0;
 strcpy(viaName, yyvsp[-1].string);
 ;
    break;}
case 241:
#line 1785 "lef.y"
{ ;
    break;}
case 242:
#line 1787 "lef.y"
{ ;
    break;}
case 243:
#line 1789 "lef.y"
{ if (lefrViaCbk) lefiVia_setResistance( &(lefrVia),
 yyvsp[-1].dval) ;
 ;
    break;}
case 244:
#line 1792 "lef.y"
{ lefDumbMode = 1000000; ;
    break;}
case 245:
#line 1793 "lef.y"
{ lefDumbMode = 0; ;
    break;}
case 246:
#line 1795 "lef.y"
{ if (lefrViaCbk) lefiVia_setTopOfStack( &(lefrVia) ) ;
 ;
    break;}
case 249:
#line 1805 "lef.y"
{
 char temp[32];
 sprintf(temp, "%g", yyvsp[0].dval);
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 yyvsp[-1].string) ;
 lefiVia_addNumProp( &(lefrVia),
 yyvsp[-1].string,
yyvsp[0].dval,
temp,
propTp) ;
 }
 ;
    break;}
case 250:
#line 1820 "lef.y"
{
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 yyvsp[-1].string) ;
 lefiVia_addProp( &(lefrVia),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 251:
#line 1832 "lef.y"
{
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 yyvsp[-1].string) ;
 lefiVia_addProp( &(lefrVia),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 252:
#line 1846 "lef.y"
{ if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 yyvsp[-1].string,
0,
0.0,
0.0,
-1) ;
 ;
    break;}
case 253:
#line 1854 "lef.y"
{ if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 yyvsp[-2].string,
1,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y,
-1) ;
 ;
    break;}
case 254:
#line 1862 "lef.y"
{ if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 yyvsp[-3].string,
1,
yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].integer) ;
 ;
    break;}
case 255:
#line 1870 "lef.y"
{ if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 yyvsp[-2].string,
0,
0.0,
0.0,
yyvsp[-1].integer) ;
 ;
    break;}
case 256:
#line 1878 "lef.y"
{lefDumbMode = 1; lefNoNum= 1;;
    break;}
case 257:
#line 1879 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 258:
#line 1882 "lef.y"
{yyval.integer = 0;;
    break;}
case 259:
#line 1883 "lef.y"
{yyval.integer = 1;;
    break;}
case 260:
#line 1884 "lef.y"
{yyval.integer = 2;;
    break;}
case 261:
#line 1885 "lef.y"
{yyval.integer = 3;;
    break;}
case 262:
#line 1886 "lef.y"
{yyval.integer = 4;;
    break;}
case 263:
#line 1887 "lef.y"
{yyval.integer = 5;;
    break;}
case 264:
#line 1888 "lef.y"
{yyval.integer = 6;;
    break;}
case 265:
#line 1889 "lef.y"
{yyval.integer = 7;;
    break;}
case 266:
#line 1890 "lef.y"
{yyval.integer = 0;;
    break;}
case 267:
#line 1891 "lef.y"
{yyval.integer = 1;;
    break;}
case 268:
#line 1892 "lef.y"
{yyval.integer = 2;;
    break;}
case 269:
#line 1893 "lef.y"
{yyval.integer = 3;;
    break;}
case 270:
#line 1894 "lef.y"
{yyval.integer = 4;;
    break;}
case 271:
#line 1895 "lef.y"
{yyval.integer = 5;;
    break;}
case 272:
#line 1896 "lef.y"
{yyval.integer = 6;;
    break;}
case 273:
#line 1897 "lef.y"
{yyval.integer = 7;;
    break;}
case 274:
#line 1900 "lef.y"
{ ;
    break;}
case 275:
#line 1902 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 276:
#line 1903 "lef.y"
{
 if (lefrViaCbk) lefiVia_addLayer( &(lefrVia),
 yyvsp[-1].string) ;
 viaLayer++;
 ;
    break;}
case 279:
#line 1916 "lef.y"
{ if (lefrViaCbk)
 lefiVia_addRectToLayer( &(lefrVia),
 yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y) ;
 ;
    break;}
case 280:
#line 1924 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 281:
#line 1925 "lef.y"
{
 /* 10/17/2001 - Wanda da Rosa, PCR 404149
 */
/*              Error if no layer in via
 */
if (!viaLayer) {
 yyerror("VIA requires at least one layer");
 CHKERR();
 }
 if (strcmp(viaName, yyvsp[0].string) != 0) {
 yyerror("END VIA name is different from VIA");
 CHKERR();
 }
 ;
    break;}
case 282:
#line 1940 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 283:
#line 1941 "lef.y"
{
 if (lefrViaRuleCbk) lefiViaRule_setName( &(lefrViaRule),
 yyvsp[0].string) ;
 viaRuleLayer = 0;
 strcpy(viaRuleName, yyvsp[0].string);
 isGenerate = 0;
 ;
    break;}
case 284:
#line 1951 "lef.y"
{
 /* 10/17/2001 - Wanda da Rosa, PCR 404163
 */
/*              Error if layer number is not equal 2.
 */
/* 11/14/2001 - Wanda da Rosa,
 */
/*              Commented out for pcr 411781
 */
/* (viaRuleLayer != 2) {
 */
/* requires two layers");
 */
if (viaRuleLayer = 0 || viaRuleLayer > 2) {
 yyerror("VIARULE requires two layers");
 CHKERR();
 }
 CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
 ;
    break;}
case 285:
#line 1973 "lef.y"
{
 isGenerate = 1;
 ;
    break;}
case 286:
#line 1977 "lef.y"
{
 /* 10/17/2001 - Wanda da Rosa, PCR 404181
 */
/*              Error if layer number is not equal 3.
 */
/* 11/14/2001 - Wanda da Rosa,
 */
/*              Commented out for pcr 411781
 */
/* (viaRuleLayer != 3) {
 */
/* requires three layers");
 */
if (viaRuleLayer = 0) {
 yyerror("VIARULE requires three layers");
 CHKERR();
 }
 if (lefrViaRuleCbk) lefiViaRule_setGenerate( &(lefrViaRule) ) ;
 CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
 ;
    break;}
case 293:
#line 2014 "lef.y"
{ lefDumbMode = 10000000; ;
    break;}
case 294:
#line 2015 "lef.y"
{ lefDumbMode = 0; ;
    break;}
case 297:
#line 2024 "lef.y"
{
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 yyvsp[-1].string) ;
 lefiViaRule_addProp( &(lefrViaRule),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 298:
#line 2036 "lef.y"
{
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 yyvsp[-1].string) ;
 lefiViaRule_addProp( &(lefrViaRule),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 299:
#line 2048 "lef.y"
{
 char temp[32];
 sprintf(temp, "%g", yyvsp[0].dval);
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 yyvsp[-1].string) ;
 lefiViaRule_addNumProp( &(lefrViaRule),
 yyvsp[-1].string,
yyvsp[0].dval,
temp,
propTp) ;
 }
 ;
    break;}
case 300:
#line 2064 "lef.y"
{
 /* 10/18/2001 - Wanda da Rosa PCR 404181
 */
/*              Make sure the 1st 2 layers in viarule has direction
 */
if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc && isGenerate) {
 yyerror("VIARULE requires the DIRECTION construct in LAYER");
 CHKERR();
 }
 viaRuleLayer++;
 ;
    break;}
case 303:
#line 2084 "lef.y"
{ if (lefrViaRuleCbk) lefiViaRule_addViaName( &(lefrViaRule),
 yyvsp[-1].string) ;
 ;
    break;}
case 304:
#line 2088 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 305:
#line 2089 "lef.y"
{ if (lefrViaRuleCbk) lefiViaRule_setLayer( &(lefrViaRule),
 yyvsp[-1].string) ;
 viaRuleHasDir = 0;
 viaRuleHasEnc = 0;
 ;
    break;}
case 308:
#line 2103 "lef.y"
{
 if (viaRuleHasEnc) {
 yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
 CHKERR();
 } else {
 if (lefrViaRuleCbk) lefiViaRule_setHorizontal( &(lefrViaRule) ) ;
 }
 viaRuleHasDir = 1;
 ;
    break;}
case 309:
#line 2113 "lef.y"
{
 if (viaRuleHasEnc) {
 yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
 CHKERR();
 } else {
 if (lefrViaRuleCbk) lefiViaRule_setVertical( &(lefrViaRule) ) ;
 }
 viaRuleHasDir = 1;
 ;
    break;}
case 310:
#line 2124 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("ENCLOSURE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (viaRuleHasDir) {
 yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
 CHKERR();
 } else {
 if (lefrViaRuleCbk) lefiViaRule_setEnclosure( &(lefrViaRule),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 }
 viaRuleHasEnc = 1;
 ;
    break;}
case 311:
#line 2140 "lef.y"
{ if (lefrViaRuleCbk) lefiViaRule_setWidth( &(lefrViaRule),
 yyvsp[-3].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 312:
#line 2145 "lef.y"
{ if (lefrViaRuleCbk)
 lefiViaRule_setRect( &(	lefrViaRule),
 yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y) ;
 ;
    break;}
case 313:
#line 2153 "lef.y"
{ if (lefrViaRuleCbk) lefiViaRule_setSpacing( &(lefrViaRule),
 yyvsp[-3].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 314:
#line 2158 "lef.y"
{ if (lefrViaRuleCbk) lefiViaRule_setResistance( &(lefrViaRule),
 yyvsp[-1].dval) ;
 ;
    break;}
case 315:
#line 2162 "lef.y"
{
 if (!viaRuleHasDir) {
 yyerror("Either OVERHANG can only be defined with DIRECTION");
 CHKERR();
 }
 if (lefrViaRuleCbk) lefiViaRule_setOverhang( &(lefrViaRule),
 yyvsp[-1].dval) ;
 ;
    break;}
case 316:
#line 2171 "lef.y"
{
 if (!viaRuleHasDir) {
 yyerror("Either METALOVERHANG can only be defined with DIRECTION");
 CHKERR();
 }
 if (lefrViaRuleCbk) lefiViaRule_setMetalOverhang( &(lefrViaRule),
 yyvsp[-1].dval) ;
 ;
    break;}
case 317:
#line 2180 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 318:
#line 2181 "lef.y"
{
 if (strcmp(viaRuleName, yyvsp[0].string) != 0) {
 yyerror("END VIARULE name is different from VIARULE");
 CHKERR();
 }
 ;
    break;}
case 319:
#line 2189 "lef.y"
{ ;
    break;}
case 320:
#line 2192 "lef.y"
{ if (lefrSpacingBeginCbk)
 	CALLBACK(lefrSpacingBeginCbk, lefrSpacingBeginCbkType, 0); ;
    break;}
case 321:
#line 2196 "lef.y"
{ if (lefrSpacingEndCbk)
 CALLBACK(lefrSpacingEndCbk, lefrSpacingEndCbkType, 0); ;
    break;}
case 324:
#line 2205 "lef.y"
{
 if (lefrSpacingCbk) {
 lefiSpacing_set( &(	lefrSpacing),
 yyvsp[-3].string,
yyvsp[-2].string,
yyvsp[-1].dval,
0) ;
 	CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
 }
 ;
    break;}
case 325:
#line 2216 "lef.y"
{
 if (lefrSpacingCbk) {
 lefiSpacing_set( &(	lefrSpacing),
 yyvsp[-4].string,
yyvsp[-3].string,
yyvsp[-2].dval,
1) ;
 	CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
 }
 ;
    break;}
case 326:
#line 2228 "lef.y"
{ lefDumbMode = 2; lefNoNum = 2; ;
    break;}
case 327:
#line 2232 "lef.y"
{ ;
    break;}
case 328:
#line 2235 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrIRDropBeginCbk)
 	 CALLBACK(lefrIRDropBeginCbk, lefrIRDropBeginCbkType, 0);
 } else
 yywarning("IRDROP is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 329:
#line 2244 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrIRDropEndCbk)
 	 CALLBACK(lefrIRDropEndCbk, lefrIRDropEndCbkType, 0);
 }
 ;
    break;}
case 332:
#line 2259 "lef.y"
{
 if (versionNum < 5.4) {
 CALLBACK(lefrIRDropCbk, lefrIRDropCbkType, &lefrIRDrop);
 }
 ;
    break;}
case 335:
#line 2272 "lef.y"
{ if (lefrIRDropCbk) lefiIRDrop_setValues( &(lefrIRDrop),
 yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 336:
#line 2278 "lef.y"
{ if (lefrIRDropCbk) lefiIRDrop_setTableName( &(lefrIRDrop),
 yyvsp[0].string) ;
 ;
    break;}
case 337:
#line 2283 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrMinFeatureCbk)
 lefiMinFeature_set( &(lefrMinFeature),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 CALLBACK(lefrMinFeatureCbk, lefrMinFeatureCbkType, &lefrMinFeature);
 } else
 yywarning("MINFEATURE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 338:
#line 2295 "lef.y"
{
 if (versionNum < 5.4) {
 CALLBACK(lefrDielectricCbk, lefrDielectricCbkType, yyvsp[-1].dval);
 } else
 yywarning("DIELECTRIC is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 339:
#line 2303 "lef.y"
{
 if (lefrNonDefaultCbk) lefiNonDefault_setName( &(lefrNonDefault),
 yyvsp[0].string) ;
 ndLayer = 0;
 numVia = 0;
 strcpy(nonDefaultRuleName, yyvsp[0].string);
 ;
    break;}
case 340:
#line 2311 "lef.y"
{
 /* 10/18/2001 - Wanda da Rosa, PCR 404189
 */
/*              At least 1 layer is required
 */
if (!ndLayer) {
 yyerror("NONDEFAULTRULE requires at least one layer");
 CHKERR();
 }
 if (!numVia) {
 yyerror("NONDEFAULTRULE requires at least one via");
 CHKERR();
 }
 if (lefrNonDefaultCbk) lefiNonDefault_end( &(lefrNonDefault) ) ;
 CALLBACK(lefrNonDefaultCbk, lefrNonDefaultCbkType, &lefrNonDefault);
 ;
    break;}
case 342:
#line 2330 "lef.y"
{
 if (strcmp(nonDefaultRuleName, yyvsp[0].string) != 0) {
 yyerror("END NONDEFAULTRULE name is different from NONDEFAULTRULE");
 CHKERR();
 }
 ;
    break;}
case 349:
#line 2351 "lef.y"
{ lefDumbMode = 10000000; ;
    break;}
case 350:
#line 2352 "lef.y"
{ lefDumbMode = 0; ;
    break;}
case 353:
#line 2361 "lef.y"
{
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 yyvsp[-1].string) ;
 lefiNonDefault_addProp( &(lefrNonDefault),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 354:
#line 2373 "lef.y"
{
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 yyvsp[-1].string) ;
 lefiNonDefault_addProp( &(lefrNonDefault),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 355:
#line 2385 "lef.y"
{
 char temp[32];
 sprintf(temp, "%g", yyvsp[0].dval);
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 yyvsp[-1].string) ;
 lefiNonDefault_addNumProp( &(lefrNonDefault),
 yyvsp[-1].string,
yyvsp[0].dval,
temp,
propTp) ;
 }
 ;
    break;}
case 356:
#line 2401 "lef.y"
{
 if (lefrNonDefaultCbk) lefiNonDefault_addLayer( &(lefrNonDefault),
 yyvsp[0].string) ;
 ndLayer++;
 strcpy(layerName, yyvsp[0].string);
 ndLayerWidth = 0;
 ndLayerSpace = 0;
 ;
    break;}
case 357:
#line 2410 "lef.y"
{
 if (strcmp(layerName, yyvsp[0].string) != 0) {
 yyerror("END LAYER name is different from LAYER");
 CHKERR();
 }
 if (!ndLayerWidth) {
 yyerror("WIDTH is required in LAYER inside NONDEFULTRULE");
 CHKERR();
 }
 if (!ndLayerSpace) {
 yyerror("SPACING is required in LAYER inside NONDEFULTRULE");
 CHKERR();
 }
 ;
    break;}
case 360:
#line 2434 "lef.y"
{
 ndLayerWidth = 1;
 if (lefrNonDefaultCbk) lefiNonDefault_addWidth( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 ;
    break;}
case 361:
#line 2440 "lef.y"
{
 ndLayerSpace = 1;
 if (lefrNonDefaultCbk) lefiNonDefault_addSpacing( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 ;
    break;}
case 362:
#line 2446 "lef.y"
{ if (lefrNonDefaultCbk)
 lefiNonDefault_addWireExtension( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 ;
    break;}
case 363:
#line 2451 "lef.y"
{
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addResistance( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 } else if (versionNum < 5.4) {
 yyerror("RESISTANCE RPERSQ is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 else if (lefrNonDefaultCbk)
 lefiNonDefault_addResistance( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 ;
    break;}
case 364:
#line 2466 "lef.y"
{
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addCapacitance( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 } else if (versionNum < 5.4) {
 yyerror("CAPACITANCE CPERSQDIST is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 else if (lefrNonDefaultCbk)
 lefiNonDefault_addCapacitance( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 ;
    break;}
case 365:
#line 2480 "lef.y"
{
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addEdgeCap( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 } else if (versionNum < 5.4) {
 yyerror("EDGECAPACITANCE is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 else if (lefrNonDefaultCbk)
 lefiNonDefault_addEdgeCap( &(lefrNonDefault),
 yyvsp[-1].dval) ;
 ;
    break;}
case 366:
#line 2495 "lef.y"
{ CALLBACK(lefrSiteCbk, lefrSiteCbkType, &lefrSite); ;
    break;}
case 367:
#line 2497 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 368:
#line 2498 "lef.y"
{
 if (lefrSiteCbk) lefiSite_setName( &(lefrSite),
 yyvsp[0].string) ;
 strcpy(siteName, yyvsp[0].string);
 ;
    break;}
case 369:
#line 2504 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 370:
#line 2505 "lef.y"
{
 if (strcmp(siteName, yyvsp[0].string) != 0) {
 yyerror("END SITE name is different from SITE");
 CHKERR();
 }
 ;
    break;}
case 373:
#line 2520 "lef.y"
{ if (lefrSiteCbk) lefiSite_setSize( &(lefrSite),
 yyvsp[-3].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 374:
#line 2525 "lef.y"
{ ;
    break;}
case 375:
#line 2527 "lef.y"
{ if (lefrSiteCbk) lefiSite_setClass( &(lefrSite),
 yyvsp[0].string) ;
 ;
    break;}
case 376:
#line 2532 "lef.y"
{yyval.string = (char*)"PAD"; ;
    break;}
case 377:
#line 2533 "lef.y"
{yyval.string = (char*)"CORE"; ;
    break;}
case 378:
#line 2534 "lef.y"
{yyval.string = (char*)"VIRTUAL"; ;
    break;}
case 379:
#line 2537 "lef.y"
{ ;
    break;}
case 382:
#line 2547 "lef.y"
{ if (lefrSiteCbk) lefiSite_setXSymmetry( &(lefrSite) ) ;
 ;
    break;}
case 383:
#line 2550 "lef.y"
{ if (lefrSiteCbk) lefiSite_setYSymmetry( &(lefrSite) ) ;
 ;
    break;}
case 384:
#line 2553 "lef.y"
{ if (lefrSiteCbk) lefiSite_set90Symmetry( &(lefrSite) ) ;
 ;
    break;}
case 385:
#line 2559 "lef.y"
{ yyval.pt.x = yyvsp[-1].dval; yyval.pt.y = yyvsp[0].dval; ;
    break;}
case 386:
#line 2561 "lef.y"
{ yyval.pt.x = yyvsp[-2].dval; yyval.pt.y = yyvsp[-1].dval; ;
    break;}
case 387:
#line 2564 "lef.y"
{ CALLBACK(lefrMacroCbk, lefrMacroCbkType, &lefrMacro); lefrDoSite = 0;
 ;
    break;}
case 389:
#line 2568 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 390:
#line 2569 "lef.y"
{
 siteDef = 0;
 symDef = 0;
 sizeDef = 0;
 pinDef = 0;
 obsDef = 0;
 origDef = 0;
 lefiMacro_clear( &(lefrMacro) ) ;
 if (lefrMacroCbk) lefiMacro_setName( &(lefrMacro),
 yyvsp[0].string) ;
 CALLBACK(lefrMacroBeginCbk, lefrMacroBeginCbkType, yyvsp[0].string);
 strcpy(macroName, yyvsp[0].string);
 ;
    break;}
case 391:
#line 2583 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 392:
#line 2584 "lef.y"
{
 if (strcmp(macroName, yyvsp[0].string) != 0) {
 yyerror("END MACRO name is different from MACRO");
 CHKERR();
 }
 CALLBACK(lefrMacroEndCbk, lefrMacroEndCbkType, yyvsp[0].string);
 ;
    break;}
case 400:
#line 2605 "lef.y"
{ ;
    break;}
case 401:
#line 2607 "lef.y"
{ ;
    break;}
case 402:
#line 2609 "lef.y"
{ ;
    break;}
case 405:
#line 2613 "lef.y"
{ ;
    break;}
case 406:
#line 2615 "lef.y"
{ ;
    break;}
case 407:
#line 2617 "lef.y"
{ ;
    break;}
case 408:
#line 2619 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setBuffer( &(lefrMacro) ) ;
 ;
    break;}
case 409:
#line 2622 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setInverter( &(lefrMacro) ) ;
 ;
    break;}
case 410:
#line 2625 "lef.y"
{ ;
    break;}
case 411:
#line 2627 "lef.y"
{ ;
    break;}
case 412:
#line 2629 "lef.y"
{ ;
    break;}
case 413:
#line 2630 "lef.y"
{lefDumbMode = 1000000;;
    break;}
case 414:
#line 2631 "lef.y"
{ lefDumbMode = 0; ;
    break;}
case 417:
#line 2639 "lef.y"
{
 if (siteDef) { /* SITE is defined before SYMMETRY */

 /* pcr 283846 suppress warning */

 yywarning("SITE is defined before SYMMETRY.");
 }
 symDef = 1;
 ;
    break;}
case 420:
#line 2657 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setXSymmetry( &(lefrMacro) ) ;
 ;
    break;}
case 421:
#line 2660 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setYSymmetry( &(lefrMacro) ) ;
 ;
    break;}
case 422:
#line 2663 "lef.y"
{ if (lefrMacroCbk) lefiMacro_set90Symmetry( &(lefrMacro) ) ;
 ;
    break;}
case 423:
#line 2668 "lef.y"
{
 char temp[32];
 sprintf(temp, "%g", yyvsp[0].dval);
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 yyvsp[-1].string) ;
 lefiMacro_setNumProperty( &(lefrMacro),
 yyvsp[-1].string,
yyvsp[0].dval,
temp,
propTp) ;
 }
 ;
    break;}
case 424:
#line 2683 "lef.y"
{
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 yyvsp[-1].string) ;
 lefiMacro_setProperty( &(lefrMacro),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 425:
#line 2695 "lef.y"
{
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 yyvsp[-1].string) ;
 lefiMacro_setProperty( &(lefrMacro),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 426:
#line 2708 "lef.y"
{
 if (lefrMacroCbk) lefiMacro_setClass( &(lefrMacro),
 yyvsp[-1].string) ;
 if (lefrMacroClassTypeCbk)
 CALLBACK(lefrMacroClassTypeCbk, lefrMacroClassTypeCbkType, yyvsp[-1].string);
 ;
    break;}
case 427:
#line 2716 "lef.y"
{yyval.string = (char*)"COVER"; ;
    break;}
case 428:
#line 2717 "lef.y"
{yyval.string = (char*)"COVER BUMP"; ;
    break;}
case 429:
#line 2718 "lef.y"
{yyval.string = (char*)"RING"; ;
    break;}
case 430:
#line 2719 "lef.y"
{yyval.string = (char*)"BLOCK"; ;
    break;}
case 431:
#line 2720 "lef.y"
{yyval.string = (char*)"BLOCK BLACKBOX"; ;
    break;}
case 432:
#line 2721 "lef.y"
{yyval.string = (char*)"NONE"; ;
    break;}
case 433:
#line 2722 "lef.y"
{yyval.string = (char*)"PAD"; ;
    break;}
case 434:
#line 2723 "lef.y"
{yyval.string = (char*)"VIRTUAL"; ;
    break;}
case 435:
#line 2725 "lef.y"
{sprintf(temp_name, "PAD %s", yyvsp[0].string);
 yyval.string = temp_name;;
    break;}
case 436:
#line 2727 "lef.y"
{yyval.string = (char*)"CORE"; ;
    break;}
case 437:
#line 2729 "lef.y"
{yyval.string = (char*)"CORNER";
 /* This token is NOT in the spec but has shown up in
 * some lef files. This exception came from LEFOUT
 * in 'frameworks'
 */

 ;
    break;}
case 438:
#line 2737 "lef.y"
{sprintf(temp_name, "CORE %s", yyvsp[0].string);
 yyval.string = temp_name;;
    break;}
case 439:
#line 2740 "lef.y"
{sprintf(temp_name, "ENDCAP %s", yyvsp[0].string);
 yyval.string = temp_name;;
    break;}
case 440:
#line 2744 "lef.y"
{yyval.string = (char*)"INPUT";;
    break;}
case 441:
#line 2745 "lef.y"
{yyval.string = (char*)"OUTPUT";;
    break;}
case 442:
#line 2746 "lef.y"
{yyval.string = (char*)"INOUT";;
    break;}
case 443:
#line 2747 "lef.y"
{yyval.string = (char*)"POWER";;
    break;}
case 444:
#line 2748 "lef.y"
{yyval.string = (char*)"SPACER";;
    break;}
case 445:
#line 2749 "lef.y"
{yyval.string = (char*)"AREAIO";;
    break;}
case 446:
#line 2752 "lef.y"
{yyval.string = (char*)"FEEDTHRU";;
    break;}
case 447:
#line 2753 "lef.y"
{yyval.string = (char*)"TIEHIGH";;
    break;}
case 448:
#line 2754 "lef.y"
{yyval.string = (char*)"TIELOW";;
    break;}
case 449:
#line 2756 "lef.y"
{
 if (ignoreVersion)
 yyval.string = (char*)"SPACER";
 else if (versionNum < 5.4) {
 yyerror("SPACER is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 else
 yyval.string = (char*)"SPACER";
 ;
    break;}
case 450:
#line 2767 "lef.y"
{
 if (ignoreVersion)
 yyval.string = (char*)"ANTENNACELL";
 else if (versionNum < 5.4) {
 yyerror("ANTENNACELL is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 else
 yyval.string = (char*)"ANTENNACELL";
 ;
    break;}
case 451:
#line 2779 "lef.y"
{yyval.string = (char*)"PRE";;
    break;}
case 452:
#line 2780 "lef.y"
{yyval.string = (char*)"POST";;
    break;}
case 453:
#line 2781 "lef.y"
{yyval.string = (char*)"TOPLEFT";;
    break;}
case 454:
#line 2782 "lef.y"
{yyval.string = (char*)"TOPRIGHT";;
    break;}
case 455:
#line 2783 "lef.y"
{yyval.string = (char*)"BOTTOMLEFT";;
    break;}
case 456:
#line 2784 "lef.y"
{yyval.string = (char*)"BOTTOMRIGHT";;
    break;}
case 457:
#line 2787 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setGenerator( &(lefrMacro),
 yyvsp[-1].string) ;
 ;
    break;}
case 458:
#line 2792 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setGenerate( &(lefrMacro),
 yyvsp[-2].string,
yyvsp[-1].string) ;
 ;
    break;}
case 459:
#line 2799 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "USER") ;
 ;
    break;}
case 460:
#line 2803 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "GENERATE") ;
 ;
    break;}
case 461:
#line 2807 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "BLOCK") ;
 ;
    break;}
case 462:
#line 2812 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrMacroCbk) lefiMacro_setPower( &(lefrMacro),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 463:
#line 2821 "lef.y"
{
 if (origDef) { /* Has multiple ORIGIN defined in a macro, stop parsing*/

 yyerror("ORIGIN statement is defined more than once. Parser stops executions.");
 CHKERR();
 }
 origDef = 1;
 if (siteDef) { /* SITE is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 yywarning("SITE is defined before ORIGIN.");
 }
 if (pinDef) { /* PIN is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 yywarning("PIN is defined before ORIGIN.");
 }
 if (obsDef) { /* OBS is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 yywarning("OBS is defined before ORIGIN.");
 }
 /* 11/22/99 - Wanda da Rosa. PCR 283846
 can be defined any order.
 if (symDef) * SYMMETRY is defined before ORIGIN *
 yywarning("SYMMETRY is defined before ORIGIN.");
 */

 /* Add back it back in per Nora request on PCR 283846 */

 /* 1/14/2000 - Wanda da Rosa, PCR 288770
 if (sizeDef) * SIZE is defined before ORIGIN *
 yywarning("SIZE is defined before ORIGIN.");
 */

 
 if (lefrMacroCbk) lefiMacro_setOrigin( &(lefrMacro),
 yyvsp[-1].pt.x,
yyvsp[-1].pt.y) ;
 if (lefrMacroOriginCbk) {
 macroNum.x = yyvsp[-1].pt.x;
 macroNum.y = yyvsp[-1].pt.y;
 CALLBACK(lefrMacroOriginCbk, lefrMacroOriginCbkType, macroNum);
 }
 ;
    break;}
case 464:
#line 2872 "lef.y"
{ if (lefrMacroCbk)
 lefiMacro_setForeign( &(lefrMacro),
 yyvsp[-1].string,
0,
0.0,
0.0,
-1) ;
 ;
    break;}
case 465:
#line 2881 "lef.y"
{ if (lefrMacroCbk)
 lefiMacro_setForeign( &(lefrMacro),
 yyvsp[-2].string,
1,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y,
-1) ;
 ;
    break;}
case 466:
#line 2890 "lef.y"
{ if (lefrMacroCbk)
 lefiMacro_setForeign( &(lefrMacro),
 yyvsp[-3].string,
1,
yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].integer) ;
 ;
    break;}
case 467:
#line 2899 "lef.y"
{ if (lefrMacroCbk)
 lefiMacro_setForeign( &(lefrMacro),
 yyvsp[-2].string,
0,
0.0,
0.0,
yyvsp[-1].integer) ;
 ;
    break;}
case 468:
#line 2908 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 469:
#line 2909 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setEEQ( &(lefrMacro),
 yyvsp[-1].string) ;
 ;
    break;}
case 470:
#line 2913 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 471:
#line 2914 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setLEQ( &(lefrMacro),
 yyvsp[-1].string) ;
 ;
    break;}
case 472:
#line 2920 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setSiteName( &(lefrMacro),
 yyvsp[-1].string) ;
 ;
    break;}
case 473:
#line 2924 "lef.y"
{
 if (lefrMacroCbk) {
 lefiMacro_setSitePattern( &(	lefrMacro),
 lefrSitePatternPtr) ;
 	lefrSitePatternPtr = 0;
 }
 ;
    break;}
case 474:
#line 2933 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; siteDef = 1;
 if (lefrMacroCbk) lefrDoSite = 1; ;
    break;}
case 475:
#line 2937 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 476:
#line 2940 "lef.y"
{
 if (siteDef) { /* SITE is defined before SIZE */

 /* pcr 283846 suppress warning
 yywarning("SITE is defined before SIZE.");
 return 1;
 */

 }
 sizeDef = 1;
 if (lefrMacroCbk) lefiMacro_setSize( &(lefrMacro),
 yyvsp[-3].dval,
yyvsp[-1].dval) ;
 if (lefrMacroSizeCbk) {
 macroNum.x = yyvsp[-3].dval;
 macroNum.y = yyvsp[-1].dval;
 CALLBACK(lefrMacroSizeCbk, lefrMacroSizeCbkType, macroNum);
 }
 ;
    break;}
case 477:
#line 2965 "lef.y"
{ CALLBACK(lefrPinCbk, lefrPinCbkType, &lefrPin);
 lefiPin_clear( &(lefrPin) ) ;
 ;
    break;}
case 478:
#line 2969 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; pinDef = 1;;
    break;}
case 479:
#line 2970 "lef.y"
{ if (lefrPinCbk) lefiPin_setName( &(lefrPin),
 yyvsp[0].string) ;
 strcpy(pinName, yyvsp[0].string);
 ;
    break;}
case 480:
#line 2975 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 481:
#line 2976 "lef.y"
{
 if (strcmp(pinName, yyvsp[0].string) != 0) {
 yyerror("END PIN name is different from PIN");
 CHKERR();
 }
 ;
    break;}
case 482:
#line 2986 "lef.y"
{ ;
    break;}
case 483:
#line 2988 "lef.y"
{ ;
    break;}
case 484:
#line 2992 "lef.y"
{ if (lefrPinCbk) lefiPin_setForeign( &(lefrPin),
 yyvsp[-1].string,
0,
0.0,
0.0,
-1) ;
 ;
    break;}
case 485:
#line 3000 "lef.y"
{ if (lefrPinCbk) lefiPin_setForeign( &(lefrPin),
 yyvsp[-2].string,
1,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y,
-1) ;
 ;
    break;}
case 486:
#line 3008 "lef.y"
{ if (lefrPinCbk) lefiPin_setForeign( &(lefrPin),
 yyvsp[-3].string,
1,
yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].integer) ;
 ;
    break;}
case 487:
#line 3016 "lef.y"
{ if (lefrPinCbk) lefiPin_setForeign( &(lefrPin),
 yyvsp[-2].string,
0,
0.0,
0.0,
-1) ;
 ;
    break;}
case 488:
#line 3024 "lef.y"
{ if (lefrPinCbk) lefiPin_setForeign( &(lefrPin),
 yyvsp[-3].string,
1,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y,
-1) ;
 ;
    break;}
case 489:
#line 3032 "lef.y"
{ if (lefrPinCbk) lefiPin_setForeign( &(lefrPin),
 yyvsp[-4].string,
1,
yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].integer) ;
 ;
    break;}
case 490:
#line 3039 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 491:
#line 3040 "lef.y"
{ if (lefrPinCbk) lefiPin_setLEQ( &(lefrPin),
 yyvsp[-1].string) ;
 ;
    break;}
case 492:
#line 3044 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setPower( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 493:
#line 3052 "lef.y"
{ if (lefrPinCbk) lefiPin_setDirection( &(lefrPin),
 yyvsp[0].string) ;
 ;
    break;}
case 494:
#line 3056 "lef.y"
{ if (lefrPinCbk) lefiPin_setUse( &(lefrPin),
 yyvsp[-1].string) ;
 ;
    break;}
case 495:
#line 3060 "lef.y"
{ ;
    break;}
case 496:
#line 3062 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setLeakage( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO LEAKAGE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 497:
#line 3070 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseThresh( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO RISETHRESH is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 498:
#line 3078 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallThresh( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO FALLTHRESH is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 499:
#line 3086 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseSatcur( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO RISESATCUR is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 500:
#line 3094 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallSatcur( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO FALLSATCUR is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 501:
#line 3102 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setVLO( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO VLO is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 502:
#line 3110 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setVHI( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO VHI is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 503:
#line 3118 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setTieoffr( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO TIEOFFR is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 504:
#line 3126 "lef.y"
{ if (lefrPinCbk) lefiPin_setShape( &(lefrPin),
 yyvsp[-1].string) ;
 ;
    break;}
case 505:
#line 3129 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 506:
#line 3130 "lef.y"
{ if (lefrPinCbk) lefiPin_setMustjoin( &(lefrPin),
 yyvsp[-1].string) ;
 ;
    break;}
case 507:
#line 3133 "lef.y"
{lefDumbMode = 1;;
    break;}
case 508:
#line 3134 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setOutMargin( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 } else
 yywarning(
 "MACRO OUTPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 509:
#line 3143 "lef.y"
{lefDumbMode = 1;;
    break;}
case 510:
#line 3144 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setOutResistance( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 } else
 yywarning(
 "MACRO OUTPUTRESISTANCE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 511:
#line 3153 "lef.y"
{lefDumbMode = 1;;
    break;}
case 512:
#line 3154 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setInMargin( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 } else
 yywarning(
 "MACRO INPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 513:
#line 3164 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCapacitance( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO CAPACITANCE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 514:
#line 3172 "lef.y"
{ if (lefrPinCbk) lefiPin_setMaxdelay( &(lefrPin),
 yyvsp[-1].dval) ;
 ;
    break;}
case 515:
#line 3176 "lef.y"
{ if (lefrPinCbk) lefiPin_setMaxload( &(lefrPin),
 yyvsp[-1].dval) ;
 ;
    break;}
case 516:
#line 3180 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setResistance( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO RESISTANCE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 517:
#line 3188 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setPulldownres( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning("MACRO PULLDOWNRES is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 518:
#line 3196 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCurrentSource( &(lefrPin),
 "ACTIVE") ;
 } else
 yywarning(
 "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 519:
#line 3205 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCurrentSource( &(lefrPin),
 "RESISTIVE") ;
 } else
 yywarning(
 "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 520:
#line 3214 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseVoltage( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning(
 "MACRO RISEVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 521:
#line 3223 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallVoltage( &(lefrPin),
 yyvsp[-1].dval) ;
 } else
 yywarning(
 "MACRO FALLVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 522:
#line 3232 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setTables( &(lefrPin),
 yyvsp[-2].string,
yyvsp[-1].string) ;
 } else
 yywarning("MACRO IV_TABLES is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 523:
#line 3241 "lef.y"
{ if (lefrPinCbk) lefiPin_setTaperRule( &(lefrPin),
 yyvsp[-1].string) ;
 ;
    break;}
case 524:
#line 3244 "lef.y"
{lefDumbMode = 1000000;;
    break;}
case 525:
#line 3245 "lef.y"
{ lefDumbMode = 0; ;
    break;}
case 526:
#line 3247 "lef.y"
{
 lefDumbMode = 0;
 if (lefrPinCbk) {
 lefiPin_addPort( &(	lefrPin),
 lefrGeometriesPtr) ;
 	lefrGeometriesPtr = 0;
 	lefrDoGeometries = 0;
 }
 ;
    break;}
case 527:
#line 3261 "lef.y"
{
 /* Since in start_macro_port it has call the Init method, here
 */
/* we need to call the Destroy method.
 */
/* Still add a null pointer to set the number of port
 */
lefiPin_addPort( &(lefrPin),
 lefrGeometriesPtr) ;
 lefrGeometriesPtr = 0;
 lefrDoGeometries = 0;
 ;
    break;}
case 528:
#line 3274 "lef.y"
{ /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 yyerror("ANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaSize( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 529:
#line 3291 "lef.y"
{ /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 yyerror("ANTENNAMETALAREA is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMetalArea( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 530:
#line 3308 "lef.y"
{ /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 yyerror("ANTENNAMETALLENGTH is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMetalLength( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 531:
#line 3325 "lef.y"
{ if (lefrPinCbk) lefiPin_setRiseSlewLimit( &(lefrPin),
 yyvsp[-1].dval) ;
 ;
    break;}
case 532:
#line 3329 "lef.y"
{ if (lefrPinCbk) lefiPin_setFallSlewLimit( &(lefrPin),
 yyvsp[-1].dval) ;
 ;
    break;}
case 533:
#line 3333 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialMetalArea( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 534:
#line 3351 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialMetalSideArea( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 535:
#line 3369 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialCutArea( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 536:
#line 3387 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaDiffArea( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 537:
#line 3405 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaGateArea( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 538:
#line 3423 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxAreaCar( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 539:
#line 3441 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxSideAreaCar( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 540:
#line 3459 "lef.y"
{ /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxCutCar( &(lefrPin),
 yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 541:
#line 3477 "lef.y"
{ /* 5.5 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.5) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 ;
    break;}
case 543:
#line 3495 "lef.y"
{
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 1) ;
 ;
    break;}
case 544:
#line 3501 "lef.y"
{
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 2) ;
 ;
    break;}
case 545:
#line 3507 "lef.y"
{
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 3) ;
 ;
    break;}
case 546:
#line 3513 "lef.y"
{
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 4) ;
 ;
    break;}
case 549:
#line 3526 "lef.y"
{
 char temp[32];
 sprintf(temp, "%g", yyvsp[0].dval);
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 yyvsp[-1].string) ;
 lefiPin_setNumProperty( &(lefrPin),
 yyvsp[-1].string,
yyvsp[0].dval,
temp,
propTp) ;
 }
 ;
    break;}
case 550:
#line 3541 "lef.y"
{
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 yyvsp[-1].string) ;
 lefiPin_setProperty( &(lefrPin),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 551:
#line 3553 "lef.y"
{
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 yyvsp[-1].string) ;
 lefiPin_setProperty( &(lefrPin),
 yyvsp[-1].string,
yyvsp[0].string,
propTp) ;
 }
 ;
    break;}
case 552:
#line 3566 "lef.y"
{yyval.string = (char*)"INPUT";;
    break;}
case 553:
#line 3567 "lef.y"
{yyval.string = (char*)"OUTPUT";;
    break;}
case 554:
#line 3568 "lef.y"
{yyval.string = (char*)"OUTPUT TRISTATE";;
    break;}
case 555:
#line 3569 "lef.y"
{yyval.string = (char*)"INOUT";;
    break;}
case 556:
#line 3570 "lef.y"
{yyval.string = (char*)"FEEDTHRU";;
    break;}
case 557:
#line 3573 "lef.y"
{
 if (lefrPinCbk) {
 	lefrDoGeometries = 1;
 	lefrGeometriesPtr = (lefiGeometries*)lefMalloc( sizeof(lefiGeometries));
 lefiGeometries_Init( 	lefrGeometriesPtr ) ;
 }
 ;
    break;}
case 559:
#line 3584 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addClass( lefrGeometriesPtr,
 yyvsp[-1].string) ;
 ;
    break;}
case 560:
#line 3590 "lef.y"
{yyval.string = (char*)"SIGNAL";;
    break;}
case 561:
#line 3591 "lef.y"
{yyval.string = (char*)"ANALOG";;
    break;}
case 562:
#line 3592 "lef.y"
{yyval.string = (char*)"POWER";;
    break;}
case 563:
#line 3593 "lef.y"
{yyval.string = (char*)"GROUND";;
    break;}
case 564:
#line 3594 "lef.y"
{yyval.string = (char*)"CLOCK";;
    break;}
case 565:
#line 3595 "lef.y"
{yyval.string = (char*)"DATA";;
    break;}
case 566:
#line 3598 "lef.y"
{yyval.string = (char*)"INPUT";;
    break;}
case 567:
#line 3599 "lef.y"
{yyval.string = (char*)"OUTPUT";;
    break;}
case 568:
#line 3600 "lef.y"
{yyval.string = (char*)"START";;
    break;}
case 569:
#line 3601 "lef.y"
{yyval.string = (char*)"STOP";;
    break;}
case 570:
#line 3605 "lef.y"
{yyval.string = (char*)""; ;
    break;}
case 571:
#line 3607 "lef.y"
{yyval.string = (char*)"ABUTMENT";;
    break;}
case 572:
#line 3608 "lef.y"
{yyval.string = (char*)"RING";;
    break;}
case 573:
#line 3609 "lef.y"
{yyval.string = (char*)"FEEDTHRU";;
    break;}
case 575:
#line 3614 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 576:
#line 3615 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addLayer( lefrGeometriesPtr,
 yyvsp[0].string) ;
 ;
    break;}
case 578:
#line 3621 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addWidth( lefrGeometriesPtr,
 yyvsp[-1].dval) ;
 ;
    break;}
case 579:
#line 3626 "lef.y"
{ if (lefrDoGeometries) lefiGeometries_addPath( lefrGeometriesPtr ) ;
 ;
    break;}
case 580:
#line 3629 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addPathIter( lefrGeometriesPtr ) ;
 ;
    break;}
case 581:
#line 3633 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addRect( lefrGeometriesPtr,
 yyvsp[-2].pt.x,
yyvsp[-2].pt.y,
yyvsp[-1].pt.x,
yyvsp[-1].pt.y) ;
 ;
    break;}
case 582:
#line 3641 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addRectIter( lefrGeometriesPtr,
 yyvsp[-3].pt.x,
yyvsp[-3].pt.y,
yyvsp[-2].pt.x,
yyvsp[-2].pt.y) ;
 ;
    break;}
case 583:
#line 3649 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 ;
    break;}
case 584:
#line 3653 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addPolygonIter( lefrGeometriesPtr ) ;
 ;
    break;}
case 585:
#line 3657 "lef.y"
{ ;
    break;}
case 589:
#line 3666 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addLayerMinSpacing( lefrGeometriesPtr,
 yyvsp[0].dval) ;
 ;
    break;}
case 590:
#line 3671 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addLayerRuleWidth( lefrGeometriesPtr,
 yyvsp[0].dval) ;
 ;
    break;}
case 591:
#line 3677 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_startList( lefrGeometriesPtr,
 yyvsp[0].pt.x,
yyvsp[0].pt.y) ;
 ;
    break;}
case 592:
#line 3684 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addToList( lefrGeometriesPtr,
 yyvsp[0].pt.x,
yyvsp[0].pt.y) ;
 ;
    break;}
case 595:
#line 3697 "lef.y"
{lefDumbMode = 1;;
    break;}
case 596:
#line 3698 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addVia( lefrGeometriesPtr,
 yyvsp[-3].pt.x,
yyvsp[-3].pt.y,
yyvsp[-1].string) ;
 ;
    break;}
case 597:
#line 3704 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 598:
#line 3706 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addViaIter( lefrGeometriesPtr,
 yyvsp[-4].pt.x,
yyvsp[-4].pt.y,
yyvsp[-2].string) ;
 ;
    break;}
case 599:
#line 3715 "lef.y"
{ if (lefrDoGeometries)
 lefiGeometries_addStepPattern( lefrGeometriesPtr,
 yyvsp[-5].dval,
yyvsp[-3].dval,
yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 600:
#line 3725 "lef.y"
{
 if (lefrDoSite) {
 	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 				 sizeof(lefiSitePattern));
 lefiSitePattern_Init( 	lefrSitePatternPtr ) ;
 lefiSitePattern_set( 	lefrSitePatternPtr,
 yyvsp[-10].string,
yyvsp[-9].dval,
yyvsp[-8].dval,
yyvsp[-7].integer,
yyvsp[-5].dval,
yyvsp[-3].dval,
yyvsp[-1].dval,
yyvsp[0].dval) ;
 	}
 ;
    break;}
case 601:
#line 3744 "lef.y"
{
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 				sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "X",
yyvsp[-4].dval,
(int)yyvsp[-2].dval,yyvsp[0].dval) ;
 }
 ;
    break;}
case 602:
#line 3755 "lef.y"
{lefDumbMode = 1000000000;;
    break;}
case 603:
#line 3756 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 604:
#line 3758 "lef.y"
{
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "Y",
yyvsp[-4].dval,
(int)yyvsp[-2].dval,yyvsp[0].dval) ;
 }
 ;
    break;}
case 605:
#line 3769 "lef.y"
{lefDumbMode = 1000000000;;
    break;}
case 606:
#line 3770 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 607:
#line 3772 "lef.y"
{
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "X",
yyvsp[-4].dval,
(int)yyvsp[-2].dval,yyvsp[0].dval) ;
 }
 ;
    break;}
case 608:
#line 3784 "lef.y"
{
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "Y",
yyvsp[-4].dval,
(int)yyvsp[-2].dval,yyvsp[0].dval) ;
 }
 ;
    break;}
case 611:
#line 3803 "lef.y"
{ if (lefrDoTrack) lefiTrackPattern_addLayer( lefrTrackPatternPtr,
 yyvsp[0].string) ;
 ;
    break;}
case 612:
#line 3808 "lef.y"
{
 if (lefrDoGcell) {
 	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
 sizeof(lefiGcellPattern));
 lefiGcellPattern_Init( 	lefrGcellPatternPtr ) ;
 lefiGcellPattern_set( 	lefrGcellPatternPtr,
 "X",
yyvsp[-4].dval,
(int)yyvsp[-2].dval,yyvsp[0].dval) ;
 }
 ;
    break;}
case 613:
#line 3820 "lef.y"
{
 if (lefrDoGcell) {
 	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
 sizeof(lefiGcellPattern));
 lefiGcellPattern_Init( 	lefrGcellPatternPtr ) ;
 lefiGcellPattern_set( 	lefrGcellPatternPtr,
 "Y",
yyvsp[-4].dval,
(int)yyvsp[-2].dval,yyvsp[0].dval) ;
 }
 ;
    break;}
case 614:
#line 3833 "lef.y"
{
 if (lefrObstructionCbk) {
 lefiObstruction_setGeometries( &(	lefrObstruction),
 lefrGeometriesPtr) ;
 	lefrGeometriesPtr = 0;
 	lefrDoGeometries = 0;
 }
 CALLBACK(lefrObstructionCbk, lefrObstructionCbkType, &lefrObstruction);
 lefDumbMode = 0;
 ;
    break;}
case 616:
#line 3854 "lef.y"
{
 obsDef = 1;
 if (lefrObstructionCbk) {
 	lefrDoGeometries = 1;
 	lefrGeometriesPtr = (lefiGeometries*)lefMalloc(
 	 sizeof(lefiGeometries));
 lefiGeometries_Init( 	lefrGeometriesPtr ) ;
 	}
 ;
    break;}
case 617:
#line 3864 "lef.y"
{ lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 618:
#line 3865 "lef.y"
{ if (lefrMacroCbk) lefiMacro_setClockType( &(lefrMacro),
 yyvsp[-1].string) ;
 ;
    break;}
case 619:
#line 3870 "lef.y"
{ ;
    break;}
case 620:
#line 3873 "lef.y"
{ /* XXXXX for macros */
;
    break;}
case 621:
#line 3877 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrTimingCbk && lefiTiming_hasData( &(lefrTiming) ) )  CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming) ;
 lefiTiming_clear( &(lefrTiming) ) ;
 } else {
 yywarning("MACRO TIMING is obsolete in 5.4. It will be ignored.");
 lefiTiming_clear( &(lefrTiming) ) ;
 }
 ;
    break;}
case 624:
#line 3895 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrTimingCbk && lefiTiming_hasData( &(lefrTiming) ) )  CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming) ;
 }
 lefDumbMode = 1000000000;
 lefiTiming_clear( &(lefrTiming) ) ;
 ;
    break;}
case 625:
#line 3903 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 626:
#line 3904 "lef.y"
{lefDumbMode = 1000000000;;
    break;}
case 627:
#line 3905 "lef.y"
{ lefDumbMode = 0;;
    break;}
case 628:
#line 3907 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addRiseFall( &(lefrTiming),
 yyvsp[-3].string,
yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 629:
#line 3913 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addRiseFallVariable( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 630:
#line 3919 "lef.y"
{ if (lefrTimingCbk) {
 	if (yyvsp[-7].string[0] == 'D' || yyvsp[-7].string[0] == 'd') /* delay */

 	 lefiTiming_addDelay( &(lefrTiming),
 yyvsp[-8].string,
yyvsp[-5].string,
yyvsp[-3].dval,
yyvsp[-2].dval,
yyvsp[-1].dval) ;
 	else
 	 lefiTiming_addTransition( &(lefrTiming),
 yyvsp[-8].string,
yyvsp[-5].string,
yyvsp[-3].dval,
yyvsp[-2].dval,
yyvsp[-1].dval) ;
 	}
 ;
    break;}
case 631:
#line 3938 "lef.y"
{ ;
    break;}
case 632:
#line 3940 "lef.y"
{ ;
    break;}
case 633:
#line 3942 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setRiseRS( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 634:
#line 3947 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setFallRS( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 635:
#line 3952 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setRiseCS( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 636:
#line 3957 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setFallCS( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 637:
#line 3962 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setRiseAtt1( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 638:
#line 3967 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setFallAtt1( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 639:
#line 3972 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setRiseTo( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 640:
#line 3977 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setFallTo( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 641:
#line 3982 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addUnateness( &(lefrTiming),
 yyvsp[-1].string) ;
 ;
    break;}
case 642:
#line 3986 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setStable( &(lefrTiming),
 yyvsp[-4].dval,
yyvsp[-2].dval,
yyvsp[-1].string) ;
 ;
    break;}
case 643:
#line 3992 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addSDF2Pins( &(lefrTiming),
 yyvsp[-7].string,
yyvsp[-6].string,
yyvsp[-5].string,
yyvsp[-3].dval,
yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 644:
#line 4001 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addSDF1Pin( &(lefrTiming),
 yyvsp[-5].string,
yyvsp[-3].dval,
yyvsp[-2].dval,
yyvsp[-2].dval) ;
 ;
    break;}
case 645:
#line 4008 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setSDFcondStart( &(lefrTiming),
 yyvsp[-1].string) ;
 ;
    break;}
case 646:
#line 4012 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setSDFcondEnd( &(lefrTiming),
 yyvsp[-1].string) ;
 ;
    break;}
case 647:
#line 4016 "lef.y"
{ if (lefrTimingCbk) lefiTiming_setSDFcond( &(lefrTiming),
 yyvsp[-1].string) ;
 ;
    break;}
case 648:
#line 4020 "lef.y"
{ /* XXXXX */
;
    break;}
case 649:
#line 4025 "lef.y"
{ yyval.string = (char*)"MPWH";;
    break;}
case 650:
#line 4027 "lef.y"
{ yyval.string = (char*)"MPWL";;
    break;}
case 651:
#line 4029 "lef.y"
{ yyval.string = (char*)"PERIOD";;
    break;}
case 652:
#line 4033 "lef.y"
{ yyval.string = (char*)"SETUP";;
    break;}
case 653:
#line 4035 "lef.y"
{ yyval.string = (char*)"HOLD";;
    break;}
case 654:
#line 4037 "lef.y"
{ yyval.string = (char*)"RECOVERY";;
    break;}
case 655:
#line 4039 "lef.y"
{ yyval.string = (char*)"SKEW";;
    break;}
case 656:
#line 4043 "lef.y"
{ yyval.string = (char*)"ANYEDGE";;
    break;}
case 657:
#line 4045 "lef.y"
{ yyval.string = (char*)"POSEDGE";;
    break;}
case 658:
#line 4047 "lef.y"
{ yyval.string = (char*)"NEGEDGE";;
    break;}
case 659:
#line 4051 "lef.y"
{ yyval.string = (char*)"ANYEDGE";;
    break;}
case 660:
#line 4053 "lef.y"
{ yyval.string = (char*)"POSEDGE";;
    break;}
case 661:
#line 4055 "lef.y"
{ yyval.string = (char*)"NEGEDGE";;
    break;}
case 662:
#line 4059 "lef.y"
{ yyval.string = (char*)"DELAY"; ;
    break;}
case 663:
#line 4061 "lef.y"
{ yyval.string = (char*)"TRANSITION"; ;
    break;}
case 664:
#line 4065 "lef.y"
{ ;
    break;}
case 665:
#line 4067 "lef.y"
{ ;
    break;}
case 666:
#line 4070 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addTableEntry( &(lefrTiming),
 yyvsp[-3].dval,
yyvsp[-2].dval,
yyvsp[-1].dval) ;
 ;
    break;}
case 667:
#line 4078 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addTableAxisNumber( &(lefrTiming),
 yyvsp[0].dval) ;
 ;
    break;}
case 668:
#line 4082 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addTableAxisNumber( &(lefrTiming),
 yyvsp[0].dval) ;
 ;
    break;}
case 669:
#line 4089 "lef.y"
{ ;
    break;}
case 670:
#line 4091 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addRiseFallSlew( &(lefrTiming),
 yyvsp[-3].dval,
yyvsp[-2].dval,
yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 671:
#line 4098 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addRiseFallSlew( &(lefrTiming),
 yyvsp[-6].dval,
yyvsp[-5].dval,
yyvsp[-4].dval,
yyvsp[-3].dval) ;
 if (lefrTimingCbk) lefiTiming_addRiseFallSlew2( &(lefrTiming),
 yyvsp[-2].dval,
yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 672:
#line 4111 "lef.y"
{ yyval.string = (char*)"RISE"; ;
    break;}
case 673:
#line 4113 "lef.y"
{ yyval.string = (char*)"FALL"; ;
    break;}
case 674:
#line 4117 "lef.y"
{ yyval.string = (char*)"INVERT"; ;
    break;}
case 675:
#line 4119 "lef.y"
{ yyval.string = (char*)"NONINVERT"; ;
    break;}
case 676:
#line 4121 "lef.y"
{ yyval.string = (char*)"NONUNATE"; ;
    break;}
case 677:
#line 4125 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addFromPin( &(lefrTiming),
 yyvsp[0].string) ;
 ;
    break;}
case 678:
#line 4129 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addFromPin( &(lefrTiming),
 yyvsp[0].string) ;
 ;
    break;}
case 679:
#line 4135 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addToPin( &(lefrTiming),
 yyvsp[0].string) ;
 ;
    break;}
case 680:
#line 4139 "lef.y"
{ if (lefrTimingCbk) lefiTiming_addToPin( &(lefrTiming),
 yyvsp[0].string) ;
 ;
    break;}
case 681:
#line 4144 "lef.y"
{
 	CALLBACK(lefrArrayCbk, lefrArrayCbkType, &lefrArray);
 lefiArray_clear( &(	lefrArray) ) ;
 	lefrSitePatternPtr = 0;
 	lefrDoSite = 0;
 ;
    break;}
case 683:
#line 4152 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 684:
#line 4153 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_setName( &(	lefrArray),
 yyvsp[0].string) ;
 	CALLBACK(lefrArrayBeginCbk, lefrArrayBeginCbkType, yyvsp[0].string);
 }
 strcpy(arrayName, yyvsp[0].string);
 ;
    break;}
case 685:
#line 4162 "lef.y"
{lefDumbMode = 1; lefNoNum = 1;;
    break;}
case 686:
#line 4163 "lef.y"
{
 if (lefrArrayCbk)
 	CALLBACK(lefrArrayEndCbk, lefrArrayEndCbkType, yyvsp[0].string);
 if (strcmp(arrayName, yyvsp[0].string) != 0) {
 yyerror("END ARRAY name is different from ARRAY");
 CHKERR();
 }
 ;
    break;}
case 687:
#line 4175 "lef.y"
{ ;
    break;}
case 688:
#line 4177 "lef.y"
{ ;
    break;}
case 689:
#line 4180 "lef.y"
{ if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;
    break;}
case 690:
#line 4182 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_addSitePattern( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 ;
    break;}
case 691:
#line 4188 "lef.y"
{lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;
    break;}
case 692:
#line 4190 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_addCanPlace( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 ;
    break;}
case 693:
#line 4196 "lef.y"
{lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;
    break;}
case 694:
#line 4198 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_addCannotOccupy( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 ;
    break;}
case 695:
#line 4204 "lef.y"
{ if (lefrArrayCbk) lefrDoTrack = 1; ;
    break;}
case 696:
#line 4205 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_addTrack( &(	lefrArray),
 lefrTrackPatternPtr) ;
 }
 ;
    break;}
case 697:
#line 4212 "lef.y"
{
 ;
    break;}
case 698:
#line 4214 "lef.y"
{ if (lefrArrayCbk) lefrDoGcell = 1; ;
    break;}
case 699:
#line 4215 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_addGcell( &(	lefrArray),
 lefrGcellPatternPtr) ;
 }
 ;
    break;}
case 700:
#line 4222 "lef.y"
{
 if (lefrArrayCbk) {
 lefiArray_setTableSize( &(	lefrArray),
 (int)yyvsp[-3].dval) ;
 }
 ;
    break;}
case 701:
#line 4229 "lef.y"
{ ;
    break;}
case 702:
#line 4232 "lef.y"
{ if (lefrArrayCbk) lefiArray_addFloorPlan( &(lefrArray),
 yyvsp[0].string) ;
 ;
    break;}
case 703:
#line 4239 "lef.y"
{ ;
    break;}
case 704:
#line 4241 "lef.y"
{ ;
    break;}
case 705:
#line 4244 "lef.y"
{ lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;
    break;}
case 706:
#line 4246 "lef.y"
{
 if (lefrArrayCbk)
 lefiArray_addSiteToFloorPlan( &(	lefrArray),
 "CANPLACE",
	lefrSitePatternPtr) ;
 ;
    break;}
case 707:
#line 4252 "lef.y"
{ if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;
    break;}
case 708:
#line 4254 "lef.y"
{
 if (lefrArrayCbk)
 lefiArray_addSiteToFloorPlan( &(	lefrArray),
 "CANNOTOCCUPY",
	lefrSitePatternPtr) ;
 ;
    break;}
case 709:
#line 4264 "lef.y"
{ ;
    break;}
case 710:
#line 4266 "lef.y"
{ ;
    break;}
case 711:
#line 4269 "lef.y"
{ if (lefrArrayCbk) lefiArray_addDefaultCap( &(lefrArray),
 (int)yyvsp[-3].dval,yyvsp[-1].dval) ;
 ;
    break;}
case 712:
#line 4274 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 713:
#line 4275 "lef.y"
{ lefAddStringMessage(yyvsp[-3].string, yyvsp[-1].string); ;
    break;}
case 714:
#line 4278 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 715:
#line 4279 "lef.y"
{ ;
    break;}
case 716:
#line 4282 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 717:
#line 4283 "lef.y"
{ lefAddNumDefine(yyvsp[-3].string, yyvsp[-1].dval); ;
    break;}
case 718:
#line 4284 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 719:
#line 4285 "lef.y"
{ lefAddStringDefine(yyvsp[-3].string, yyvsp[-1].string); ;
    break;}
case 720:
#line 4286 "lef.y"
{lefDumbMode=1;lefNlToken=TRUE;;
    break;}
case 721:
#line 4287 "lef.y"
{ lefAddBooleanDefine(yyvsp[-3].string, yyvsp[-1].integer); ;
    break;}
case 723:
#line 4292 "lef.y"
{lefNlToken = FALSE;;
    break;}
case 724:
#line 4293 "lef.y"
{lefNlToken = FALSE;;
    break;}
case 729:
#line 4306 "lef.y"
{yyval.dval = yyvsp[-2].dval + yyvsp[0].dval; ;
    break;}
case 730:
#line 4307 "lef.y"
{yyval.dval = yyvsp[-2].dval - yyvsp[0].dval; ;
    break;}
case 731:
#line 4308 "lef.y"
{yyval.dval = yyvsp[-2].dval * yyvsp[0].dval; ;
    break;}
case 732:
#line 4309 "lef.y"
{yyval.dval = yyvsp[-2].dval / yyvsp[0].dval; ;
    break;}
case 733:
#line 4310 "lef.y"
{yyval.dval = -yyvsp[0].dval;;
    break;}
case 734:
#line 4311 "lef.y"
{yyval.dval = yyvsp[-1].dval;;
    break;}
case 735:
#line 4313 "lef.y"
{yyval.dval = (yyvsp[-4].integer != 0) ? yyvsp[-2].dval : yyvsp[0].dval;;
    break;}
case 736:
#line 4314 "lef.y"
{yyval.dval = yyvsp[0].dval;;
    break;}
case 737:
#line 4317 "lef.y"
{yyval.integer = comp_num(yyvsp[-2].dval,yyvsp[-1].integer,yyvsp[0].dval);;
    break;}
case 738:
#line 4318 "lef.y"
{yyval.integer = yyvsp[-2].dval != 0 && yyvsp[0].dval != 0;;
    break;}
case 739:
#line 4319 "lef.y"
{yyval.integer = yyvsp[-2].dval != 0 || yyvsp[0].dval != 0;;
    break;}
case 740:
#line 4320 "lef.y"
{yyval.integer = comp_str(yyvsp[-2].string,yyvsp[-1].integer,yyvsp[0].string);;
    break;}
case 741:
#line 4321 "lef.y"
{yyval.integer = yyvsp[-2].string[0] != 0 && yyvsp[0].string[0] != 0;;
    break;}
case 742:
#line 4322 "lef.y"
{yyval.integer = yyvsp[-2].string[0] != 0 || yyvsp[0].string[0] != 0;;
    break;}
case 743:
#line 4323 "lef.y"
{yyval.integer = yyvsp[-2].integer == yyvsp[0].integer;;
    break;}
case 744:
#line 4324 "lef.y"
{yyval.integer = yyvsp[-2].integer != yyvsp[0].integer;;
    break;}
case 745:
#line 4325 "lef.y"
{yyval.integer = yyvsp[-2].integer && yyvsp[0].integer;;
    break;}
case 746:
#line 4326 "lef.y"
{yyval.integer = yyvsp[-2].integer || yyvsp[0].integer;;
    break;}
case 747:
#line 4327 "lef.y"
{yyval.integer = !yyval.integer;;
    break;}
case 748:
#line 4328 "lef.y"
{yyval.integer = yyvsp[-1].integer;;
    break;}
case 749:
#line 4330 "lef.y"
{yyval.integer = (yyvsp[-4].integer != 0) ? yyvsp[-2].integer : yyvsp[0].integer;;
    break;}
case 750:
#line 4331 "lef.y"
{yyval.integer = 1;;
    break;}
case 751:
#line 4332 "lef.y"
{yyval.integer = 0;;
    break;}
case 752:
#line 4336 "lef.y"
{
 yyval.string = (char*)lefMalloc(strlen(yyvsp[-2].string)+strlen(yyvsp[0].string)+1);
 strcpy(yyval.string,yyvsp[-2].string);
 strcat(yyval.string,yyvsp[0].string);
 ;
    break;}
case 753:
#line 4342 "lef.y"
{ yyval.string = yyvsp[-1].string; ;
    break;}
case 754:
#line 4344 "lef.y"
{
 lefDefIf = TRUE;
 if (yyvsp[-4].integer != 0) {
 	yyval.string = yyvsp[-2].string;
 } else {
 	yyval.string = yyvsp[0].string;
 }
 ;
    break;}
case 755:
#line 4353 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 756:
#line 4356 "lef.y"
{yyval.integer = C_LE;;
    break;}
case 757:
#line 4357 "lef.y"
{yyval.integer = C_LT;;
    break;}
case 758:
#line 4358 "lef.y"
{yyval.integer = C_GE;;
    break;}
case 759:
#line 4359 "lef.y"
{yyval.integer = C_GT;;
    break;}
case 760:
#line 4360 "lef.y"
{yyval.integer = C_EQ;;
    break;}
case 761:
#line 4361 "lef.y"
{yyval.integer = C_NE;;
    break;}
case 762:
#line 4362 "lef.y"
{yyval.integer = C_EQ;;
    break;}
case 763:
#line 4363 "lef.y"
{yyval.integer = C_LT;;
    break;}
case 764:
#line 4364 "lef.y"
{yyval.integer = C_GT;;
    break;}
case 765:
#line 4368 "lef.y"
{ CALLBACK(lefrPropBeginCbk, lefrPropBeginCbkType, 0); ;
    break;}
case 766:
#line 4370 "lef.y"
{
 CALLBACK(lefrPropEndCbk, lefrPropEndCbkType, 0);
 lefRealNum = 0; /* just want to make sure it is reset */

 ;
    break;}
case 767:
#line 4379 "lef.y"
{ ;
    break;}
case 768:
#line 4381 "lef.y"
{ ;
    break;}
case 769:
#line 4384 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 770:
#line 4387 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "library",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrLibProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 771:
#line 4396 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 772:
#line 4399 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "componentpin",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrCompProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 773:
#line 4408 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 774:
#line 4411 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "pin",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrPinProp),
 yyvsp[-2].string,
lefPropDefType) ;
 
 ;
    break;}
case 775:
#line 4421 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 776:
#line 4424 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "macro",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrMacroProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 777:
#line 4433 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 778:
#line 4436 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "via",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrViaProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 779:
#line 4445 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 780:
#line 4448 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "viarule",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrViaRuleProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 781:
#line 4457 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 782:
#line 4460 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "layer",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrLayerProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 783:
#line 4469 "lef.y"
{lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;
    break;}
case 784:
#line 4472 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropType( &(lefrProp),
 "nondefaultrule",
yyvsp[-2].string) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 lefiPropType_setPropType( &(lefrNondefProp),
 yyvsp[-2].string,
lefPropDefType) ;
 ;
    break;}
case 785:
#line 4483 "lef.y"
{ lefRealNum = 0 ;
    break;}
case 786:
#line 4484 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropInteger( &(lefrProp) ) ;
 lefPropDefType = 'I'
 ;
    break;}
case 787:
#line 4488 "lef.y"
{ lefRealNum = 1 ;
    break;}
case 788:
#line 4489 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropReal( &(lefrProp) ) ;
 lefPropDefType = 'R';
 lefRealNum = 0;
 ;
    break;}
case 789:
#line 4495 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropString( &(lefrProp) ) ;
 lefPropDefType = 'S'
 ;
    break;}
case 790:
#line 4500 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropQString( &(lefrProp),
 yyvsp[0].string) ;
 lefPropDefType = 'Q'
 ;
    break;}
case 791:
#line 4506 "lef.y"
{
 if (lefrPropCbk) lefiProp_setPropNameMapString( &(lefrProp),
 yyvsp[0].string) ;
 lefPropDefType = 'S'
 ;
    break;}
case 792:
#line 4516 "lef.y"
{
 ;
    break;}
case 793:
#line 4519 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_setSpacingRange( &(lefrLayer),
 yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 795:
#line 4527 "lef.y"
{
 if (lefrLayerCbk) {
 lefiLayer_setSpacingLength( &(lefrLayer),
 yyvsp[0].dval) ;
 }
 ;
    break;}
case 796:
#line 4534 "lef.y"
{
 if (lefrLayerCbk) {
 lefiLayer_setSpacingLength( &(lefrLayer),
 yyvsp[-3].dval) ;
 lefiLayer_setSpacingLengthRange( &(lefrLayer),
 yyvsp[-1].dval,
yyvsp[0].dval) ;
 }
 ;
    break;}
case 797:
#line 4547 "lef.y"
{ ;
    break;}
case 798:
#line 4549 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_setSpacingRangeUseLength( &(lefrLayer) ) ;
 ;
    break;}
case 799:
#line 4554 "lef.y"
{
 if (lefrLayerCbk) {
 lefiLayer_setSpacingRangeInfluence( &(lefrLayer),
 yyvsp[0].dval) ;
 lefiLayer_setSpacingRangeInfluenceRange( &(lefrLayer),
 -1,
-1) ;
 }
 ;
    break;}
case 800:
#line 4564 "lef.y"
{
 if (lefrLayerCbk) {
 lefiLayer_setSpacingRangeInfluence( &(lefrLayer),
 yyvsp[-3].dval) ;
 lefiLayer_setSpacingRangeInfluenceRange( &(lefrLayer),
 yyvsp[-1].dval,
yyvsp[0].dval) ;
 }
 ;
    break;}
case 801:
#line 4574 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_setSpacingRangeRange( &(lefrLayer),
 yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 802:
#line 4584 "lef.y"
{ ;
    break;}
case 803:
#line 4586 "lef.y"
{ lefiProp_setRange( &(lefrProp),
 yyvsp[-1].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 804:
#line 4594 "lef.y"
{ ;
    break;}
case 805:
#line 4596 "lef.y"
{ if (lefrPropCbk) lefiProp_setNumber( &(lefrProp),
 yyvsp[0].dval) ;
 ;
    break;}
case 807:
#line 4603 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 808:
#line 4604 "lef.y"
{
 if (lefrLayerCbk)
 lefiLayer_setSpacingName( &(lefrLayer),
 yyvsp[0].string) ;
 ;
    break;}
case 809:
#line 4610 "lef.y"
{
 if (versionNum < 5.5) {
 yyerror("ADJACENTCUTS is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingAdjacent( &(lefrLayer),
 yyvsp[-2].dval,
yyvsp[0].dval) ;
 ;
    break;}
case 810:
#line 4624 "lef.y"
{ yyval.string = 0; ;
    break;}
case 811:
#line 4625 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 812:
#line 4626 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 813:
#line 4631 "lef.y"
{lefDumbMode = 1; lefNoNum = 1; ;
    break;}
case 814:
#line 4632 "lef.y"
{ yyval.string = yyvsp[0].string; ;
    break;}
case 815:
#line 4637 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrNoiseMarginCbk) {
 lefrNoiseMargin.low = yyvsp[-2].dval;
 lefrNoiseMargin.high = yyvsp[-1].dval;
 }
 CALLBACK(lefrNoiseMarginCbk, lefrNoiseMarginCbkType, &lefrNoiseMargin);
 } else
 yywarning(
 "UNIVERSALNOISEMARGIN is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 816:
#line 4650 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrEdgeRateThreshold1Cbk) {
 CALLBACK(lefrEdgeRateThreshold1Cbk,
 lefrEdgeRateThreshold1CbkType, yyvsp[-1].dval);
 }
 } else
 yywarning("EDGERATETHRESHOLD1 is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 817:
#line 4661 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrEdgeRateThreshold2Cbk) {
 	 CALLBACK(lefrEdgeRateThreshold2Cbk,
 	 lefrEdgeRateThreshold2CbkType, yyvsp[-1].dval);
 }
 } else
 yywarning("EDGERATETHRESHOLD2 is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 818:
#line 4672 "lef.y"
{
 if (versionNum < 5.4) {
 if (lefrEdgeRateScaleFactorCbk) {
 	 CALLBACK(lefrEdgeRateScaleFactorCbk,
 	 lefrEdgeRateScaleFactorCbkType, yyvsp[-1].dval);
 }
 } else
 yywarning(
 "EDGERATESCALEFACTOR is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 819:
#line 4684 "lef.y"
{ if (lefrNoiseTableCbk) lefiNoiseTable_setup( &(lefrNoiseTable),
 (int)yyvsp[0].dval) ;
 ;
    break;}
case 820:
#line 4688 "lef.y"
{ ;
    break;}
case 821:
#line 4692 "lef.y"
{
 if (versionNum < 5.4) {
 CALLBACK(lefrNoiseTableCbk, lefrNoiseTableCbkType, &lefrNoiseTable);
 } else
 yywarning("NOISETABLE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 824:
#line 4707 "lef.y"
{ if (lefrNoiseTableCbk)
 {
 lefiNoiseTable_newEdge( &(lefrNoiseTable) ) ;
 lefiNoiseTable_addEdge( &(lefrNoiseTable),
 yyvsp[-1].dval) ;
 }
 ;
    break;}
case 825:
#line 4715 "lef.y"
{ ;
    break;}
case 826:
#line 4718 "lef.y"
{ if (lefrNoiseTableCbk) lefiNoiseTable_addResistance( &(lefrNoiseTable) ) ;
 ;
    break;}
case 828:
#line 4725 "lef.y"
{ if (lefrNoiseTableCbk)
 lefiNoiseTable_addResistanceNumber( &(lefrNoiseTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 829:
#line 4730 "lef.y"
{ if (lefrNoiseTableCbk)
 lefiNoiseTable_addResistanceNumber( &(lefrNoiseTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 832:
#line 4741 "lef.y"
{ if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimLength( &(	lefrNoiseTable),
 yyvsp[-1].dval) ;
 ;
    break;}
case 833:
#line 4746 "lef.y"
{ ;
    break;}
case 834:
#line 4750 "lef.y"
{ if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimNoise( &(lefrNoiseTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 835:
#line 4755 "lef.y"
{ if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimNoise( &(lefrNoiseTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 836:
#line 4761 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_setup( &(lefrCorrectionTable),
 (int)yyvsp[-1].dval) ;
 ;
    break;}
case 837:
#line 4766 "lef.y"
{ ;
    break;}
case 838:
#line 4770 "lef.y"
{
 if (versionNum < 5.4) {
 CALLBACK(lefrCorrectionTableCbk, lefrCorrectionTableCbkType,
 &lefrCorrectionTable);
 } else
 yywarning("CORRECTIONTABLE is obsolete in 5.4. It will be ignored.");
 ;
    break;}
case 841:
#line 4785 "lef.y"
{ if (lefrCorrectionTableCbk)
 {
 lefiCorrectionTable_newEdge( &(lefrCorrectionTable) ) ;
 lefiCorrectionTable_addEdge( &(lefrCorrectionTable),
 yyvsp[-1].dval) ;
 }
 ;
    break;}
case 842:
#line 4793 "lef.y"
{ ;
    break;}
case 843:
#line 4796 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistance( &(lefrCorrectionTable) ) ;
 ;
    break;}
case 844:
#line 4800 "lef.y"
{ ;
    break;}
case 845:
#line 4804 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistanceNumber( &(lefrCorrectionTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 846:
#line 4809 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistanceNumber( &(lefrCorrectionTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 849:
#line 4821 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimLength( &(lefrCorrectionTable),
 yyvsp[-1].dval) ;
 ;
    break;}
case 850:
#line 4826 "lef.y"
{ ;
    break;}
case 851:
#line 4830 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimCorrection( &(	lefrCorrectionTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 852:
#line 4835 "lef.y"
{ if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimCorrection( &(	lefrCorrectionTable),
 yyvsp[0].dval) ;
 ;
    break;}
case 853:
#line 4844 "lef.y"
{ /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 yyerror("INPUTPINANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 CALLBACK(lefrInputAntennaCbk, lefrInputAntennaCbkType, yyvsp[-1].dval);
 ;
    break;}
case 854:
#line 4862 "lef.y"
{ /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 yyerror("OUTPUTPINANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 CALLBACK(lefrOutputAntennaCbk, lefrOutputAntennaCbkType, yyvsp[-1].dval);
 ;
    break;}
case 855:
#line 4880 "lef.y"
{ /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 yyerror("INOUTPINANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 CALLBACK(lefrInoutAntennaCbk, lefrInoutAntennaCbkType, yyvsp[-1].dval);
 ;
    break;}
case 856:
#line 4898 "lef.y"
{ /* 5.4 syntax */

 /* 11/12/2002 - this is obsoleted in 5.5, suppose should be ingored */

 /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 CALLBACK(lefrAntennaInputCbk, lefrAntennaInputCbkType, yyvsp[-1].dval);
 ;
    break;}
case 857:
#line 4919 "lef.y"
{ /* 5.4 syntax */

 /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */

 /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 CALLBACK(lefrAntennaInoutCbk, lefrAntennaInoutCbkType, yyvsp[-1].dval);
 ;
    break;}
case 858:
#line 4940 "lef.y"
{ /* 5.4 syntax */

 /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */

 /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
 CHKERR();
 } else if (use5_3) {
 yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 CALLBACK(lefrAntennaOutputCbk, lefrAntennaOutputCbkType, yyvsp[-1].dval);
 ;
    break;}
case 859:
#line 4975 "lef.y"
{ ;
    break;}
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
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

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 4977 "lef.y"

 
