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

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifndef _WIN32
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#if sun == 1
#include <procfs.h>
#include <fcntl.h>
#endif
#endif

#include <stdlib.h>
#include <assert.h>

#include "logger.h"

using namespace std;

static unsigned int gListOfPrimes [] = {49978783 , 18409199 , 1299827
, 981493 , 779377 , 530279 , 1176221 , 143567 , 30389 , 6869 , 1049 ,
149, 11, 3,2,1 };

int Ath__double2int (double v)
{
  int iv = (int) v;
  if (iv > 0)
  {
    if (v > (double)iv + 0.5)
      iv++;
  }
  else if (iv < 0)
  {
    if ((double)iv > v + 0.5)
      iv--;
  }
  return iv;
}
namespace odb {

static unsigned int l_find_largest_prime_below_number(unsigned int
number) {
	assert(number>3);
	int i=0;
	while (gListOfPrimes[i] > number) 
		i++;
	return gListOfPrimes[i];
}

unsigned int AthHashFunction(char *key, unsigned int len,
unsigned int prime)
{
  unsigned int hash, i;
  for (hash=len, i=0; i<len; ++i)
    hash = (hash<<4)^(hash>>28)^key[i];
  return (hash % prime);
}

int AthGetProcessMem(uint64 *size, uint64 *res)
{
#ifndef _WIN32
#if unix == 1
#if sun == 1

#define PAGE_SIZE 1024
    // for Solaris, read /proc/pid/psinfo and get the right fields
    psinfo_t psi;
    char buff[1024];
    int pid = getpid();
    int fd = -1;
    int sz;

    sprintf(buff,"/proc/%d/psinfo",pid);
    fd = open(buff,O_RDONLY);
    if ( fd < 0 )
    {
        return -1;
    }
    sz = read(fd,&psi,sizeof(psinfo_t));

    close(fd);
    if (sz != sizeof(psinfo_t))
    {
        return -1;
    }

    *size = psi.pr_size * PAGE_SIZE;
    *res=psi.pr_rssize * PAGE_SIZE;

    return 0;

#elif linux == 1
#define PAGE_SIZE 4096
    // for Linux, parse the first two fields from /proc/pid/statm
    char buff[1024];
    int pid = getpid();
    sprintf(buff,"/proc/%d/statm",pid);

    FILE *f = fopen(buff,"r");
    int cnt = fscanf(f,"%llu %llu", size, res);
    fclose(f);

    if( 2 != cnt )
    {
        return -1;
    }

    *size = *size * PAGE_SIZE;
    *res = *res * PAGE_SIZE;

    return 0;

#endif
#endif
#endif
return 0;
}

uint64 max_res = 0;
uint64 max_size = 0;

void AthSignalInstaller(int signo, void (*signal_handler)(int))
{
#ifndef _WIN32
    struct sigaction act;
    act.sa_handler = signal_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if(sigaction(signo, &act,NULL) == -1)
    {
        perror("signal:");
    }
    //printf("Installed signal handler!\n");
#endif
}



void AthMaxMem(uint64 size, uint64 res)
{
    if ( size > max_size )
        max_size = size;

    if ( res > max_res )
        max_res = res;
}
void AthMemCounter(int signo)
{
    uint64 size;
    uint64 res;
    AthGetProcessMem(&size,&res);
    AthMaxMem(size,res);
}

// for scaling mem output
#define MS (1024*1024)
#define GS (1024*1024*1024)
void AthMemCounterp(int signo)
{
    uint64 size;
    uint64 res;
    AthGetProcessMem(&size,&res);
    AthMaxMem(size,res);

    double psize = ((double)size)/MS;
    double pres = ((double)res)/MS;
    if ( psize > 1024.0 )
    {
        fprintf(stdout,"AthMemCounter (size/res): %.2f GB / %.2f GB\n",psize/1024, pres/1024);
    } else {
        fprintf(stdout,"AthMemCounter (size/res): %.2f MB / %.2f MB\n",psize, pres);
    }
}


int AthResourceLog(char *title, int ss)
{
#ifdef _WIN32
	return 0;
#endif

#ifndef _WIN32
  static struct tms ctp;
  static long wtp = 0;
  static double mmp = 0.0;

  // mallinfo does not work on 64 bit.
  // tttt_use_mallinfo is set 0.
  // change it to 1 and rebuild before trying mallinfo on 32 bit.

  if (ss == -2)
    return 0;

  uint64 res; uint64 size;

  double mmn;
#ifdef OLD_MALLOC
  static struct mallinfo mminfo;
    mminfo = mallinfo();
    mmn = (double)((uint)mminfo.arena) + (double)((uint)mminfo.hblkhd);
#else
    if ( AthGetProcessMem(&size, &res) != 0 )
    {
        return 0;
    }

    AthMaxMem(size,res);
  
    mmn = (double)size;
#endif

  if (ss == -1)
  {
    if (mmn != mmp)
      return 1;
    return 0;
  }

  struct tms ctn;
  long wtn, wtd;
  time (&wtn);
  times (&ctn);
  int ticks = sysconf(_SC_CLK_TCK);
  if (wtp)
  {
    if (title && title[0])
      notice (0, "%s:  ", title);

    wtd = wtn - wtp;
    notice (0, "ELAPSE %.8s  ", asctime(gmtime(&wtd)) + 11);

    if (ss)
    {
        notice (0, "CPU %ld cs  ", (ctn.tms_utime+ctn.tms_stime) - (ctp.tms_utime+ctp.tms_stime));
        notice (0, "MEM %d B (%d B)", (int)(mmn), (int)(mmn-mmp));
        notice(0," MAX %d M",(int)max_size);
        notice (0, "  %s\n",ctime(&wtn));
    }
    else
    {
        int imeg = 1024*1024;
        notice (0, "CPU %ld sec  ", ((ctn.tms_utime+ctn.tms_stime) - (ctp.tms_utime+ctp.tms_stime))/ticks);
        notice (0, "MEM %d M (%d M)", (int)(mmn/imeg), (int)((mmn-mmp)/imeg));
        notice(0," MAX %d M",(int)((double)max_size/imeg));
        notice (0, "  %s\n",ctime(&wtn));
    }
  }
  wtp = wtn;
  ctp = ctn;
  mmp = mmn;

  // reset maximums after they were printed
  max_size = size;
  max_res = res;

  return 1;
#endif
}

extern "C" void cAthResourceLog(char *title)
{
  AthResourceLog(title, 0);
}

int Ath__double2int (double v)
{
  int iv = (int) v;
  if (iv > 0)
  {
    if (v > (double)iv + 0.5)
      iv++;
  }
  else if (iv < 0)
  {
    if ((double)iv > v + 0.5)
      iv--;
  }
  return iv;
}


#ifndef _WIN32
struct timeval ath_stime;
struct timeval ath_etime;
#endif

void start_timer()
{
#ifndef _WIN32
    gettimeofday( &ath_stime, NULL);
#endif
    //printf("TIMER START (%d.%d): \n",ath_stime.tv_sec,ath_stime.tv_usec);
}

void end_timer(char *msg)
{
#ifndef _WIN32
    gettimeofday( &ath_etime, NULL);
    int sdiff = ath_etime.tv_sec - ath_stime.tv_sec;
    int udiff = ath_etime.tv_usec - ath_stime.tv_usec;
    if ( udiff < 0 )
    {
        // borrow a second to cover microseconds
        udiff += 1000000;
        sdiff--;
    }
    udiff /= 10000;

    debug("TIMER","A","TIMER: %s ran in %d.%02d seconds\n",msg,sdiff,udiff);

#endif
}


#ifndef _WIN32
struct timespec ath_shtime;
struct timespec ath_ehtime;
#endif

void start_htimer()
{
#if ! defined(_WIN32) && ! defined(__CYGWIN__)
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ath_shtime);
#endif
}

void end_htimer(char *msg)
{
#if ! defined(_WIN32) && ! defined(__CYGWIN__)
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ath_ehtime);
    int sdiff = ath_ehtime.tv_sec - ath_shtime.tv_sec;
    long ndiff = ath_ehtime.tv_nsec - ath_shtime.tv_nsec;
    if ( ndiff < 0 )
    {
        // borrow a second to cover microseconds
        ndiff += 1000000000;
        sdiff--;
    }

    debug("HTIMER","A","HTIMER: %s ran in %d.%09d seconds\n",msg,sdiff,ndiff);

#endif
}


}
