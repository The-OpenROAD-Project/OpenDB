///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <stdarg.h>
#ifndef _WIN32
#include "zlib.h"
#endif
#include "xf.h"

#define AF_COMP_GZ 0xfedc
#define AF_UNCOMP 0xcdef
#define AF_ENCRYPT 0xcddc

typedef struct
{
    int type;
    FILE *f;
#ifndef _WIN32
    gzFile gzf;
#endif
} AFILE;

#ifndef _WIN32
typedef struct gz_stream {
    z_stream stream;
    int      z_err;   /* error code for last stream operation */
    int      z_eof;   /* set if end of input file */
    FILE     *file;   /* .gz file */
    Byte     *inbuf;  /* input buffer */
    Byte     *outbuf; /* output buffer */
    uLong    crc;     /* crc32 of uncompressed data */
    char     *msg;    /* error message */
    char     *path;   /* path name for debugging only */
    int      transparent; /* 1 if input file is not a .gz file */
    char     mode;    /* 'w' or 'r' */
    z_off_t  start;   /* start of compressed data in file (header skipped) */
    z_off_t  in;      /* bytes into deflate or inflate */
    z_off_t  out;     /* bytes out of deflate or inflate */
    int      back;    /* one character push-back */
    int      last;    /* true if push-back is last character */
} gz_stream;
#endif



int ATH__check_fp(AFILE* fp);
int ATH__check_suffix(char *name);

/* output methods */
int ATH__fprintf(AFILE* fp, const char * format, ... );
AFILE *ATH__fopen(char *name, char *mode);
AFILE *ATH__fopen(char *name, char *mode, long type);
void ATH__fclose(AFILE *f);
int ATH__fwrite(const void *ptr, size_t size, size_t nmemb, AFILE *fp);

int ATH__fputc(int c, AFILE *fp);
int ATH__putc(int c, AFILE *fp);
int ATH__fputs(const char *s, AFILE *fp);

int ATH__fflush(AFILE* fp);

/* input methods */
size_t ATH__fread(void *ptr, size_t size, size_t nmemb, AFILE *fp);
int ATH__fgetc(AFILE *fp);
char *ATH__fgets(char *s, int size, AFILE *fp);
int ATH__getc(AFILE *fp);
#ifdef NEW_ZLIB
int ATH__ungetc(int c, AFILE *fp);
#endif

int ATH__feof(AFILE* fp);

