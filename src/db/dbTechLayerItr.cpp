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

#include "dbTechLayerItr.h"
#include "dbTech.h"
#include "dbTechLayer.h"
#include "dbTable.h"

namespace odb {

bool dbTechLayerItr::reversible()
{
    return false;
}

bool dbTechLayerItr::orderReversed()
{
    return false;
}

void dbTechLayerItr::reverse(register dbObject * parent)
{
}

uint dbTechLayerItr::sequential()
{
    return 0;
}

uint dbTechLayerItr::size( register dbObject * parent )
{
    register uint id;
    register uint cnt = 0;

    for( id = dbTechLayerItr::begin(parent); id != dbTechLayerItr::end(parent); id = dbTechLayerItr::next(id) )
        ++cnt;
   
    return cnt; 
}

uint dbTechLayerItr::begin( register dbObject * parent )
{
    _dbTech * tech = (_dbTech *) parent;
    return (uint) tech->_bottom;
}

uint dbTechLayerItr::end( register dbObject * parent )
{
    return 0;
}

uint dbTechLayerItr::next( register uint id, ... )
{
    _dbTechLayer * layer = _layer_tbl->getPtr(id);
    return layer->_upper;
}

dbObject * dbTechLayerItr::getObject( register uint id, ... )
{
    return _layer_tbl->getPtr(id);
}

} // namespace
