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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef WIN32
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#endif
#include <signal.h>
#include <time.h>

#include "logger.h"

extern "C" {
void Zrouter_InitLogger(Tcl_Interp* interp)
{
  odb::Logger::initLogger(interp);
}

int _getBits(ClientData /* unused: data */,
             Tcl_Interp* interp,
             int /* unused: objc */,
             Tcl_Obj* CONST /* unused: objv */[])
{
#if defined __linux__
#if defined __i386__
  Tcl_AppendResult(interp, "32", NULL);
#elif defined __x86_64__
  Tcl_AppendResult(interp, "64", NULL);
#endif

#else
  // check for non-linux
#if defined __sparc
  // assume 64-bit for sparc
#if defined __arch64__
  Tcl_AppendResult(interp, "64", NULL);
#else
  Tcl_AppendResult(interp, "32", NULL);
#endif
#endif
#endif

  return TCL_OK;
}
int _getCompileTime(ClientData /* unused: data */,
                    Tcl_Interp* interp,
                    int /* unused: objc */,
                    Tcl_Obj* CONST /* unused: objv */[])
{
  // dimitri not sure on the effect of the change
  // Tcl_AppendResult(interp,__DATE__" "__TIME__,NULL);
  Tcl_AppendResult(interp, __DATE__, __TIME__, NULL);
  return TCL_OK;
}

int _getCVSData(ClientData /* unused: data */,
                Tcl_Interp* /* unused: interp */,
                int /* unused: objc */,
                Tcl_Obj* CONST /* unused: objv */[])
{
  // Tcl_AppendResult(interp,CVS_STRING,NULL);
  return TCL_OK;
}

#ifdef WIN32
int _getCPUTime(ClientData     data,
                Tcl_Interp*    interp,
                int            objc,
                Tcl_Obj* CONST objv[])
{
  return 0;
}
#else
static int ticks = sysconf(_SC_CLK_TCK);
int _getCPUTime(ClientData /* unused: data */,
                Tcl_Interp* interp,
                int /* unused: objc */,
                Tcl_Obj* CONST /* unused: objv */[])
{
  struct tms cput;
  times(&cput);
  char buff[128];

  sprintf(buff, "%ld", (cput.tms_utime + cput.tms_stime) / ticks);

  Tcl_AppendResult(interp, buff, NULL);
  return TCL_OK;
}
#endif
}

