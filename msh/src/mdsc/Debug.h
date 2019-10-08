/* ATHENA DESIGN SYSTEMS, 2005.*/

/*=============================================================================
 * FILE: Debug.h
 *=============================================================================
 * Declarations for the "Debug" class, a thread safe "object" table
 *=============================================================================
 */
#ifndef DEBUG_DOT_H
#define DEBUG_DOT_H

#include <stdio.h>
#include <stdarg.h>

int DebugGetLevel();
void DebugSetLevel(int level);
FILE *DebugGetFp();
void DebugSetFp(FILE *fp);
void Debug(int debuglevel, const char*format, ...);

#endif /* DEBUG_DOT_H */

