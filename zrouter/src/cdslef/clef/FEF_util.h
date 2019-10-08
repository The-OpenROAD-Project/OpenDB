/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.5, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */


#ifndef FEF_util
#define FEF_util

/* #include <iostream.h> */
#include <stdlib.h>
#include <stdio.h>

/* FEF_util.h : utility routines */

#ifdef WIN32
#	define MAXPATHLEN 256	/* really defined in <sys/param.h> */
#endif

/* note these are compatible with Valid/edb orientation, and NOT
 * compatible with LEF/DEF orientation.
 */
enum {	ORIENT_0,	/* 0 no transforms */
	ORIENT_90,	/* 1 rot  90 counterclockwise */
	ORIENT_180,	/* 2 rot 180 counterclockwise */
	ORIENT_270,	/* 3 rot 270 counterclockwise */
	ORIENT_F_0,	/* 4 flip (negate x) */
	ORIENT_F_90,	/* 5 flip (negate x) then rot  90 counterclockwise */
	ORIENT_F_180,	/* 6 flip (negate x) then rot 180 counterclockwise */
	ORIENT_F_270 };	/* 7 flip (negate x) then rot 270 counterclockwise */

/* names related to orientation codes */
/* extern char *fef_orient_names[]; */ /* e.g. 'N' , 'FN' etc */

/* string used in file dialog to match all files */
#ifdef WIN32
#define ALL_FILES	"All files (*.*)|*.*||"
#else
#define ALL_FILES	"All files (*)|*||"
#endif

#ifndef NINT
#define NINT(x) ( (int)((x) >= 0 ? (x)+0.5 : (x)-0.5 ) )
#endif

#ifndef ABS
#define ABS(x) ( (x) < 0 ? -(x) : (x) )
#endif

#ifndef MAX
#define MAX(x,y) ((x) > (y)? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) < (y)? (x) : (y))
#endif

#ifndef Streq
#define Streq(s1,s2)  (!strcmp( (s1), (s2) ))
#endif

/* Random number macros
 *   the fact that we are getting only 15 bit random numbers from rand()
 * limits the utility of these macros.  We are using rand() rather than
 * random() since the WIN32 doesnot support random().  Note if anyone tries
 * to make a 32 bit tool out of these, it will be a good idea to change the
 * "(float)"s to "(double)"s.
 *   Also, nothing is done here to deal with the very poor randomness of
 * the low order bits of rand().
 */

#ifndef SEED_GENERATOR
#define SEED_GENERATOR(seed) (srand((seed)))
#endif

#ifndef  RAND_MAX
# define RAND_MAX	0x7fff	/* max value returned by rand() */
#endif

#ifndef RANDOM_FRACTION
#define RANDOM_FRACTION() \
    ( ((float) rand()) * ((float) 1.0 / (float) RAND_MAX) )
#endif

#ifndef RANDOM_FLOAT
#define RANDOM_FLOAT(lo,hi) \
    ( ( RANDOM_FRACTION() * \
	  ( (float) (hi) - (float) (lo) ) ) + (float) (lo) )
#endif

#ifndef RANDOM_INTEGER
#define RANDOM_INTEGER(lo,hi) \
    ( (int) floor(RANDOM_FRACTION()*(((hi)-(lo))+ (float) 0.999999))+(lo) )
#endif
			


#ifndef IN_INCLUSIVE_RANGE
#define IN_INCLUSIVE_RANGE( value, lo, hi ) \
    ( (value) >= (lo)  &&  (value) <= (hi) )
#endif

/* Return an integer percentage (0-100) of a part relative to a whole */
#ifndef PERCENT
#define PERCENT( part, whole ) \
    int( \
         ( \
           ( float((part)) / float((whole)) ) \
	   * float(100.0) \
	 ) \
	 + float(0.5) \
       )
#endif
							     
/* Given percent as an integer (0-100), return the corresponding portion
 * of the whole, rounding off to the nearest integer. */
#ifndef PERCENT_OF
#define PERCENT_OF( percent, whole ) \
    int( \
         ( \
           ( \
             float((percent)) / float(100.0) \
	   ) \
	   * float((whole)) \
	 ) + float(0.5) \
       )