namespace odb {
Logger*     Logger::_logger = NULL;
static char _ath_logbuffer[_ATH_LOGBUFLEN];

typedef struct deb_rec
{
  char* mod;
  char* except;
  char* only;
} DEBREC;

#define NUMDEBMOD 64
DEBREC debstatus[NUMDEBMOD];
int    debcnt = 0;

#define MAX_WARN_STR 4096
#define WARN_CNT 50
typedef struct warn_rec
{
  char* msg;
  int   cnt;
  int   maxcnt;
} WARNREC;
WARNREC warnstr[MAX_WARN_STR];

int warncnt = 0;

void _freeDebug(void* /* unused: data */)
{
  for (int i = 0; i < debcnt; i++) {
    free(debstatus[i].mod);
    free(debstatus[i].only);
    free(debstatus[i].except);
  }
}

void _freeWarnCnt(void* /* unused: data */)
{
  for (int i = 0; i < warncnt; i++) {
    free(warnstr[i].msg);
  }
}

void dumpDebug()
{
  for (int i = 0; i < debcnt; i++) {
    notice(0,
           "mod: %s, only: %s, except: %s\n",
           debstatus[i].mod,
           debstatus[i].only,
           debstatus[i].except);
  }
}

void dumpWarn()
{
  for (int i = 0; i < warncnt; i++) {
    notice(0,
           "sig: %s, cnt: %d, max: %d\n",
           warnstr[i].msg,
           warnstr[i].cnt,
           warnstr[i].maxcnt);
  }
}

// Testing function for memory counter
int _allocMem(ClientData /* unused: data */,
              Tcl_Interp*    interp,
              int            objc,
              Tcl_Obj* CONST objv[])
{
  static char* mem  = NULL;
  int          size = 1;
  if (objc >= 2) {
    Tcl_GetIntFromObj(interp, objv[1], &size);
  }

  size *= 1024 * 1024;

  if (mem == NULL) {
    mem = (char*) malloc(size);
    notice(0, "Alloc mem: %d / %p\n", size, mem);
    memset(mem, 'X', size);
  } else {
    notice(0, "Free mem: %p\n", mem);
    free(mem);
    mem = NULL;
  }
  return TCL_OK;
}

int verboseStatus = 1;

extern void AthMemCounter(int signo);
extern void AthMemCounterp(int signo);
extern void AthSignalInstaller(int signo, void (*signal_handler)(int));

#define SECOND 1000000
#ifdef WIN32
int _enableMemCount(ClientData     data,
                    Tcl_Interp*    interp,
                    int            objc,
                    Tcl_Obj* CONST objv[])
{
  return TCL_OK;
}
#else
int _enableMemCount(ClientData /* unused: data */,
                    Tcl_Interp* interp,
                    int objc,
                    Tcl_Obj* CONST objv[])
{
  int status = 1;
  if (objc >= 2) {
    Tcl_GetIntFromObj(interp, objv[1], &status);
  }

  double delay = 5.0;
  if (objc >= 3) {
    Tcl_GetDoubleFromObj(interp, objv[2], &delay);
  }

  if (delay < 0.1)
    delay = 0.1;

  int interval = (int) (delay * SECOND);
  // notice(0,"Interval Delay: %d\n",interval);

  switch (status) {
    case 0:
      AthSignalInstaller(SIGALRM, NULL);
      ualarm(0, 0);
      notice(0, "OpenDB Memory Counter Disabled.\n");
      return TCL_OK;

    case 1:
      AthSignalInstaller(SIGALRM, AthMemCounter);
      notice(
          0,
          "OpenDB Memory Counter installed in silent mode with interval %f.\n",
          delay);
      break;

    case 2:
      AthSignalInstaller(SIGALRM, AthMemCounterp);
      notice(
          0,
          "OpenDB Memory Counter installed in display mode with interval %f.\n",
          delay);
      break;

    default:
      notice(0, "Invalid value %d to enableMemCount!\n", status);
      return TCL_ERROR;
  }
  ualarm(50000, interval);

  return TCL_OK;
}
#endif

int _setVerbose(ClientData /* unused: data */,
                Tcl_Interp*    interp,
                int            objc,
                Tcl_Obj* CONST objv[])
{
  int status = 1;
  if (objc >= 2) {
    Tcl_GetIntFromObj(interp, objv[1], &status);
  }

  notice(0, "Set verbose status to: %d\n", status);
  verboseStatus = status;

  return TCL_OK;
}

char last_failed_mod[1024] = "";
int  _setDebug(ClientData /* unused: data */,
               Tcl_Interp* /* unused: interp */,
               int /* unused: objc */,
               Tcl_Obj* CONST objv[])
{
  char* mod    = Tcl_GetString(objv[1]);
  char* only   = Tcl_GetString(objv[2]);
  char* except = Tcl_GetString(objv[3]);
  bool  flag   = false;
  int   idx    = 0;
  for (int i = 0; i < debcnt; i++) {
    if (strcmp(debstatus[i].mod, mod) == 0) {
      idx  = i;
      flag = true;
      break;
    }
  }
  // notice(0,"debug: %s, only: %s, except: %s, flag:
  // %d\n",mod,only,except,flag);
  if (flag == false) {
    if (debcnt == NUMDEBMOD) {
      notice(0, "Out of space to add another debugging module!\n");
      return TCL_ERROR;
    }
    debstatus[debcnt].mod    = strdup(mod);
    debstatus[debcnt].only   = strdup(only);
    debstatus[debcnt].except = strdup(except);

    debcnt++;
  } else {
    free(debstatus[idx].only);
    free(debstatus[idx].except);

    debstatus[idx].only   = strdup(only);
    debstatus[idx].except = strdup(except);
  }
  // dumpDebug();
  last_failed_mod[0] = '\0';
  return TCL_OK;
}

#ifndef _WIN32
struct timespec timer_list[NUM_TIMERS];
struct timespec timer_etime;
bool            timer_init = false;
#endif

int _new_htimer(void)
{
#if !defined(_WIN32) && !defined(__CYGWIN__)
  if (timer_init == false) {
    for (int i = 0; i < NUM_TIMERS; i++) {
      timer_list[i].tv_sec = 0;
    }
    timer_init = true;
  }
  int idx = -1;
  for (int i = 0; i < NUM_TIMERS; i++) {
    if (timer_list[i].tv_sec == 0) {
      idx = i;
      break;
    }
  }

  if (idx >= 0) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer_list[idx]);
    debug("HTIMER",
          "A",
          "Starting values (%d): %d, %d\n",
          idx,
          timer_list[idx].tv_sec,
          timer_list[idx].tv_nsec);
    return idx;
  } else {
    debug("HTIMER", "A", "Could not find free timer!\n");
    return -1;
  }
