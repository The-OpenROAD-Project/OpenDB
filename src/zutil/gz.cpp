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

/* gz.cpp - Transparent compresses/non-compressed FILE I/O
   Author: Mattias Hembruch
*/

#ifndef _WIN32

#include <stdio.h>
#include <stdarg.h>
#include "zlib.h"
#include "gz.h"

#ifdef STDC
#  include <string.h>
#  include <stdlib.h>
#endif

#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define GZ_SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)

#ifndef ENCRYPT_SUFFIX
#  define ENCRYPT_SUFFIX ".xf"
#endif
#define ENCRYPT_SUFFIX_LEN (sizeof(ENCRYPT_SUFFIX)-1)

#define MAX_NAME_LEN 1024

#ifdef MAXSEG_64K
#  define local static
   /* Needed for systems with limitation on stack size. */
#else
#  define local
#endif

// buffer
#define _ATH_BUFFLEN 128*1024
static char _ath_buffer[_ATH_BUFFLEN];


AFILE *ATH__fopen(char *name, char *mode)
{
    
    int type = ATH__check_suffix(name);

    AFILE *f = (AFILE*)malloc(sizeof(AFILE));
    f->type = type;
#ifdef _WIN32
    f->type = AF_UNCOMP;
#endif
    switch(type)
    {
        case AF_UNCOMP:
        case AF_ENCRYPT:
            f->f = fopen(name,mode);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            f->gzf = gzopen(name,mode);
            break;
#endif
        default:
            free(f);
            f=NULL;
            break;
    }
    return f;

}
AFILE *ATH__fopen(char *name, char *mode, long type)
{
    
    AFILE *f = (AFILE*)malloc(sizeof(AFILE));
    f->type = type;
    int rc = ATH__check_fp(f);
    if( rc ) f->type=0;

    switch(type)
    {
        case AF_UNCOMP:
        case AF_ENCRYPT:
            f->f = fopen(name,mode);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            f->gzf = gzopen(name,mode);
            break;
#endif
        default:
            free(f);
            f=NULL;
            break;
    }
    return f;

}

