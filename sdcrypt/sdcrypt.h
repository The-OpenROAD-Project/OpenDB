//*****************************************************************************
// sdctryp.h
// This file contains Sd-encrtption methods using BlowFish
//
//*****************************************************************************/

/*****************************************************************
 * Copyright (c) 2003 - Synchronous Design Automation            *
 * Distribution,Modification or Use without a license agreement  *
 * with Synchronous Design Automation is not permitted.          * 
 * Contact: info@synchronous-da.com                              *
 *****************************************************************/




// *****************************************************************************
// Class: sdCrypt
//
//
// *****************************************************************************


#ifndef SD_CRYPT_H
#define SD_CRYPT_H

#include <stdio.h>
#include <stdlib.h>
 


#define SD_CIPHER_KEY "syncda-athena-ds"
#define SD_KEY_LENGTH 16




class sdCrypt { 



 public:
  static const char* gen_token(const char* key,int keylen) ; 
			       
  static const char* decrypt_token(const char* key,int keylen,
				   const char* cipher,
				   char* time_buf,
				   char* dat_buf) ; 
  



} ; 


#endif