#else
  return 0;
#endif
}

void _get_htimer(int idx, int& sdiff, int& ndiff)
{
#if !defined(_WIN32) && !defined(__CYGWIN__)

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer_etime);

  if ((idx < 0) || (idx >= NUM_TIMERS) || (timer_list[idx].tv_sec == 0)) {
    debug("HTIMER", "A", "Invalid HTIMER index %d\n", idx);
    return;
  }

  sdiff = timer_etime.tv_sec - timer_list[idx].tv_sec;
  ndiff = timer_etime.tv_nsec - timer_list[idx].tv_nsec;
  // timer_list[idx].tv_sec = 0;
  if (ndiff < 0) {
    // reverse, to avoid problem of different resolutions..
    ndiff = timer_list[idx].tv_nsec - timer_etime.tv_nsec;
    sdiff--;
  }
  debug("HTIMER",
        "A",
        "End values (%d): %d, %d (%d,%d)\n",
        idx,
        timer_etime.tv_sec,
        timer_etime.tv_nsec,
        sdiff,
        ndiff);
#endif
}

void _reset_htimer(int idx)
{
#if !defined(_WIN32) && !defined(__CYGWIN__)
  timer_list[idx].tv_sec = 0;
#endif
}

void _show_htimer(char* msg, char* mod, char* tag, int idx)
{
#if !defined(_WIN32) && !defined(__CYGWIN__)
  int sdiff = 0;
  int ndiff = 0;
  _get_htimer(idx, sdiff, ndiff);
  _reset_htimer(idx);

  debug(mod, tag, "%s: %s ran in %d.%09d seconds\n", mod, msg, sdiff, ndiff);

#endif
}

int _startTimer(ClientData /* unused: data */,
                Tcl_Interp* interp,
                int /* unused: objc */,
                Tcl_Obj* CONST /* unused: objv */[])
{
  char buff[128];
  sprintf(buff, "%d", _new_htimer());
  // notice(0,"ST: %s\n",buff);
  Tcl_AppendResult(interp, buff, NULL);
  return TCL_OK;
}

int _showTimer(ClientData /* unused: data */,
               Tcl_Interp* interp,
               int /* unused: objc */,
               Tcl_Obj* CONST objv[])
{
  int   idx;
  char* msg = Tcl_GetString(objv[1]);
  char* mod = Tcl_GetString(objv[2]);
  char* tag = Tcl_GetString(objv[3]);
  Tcl_GetIntFromObj(interp, objv[4], &idx);
  // notice(0,"SHOW T: %s, %d\n",msg,idx);
  _show_htimer(msg, mod, tag, idx);
  return TCL_OK;
}

int _resetTimer(ClientData /* unused: data */,
                Tcl_Interp* interp,
                int /* unused: objc */,
                Tcl_Obj* CONST objv[])
{
  int idx;
  Tcl_GetIntFromObj(interp, objv[1], &idx);
  _reset_htimer(idx);
  return TCL_OK;
}

