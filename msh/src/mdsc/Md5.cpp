/* ATHENA DESIGN SYSTEMS, 2005.*/

/*============================================================================
 * FILE: md5.c
 *============================================================================
 * This is a vanilla implementation of the MD5 Message-Digest algorithm
 * as defined in RFC 1321.
 *
 * It compiles both under C & C++ (and it actually works in both cases! :-)
 * It runs fine both on small and big endian systems.
 *
 * See file "md5.man" for more information.
 *============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Md5.h"

/*============================================================================
 * MISC DEFINITIONS
 *============================================================================
 */

/*
 * MASK32: 32 bits mask
 */
#define MASK32  (0xFFFFFFFF)

/*
 * F,G,H,I : these are the functions defined in RFC 1321. Here they
 * are implemented as macros for efficiency.
 */
#define F(x,y,z)        ( ((x)&(y)) | ((~(x))&(z)) )
#define G(x,y,z)        ( ((x)&(z)) | ((~(z))&(y)) )
#define H(x,y,z)        ( (x) ^ (y) ^ (z) )
#define I(x,y,z)        ( (y) ^ ((x) | (~(z))) )

/*
 * ROTATE_LEFT: Circular shifting of a 32 bit value.
 * NOTE: I do an "&" operation with MASK32 before shifting right,
 * so that this works even if "x" has a size larger than 32!
 * (just in case you happen to have such a system!)
 */
#define ROTATE_LEFT(x,n)        ( ((x)<<n) | (((x)&MASK32) >> (32-(n))) )

/*
 * ROUND1/2/3/4 : These are the 4 "rounds" of manipulating the
 * registers as defined in RFC1321.
 */
#define ROUND1(a, b, c, d, x, s, i) { \
            (a) = (a) + F((b),(c),(d)) + (x) + T[(i)]; \
            (a) = ROTATE_LEFT((a), (s)); \
            (a) += (b); }
#define ROUND2(a, b, c, d, x, s, i) { \
            (a) = (a) + G((b),(c),(d)) + (x) + T[(i)]; \
            (a) = ROTATE_LEFT((a), (s)); \
            (a) += (b); }
#define ROUND3(a, b, c, d, x, s, i) { \
            (a) = (a) + H((b),(c),(d)) + (x) + T[(i)]; \
            (a) = ROTATE_LEFT((a), (s)); \
            (a) += (b); }
#define ROUND4(a, b, c, d, x, s, i) { \
            (a) = (a) + I((b),(c),(d)) + (x) + T[(i)]; \
            (a) = ROTATE_LEFT((a), (s)); \
            (a) += (b); }


/*
 * BYTES_TO_WORD: given a char buffer containing 4 bytes, generate
 * the equivalent 32bit word (low order byte first)
 */
#define BYTES_TO_WORD(b) (((unsigned char*)b)[0]) \
        | ( (((unsigned char*)b)[1]) << 8 ) \
        | ( (((unsigned char*)b)[2]) << 16 ) \
        | ( (((unsigned char*)b)[3]) << 24 )

/*
 * SHOWSTATE: Used for debugging - Normally turned off!
 */
#define SHOWSTATE(a,b,c,d,str)

/*============================================================================
 * TYPE DEFINITIONS
 *============================================================================
 */

/*
 * MyUInt32: this should be an unsigned integer at least 32 bits long.
 * NOTE: It is ok if it is longer than that! (the code is carefully written
 * so that this is not a problem!)
 */
typedef unsigned int MyUInt32;
/* typedef unsigned long long MyUInt32; */ /* only for debugging/testing */


/*
 * MyState: This is the structure that keeps our current state.
 * It is created when "md5_init" is called and is passed to the succesive
 * calls to "md5_update" and "md5_end".
 */
typedef struct {
    MyUInt32 regs[4];           /* Our A,B,C,D registers */
    unsigned char buf[64];      /* Keep the most recent data bytes */
    int nbuf;                   /* num. of bytes in "buf" */

    MyUInt32 total_bytes_hi;
    MyUInt32 total_bytes_lo;    /* count number of *bytes* (not bits)
                                 * processed so far. This is supposed to
                                 * be a 64 bit value, so I use 2 integers.
                                 */
} MyState;


/*============================================================================
 * VARIABLES LOCAL TO THIS FILE
 *============================================================================
 */

/*
 * This is the "T" table (see RFC 1321 for more details).
 * It is defined as:
 *      T[i] = integer_part_of( 4294967296 * fabs(sin(i)) )
 *      for i=1..64
 */