#endif

/* map from 2D to 1D */
#define ARR(sizearray,i,j)  ((j)*(sizearray)+(i))

/* basic word sizes etc */
#define IntSize 		4
#define ShortSize		2

#define MAX_UNSIGNED_3BYTE	0xFFFFFF
#define MAX_UNSIGNED_SHORT	0xFFFF
#define MAX_UNSIGNED_CHAR	0xFF
#define BIGDOUBLE		1e20
#define BYTE_MULT		256

extern
#ifdef _cplusplus
"C"
#endif
void lefiNerr(int);
extern
#ifdef _cplusplus
"C"
#endif
void lefiNwarn(int);

#define NERRNR -1	/* null value for nerr */
#define nerrnr NERRNR

#ifdef NOT_USED
void UTIL_structure_copy( char *target, char *source, int num_bytes);

void Util_Orientate(int orientation, int *x, int *y); /* apply "orientation" */
void UnUtil_Orientate(int orientation, int *x, int *y); /* inverse of Util_Orientate() */
void Util_Orientate(int orientation, int *x, int *y, int originx, int originy);
  /* *x and *y start out relative to origin, then are absolute at end */

void swap( int *i, int *j);
#endif

#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef _WINNT_
typedef unsigned char BOOLEAN;
#endif

/* the number of wasted bytes each alloc */
/* note: we really need one of these options ! */
#ifndef MALLOC_OVERHEAD
#	ifdef ENERGIZE
#	define MALLOC_OVERHEAD 0 /* 27 */
#	endif
#endif
#ifndef MALLOC_OVERHEAD
#	ifndef WIN32
#	define MALLOC_OVERHEAD 0 /* 11 */
#	endif
#endif
#ifndef MALLOC_OVERHEAD
#	define MALLOC_OVERHEAD 0 /* 11 */
#endif

/* debug flags */

#define MAX_DEBUG_FLAGS 256
extern char debug_flags[MAX_DEBUG_FLAGS];
extern char global_debug;

void Initialize_debug_flags();

/* the zero flag is used to run LENGTHY checking all over the */
/*  place to check for errors */
#define EXTREME_DEBUG (debug_flags[0])

/* flag to turn off UI_parser */
#define DISABLE_PARSER (debug_flags[23])

#ifdef NOT_USED

/* global static text strings and arrays */
extern const char * const TRUEFALSE[];
extern const char * const ONOFF[];
extern const char * const fwONOFF[];
extern const char pON[];
extern const char pOFF[];
extern const char pTRUE[];
extern const char pFALSE[];


/* structure for getting at the bits in a byte */
struct byte_bits
	{
	unsigned b0 : 1;
	unsigned b1 : 1;
	unsigned b2 : 1;
	unsigned b3 : 1;
	unsigned b4 : 1;
	unsigned b5 : 1;
	unsigned b6 : 1;
	unsigned b7 : 1;
	};

/* unknown name */
#define EMPTY   "?empty?"

/* just use printf for debug if not running under windows */
/* , the trace() routine strips color codes from messages */
#ifdef TRACE
#	undef TRACE
#endif
#ifdef WIN32
#	define TRACE UI_message
#else
	void trace(char *format, ... );
#	define TRACE trace
#endif

/* textual meter functions */
void meter_init(int max_data,char *title);
void meter_update(int amount);
void meter_terminate();
extern char meter_visible;

/* sparse matrix routines */
int SparseIndex(int v1, int v2, int max);
int SizeSparse(int max);

/* routine to update min/max from a value */
void MinMax(int *low,int *high, int test);

/* a macro to do the same */
#define MINMAX(low,high,test)			\
	if((test)<(low))(low)=(test);		\
	if((test)>(high))(high)=(test);

/* handy blank string */
extern const char blankstring[];

/* count estimator function ( for compilers ) */
int EstimateNumbers(const char *type,int file_size);

/*  The search here covers from allx to aurx and ally to aury, inclusive. */
class Search2D {
  int x, y;
  int llx, lly, urx, ury;
public:
  Search2D(    int allx, int ally, int aurx, int aury);
  void Restart(int allx, int ally, int aurx, int aury);
  int operator()();
  int X() { return x; }
  int Y() { return y; }
};