int _getTimer(ClientData /* unused: data */,
              Tcl_Interp* interp,
              int /* unused: objc */,
              Tcl_Obj* CONST objv[])
{
  int idx;
  Tcl_GetIntFromObj(interp, objv[1], &idx);
  int ndiff, sdiff;
  _get_htimer(idx, sdiff, ndiff);

  char buff[64];

  sprintf(buff, "%d.%09d", sdiff, ndiff);

  Tcl_AppendResult(interp, buff, NULL);
  return TCL_OK;
}

int _getCWToken(ClientData /* unused: data */,
                Tcl_Interp* interp,
                int /* unused: objc */,
                Tcl_Obj* CONST /* unused: objv */[])
{
  // const char *token = sdCrypt::gen_token(SD_CIPHER_KEY,SD_KEY_LENGTH);
  const char* token = "ABCDEFG_NEFELUS";
  // notice(0,"CW Token: %s\n",token);
  Tcl_AppendResult(interp, token, NULL);
  return TCL_OK;
}

void resetWarningCount(const char* msg, int maxcnt, int cnt)
{
  for (int i = 0; i < warncnt; i++) {
    if (strcmp(warnstr[i].msg, msg) == 0) {
      warnstr[i].maxcnt = maxcnt;
      warnstr[i].cnt    = cnt;
      // dumpWarn();
      return;
    }
  }

  if (warncnt == MAX_WARN_STR) {
    // can't store anymore
    notice(0,
           "Not enough space to store warning signature -"
           " increase MAX_WARN_STR from %d\n",
           MAX_WARN_STR);
    return;
  }
  // if we get here, signature not found
  warnstr[warncnt].msg    = strdup(msg);
  warnstr[warncnt].cnt    = 0;
  warnstr[warncnt].maxcnt = maxcnt;

  warncnt++;

  return;
}

int _setWarnCount(ClientData /* unused: data */,
                  Tcl_Interp* interp,
                  int /* unused: objc */,
                  Tcl_Obj* CONST objv[])
{
  char* msg = Tcl_GetString(objv[1]);
  int   max;
  Tcl_GetIntFromObj(interp, objv[2], &max);
  int cnt;
  Tcl_GetIntFromObj(interp, objv[3], &cnt);

  resetWarningCount(msg, max, cnt);
  return TCL_OK;
}

