/*
 * This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 * Distribution,  Product Version 5.5, and is subject to the Cadence LEF/DEF
 * Open Source License Agreement.   Your  continued  use  of this file
 * constitutes your acceptance of the terms of the LEF/DEF Open Source
 * License and an agreement to abide by its  terms.   If you  don't  agree
 * with  this, you must remove this and any other files which are part of the
 * distribution and  destroy any  copies made.
 * 
 * For updates, support, or to become part of the LEF/DEF Community, check
 * www.openeda.org for details.
 */

/*
 * FILE: crypt.c 
 *  The encryption/decryption functions have been disabled
 * 
 */
#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32
#include <io.h>
#else				/* not WIN32 */
#include <unistd.h>
#endif				/* WIN32 */

#define BUF_SIZE_BIT	10
#define BUF_SIZE	(1 << BUF_SIZE_BIT)

void 
encClearBuf(FILE * fp)
{
}

/*
 * Need to set the encrypted flag to 1
 */
void 
encWritingEncrypted()
{
    printf("Encryption has been disabled.\n");
}




void 
encPrint(FILE * fp, char *format,...)
{
  va_list ap;

  char   *s;

  char    buf[BUF_SIZE];

  va_start(ap, format);
  (void) vsprintf(buf, format, ap);
  va_end(ap);

  fprintf(fp, "%s", buf);
  return;
}