void ATH__fclose(AFILE *f)
{
    if( f == NULL ) return;

    switch(f->type)
    {
        case AF_UNCOMP:
        case AF_ENCRYPT:
            fclose(f->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            gzclose(f->gzf);
            break;
#endif
        default:
            break;
    }
    free(f);
}

int ATH__fflush(AFILE* fp)
{
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;
    switch(fp->type)
    {
        case AF_UNCOMP: 
        case AF_ENCRYPT:
            rc = fflush(fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rc = gzflush(fp->gzf,Z_SYNC_FLUSH);
            break;
#endif
        default:
            rc = -1; break;
    }
    return rc;
}

/* OUTPUT methods */
int ATH__check_fp(AFILE* fp)
{
    if( fp == NULL )
        return -1;

    if( (fp->type != AF_COMP_GZ) && (fp->type!=AF_UNCOMP) && (fp->type!=AF_ENCRYPT) )
    {
        fprintf(stderr,"Error: Not a valid AFILE *!\n");
        return -1;
    }
    return 0;
}

int ATH__check_suffix(char *name)
{
    if( name == NULL )
    {
        return -1;
    }
    int len = strlen(name);
    if( strncmp(name+len-GZ_SUFFIX_LEN,GZ_SUFFIX,GZ_SUFFIX_LEN+1) == 0 )
    {
        //fprintf(stderr,"Compressed!\n");
        return AF_COMP_GZ;
    }
    if( strncmp(name+len-ENCRYPT_SUFFIX_LEN,ENCRYPT_SUFFIX,ENCRYPT_SUFFIX_LEN+1) == 0 )
    {
        return AF_ENCRYPT;
    }

    //fprintf(stderr,"Uncompressed!\n");

    return AF_UNCOMP;
}

int ATH__fprintf(AFILE* fp, const char * format, ... )
{

    int len;
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;

    va_list args;
    va_start(args, format);
    switch(fp->type)
    {
        case AF_UNCOMP: 
            len = vfprintf(fp->f,format,args);
            break;
#ifndef _WIN32
        case AF_COMP_GZ: 
            len = vsnprintf(_ath_buffer,sizeof(_ath_buffer),format,args);
            gzwrite(fp->gzf,_ath_buffer,len);
            break;
#endif
        case AF_ENCRYPT:
            len = vsnprintf(_ath_buffer,sizeof(_ath_buffer),format,args);
            hide(_ath_buffer,len);
            fwrite(_ath_buffer,1,len,fp->f);
        default:
            len = 0;
            break;
    }
    va_end(args);
    return len;
}

int ATH__fwrite(const void *ptr, size_t size, size_t nmemb, AFILE* fp)
{
    int rc = ATH__check_fp(fp);
    int len;
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rc = fwrite(ptr,size,nmemb,fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rc = gzwrite(fp->gzf,(void*)ptr,size*nmemb);
            break;
#endif
        case AF_ENCRYPT:
            len=size*nmemb;
            memcpy(_ath_buffer,ptr,len);
            hide(_ath_buffer,len);
            rc = fwrite(_ath_buffer,1,len,fp->f);
            break;
        default:
            rc = 0;
    }
    return rc;
}

int ATH__fputs(const char* s, AFILE *fp)
{
    int len;
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rc = fputs(s,fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rc = gzputs(fp->gzf,s);
            break;
#endif
        case AF_ENCRYPT:
            len = strlen(s);
            strcpy(_ath_buffer,s);
            hide(_ath_buffer,len);
            rc = fwrite(_ath_buffer,1,len,fp->f);
        default:
            rc = -1;
            break;
    }
    return rc;
}

int ATH__putc(int c, AFILE *fp)
{
    return ATH__fputc(c,fp);
}

int ATH__fputc(int c, AFILE *fp)
{
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rc = fputc(c,fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rc = gzputc(fp->gzf,c);
            break;
#endif
        case AF_ENCRYPT:
            _ath_buffer[0] = (char)c;
            hide(_ath_buffer,1);
            rc = fputc(_ath_buffer[0],fp->f);
        default:
            rc = -1;
            break;
    }
    return rc;
}

int ATH__feof(AFILE* fp)
{
    int rc = ATH__check_fp(fp);
    gz_stream *s;
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
        case AF_ENCRYPT:
            rc = feof(fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            s = (gz_stream*)fp->gzf;
            rc = !s->stream.avail_in;
            break;
#endif
        default:
            rc = -1;
            break;
    }
    return rc;


}

/* INPUT methods */
int ATH__fread(void *ptr, size_t size, size_t nmemb, AFILE* fp)
{
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rc = fread(ptr,size,nmemb,fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            /* set rc = # of items read */
            rc = gzread(fp->gzf,ptr,size*nmemb);
            if( rc >=(size*nmemb) )
                rc /= size;
            break;
#endif
        case AF_ENCRYPT:
            rc = fread(ptr,size,nmemb,fp->f);
            unhide((char*)ptr,rc);
            break;
        default:
            rc = -1;
            break;
    }
    return rc;
}

int ATH__getc(AFILE *fp)
{
    return ATH__fgetc(fp);
}

int ATH__fgetc(AFILE *fp)
{
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rc = fgetc(fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rc = gzgetc(fp->gzf);
            break;
#endif
        case AF_ENCRYPT:
            rc = fgetc(fp->f);
            _ath_buffer[0] = (char)rc;
            unhide(_ath_buffer,1);
            rc = _ath_buffer[0];
            break;
        default:
            rc = -1;
            break;
    }
    return rc;
}

char *ATH__fgets(char *s, int size, AFILE *fp)
{
    int rc = ATH__check_fp(fp);
    if( rc ) return NULL;
    char *rv;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rv = fgets(s,size,fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rv = gzgets(fp->gzf,s,size);
            break;
#endif
        case AF_ENCRYPT:
            rv = fgets(s,size,fp->f);
            if ( rv != NULL )
            {
                unhide(s,strlen(rv));
            }
            break;
        default:
            rv = NULL;
            break;
    }
    return rv;
}

#ifdef NEW_ZLIB
int ATH__ungetc(int c, AFILE *fp)
{
    int rc = ATH__check_fp(fp);
    if( rc ) return rc;

    switch(fp->type)
    {
        case AF_UNCOMP:
            rc = ungetc(c,fp->f);
            break;
#ifndef _WIN32
        case AF_COMP_GZ:
            rc = gzungetc(c,fp->gzf);
            break;
#endif
        default:
            rc = -1;
            break;
    }
    return rc;
}
#endif

#endif
