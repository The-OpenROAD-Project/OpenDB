#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "db.h"
#include "helper.cpp"
#include <string>

using namespace odb;
using namespace std;
BOOST_AUTO_TEST_SUITE(test_suite)
dbDatabase* db;
dbLib*      lib;
dbBlock*    block;
BOOST_AUTO_TEST_CASE(test_mod_create_find)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  BOOST_ASSERT(dbModule::create(block,"mod1")!=nullptr);
  BOOST_ASSERT(dbModule::create(block,"mod1")==nullptr);
  BOOST_ASSERT(string(block->findModule("mod1")->getName())=="mod1");
}
BOOST_AUTO_TEST_CASE(test_modinst_create_find)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  auto master_mod = dbModule::create(block,"master_mod");
  auto parent_mod = dbModule::create(block,"parent_mod");
  BOOST_ASSERT(dbModInst::create(parent_mod,master_mod,"i1")!=nullptr);
  BOOST_ASSERT(dbModInst::create(parent_mod,master_mod,"i1")==nullptr);
  auto modInst = parent_mod->findModInst("i1");
  BOOST_ASSERT(string(modInst->getName())=="i1");
  BOOST_ASSERT(parent_mod->getModInsts().size()==1);
}
BOOST_AUTO_TEST_CASE(test_modinst_destroy)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  auto master_mod = dbModule::create(block,"master_mod");
  auto parent_mod = dbModule::create(block,"parent_mod");
  auto i1 = dbModInst::create(parent_mod,master_mod,"i1");
  dbModInst::destroy(i1);
  BOOST_ASSERT(parent_mod->getModInsts().size()==0);
  BOOST_ASSERT(parent_mod->findModInst("i1")==nullptr);
}
BOOST_AUTO_TEST_CASE(test_module_destroy)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  auto master_mod = dbModule::create(block,"master_mod");
  auto parent_mod = dbModule::create(block,"parent_mod");
  auto i1 = dbModInst::create(parent_mod,master_mod,"i1");
  dbModule::destroy(parent_mod);
  BOOST_ASSERT(block->findModule("parent_mod")==nullptr);
  BOOST_ASSERT(block->getModules().size()==1);
}
BOOST_AUTO_TEST_CASE(test_module_insts)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  lib       = db->findLib("lib1");
  auto mod1 = dbModule::create(block,"mod1");
  auto mod2 = dbModule::create(block,"mod2");
  auto i1 = dbInst::create(block,lib->findMaster("and2"),"i1");
  auto i2 = dbInst::create(block,lib->findMaster("or2"),"i2");
  auto i3 = dbInst::create(block,lib->findMaster("and2"),"i3");
  auto i4 = dbInst::create(block,lib->findMaster("or2"),"i4");
  mod1->addInst(i1);
  mod2->addInst(i3);
  mod1->addInst(i2);
  mod2->addInst(i4);
  BOOST_ASSERT(mod1->getModuleInsts().size()==2);
  BOOST_ASSERT(mod2->getModuleInsts().size()==2);
  int i = 0;
  for(auto inst : mod1->getModuleInsts())
  {
    BOOST_ASSERT( i++==0 ? string(inst->getName())=="i2" :  string(inst->getName())=="i1");
    BOOST_ASSERT( string(inst->getModule()->getName())=="mod1");
  }
  i = 0;
  for(auto inst : mod2->getModuleInsts())
  {
    BOOST_ASSERT( i++==0 ? string(inst->getName())=="i4" :  string(inst->getName())=="i3");
    BOOST_ASSERT( string(inst->getModule()->getName())=="mod2");
  }
  
  mod1->addInst(i3);
  BOOST_ASSERT(mod1->getModuleInsts().size()==3);
  BOOST_ASSERT(mod2->getModuleInsts().size()==1);
  BOOST_ASSERT( string(i3->getModule()->getName())=="mod1");
  dbInst* inst = *(mod1->getModuleInsts().begin());
  BOOST_ASSERT( string(inst->getName())=="i3");
  
  mod2->removeInst(i4);
  BOOST_ASSERT(mod2->getModuleInsts().size()==0);
  BOOST_ASSERT(i4->getModule()==nullptr);
  
  dbInst::destroy(i3);
  BOOST_ASSERT(mod1->getModuleInsts().size()==2);
  inst = *(mod1->getModuleInsts().begin());
  BOOST_ASSERT( string(inst->getName())=="i2");

  dbModule::destroy(mod1);
  BOOST_ASSERT(i2->getModule()==nullptr);
}
BOOST_AUTO_TEST_SUITE_END()