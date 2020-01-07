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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef ADS_ZIMPLEMENTS_HPP
#define ADS_ZIMPLEMENTS_HPP

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZSESSION_H
#include "ZSession.h"
#endif

#ifndef ADS_ZNAMESPACE_H
#include "ZNamespace.h"
#endif

namespace odb {

template <class CLASS, class INTERFACE>
ZImplements<CLASS, INTERFACE>::~ZImplements()
{
}

template <class CLASS, class INTERFACE>
uint ZImplements<CLASS, INTERFACE>::AddRef()
{
  return _ref_cnt.inc();
}

template <class CLASS, class INTERFACE>
uint ZImplements<CLASS, INTERFACE>::Release()
{
  int cnt = _ref_cnt.dec();

  if (cnt == 0) {
    _context._session->_ns->removeZObject((ZObject*) this);
    delete this;
  }

  return cnt;
}

template <class CLASS, class INTERFACE>
int ZImplements<CLASS, INTERFACE>::QueryInterface(ZInterfaceID iid, void** p)
{
  if (iid == (ZInterfaceID) ZObject::ZIID) {
    ZObject* o = (ZObject*) this;
    o->AddRef();
    *p = o;
    return Z_OK;
  }

  else if (iid == (ZInterfaceID) INTERFACE::ZIID) {
    INTERFACE* o = (INTERFACE*) this;
    o->AddRef();
    *p = o;
    return Z_OK;
  }

  *p = NULL;
  return Z_ERROR_NO_INTERFACE;
}

}  // namespace odb

#endif
