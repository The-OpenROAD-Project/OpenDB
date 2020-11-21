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
#include "dbModule.h"

#include "db.h"
#include "dbBlock.h"
#include "dbDatabase.h"
#include "dbDiff.hpp"
#include "dbHashTable.hpp"
#include "dbModInst.h"
#include "dbSet.h"
#include "dbTable.h"
#include "dbTable.hpp"
// User Code Begin includes
#include "dbInst.h"
#include "dbModuleInstItr.h"
// User Code End includes
namespace odb {

// User Code Begin definitions
// User Code End definitions
template class dbTable<_dbModule>;

bool _dbModule::operator==(const _dbModule& rhs) const
{
  if (_name != rhs._name)
    return false;

  if (_next_entry != rhs._next_entry)
    return false;

  if (_insts != rhs._insts)
    return false;

  if (*_modinst_tbl != *rhs._modinst_tbl)
    return false;

  // User Code Begin ==
  if (_modinst_hash != rhs._modinst_hash)
    return false;
  // User Code End ==
  return true;
}
bool _dbModule::operator<(const _dbModule& rhs) const
{
  // User Code Begin <
  // User Code End <
  return true;
}
void _dbModule::differences(dbDiff&          diff,
                            const char*      field,
                            const _dbModule& rhs) const
{
  DIFF_BEGIN

  DIFF_FIELD(_name);

  DIFF_FIELD(_next_entry);

  DIFF_FIELD(_insts);

  DIFF_TABLE(_modinst_tbl);

  // User Code Begin differences
  if (!diff.deepDiff())
    DIFF_HASH_TABLE(_modinst_hash);
  // User Code End differences
  DIFF_END
}
void _dbModule::out(dbDiff& diff, char side, const char* field) const
{
  DIFF_OUT_BEGIN

  DIFF_OUT_FIELD(_name);

  DIFF_OUT_FIELD(_next_entry);

  DIFF_OUT_FIELD(_insts);

  DIFF_OUT_TABLE(_modinst_tbl);

  // User Code Begin out
  if (!diff.deepDiff())
    DIFF_OUT_HASH_TABLE(_modinst_hash);
  // User Code End out
  DIFF_END
}
_dbModule::_dbModule(_dbDatabase* db)
{
  _modinst_tbl = new dbTable<_dbModInst>(
      db, this, (GetObjTbl_t) &_dbModule::getObjectTable, dbModInstObj);
  ZALLOCATED(_modinst_tbl);
  // User Code Begin constructor
  _modinst_hash.setTable(_modinst_tbl);
  // User Code End constructor
}
_dbModule::_dbModule(_dbDatabase* db, const _dbModule& r)
{
  _name        = r._name;
  _next_entry  = r._next_entry;
  _insts       = r._insts;
  _modinst_tbl = new dbTable<_dbModInst>(db, this, *r._modinst_tbl);
  ZALLOCATED(_modinst_tbl);
  // User Code Begin CopyConstructor
  _modinst_hash.setTable(_modinst_tbl);
  // User Code End CopyConstructor
}

dbIStream& operator>>(dbIStream& stream, _dbModule& obj)
{
  stream >> obj._name;
  stream >> obj._next_entry;
  stream >> obj._modinst_hash;
  stream >> obj._insts;
  stream >> *obj._modinst_tbl;
  // User Code Begin >>
  // User Code End >>
  return stream;
}
dbOStream& operator<<(dbOStream& stream, const _dbModule& obj)
{
  stream << obj._name;
  stream << obj._next_entry;
  stream << obj._modinst_hash;
  stream << obj._insts;
  stream << *obj._modinst_tbl;
  // User Code Begin <<
  // User Code End <<
  return stream;
}
dbObjectTable* _dbModule::getObjectTable(dbObjectType type)
{
  switch (type) {
    case dbModInstObj:
      return _modinst_tbl;
      // User Code Begin getObjectTable
    // User Code End getObjectTable
    default:
      break;
  }
  return getTable()->getObjectTable(type);
}
_dbModule::~_dbModule()
{
  delete _modinst_tbl;
}
////////////////////////////////////////////////////////////////////
//
// dbModule - Methods
//
////////////////////////////////////////////////////////////////////

char* dbModule::getName() const
{
  _dbModule* obj = (_dbModule*) this;

  return obj->_name;
}

dbSet<dbModInst> dbModule::getModInsts() const
{
  _dbModule* obj = (_dbModule*) this;
  return dbSet<dbModInst>(obj, obj->_modinst_tbl);
}

// User Code Begin dbModulePublicMethods
void dbModule::addInst(dbInst* inst_)
{
  _dbModule* module = (_dbModule*) this;
  _dbInst*   inst   = (_dbInst*) inst_;
  _dbBlock*  block  = (_dbBlock*) module->getOwner();

  if (inst->_module != 0) {
    dbModule* mod = dbModule::getModule((dbBlock*) block, inst->_module);
    mod->removeInst(inst_);
  }

  inst->_module = module->getOID();
  inst->_module_next = module->_insts;
  module->_insts = inst->getOID();
}

void dbModule::removeInst(dbInst* inst_)
{
  _dbModule* module = (_dbModule*) this;
  _dbInst*   inst   = (_dbInst*) inst_;
  _dbBlock*  block  = (_dbBlock*) module->getOwner();
  if(inst->_module!=module->getOID())
    return;
  uint id = inst->getOID();

  _dbInst* prev = NULL;
  uint     cur  = module->_insts;
  while (cur) {
    _dbInst* c = block->_inst_tbl->getPtr(cur);
    if (cur == id) {
      if (prev == NULL)
        module->_insts = inst->_module_next;
      else
        prev->_module_next = inst->_module_next;
      break;
    }
    prev = c;
    cur  = c->_module_next;
  }
  inst->_module = 0;
  inst->_module_next = 0;
}

dbSet<dbInst> dbModule::getModuleInsts()
{
  _dbModule* module = (_dbModule*) this;
  _dbBlock*  block  = (_dbBlock*) module->getOwner();
  return dbSet<dbInst>(module, block->_module_inst_itr);
}

dbModInst* dbModule::findModInst(const char* name)
{
  _dbModule* module = (_dbModule*) this;
  return (dbModInst*) module->_modinst_hash.find(name);
}

dbModule* dbModule::create(dbBlock* block, const char* name)
{
  _dbBlock* _block = (_dbBlock*) block;
  if (_block->_module_hash.hasMember(name))
    return nullptr;
  _dbModule* module = _block->_module_tbl->create();
  module->_name     = strdup(name);
  ZALLOCATED(module->_name);
  _block->_module_hash.insert(module);
  return (dbModule*) module;
}

void dbModule::destroy(dbModule* module)
{
  _dbModule* _module = (_dbModule*) module;
  _dbBlock*  block   = (_dbBlock*) _module->getOwner();
  for (auto modinst : module->getModInsts())
    modinst->destroy(modinst);
  for (auto inst : module->getModuleInsts())
  {
    _dbInst* _inst = (_dbInst*)inst;
    _inst->_module = 0;
    _inst->_module_next = 0;
  }
  dbProperty::destroyProperties(_module);
  block->_module_hash.remove(_module);
  block->_module_tbl->destroy(_module);
}
dbModule* dbModule::getModule(dbBlock* block_, uint dbid_)
{
  _dbBlock* block = (_dbBlock*) block_;
  return (dbModule*) block->_module_tbl->getPtr(dbid_);
}
// User Code End dbModulePublicMethods
}  // namespace odb
   // Generator Code End 1