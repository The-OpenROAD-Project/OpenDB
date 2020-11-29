///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2020, OpenRoad Project
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
#include "dbGroup.h"

#include "db.h"
#include "dbBlock.h"
#include "dbDatabase.h"
#include "dbDiff.hpp"
#include "dbHashTable.hpp"
#include "dbInst.h"
#include "dbModInst.h"
#include "dbModule.h"
#include "dbTable.h"
#include "dbTable.hpp"
// User Code Begin includes
#include "dbBox.h"
// User Code End includes
namespace odb {

// User Code Begin definitions
// User Code End definitions
template class dbTable<_dbGroup>;

bool _dbGroup::operator==(const _dbGroup& rhs) const
{
  if (_flags._type != rhs._flags._type)
    return false;

  if (_name != rhs._name)
    return false;

  if (_next_entry != rhs._next_entry)
    return false;

  if (_group_next != rhs._group_next)
    return false;

  if (_parent_group != rhs._parent_group)
    return false;

  if (_box != rhs._box)
    return false;

  if (_insts != rhs._insts)
    return false;

  if (_modinsts != rhs._modinsts)
    return false;

  if (_groups != rhs._groups)
    return false;

  // User Code Begin ==
  // User Code End ==
  return true;
}
bool _dbGroup::operator<(const _dbGroup& rhs) const
{
  // User Code Begin <
  // User Code End <
  return true;
}
void _dbGroup::differences(dbDiff&         diff,
                           const char*     field,
                           const _dbGroup& rhs) const
{
  DIFF_BEGIN

  DIFF_FIELD(_flags._type);

  DIFF_FIELD(_name);

  DIFF_FIELD(_next_entry);

  DIFF_FIELD(_group_next);

  DIFF_FIELD(_parent_group);

  DIFF_FIELD(_box);

  DIFF_FIELD(_insts);

  DIFF_FIELD(_modinsts);

  DIFF_FIELD(_groups);

  // User Code Begin differences
  // User Code End differences
  DIFF_END
}
void _dbGroup::out(dbDiff& diff, char side, const char* field) const
{
  DIFF_OUT_BEGIN

  DIFF_OUT_FIELD(_flags._type);

  DIFF_OUT_FIELD(_name);

  DIFF_OUT_FIELD(_next_entry);

  DIFF_OUT_FIELD(_group_next);

  DIFF_OUT_FIELD(_parent_group);

  DIFF_OUT_FIELD(_box);

  DIFF_OUT_FIELD(_insts);

  DIFF_OUT_FIELD(_modinsts);

  DIFF_OUT_FIELD(_groups);

  // User Code Begin out
  // User Code End out
  DIFF_END
}
_dbGroup::_dbGroup(_dbDatabase* db)
{
  // User Code Begin constructor
  // User Code End constructor
}
_dbGroup::_dbGroup(_dbDatabase* db, const _dbGroup& r)
{
  _flags._type       = r._flags._type;
  _flags._spare_bits = r._flags._spare_bits;
  _name              = r._name;
  _next_entry        = r._next_entry;
  _group_next        = r._group_next;
  _parent_group      = r._parent_group;
  _box               = r._box;
  _insts             = r._insts;
  _modinsts          = r._modinsts;
  _groups            = r._groups;
  // User Code Begin CopyConstructor
  // User Code End CopyConstructor
}

dbIStream& operator>>(dbIStream& stream, _dbGroup& obj)
{
  uint* _flags_bit_field = (uint*) &obj._flags;
  stream >> *_flags_bit_field;
  stream >> obj._name;
  stream >> obj._next_entry;
  stream >> obj._group_next;
  stream >> obj._parent_group;
  stream >> obj._box;
  stream >> obj._insts;
  stream >> obj._modinsts;
  stream >> obj._groups;
  // User Code Begin >>
  // User Code End >>
  return stream;
}
dbOStream& operator<<(dbOStream& stream, const _dbGroup& obj)
{
  uint* _flags_bit_field = (uint*) &obj._flags;
  stream << *_flags_bit_field;
  stream << obj._name;
  stream << obj._next_entry;
  stream << obj._group_next;
  stream << obj._parent_group;
  stream << obj._box;
  stream << obj._insts;
  stream << obj._modinsts;
  stream << obj._groups;
  // User Code Begin <<
  // User Code End <<
  return stream;
}
dbObjectTable* _dbGroup::getObjectTable(dbObjectType type)
{
  switch (type) {
      // User Code Begin getObjectTable
    // User Code End getObjectTable
    default:
      break;
  }
  return getTable()->getObjectTable(type);
}
_dbGroup::~_dbGroup()
{
}
////////////////////////////////////////////////////////////////////
//
// dbGroup - Methods
//
////////////////////////////////////////////////////////////////////

void dbGroup::setParentGroup(dbGroup* _parent_group)
{
  _dbGroup* obj = (_dbGroup*) this;

  obj->_parent_group = _parent_group->getImpl()->getOID();
}

dbGroup* dbGroup::getParentGroup() const
{
  _dbGroup* obj = (_dbGroup*) this;

  _dbBlock* par = (_dbBlock*) obj->getOwner();
  if (obj->_parent_group == 0)
    return NULL;
  return (dbGroup*) par->_group_tbl->getPtr(obj->_parent_group);
}

void dbGroup::setBox(dbBox* _box)
{
  _dbGroup* obj = (_dbGroup*) this;

  obj->_box = _box->getImpl()->getOID();
}

dbBox* dbGroup::getBox() const
{
  _dbGroup* obj = (_dbGroup*) this;

  _dbBlock* par = (_dbBlock*) obj->getOwner();
  if (obj->_box == 0)
    return NULL;
  return (dbBox*) par->_box_tbl->getPtr(obj->_box);
}

void dbGroup::setType(uint _type)
{
  _dbGroup* obj = (_dbGroup*) this;

  obj->_flags._type = _type;
}

uint dbGroup::getType() const
{
  _dbGroup* obj = (_dbGroup*) this;

  return obj->_flags._type;
}

// User Code Begin dbGroupPublicMethods
void dbGroup::addModInst(dbModInst* modinst)
{

}

void dbGroup::removeModInst(dbModInst* modinst)
{

}

dbSet<dbModInst> dbGroup::getModInsts()
{

}

void dbGroup::addInst(dbInst* inst)
{
  
}
// User Code End dbGroupPublicMethods
}  // namespace odb
   // Generator Code End 1