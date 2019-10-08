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

#ifndef ADS_CRAWLER_H
#define ADS_CRAWLER_H

#ifndef ADS_TM_H
#include "tm.h"
#endif

#include "IZdcr.h"
#include "IZgui.h"

#include "dpt.h"

#define TCL_METHOD(name) int name( ZArgs * in, ZArgs * out )

BEGIN_NAMESPACE_ADS

class Dcr : public ZTechModule<Dcr>
{
	private:
		
		ZPtr<IZdcr> _dcr;
        	dpt *_zptPtr;

		
	public:
		Dcr( ZArgs * in, ZArgs * out );
		~Dcr();
		
		TCL_METHOD ( init ) ;
		TCL_METHOD ( get ) ;
		TCL_METHOD ( attach ) ;
		TCL_METHOD ( chip_get ) ;
		TCL_METHOD ( tiles_get ) ;
		TCL_METHOD ( chip_attr_get ) ;
		TCL_METHOD ( inspect ) ;
		TCL_METHOD ( add_menu ) ;
		TCL_METHOD ( add_sub_menu ) ;
		TCL_METHOD ( config ) ;
		TCL_METHOD ( clear ) ;
		TCL_METHOD ( paint ) ;
		TCL_METHOD ( image ) ;
		TCL_METHOD ( colors ) ;
		TCL_METHOD ( bc_get ) ;
		TCL_METHOD ( test_milos ) ;
		TCL_METHOD ( test_ade ) ;

		ZPtr<IZdcr> getDcr() { return _dcr; }
//		void setGui(ZPtr<IZgui> z) { _gui= z; }

	private:
		// put some here
		
};

END_NAMESPACE_ADS
#endif