void Logger::initLogger(Tcl_Interp* interp)
{
  _logger = new Logger(interp);
  Tcl_CreateObjCommand(interp, "sys::getBits", _getBits, NULL, NULL);
  Tcl_CreateObjCommand(
      interp, "sys::getCompileTime", _getCompileTime, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::getCVSData", _getCVSData, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::getCPUTime", _getCPUTime, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::setDebug", _setDebug, NULL, _freeDebug);
  Tcl_CreateObjCommand(
      interp, "sys::setWarnCount", _setWarnCount, NULL, _freeWarnCnt);
  Tcl_CreateObjCommand(interp, "sys::setVerbose", _setVerbose, NULL, NULL);
  Tcl_CreateObjCommand(
      interp, "sys::enableMemCount", _enableMemCount, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::allocMem", _allocMem, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::startTimer", _startTimer, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::showTimer", _showTimer, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::getTimer", _getTimer, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::resetTimer", _resetTimer, NULL, NULL);
  Tcl_CreateObjCommand(interp, "sys::getCWToken", _getCWToken, NULL, NULL);
}

Logger* Logger::getLogger()
{
  if (_logger == NULL) {
    // fprintf(stderr,"Logger is NULL!\n");
    return NULL;
  } else {
    return _logger;
  }
}

Logger* logptr;

int idle(int level)
{
  logptr = Logger::getLogger();
  if (logptr != NULL) {
    return logptr->idle(level);
  }
  // can't do anything if there's no logptr - no _interp * to use

  return TCL_OK;
}

int isDebug(const char* mod, const char* tag)
{
  // see if message should be sent
  static int last_idx = -1;

  if (mod != NULL) {
    if ((last_failed_mod[0] != '\0')
        && (strcmp(mod, (char*) last_failed_mod) == 0)) {
      return 0;
    }

    if (tag == NULL) {
      return 0;
    }

    bool flag = false;
    int  idx  = -1;

    if (last_idx != -1) {
      if (strcmp(debstatus[last_idx].mod, mod) == 0) {
        idx = last_idx;
      }
    }

    if (idx == -1) {
      // find the module
      for (int i = 0; i < debcnt; i++) {
        if (strcmp(debstatus[i].mod, mod) == 0) {
          flag               = true;
          idx                = i;
          last_idx           = idx;
          last_failed_mod[0] = '\0';
          break;
        }
      }
      if (flag == false) {
        // module non-null, not found for debugging
        strcpy(last_failed_mod, mod);
        return 0;
      }
    }

    // check for actual debugging..
    flag     = false;
    int tlen = strlen(tag);
    int olen = strlen(debstatus[idx].only);

    // check if debugging was set
    if (olen > 0) {
      // see if there's a tag match
      for (int i = 0; i < tlen; i++) {
        for (int j = 0; j < olen; j++) {
          if (tag[i] == debstatus[idx].only[j]) {
            flag = true;
            break;
          }
        }
        if (flag == true)
          break;
      }
      if (flag == false)
        return 0;
    } else {
      // no "only" setup, see if in except list
      int elen = strlen(debstatus[idx].except);

      // check for total reset
      if (elen == 1 && debstatus[idx].except[0] == '*')
        return 0;

      for (int i = 0; i < tlen; i++) {
        for (int j = 0; j < elen; j++) {
          if (tag[i] == debstatus[idx].except[j]) {
            return 0;
          }
        }
      }
    }

    return 1;
  } else {
    return 1;
  }
}

int debug(const char* mod, const char* tag, const char* msg, ...)
{
  // fprintf(stderr,"Debug message: %s-%s-%s\n",mod,tag,msg);
  if (isDebug(mod, tag) == 1) {
    // if we get to here, we're good to send

    va_list args;
    va_start(args, msg);

    vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
    va_end(args);

    logptr = Logger::getLogger();
    if (logptr != NULL)
      return logptr->notice(0, _ath_logbuffer, 0);
    else
      return fprintf(stderr, "Debug %s:%s %s", mod, tag, _ath_logbuffer);
  } else {
    return TCL_OK;
  }
}

int checkWarning(const char* msg)
{
  // dumpWarn();
  static int last_idx = -1;

  int idx = -1;

  if (last_idx != -1) {
    if (strcmp(warnstr[last_idx].msg, msg) == 0) {
      idx = last_idx;
    }
  }

  if (idx == -1) {
    for (int i = 0; i < warncnt; i++) {
      if (strcmp(msg, warnstr[i].msg) == 0) {
        idx      = i;
        last_idx = idx;
        break;
      }
    }
    if (idx == -1) {
      if (warncnt == MAX_WARN_STR) {
        notice(0,
               "Not enough space to store warning signature -"
               " increase MAX_WARN_STR from %d\n",
               MAX_WARN_STR);
        return 0;
      }
      // not found - first time for this message
      warnstr[warncnt].msg    = strdup(msg);
      warnstr[warncnt].cnt    = 1;
      warnstr[warncnt].maxcnt = WARN_CNT;

      idx = warncnt;
      warncnt++;
    } else {
      warnstr[idx].cnt++;
    }
  } else {
    warnstr[idx].cnt++;
  }
  // dumpWarn();

  if (warnstr[idx].cnt > warnstr[idx].maxcnt) {
    // exceeded our maximum - don't print
    return 1;
  }
  return 0;
}

/* ********************************************************************
   ********************************************************************
                                  IMPORTANT!!!
               If you change the methods below, update tm.cpp to
              reflect the same behavior, or there will be trouble.
   ********************************************************************
   ******************************************************************** */

int milos(int code, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    char milos_id[128];
    snprintf(milos_id, 128, ":%d", code);
    return logptr->event("milos", milos_id, _ath_logbuffer, 0);
  } else
    return fprintf(stderr, "Milos %d: %s", code, _ath_logbuffer);
}

int ade(int code, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    char ade_id[128];
    snprintf(ade_id, 128, ":%d", code);
    return logptr->event("ade", ade_id, _ath_logbuffer, 0);
  } else {
    if (fprintf(stderr, "ADE %d: %s", code, _ath_logbuffer) >= 0)
      return TCL_OK;
    else
      return TCL_ERROR;
  }
}

