#ifndef LEFZLIB_H
#define LEFZLIB_H

typedef void* lefGZFile;

/* 
 * Name: lefGZipOpen
 * Description: Open a gzip file
 * Returns: A file pointer
 */
extern lefGZFile lefGZipOpen(const char* gzipFile, const char* mode);

/* 
 * Name: lefGZipClose
 * Description: Close a gzip file
 * Returns: 0 if no errors
 */
extern int lefGZipClose(lefGZFile filePtr);

/*
 * Name: lefrReadGZip
 * Description: Parse a lef gzip file
 * Returns: 0 if no errors
 */
extern int lefrReadGZip(lefGZFile file, const char* gzipFile, void* uData);

#endif
