///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef ADS_SDCDB_H
#define ADS_SDCDB_H

#ifndef ADS_H
#include "ads.h"
#endif

#include "db.h"
#include "dbShape.h"
#include "parse.h"
#include "darr.h"

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif



BEGIN_NAMESPACE_ADS


class Sdc_db
{

private:

	char *_inFile;
	char *_outFile;
	FILE *_inFP;
	FILE *_outFP;
	dbBlock *_block;
	char* _idName;

	char* _commandName;
	uint _currentLine;

	Ath__parser *_parser;

public:
	Sdc_db();
	~Sdc_db();
	char* name2id(char *name, char idPrefix);
	bool getPins(Ath__parser *listParser, Ath__parser *itermParser, char *keyword, char idPrefix, bool debug);
	bool parseNameList(Ath__parser *wordParser, char *keyword, char idPrefix, bool virtualFlag, bool debug);
	void translate(dbBlock * block, const char *infile, const char *outfile);
	
};

END_NAMESPACE_ADS

#endif
