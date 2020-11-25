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

// Generator Code Begin 1
#include "dbModuleItr.h"

#include "dbModule.h"
#include "dbTable.h"
// User Code Begin includes
// User Code End includes

namespace odb {

////////////////////////////////////////////////////////////////////
//
// dbModuleItr - Methods
//
////////////////////////////////////////////////////////////////////

bool dbModuleItr::reversible()
{
  return true;
}

bool dbModuleItr::orderReversed()
{
  return true;
}

void dbModuleItr::reverse(dbObject* parent)
{
  // User Code Begin reverse
  _dbModule* module = (_dbModule*) parent;
  uint       id     = module->_children;
  uint       list   = 0;

  while (id != 0) {
    _dbModule* child      = _module_tbl->getPtr(id);
    uint        n         = child->_next_child;
    child->_next_child    = list;
    list                  = id;
    id                    = n;
  }
  module->_children = list;
  // User Code End reverse
}

uint dbModuleItr::sequential()
{
  return 0;
}

uint dbModuleItr::size(dbObject* parent)
{
  uint id;
  uint cnt = 0;

  for (id = dbModuleItr::begin(parent); id != dbModuleItr::end(parent);
       id = dbModuleItr::next(id))
    ++cnt;

  return cnt;
}

uint dbModuleItr::begin(dbObject* parent)
{
  // User Code Begin begin
  _dbModule* module = (_dbModule*) parent;
  return module->_children;
  // User Code End begin
}

uint dbModuleItr::end(dbObject* /* unused: parent */)
{
  return 0;
}

uint dbModuleItr::next(uint id, ...)
{
  // User Code Begin next
  _dbModule* module = _module_tbl->getPtr(id);
  return module->_next_child;
  // User Code End next
}

dbObject* dbModuleItr::getObject(uint id, ...)
{
  return _module_tbl->getPtr(id);
}
// User Code Begin methods
// User Code End methods
}  // namespace odb
   // Generator Code End 1