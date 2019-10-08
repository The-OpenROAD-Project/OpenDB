/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.5, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */

void lefAddStringDefine(char *token, char *string);
void lefAddBooleanDefine(char *token, int val);
void lefAddNumDefine(char *token, double val);
void lefAddStringMessage(char *token, char *string);
void yyerror(const char *s);
void yywarning(const char *s);
void* lefMalloc(int lef_size);
void* lefRealloc(void *name, int lef_size);
void lefFree(void *name);
#ifdef __cplusplus
extern "C" int yylex();
extern "C" void lex_init();
extern "C" int defyyparse();
extern "C" void lex_un_init();
#else
extern int yylex();
extern void lex_init();
extern int defyyparse();
extern void lex_un_init();
#endif
int fake_ftell();