static MyUInt32 T[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

/*============================================================================
 * FUNCTIONS LOCAL TO THIS FILE
 *============================================================================
 */
static void process16(MyState *state, MyUInt32 *x);

/*----------------------------------------------------------------------------
 * process16
 *----------------------------------------------------------------------------
 * This is the heart of the algorithm. It process input data (plus
 * whatever padding the algorithm requires) in chunks of 16 32bit integers.
 *
 * Arguments:
 *      state:  our state (this includes the 4 registers which hold
 *              the result of the checksum's computation) and the total
 *              number of bytes processed.
 *----------------------------------------------------------------------------
 */
static void process16(MyState *state, MyUInt32 *x)
{
    MyUInt32 a, b, c, d;

    a = state->regs[0];
    b = state->regs[1];
    c = state->regs[2];
    d = state->regs[3];

    SHOWSTATE(x[0], x[1], x[2], x[3], "DATA0-3");
    SHOWSTATE(x[4], x[5], x[6], x[7], "DATA4-3");
    SHOWSTATE(x[8], x[9], x[10], x[11], "DATA8-3");
    SHOWSTATE(x[12], x[13], x[14], x[15], "DATA12-3");
    SHOWSTATE(a,b,c,d, "Before");


    SHOWSTATE(a,b,c,d, "Before");
    ROUND1(a,b,c,d,x[0],7,1-1);
    ROUND1(d,a,b,c,x[1],12,2-1);
    ROUND1(c,d,a,b,x[2],17,3-1);
    ROUND1(b,c,d,a,x[3],22,4-1);
    SHOWSTATE(a,b,c,d, "X1");
    ROUND1(a,b,c,d,x[4],7,5-1);
    ROUND1(d,a,b,c,x[5],12,6-1);
    ROUND1(c,d,a,b,x[6],17,7-1);
    ROUND1(b,c,d,a,x[7],22,8-1);
    SHOWSTATE(a,b,c,d, "X2");
    ROUND1(a,b,c,d,x[8],7,9-1);
    ROUND1(d,a,b,c,x[9],12,10-1);
    ROUND1(c,d,a,b,x[10],17,11-1);
    ROUND1(b,c,d,a,x[11],22,12-1);
    SHOWSTATE(a,b,c,d, "X3");
    ROUND1(a,b,c,d,x[12],7,13-1);
    ROUND1(d,a,b,c,x[13],12,14-1);
    ROUND1(c,d,a,b,x[14],17,15-1);
    ROUND1(b,c,d,a,x[15],22,16-1);

    SHOWSTATE(a,b,c,d, "AfterRound1");

    ROUND2(a,b,c,d,x[1],5,17-1);
    ROUND2(d,a,b,c,x[6],9,18-1);
    ROUND2(c,d,a,b,x[11],14,19-1);
    ROUND2(b,c,d,a,x[0],20,20-1);
    ROUND2(a,b,c,d,x[5],5,21-1);
    ROUND2(d,a,b,c,x[10],9,22-1);
    ROUND2(c,d,a,b,x[15],14,23-1);
    ROUND2(b,c,d,a,x[4],20,24-1);
    ROUND2(a,b,c,d,x[9],5,25-1);
    ROUND2(d,a,b,c,x[14],9,26-1);
    ROUND2(c,d,a,b,x[3],14,27-1);
    ROUND2(b,c,d,a,x[8],20,28-1);
    ROUND2(a,b,c,d,x[13],5,29-1);
    ROUND2(d,a,b,c,x[2],9,30-1);
    ROUND2(c,d,a,b,x[7],14,31-1);
    ROUND2(b,c,d,a,x[12],20,32-1);

    SHOWSTATE(a,b,c,d, "AfterRound2");

    ROUND3(a,b,c,d,x[5],4,33-1);
    ROUND3(d,a,b,c,x[8],11,34-1);
    ROUND3(c,d,a,b,x[11],16,35-1);
    ROUND3(b,c,d,a,x[14],23,36-1);
    ROUND3(a,b,c,d,x[1],4,37-1);
    ROUND3(d,a,b,c,x[4],11,38-1);
    ROUND3(c,d,a,b,x[7],16,39-1);
    ROUND3(b,c,d,a,x[10],23,40-1);
    ROUND3(a,b,c,d,x[13],4,41-1);
    ROUND3(d,a,b,c,x[0],11,42-1);
    ROUND3(c,d,a,b,x[3],16,43-1);
    ROUND3(b,c,d,a,x[6],23,44-1);
    ROUND3(a,b,c,d,x[9],4,45-1);
    ROUND3(d,a,b,c,x[12],11,46-1);
    ROUND3(c,d,a,b,x[15],16,47-1);
    ROUND3(b,c,d,a,x[2],23,48-1);

    SHOWSTATE(a,b,c,d, "AfterRound3");

    ROUND4(a,b,c,d,x[0],6,49-1);
    ROUND4(d,a,b,c,x[7],10,50-1);
    ROUND4(c,d,a,b,x[14],15,51-1);
    ROUND4(b,c,d,a,x[5],21,52-1);
    ROUND4(a,b,c,d,x[12],6,53-1);
    ROUND4(d,a,b,c,x[3],10,54-1);
    ROUND4(c,d,a,b,x[10],15,55-1);
    ROUND4(b,c,d,a,x[1],21,56-1);
    ROUND4(a,b,c,d,x[8],6,57-1);
    ROUND4(d,a,b,c,x[15],10,58-1);
    ROUND4(c,d,a,b,x[6],15,59-1);
    ROUND4(b,c,d,a,x[13],21,60-1);
    ROUND4(a,b,c,d,x[4],6,61-1);
    ROUND4(d,a,b,c,x[11],10,62-1);
    ROUND4(c,d,a,b,x[2],15,63-1);
    ROUND4(b,c,d,a,x[9],21,64-1);

    SHOWSTATE(a,b,c,d, "AfterRound4");

    state->regs[0] += a;
    state->regs[1] += b;
    state->regs[2] += c;
    state->regs[3] += d;

    SHOWSTATE(a,b,c,d, "AfterAdd");
}

/*----------------------------------------------------------------------------
 * word_to_bytes
 *----------------------------------------------------------------------------
 * Given a 32 bit word, store it in a buffer of 4 characters (low order
 * byte first)
 *
 * Arguments:
 *      x : the 32bit word
 *      b : the buffer where we store the result (must have a size >= 4)
 *----------------------------------------------------------------------------
 */
void word_to_bytes(MyUInt32 x, unsigned char *b)
{
    b[0] = x & 0xff; x = (x>> 8);
    b[1] = x & 0xff; x = (x>> 8);
    b[2] = x & 0xff; x = (x>> 8);
    b[3] = x & 0xff;
}

/*----------------------------------------------------------------------------
 * md5_init             -- PUBLIC
 *----------------------------------------------------------------------------
 * Initialize the checksum calculations.
 * We allocate a handler that should be passed to "md5_update" and
 * "md5_end". This handler is casted to (void *) to hide any implementation
 * details from the caller.
 *
 * Returns:
 *      A handler (casted to void *) or NULL if we run out of memory
 *----------------------------------------------------------------------------
 */
void *md5_init()
{
    unsigned char buf[4];
    MyState *state;

    /*
     * Sanity check! Make sure that we have at least 32 bits!
     */
    if (sizeof(MyUInt32) < 32/8) {
        fprintf(stderr, "ERROR: md5_init MyUInt32 is less than 32 bits\n");
        fprintf(stderr, "       You must change the corresponding typedef\n");
        fprintf(stderr, "       and recompile!!!!\n");
        exit(13);
    }
    state = (MyState *)malloc(sizeof(MyState));
    if (state == NULL) {
        return NULL;
    }

    state->regs[0] = 0x67452301;
    state->regs[1] = 0xefcdab89;
    state->regs[2] = 0x98badcfe;
    state->regs[3] = 0x10325476;

    state->nbuf = 0;
    state->total_bytes_lo = 0;
    state->total_bytes_hi = 0;

    return (void *)state;
}

/*----------------------------------------------------------------------------
 * md5_update
 *----------------------------------------------------------------------------
 * This function is called to give more data bytes to the md5sum calculation
 * algorithm.
 *
 * Arguments:
 *      handle: The handle that was returned by "md5_init"
 *      buf   : the buffer containing the data bytes
 *      buflen: number of bytes in data buffer
 *----------------------------------------------------------------------------
 */
void md5_update(void *handle, const char *buf, int buflen)
{
    int count;
    int i;
    MyUInt32 x[16];
    MyState *state;

    /*
     * Retrieve our current state
     */
    state = (MyState *)handle;
    
    /*
     * Increment the total number of bytes processed so far
     * by "buflen".
     * NOTE: The result is stored in 2 integers.
     * NOTE2: Make sure that this works even if the integers are > 32 bits!
     */
    state->total_bytes_lo = (state->total_bytes_lo + buflen) & MASK32;
    if (state->total_bytes_lo < buflen) {
        state->total_bytes_hi += 1;
    }

    /*
     * Process all input bytes
     */
    for (;;) {
        int n = 64 - state->nbuf;
        if (buflen < n) {
            /*
             * Not enough bytes to fill in the 64byte buffer.
             * Just copy them and exit the loop
             */
            memcpy(state->buf + state->nbuf, buf, buflen);
            state->nbuf += buflen;
            break;
        } else {
            /*
             * We have enough bytes to fill in our 64 byte buffer.
             * Copy them, process them, and update "buf" and "buflen"
             * accordingly and loop again.
             */
            memcpy(state->buf + state->nbuf, buf, n);
            for (i=0; i<16; i++) {
                x[i] = BYTES_TO_WORD(state->buf + 4*i);
            }
            process16(state, x);
            state->nbuf = 0;
            buf += n;
            buflen -= n;
        }
    }
}

/*----------------------------------------------------------------------------
 * md5_end -- PUBLIC
 *----------------------------------------------------------------------------
 * This should be called when we finished reading data bytes in order
 * to calculate the final checksum
 *
 * Arguments:
 *      handle: The handle that was returned by "md5_init"
 *      result: This must be an array with (at least) 4 unsinged ints.
 *              The final MD5 checksum will be stored there.
 *              The first byte of the checksum will be te low order
 *              byte of result[0] and the last the high order byte
 *              of result[4].
 *----------------------------------------------------------------------------
 */
void md5_end(void *handle, unsigned int *result)
{
    //uint64 datalen;
    MyUInt32 datalen_lo, datalen_hi;
    int tmp, npad;
    char b;
    char buf[8];
    MyState *state = (MyState *)handle;

    /*
     * Remember total number of data bytes read so far.
     */
    datalen_lo = state->total_bytes_lo;
    datalen_hi = state->total_bytes_hi;
    
    /*
     * For sure we must pad with at least one
     * byte, the byte 1000000 = 0x80
     */
    b = 0x80;
    md5_update(state, &b, 1);
    
    /*
     * Then we must pad with extra zero bytes so that
     * the total byte length (data bytes read so far + pading bytes)
     * must be congruent to 56 modulo 64
     * (i.e. the number of bits must be congruent to 448 mod 512)
     */
    b = 0;
    while (state->total_bytes_lo % 64 != 56) {
        md5_update(state, &b, 1);
    }

    /*
     * Now add the number of *bits* (not bytes!)
     * So we have to multiply by 8!
     */
    datalen_hi = datalen_hi << 3 | ((datalen_lo & MASK32) >> 29);
    datalen_lo = (datalen_lo << 3) & MASK32;

    /*
     * Add these 64 bits (low order first) to the padding...
     */
    word_to_bytes(datalen_lo, (unsigned char *)buf);
    md5_update(state, buf, 4);
    word_to_bytes(datalen_hi, (unsigned char *)buf);
    md5_update(state, buf, 4);

    /*
     * Store the result
     */
    result[0] = (unsigned int) state->regs[0];
    result[1] = (unsigned int) state->regs[1];
    result[2] = (unsigned int) state->regs[2];
    result[3] = (unsigned int) state->regs[3];

    /*
     * Destroy the handle
     */
    memset(handle, 0xff, sizeof(MyState));
    free(handle);
}


/*----------------------------------------------------------------------------
 * md5_result_to_string -- PUBLIC
 *----------------------------------------------------------------------------
 * Given the md5 checksum as an array of 4 unsigned ints, calculate
 * its string representation (just the 4 ints printed in hex)
 *
 * Arguments:
 *      md5sum: the array of 4 unsigned ints
 *      buf   : the buffer where the result will be stored.
 *              This must be at least 33 bytes long (32 hex digits + the
 *              null)
 *----------------------------------------------------------------------------
 */
void md5_result_to_string(const unsigned int *md5sum, char *buf)
{
    int i, j;
    char tmp[10];

    buf[0] = '\0';
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            int byte = (md5sum[i] >> (j*8)) & 0xff;
            sprintf(tmp, "%02x", byte);
            strcat(buf, tmp);
        }
    }
}

