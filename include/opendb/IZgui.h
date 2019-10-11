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

#ifndef ADS_DCR_IZGUI_H
#define ADS_DCR_IZGUI_H

#include "IZdcr.h"
#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif


BEGIN_NAMESPACE_ADS

///
/// IZgui
///
/// 

class IZgui : public ZObject
{
  public:
	
	ZINTERFACE_ID(IZgui);

	virtual void init(const char *name)=0; 
    virtual char *setName(const char *name)=0;
    virtual char *getName()=0;
	virtual void chip_get(ZPtr<IZdcr> dcr)=0;
	virtual void setDcr(ZPtr<IZdcr> dcr)=0;
	virtual uint getBbox(int *x1, int *y1, int *x2, int *y2)=0;
	virtual void setGuiContext(void *x, void *blk=NULL)=0;
	virtual void inspect(ZPtr<IZdcr> dcr)=0;

};

END_NAMESPACE_ADS

#endif
