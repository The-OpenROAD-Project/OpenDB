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
#include "rpl.h"
#include "rpl_Tcl.h"
#include <logger.h>

#include "replaceMain.h"

#define TCL_IN_ARGS(name) name * in_args = (name *) in

#define TCL_OUT_ARGS(name) name * out_args = (name *) out

BEGIN_NAMESPACE_ADS

RPL::RPL(ZArgs * in, ZArgs * out) : ZTechModule<RPL>(in, out)
{
	//	ZIn_Ext_Ext * in_args = (ZIn_Ext_Ext *) in;
	// _ext = new extMain(5);
	// _ext->setDB(_db);
	// _tree = NULL;
	
	_main = new replaceMain(_db);
}
RPL::~RPL()
{
	// delete _ext;
}
TCL_METHOD(RPL::test_cmd)
{
	TCL_IN_ARGS(ZIn_RPL_test_cmd);
/*
	TCL_IN_ARGS(ZIn_RPL_test_cmd);
	int setMin = in_args->set_min();
	int setMax = in_args->set_max();
	int setTyp = in_args->set_typ();
	*/
	// int setTyp = in_args->file();
	warning(0, "file name= %s\n", in_args->file() ); 

	debug("DB_ECO","R","REDO ECO: create dbInstObj %s\n",in_args->file());


	return TCL_OK;
}

TCL_METHOD(RPL::test_init)
{
	TCL_IN_ARGS(ZIn_RPL_test_init);

	_main->test_init(in_args->msg());
	_main->init_val(in_args->val());
	return TCL_OK;
}
TCL_METHOD(RPL::dump_init)
{
	_main->dump_init();
	return TCL_OK;
}
/*************************************** placer functions *****************/
TCL_METHOD(RPL::read_db)
{
	notice(0, "RPL::read_db\n");
	TCL_IN_ARGS(ZIn_RPL_read_db);

	// _main->inputDesign();

	return TCL_OK;
}
TCL_METHOD(RPL::dump)
{
	notice(0, "RPL::dump\n");

	TCL_IN_ARGS(ZIn_RPL_dump);
	const char *out_file= in_args->file();
	FILE* outFP= fopen(out_file, "w");

	// _main->Dump(outFP);
	fclose(outFP);

	return TCL_OK;
}
TCL_METHOD(RPL::init_place)
{
	TCL_IN_ARGS(ZIn_RPL_init_place);
	notice(0, "Loging: notice: rpl.cpp: init_place\n");
	warning(0, "Loging: rpl.cpp: init_place");
	info(0, "Loging: info: rpl.cpp: init_place\n");

	// works debug("DBG_PLACE","R","REDO ECO: create dbInstObj %s\n","DBG_PLACE");
	// requires: ::zr.zlog enable_debug_log -module "DBG_PLACE" -only "R"
	

	// _main->initPlace();
	
	return TCL_OK;
}
END_NAMESPACE_ADS