/*----------------------------------------------------------------------------
 * md5_file
 *----------------------------------------------------------------------------
 * Calculate the checksum of a file given its name
 *----------------------------------------------------------------------------
 */
int md5_file(const char *fname, unsigned int *result)
{
    FILE *fp;
    bool must_close;

    /*
     * Open file. If "fname" is NULL use stdin
     */
    if (fname == NULL) {
        fp = stdin;
        must_close = false;
    } else {
        fp = fopen(fname, "r");
        if (fp == NULL) {
            return -1;
        }
        must_close = true;
    }
    
    int retval = md5_fp(fp, result);

    if (must_close) {
        fclose(fp);
    }

    return retval;
}

/*----------------------------------------------------------------------------
 * md5_fp
 *----------------------------------------------------------------------------
 * Calculate the checksum of a file given an open file descriptor
 *----------------------------------------------------------------------------
 */
int md5_fp(FILE *fp, unsigned int *result)
{
    void *handle;
    char buf[100*1024];
    int n;
    unsigned int dummy[4];

    /*
     * Initialize handle
     */
    handle = md5_init();

    /*
     * Read file one buffer at the time
     */
    for (;;) {
        n = fread(buf, (size_t)1, sizeof(buf), fp);
        if (n == 0) {
            /*
             * EOF reached
             */
            break;
        }
        if (n < 0) {
            /*
             * I/O error. Just cleanup & return
             */
            fclose(fp);
            md5_end(handle, dummy);
            return(-1);
        }
        /*
         * OK, process the new data
         */
        md5_update(handle, buf, n);
    }

    md5_end(handle, result);
    return(0);
}


