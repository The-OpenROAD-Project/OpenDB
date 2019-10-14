#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "zlib.h"
#include "lefzlib.h"
#include "lefrReader.h"

/*
 * Private functions:
 */
size_t lefGZip_read(FILE* file, char* buf, size_t len) {
  return (gzread((gzFile)file, buf, len));
}

/*
 * Public functions:
 */
lefGZFile 
lefGZipOpen(const char *gzipPath, const char* mode) {
  lefGZFile fptr;

  if (!gzipPath)
    return NULL;

  if (fptr = gzopen(gzipPath, mode)) {
    /* successfully open the gzip file */
    /* set the read function to read from a compressed file */
    lefrSetReadFunction(lefGZip_read);
    return (lefGZFile)fptr;
  } else
    return NULL;
}

int lefGZipClose(lefGZFile filePtr) {
  return (gzclose((gzFile)filePtr));
}

int lefrReadGZip(lefGZFile file, const char* gzipFile, lefiUserData uData) {
  return lefrRead((FILE*)file, gzipFile, uData);
}
