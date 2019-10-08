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

/* declarations for structures to store the contents of liberty files */

typedef struct liberty_name_list
{
		char *name;
		int prefix_len;
		struct liberty_name_list *next;
} liberty_name_list;

typedef enum liberty_attribute_type 
{
	LIBERTY__SIMPLE,
	LIBERTY__COMPLEX
} liberty_attribute_type;

typedef enum liberty_attribute_value_type 
{
	LIBERTY__VAL_STRING,
	LIBERTY__VAL_DOUBLE,
	LIBERTY__VAL_BOOLEAN,
	LIBERTY__VAL_INT,
	LIBERTY__VAL_UNDEFINED
} liberty_attribute_value_type;

struct liberty_attribute_value
{
		liberty_attribute_value_type type;
		union
		{
				char *string_val;
				int int_val;
				double double_val;
				int bool_val;
		} u;
		struct liberty_attribute_value *next; /* for lists */
};

typedef struct liberty_attribute_value liberty_attribute_value;