/* utility unions */
union IntUnionType
	{
	int i;
	unsigned char chrs[IntSize];
	};
extern IntUnionType IntUnion;

union ShortUnionType
	{
	unsigned short s;
	unsigned char chrs[ShortSize];
	};
extern ShortUnionType ShortUnion;

/**********************************************************************
 * sort 2 coords (for getting box right side out)
 **********************************************************************/
inline void fef_sort_coords(int& lower, int& upper)
{
  int temp;
  if (lower > upper) {
    temp  = lower;
    lower = upper;
    upper = temp;
  }
} // fef_sort_coords

// limits to memory increase
void Util_memory_increase(int *size,int must_exceed);
void Util_reset_memory_jump(int new_size);

// routines to byteswap
struct int_byte_swapper
	{
	union
		{
		int i;
		char b[4];
		};
	int_byte_swapper() {}
	int_byte_swapper(int b0,int b1,int b2,int b3)
		{ b[0]=b0; b[1]=b1; b[2]=b2; b[3]=b3; }
	};
struct short_byte_swapper
	{
	union
		{
		short i;
		char b[2];
		};
	};
struct unsigned_short_byte_swapper
	{
	union
		{
		unsigned short i;
		char b[2];
		};
	};
void intbswap(int &i);
void shortbswap(short &i);
void unsigned_shortbswap(unsigned short &i);

//
// UTIL_in_test_mode --
//	checks if we are running the test in a test mode, in which case we
//	may want to limit or expand the amount of output we produce depending
//	on the test mode.
//
// Test modes:
//   test_mode
//	We are in some form of test mode.
//   regression_mode
//	We are in regression test mode, programs should avoid printing out
//	times, full pathnames, etc. that will cause unimportant differences
//	when comparing to golden files.
//   performance_mode
//	We are running a performance test.  It may be useful to output more
//	detailed (frequent) accounting of where time is spent.  It may also
//	be worthwhile to print information about memory usage.
//
enum UTIL_test_mode { test_mode, regression_mode, performance_mode };

extern int UTIL_in_test_mode( UTIL_test_mode mode = regression_mode );


// asserts
//	to use the assert package, you have to include this file and, after the
// include, say "ASRT_STRINGS;" to declare the string(s) as static variables.
//
//		#include "FEF_util.h"
//		ASRT_STRINGS();
//
// declare where the file is (if you are getting complaints about
//     asrt_strings_file_name being endefined, just say "ASRT_STRINGS();"
// in your module)

#define ASRT_STRINGS static char *asrt_strings_file_name = __FILE__

#define   asrt			ASRT
#define   asrtd			ASRTD
#define   pd			PD
#define   asrt_strings		ASRT_STRINGS

#define   ASRT(expr, nerr_nr) \
        ((expr) || asrt_handler(#expr, asrt_strings_file_name, __LINE__, \
	/**/							nerr_nr))

#ifdef DEBUG
# define ASRTD ASRT
#else
// become nothing
# define ASRTD(expr, nerr_nr)
#endif

//     note if you want to print anything (including pointers) in hex, cast
// it to unsigned.
#define PD(expr) (pd_internal(#expr, (expr)))

// support asrt, asrtd ...
extern int /*never returns*/ asrt_handler(char *expr_str, char *fn, int ln,
/**/							int nerr_nr);

extern int /*always 0*/ pd_internal(char *exprstr, int n);
extern int /*always 0*/ pd_internal(char *exprstr, unsigned u);
extern int /*always 0*/ pd_internal(char *exprstr, double d);
extern int /*always 0*/ pd_internal(char *exprstr, char *str);

// name mapper mechanism
int UTIL_setmunge(char **map,char *codes);
void UTIL_munge(char *str,char *map);

// string comparison
#ifdef WIN32
#	define UTIL_stricmp(str1,str2) _stricmp(str1,str2)
#else
#	define UTIL_stricmp(str1,str2) strcasecmp(str1,str2)
#endif

int UTIL_strcmp(char *str1,char *str2,int case_sensitive);
//     converts a database coord to a user-readable string, in user
// coordinates.  At the time of this writing, that is simply dividing
// database coords down by 100.
void UTIL_coord_to_string(char *buf, int coord);

