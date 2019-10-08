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

#include <errno.h>
#include <ctype.h>
#include "sdc_db.h"
#include "parse.h"
#include <logger.h>

BEGIN_NAMESPACE_ADS

char* Sdc_db::name2id(char *name, char idPrefix)
{
	uint id= 0;
	if (idPrefix=='N') { // netName
		dbNet *net= _block->findNet(name);
		if (net!=NULL)
			id= net->getId();
	}
	else if (idPrefix=='I') { // instName
		dbInst *inst= _block->findInst(name);
		if (inst!=NULL)
			id= inst->getId();
	}
	else if (idPrefix=='B') { // instName
		dbBTerm *bterm= _block->findBTerm(name);
		if (bterm!=NULL)
			id= bterm->getId();
	}
	if (id==0) {
		warning(0, "Command %s, line %d, Cannot find name %s in db; keep same name\n", 
			_commandName, _currentLine, name);
		return NULL;
	}
	sprintf(_idName, "%c%d", idPrefix, id);

	return _idName;
}
bool Sdc_db::parseNameList(Ath__parser *listParser, char *keyword, char idPrefix, bool virtualFlag, bool debug)
{
	if (! listParser->isKeyword(0, keyword)) 
		return false;
	
	fprintf(_outFP, "[%s {", keyword);

	for (int jj= 1; jj<listParser->getWordCnt(); jj++) {
		char *name= name2id(listParser->get(jj), idPrefix);
		if (name==NULL) // keep original name! TODO ---> check-it
			name= listParser->get(jj);

		fprintf(_outFP, "%s ", name);
	}
	fprintf(_outFP, "}] ");

	return true;
}
bool Sdc_db::getPins(Ath__parser *listParser, Ath__parser *itermParser, char *keyword, char idPrefix, bool debug)
{
	itermParser->resetSeparator(listParser->get(2));
	
	fprintf(_outFP, "[%s %s %s {", keyword, listParser->get(1), listParser->get(2));

	for (int jj= 3; jj<listParser->getWordCnt(); jj++) {
		itermParser->mkWords(listParser->get(jj));

		char *instIdName= name2id(itermParser->get(0), idPrefix);
		if (instIdName==NULL) // keep original name! TODO ---> check-it
			instIdName= itermParser->get(0);

		fprintf(_outFP, "%s%c%s ", instIdName, listParser->get(2)[1], itermParser->get(1));
	}
	fprintf(_outFP, "}] ");

	return true;
}
void Sdc_db::translate(dbBlock* block, const char *infile, const char *outfile)
{
	_block= block;
	_outFP= stdout;
	if (outfile!=NULL) {
		_outFP= fopen(outfile, "w");
	}

	Ath__parser commandParser;
	commandParser.resetSeparator(" ");

	Ath__parser itermParser(1024, 10, 1024);
	Ath__parser listParser(7000000, 100000, 512);
	listParser.resetSeparator(" {}");
	Ath__parser parser(5000000, 10, 5000000);
	parser.openFile((char *)infile);
	parser.resetSeparator("[]");
	
	//parser.setDbg(2);
	char continuationChar= '\\';
	while (parser.parseNextLine(continuationChar)>0) {  // line by line with token separators=[]
		//parser.printWords(stdout);

		_currentLine= parser.getLineNum();
		commandParser.mkWords(parser.get(0));
		_commandName= commandParser.get(0);

		for (int ii= 0; ii<parser.getWordCnt(); ii++) {

			if (!((strstr(parser.get(ii), "get_pins")!=NULL) || 
				(strstr(parser.get(ii), "get_clocks")!=NULL) ||
				(strstr(parser.get(ii), "get_ports")!=NULL))) { // print as is if not [get_xxx ]

				parser.printWord(ii, _outFP, "");
				//parser.printWord(ii, stdout, "");
				continue;
			}
		
			listParser.mkWords(parser.get(ii)); // parse list { .... }

			if (parseNameList(&listParser, "get_ports", 'B', false, true))
				continue;
			if (parseNameList(&listParser, "get_clocks", 'N', false, true))
				continue;
			if (listParser.isKeyword(0, "get_pins")) {
				if (!getPins(&listParser, &itermParser, "get_pins", 'I', true))
					continue;
			}
		}
	}
	fprintf(_outFP, "\n");

	if (outfile!=NULL)
		fclose(_outFP);
}
Sdc_db::Sdc_db()
{
	_idName= new char[128];
}
Sdc_db::~Sdc_db()
{
	delete [] _idName;
}
END_NAMESPACE_ADS