int notice(int code, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    char notice_id[128];
    snprintf(notice_id, 128, ":%d", code);
    return logptr->event("notice", notice_id, _ath_logbuffer, 0);
  } else {
    if (fprintf(stderr, "Notice %d: %s", code, _ath_logbuffer) >= 0)
      return TCL_OK;
    else
      return TCL_ERROR;
  }
}

int verbose(int code, const char* msg, ...)
{
  if (verboseStatus != 1)
    return TCL_OK;

  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    char verbose_id[128];
    snprintf(verbose_id, 128, ":%d", code);
    return logptr->event("verbose", verbose_id, _ath_logbuffer, 0);
  } else {
    if (fprintf(stderr, "Verbose %d: %s", code, _ath_logbuffer) >= 0)
      return TCL_OK;
    else
      return TCL_ERROR;
  }
}

int info(int code, const char* msg, ...)
{
  if (checkWarning(msg) == 1)
    return TCL_OK;

  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    char info_id[128];
    snprintf(info_id, 128, ":%d", code);
    return logptr->event("info", info_id, _ath_logbuffer, 0);
  } else {
    if (fprintf(stderr, "Info %d: %s", code, _ath_logbuffer) >= 0)
      return TCL_OK;
    else
      return TCL_ERROR;
  }
}

int warning(int code, const char* msg, ...)
{
  if (checkWarning(msg) == 1)
    return TCL_OK;

  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    // logptr->warning(code,_ath_logbuffer,0);
    char warning_id[128];
    snprintf(warning_id, 128, ":%d", code);
    return logptr->event("warning", warning_id, _ath_logbuffer, 0);
  } else {
    if (fprintf(stderr, "Warning %d: %s", code, _ath_logbuffer) >= 0)
      return TCL_OK;
    else
      return TCL_ERROR;
  }
}

void error(int code, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);
  va_end(args);

  logptr = Logger::getLogger();
  if (logptr != NULL) {
    char error_id[128];
    snprintf(error_id, 128, ":%d", code);
    logptr->event("error", error_id, _ath_logbuffer, 0);
  } else
    fprintf(stderr, "Error %d: %s", code, _ath_logbuffer);
}

// Added reporting into a file - payam 02/01/2006
int fnotice(FILE* file, int code, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  if (file)
    vfprintf(file, msg, args);
  else {
    vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);

    logptr = Logger::getLogger();
    if (logptr != NULL) {
      char notice_id[128];
      snprintf(notice_id, 128, ":%d", code);
      return logptr->event("notice", notice_id, _ath_logbuffer, 0);
    } else {
      if (fprintf(stderr, "No Logger: notice %d: %s\n", code, _ath_logbuffer)
          >= 0)
        return TCL_OK;
      else
        return TCL_ERROR;
    }
  }

  va_end(args);
  return TCL_OK;
}

int fwarning(FILE* file, int code, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);

  if (file) {
    fprintf(file, "WARNING: ");
    vfprintf(file, msg, args);
  } else {
    vsnprintf(_ath_logbuffer, sizeof(_ath_logbuffer), msg, args);

    logptr = Logger::getLogger();
    if (logptr != NULL) {
      char warning_id[128];
      snprintf(warning_id, 128, ":%d", code);
      return logptr->event("warning", warning_id, _ath_logbuffer, 0);
    } else {
      if (fprintf(stderr, "No Logger: warning %d: %s\n", code, _ath_logbuffer)
          >= 0)
        return TCL_OK;
      else
        return TCL_ERROR;
    }
  }

  va_end(args);
  return TCL_OK;
}

}  // namespace odb