//   determine if an integer is equal to any of the integers given in a
// variable-length list
inline int /*bool*/ fef_in(int a, int b, int c)
/**/	{ return a == b  ||  a == c; }
inline int /*bool*/ fef_in(int a, int b, int c, int d)
/**/	{ return a == b  ||  fef_in(a, c, d); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e)
/**/	{ return a == b  ||  fef_in(a, c, d, e); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e, int f)
/**/	{ return a == b  ||  fef_in(a, c, d, e, f); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e, int f, int g)
/**/	{ return a == b  ||  fef_in(a, c, d, e, f, g); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e, int f, int g,
/**/			   int h)
/**/	{ return a == b  ||  fef_in(a, c, d, e, f, g, h); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e, int f, int g,
/**/			   int h, int i)
/**/	{ return a == b  ||  fef_in(a, c, d, e, f, g, h, i); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e, int f, int g,
/**/			   int h, int i, int j)
/**/	{ return a == b  ||  fef_in(a, c, d, e, f, g, h, i, j); }
inline int /*bool*/ fef_in(int a, int b, int c, int d, int e, int f, int g,
/**/			   int h, int i, int j, int k)
/**/	{ return a == b  ||  fef_in(a, c, d, e, f, g, h, i, j, k); }

void    UTIL_fill_bytes(char *p, char byte, int count);
#define UTIL_FILL_BYTES(p, byte) \
       (UTIL_fill_bytes((char *)(p), char(byte), sizeof(*(p))))

// can evaluate at runtime, doesnot look like a constant to compiler
#ifdef DEBUG
  inline int/*bool*/ fef_debug() { return 1; }
#else
  inline int/*bool*/ fef_debug() { return 0; }
#endif

// useful for getting out of "constant in conditional" situations
inline int fef_zero() { return 0; }

// register/retrieve program name
void UI_SetProgramName(const char *given);
char *UI_ProgramName(); 

class fef_bit_vector {
private:
  int num_used, bytes_allocated;	// num_used is bits, not bytes
  char *pbits;
public:
  fef_bit_vector(int bit_count);
  ~fef_bit_vector();
  int/*bool*/ valid(int ibit) {
    return ibit >= 0  &&  ibit < this->num_used;
  }
  void set(int ibit, int value = 1) {		// write
#   if DEBUG
      if (!valid(ibit)) lefiNerr(678);
#   endif
    if (value) pbits[ibit >> 3] |=  (1 << (ibit & 7));
    else       pbits[ibit >> 3] &= ~(1 << (ibit & 7));
  }
  int/*bool*/ operator()(int ibit) {			// read
#   if DEBUG
      if (!valid(ibit)) lefiNerr(677);
#   endif
    return !!(pbits[ibit >> 3]  &  (1 << (ibit & 7)));
  }
  int read(int ibit) {		// redundant with ()
    return (*this)(ibit);
  }
}; // fef_bit_vector

// Tests format string for OKness.  Put here since it does not use any
// other routines, and Property and bspf use it, and different programs use
// one and not the other
int synth_rc_format_string_OK(const char *str);

// class for executing a program
class Exe
{
  public:
    Exe( char* exe_name ); // throw( char* )
    ~Exe();

    int run( char* args, ostream& out_stream );
    int run( char* args);
    
  private:
    char*	name_;
    char*	args_;
    FILE*	r_pipe_;
};


class stats {
private:
   double low;
   double high;
   double sum;
   double sum2;		/* sum of the squares */
   int N;
public:
   int how_many() { return N;}
   double average() { return N > 0 ? sum/N : 0.0;}
   double highest() { return high;}
   double lowest() { return low;}
   void init_stat_entry();
   void update_stat_entry(double x);
   void compute_stat(double *avg, double *low, double *high, double *std_dev);
   void fake_state(int Nsamp, double avg, double std_dev);
   };
 
int UTIL_ceil(int p, int q);
int UTIL_floor(int p, int q);

char *UTIL_skip_white(   char *start_string);
char *UTIL_skip_nonwhite(char *start_string); 

int UTIL_getpid();
#endif

#endif
