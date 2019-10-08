
#include <iostream>
#include <fstream>
#include "Blowfish.h"
#include "sdcrypt.h"

using namespace std;

main(int argc,const char** argv)
{
	const char* token=sdCrypt::gen_token(SD_CIPHER_KEY,SD_KEY_LENGTH) ; 
					     
	printf("Token Generated: %s\n",token) ; 

	return 0 ; 
}

