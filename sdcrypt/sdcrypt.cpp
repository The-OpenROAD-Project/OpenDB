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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ctype.h>
#include <time.h>
#include "Blowfish.h"
#include "sdcrypt.h"





const char*
sdCrypt::gen_token(const char* key,int keylen)
{

  

  
  CBlowFish oBlowFish((unsigned char*)key, keylen);
  
  
  // Create Encrypted String
  /* Data String consists of the following:
   * <mmddyyyy><hhmmss><padding> -- total 32bytes
   * time=6bytes, <mmddyyyy>=8bytes, padding=18bytes
   */
  // Get current time and data..
  time_t curr_t ;
  time(&curr_t) ; 
  tm *tm_t  = localtime(&curr_t) ; 

  int cur_mo = (int)tm_t->tm_mon+1 ;
  int cur_day = (int)tm_t->tm_mday ; 
  int cur_yr = (int)tm_t->tm_year+1900 ; 

  
  /* Generate 8 byte date string for cipher */
  char dt_cipher[16] ; 
  char mo_buf[16] ; 
  char day_buf[16] ; 
  char time_cipher[16] ; 
  if(cur_mo < 10) { 
    sprintf(mo_buf,"0%d",cur_mo) ; 
  } else { 
    sprintf(mo_buf,"%d",cur_mo) ; 
  }
  if(cur_day < 10) { 
    sprintf(day_buf,"0%d",cur_day) ; 
  } else { 
    sprintf(day_buf,"%d",cur_day) ; 
  }
  
  // Convert Time
  char mm_buf[16] ; 
  char ss_buf[16] ; 
  char hh_buf[16] ; 
  int mm=(int)tm_t->tm_min ; 
  int ss=(int)tm_t->tm_sec ;  
  int hh=(int)tm_t->tm_hour ; 
  if(mm < 10) { 
    sprintf(mm_buf,"0%d",mm) ; 
  } else { 
    sprintf(mm_buf,"%d",mm) ; 
  }
  if(ss < 10) { 
    sprintf(ss_buf,"0%d",ss) ; 
  } else { 
    sprintf(ss_buf,"%d",ss) ; 
  }
  if(hh < 10) { 
   sprintf(hh_buf,"0%d",hh) ; 
  } else { 
    sprintf(hh_buf,"%d",hh) ; 
  } 

  // Create date and time string
  sprintf(dt_cipher,"%s%s%d",mo_buf,day_buf,cur_yr) ; 
  sprintf(time_cipher,"%s%s%s",hh_buf,mm_buf,ss_buf) ; 
  

  char szDataIn1[33] ; 
  int len=strlen(dt_cipher) ; 

  /* Now Pad with the date-string, ddmmyyyy, 8bytes */
  for(int i=0;i<len;i++) { 
    szDataIn1[i]=dt_cipher[i] ; 
  }
  

  len=strlen(time_cipher) ; 
  for(int i=0;i<len;i++) { 
    szDataIn1[8+i]=time_cipher[i] ; 
  }

  // Date + Time = 14bytes..
  // Remaining bytes = 32-14 = 18bytes..
  // Padding = 18bytes
  // Vendor key,
  // Padd rest..
  int pad=18 ; 
  int tlen=14; 
  for(int i=0;i<pad;i++) { 
    szDataIn1[tlen+i]='0' ; // String terminator..    
  }
  szDataIn1[32]='\0' ; 

  char szDataIn[33];
  char szDataOut[33];
  memset(szDataIn, 0, 33);
  memset(szDataOut, 0, 33);

  
  unsigned char hbuf[16] ; 
  unsigned char obuf[16] ; 
  
  strcpy(szDataIn, szDataIn1);
  memset(obuf, 0, 33 ) ; 
  // Enctrpt
  CBlowFish::HexStr2CharStr(szDataIn, hbuf, 16);
  oBlowFish.Encrypt((unsigned char*)hbuf, 
		    (unsigned char*)obuf, 16, CBlowFish::ECB);
  CBlowFish::CharStr2HexStr(obuf, szDataOut, 16);
  return(strdup(szDataOut)) ; 
  
}



const char*
sdCrypt::decrypt_token(const char* key,int keylen,
		       const char* cipher,
		       char* time_buf,
		       char* date_buf) { 
		       

  
  CBlowFish oBlowFish((unsigned char*)key, keylen);
  
    
  // Create Encrypted String
  char szDataIn1[33] ; 
  
  sprintf(szDataIn1,"%s",cipher) ;
  
  char szDataIn[33];
  char szDataOut[33];
  memset(szDataIn, 0, 33);
  memset(szDataOut, 0, 33);
  
  // Decrypt
  unsigned char hbuf[16] ; 
  unsigned char obuf[16] ; 
  strcpy(szDataIn, szDataIn1);
  memset(szDataOut, 0, 33) ; 
  memset(obuf, 0, 16) ; 
  
  CBlowFish::HexStr2CharStr(szDataIn, hbuf, 16);
  oBlowFish.Decrypt((unsigned char*)hbuf, 
		     (unsigned char*)obuf, 32, CBlowFish::ECB);

  
  CBlowFish::CharStr2HexStr(obuf, szDataOut, 16);
  /* Data String consists of the following:
   * <mmddyyyy><hhmmss><padding> -- total 32bytes
   * time=6bytes, <mmddyyyy>=8bytes, padding=18bytes
   */
  // De-token Decrypted String
  for(int i=0;i<8;i++) { 
    date_buf[i]=szDataOut[i] ;  // Date
  }
  date_buf[8]='\0' ; 
  for(int i=0;i<6;i++) { 
    time_buf[i]=szDataOut[8+i] ;  // Time
  }
  time_buf[6]='\0' ; 



  return((const char*)strdup((const char*)szDataOut)) ; 
  
}
