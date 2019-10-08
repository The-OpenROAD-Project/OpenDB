/*
 BSD 3-Clause License

 Copyright (c) 2019, The Regents of the University of California
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sver.h"

BEGIN_NAMESPACE_ADS
static sver * Sver = NULL;
void sver_parser_set_sver(sver *x) { Sver = x; }
END_NAMESPACE_ADS

USING_NAMESPACE_ADS;

extern int sver_parser_lex(void);
extern void sver_parser_error(char *s);
extern void sver_parser_unrec();
extern int sver_parser_lineno;
extern char * sver_parser_text;

static char sver_text1[4096];
static char sver_text2[4096];

%}

%token ID DOTID CONID OPEN CLOSE CURLY CLOSE_CURLY COMMA SEMI MODULE ENDMODULE 
%token INPUT INOUT OUTPUT WIRE BUSRANGE ASSIGN EQ

%%

top	: stmt_list
	;

stmt_list	: stmt
		| stmt_list stmt
		;

stmt	:	MODULE module_name OPEN mod_pin_list CLOSE SEMI { }
	|       MODULE module_name OPEN CLOSE SEMI { }
	|       MODULE module_name SEMI { }
	|	ENDMODULE { Sver->stop_module(); }
	|	INPUT tok_list SEMI
	|	INOUT tok_list SEMI
	|	OUTPUT tok_list SEMI
	|	WIRE tok_list SEMI
	|	ID { strcpy(sver_text1,sver_parser_text); } rest1 SEMI
	|	ASSIGN tok_list SEMI
	;

rest1	:	{ sver_parser_unrec(); }
	|	ID { strcpy(sver_text2,sver_parser_text); } rest2
	;

rest2	:	OPEN { Sver->start_inst(sver_text2,sver_text1); } 
			ipin_list CLOSE { Sver->stop_inst(); }
	|	ID tok_list { sver_parser_unrec(); }
	|	{ sver_parser_unrec(); }
	;

module_name	: ID { Sver->start_module(sver_parser_text); }

mod_pin_list	: mod_pin
		| mod_pin_list COMMA mod_pin
		;

mod_pin		: ID { Sver->add_module_pin(sver_parser_text); }

ipin_list	:
		| ne_ipin_list
		;

ne_ipin_list	: ipin
		| ne_ipin_list COMMA ipin
		;

id_list		: ID
		| CONID
		| id_list COMMA ID
		| id_list COMMA CONID
		;

ipin		: DOTID { strcpy(sver_text1,sver_parser_text); } OPEN
                   ipinval
                    CLOSE
		;

ipinval		: {}
		| ID { Sver->add_inst_pin(sver_text1,sver_parser_text); }
		| CONID {}
		| CURLY id_list CLOSE_CURLY {}
		;
		

tok_list	: 
		| tok_list one_tok
		;

one_tok	: ID
        | BUSRANGE
	| OPEN
	| CLOSE
	| COMMA
	| EQ
	;

%%
