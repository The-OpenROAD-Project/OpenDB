
/* bison input file */
%{
/******************************************************************************
    Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SYNOPSYS Open Source License Version 1.0  (the "License"); 
  you may not use this file except in compliance with such restrictions 
  and limitations. You may obtain instructions on how to receive a copy of 
  the License at

  http://www.synopsys.com/partners/tapin/tapinprogram.html. 

  Software distributed by Original Contributor under the License is 
  distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
  expressed or implied. See the License for the specific language governing 
  rights and limitations under the License.

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "sy_lib.h"
#include "liberty_structs.h"
#include "sy_palloc.h"
#include "sy_pi.h"
	
USING_NAMESPACE_ADS;

int Sy_lineno;
int Sy_syntax_errors;
extern char *Sy_filename;
extern int liberty_parser_lex(void);
void yyerror(char *s);
 
 
%}

%union {
	char *str;
	double num;
	liberty_attribute_value *val;
		
}


%token COMMA SEMI LPAR RPAR LCURLY RCURLY COLON KW_DEFINE KW_DEFINE_GROUP KW_TRUE KW_FALSE

%token <num> NUM
%token <str> STRING IDENT

%type <group>     group file statements statement
%type <def>       define define_group
%type <val> param_list attr_val
%type <str>	s_or_i
%%

file	: {Sy_lineno = 1; Sy_syntax_errors= 0;} group {}
		;

group	: group_head LCURLY  statements RCURLY {sy_group_wrap();}
        | group_head LCURLY  RCURLY { sy_group_wrap();}
		;


statements 	: statement {}
		 	| statements statement  {}
			;


statement 	: simple_attr {}
			| complex_attr {}
			| define {}
			| define_group {}
			| group  {}
			;

simple_attr	: IDENT COLON attr_val { sy_simple_attr($1,$3);} SEMI
			| IDENT COLON attr_val { sy_simple_attr($1,$3);}
			;

complex_attr 	: complex_head  SEMI  
                | complex_head  
				;

group_head	: IDENT LPAR param_list RPAR { sy_group_start($1,$3);}
        | IDENT LPAR RPAR            { sy_group_start($1,NULL);}
		;


complex_head	: IDENT LPAR param_list RPAR { sy_complex($1,$3);}
        | IDENT LPAR RPAR            { sy_complex($1,NULL);}
		;


param_list  : attr_val {$$=$1;}
            | param_list COMMA attr_val
              {
				  liberty_attribute_value *v;
				  for(v=$1; v; v=v->next)
				  {
					  if(!v->next)
					  {
						  v->next = $3;
						  break;
					  }
				  }
				  $$ = $1;
			  }
            | param_list attr_val
              {
				  liberty_attribute_value *v;
				  for(v=$1; v; v=v->next)
				  {
					  if(!v->next)
					  {
						  v->next = $2;
						  break;
					  }
				  }
				  $$ = $1;
			  }
			;

define 	: KW_DEFINE LPAR s_or_i COMMA s_or_i COMMA s_or_i RPAR SEMI  
		{ sy_define($3,$5,$7);}
		;


define_group : KW_DEFINE_GROUP LPAR s_or_i COMMA s_or_i RPAR SEMI
            { sy_define_group($3,$5);}
		;

s_or_i  : STRING {$$ = $1;}
		| IDENT {$$=$1;}
		;

attr_val : NUM { $$= sy_attr_calloc();
                 $$->type = LIBERTY__VAL_DOUBLE;
				 $$->u.double_val = $1;
               }

         | s_or_i
               {
				   $$= sy_attr_calloc();
				   $$->type = LIBERTY__VAL_STRING;
				   $$->u.string_val = $1;
			   }
         | KW_TRUE
               {
				   $$= sy_attr_calloc();
				   $$->type = LIBERTY__VAL_BOOLEAN;
				   $$->u.bool_val = 1;
			   }
         | KW_FALSE
               {
				   $$= sy_attr_calloc();
				   $$->type = LIBERTY__VAL_BOOLEAN;
				   $$->u.bool_val = 0;
			   }
		 ;
%%

void yyerror(char *s)
{
    printf("ERROR ===\nERROR === %s file: %s, line number %d\nERROR ===\n\n", s, Sy_filename, Sy_lineno);
    Sy_syntax_errors++;
}

