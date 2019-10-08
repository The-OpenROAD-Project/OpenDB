/* ATHENA DESIGN SYSTEMS, 2005.*/

/*============================================================================
 * FILE: md5.h
 *============================================================================
 * This is a the header file for the implementation of the MD5
 * Message-Digest algorithm as defined in RFC 1321.
 *
 * It compiles both under C & C++ (and it actually works in both cases! :-)
 * It runs fine both on small and big endian systems.
 *
 * See file "md5.man" for more information.
 *============================================================================
 */
#ifndef MD5_DOT_H               /* include this file only once */
#define MD5_DOT_H


extern void *md5_init(void);
extern void md5_update(void *handle, const char *buf, int buflen);
extern void md5_end(void *handle, unsigned int *result);
extern void md5_result_to_string(const unsigned int *md5sum, char *buf);
extern int md5_file(const char *fname, unsigned int *result);
extern int md5_fp(FILE *fp, unsigned int *result);
extern void md5_string(const char *str, unsigned int *result);

#endif /* MD5_DOT_H */