/*----------------------------------------------------------------------------
 * md5_string
 *----------------------------------------------------------------------------
 * Calculate the md5 checksum of the given string.
 *
 * Arguments:
 *      str: the string
 *      result: this is where the result will be stored.
 *----------------------------------------------------------------------------
 */
void md5_string(const char *str, unsigned int *result)
{
    void *handle;

    handle = md5_init();
    md5_update(handle, str, strlen(str));
    md5_end(handle, result);
}

/*============================================================================
 * STANDALONE EXECUTION
 *
 * If "STANDALONE" is #defined, then this program can be compiled
 * as a standalone program.
 * If given the argument "-test1" runs a testsuite to make sure that
 * everything is ok.
 * Else, it treats each command line argument as a file and prints
 * its checksum. The argument "-" means used stdin.
 *============================================================================
 */
/*#define STANDALONE*/
#ifdef STANDALONE

typedef struct {
    char *msg;
    char *md5sum;
} TestCase;

static TestCase TestCaseTable[] = {
    {"", "d41d8cd98f00b204e9800998ecf8427e"},
    {"a", "0cc175b9c0f1b6a831c399e269772661"},
    {"abc", "900150983cd24fb0d6963f7d28e17f72"},
    {"message digest", "f96b697d7cb7938d525a2f31aaf161d0"},
    {"abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b"},
    {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            "d174ab98d277d9f5a5611c2c9f419d9f"},
    {"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "57edf4a22be3c955ac49da2e2107b67a"},
    
};

static void test_suite1()
{
    int ncases = sizeof(TestCaseTable) / sizeof(TestCaseTable[0]);
    int i;
    unsigned int md5sum[4];
    char buf[100];

    printf("RUNNING TEST SUITE\n");
    for (i=0; i<ncases; i++) {
        md5_string(TestCaseTable[i].msg, md5sum);
        md5_result_to_string(md5sum, buf);
        printf("MD5(%s) = %s\n", TestCaseTable[i].msg, buf);
        if (strcmp(buf, TestCaseTable[i].md5sum)) {
            fprintf(stderr, "ERROR: CHECKSUM SHOULD HAVE BEEN: %s\n",
                TestCaseTable[i].md5sum);
            exit(1);
        }
    }
    printf("%d TEST CASES OK!\n", ncases);
}

main(int argc, char *argv[])
{
    int i;


    for (i=1; i<argc; i++) {
        if (!strcmp(argv[i], "-test1")) {
            test_suite1();
        } else {
            char buf[33];
            unsigned int result[4];
            char *fname;
            fname = argv[i];
            if (!strcmp(fname, "-")) {
                fname = NULL;
            }
            if (md5_file(fname, result)) {
                fprintf(stderr, "I/O Error in \"%s\": %s\n",
                        argv[i], strerror(errno));
                exit(1);
            }
            md5_result_to_string(result, buf);
            printf("%s", buf);
            if (fname) {
                printf(" %s\n", argv[i]);
            } else {
                printf(" -\n");
            }
        }
    }

    exit(0);
}

#endif /* STANDALONE */
