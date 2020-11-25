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
BOOST_AUTO_TEST_CASE(test_default)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  lib       = db->findLib("lib1");
  //dbModule::create() Succeed
  BOOST_ASSERT(dbModule::create(block,"parent_mod")!=nullptr);
  dbModule* master_mod = dbModule::create(block,"master_mod");
  //dbModule::create() rejected
  BOOST_ASSERT(dbModule::create(block,"parent_mod")==nullptr);
  //dbBlock::findModule()
  dbModule* parent_mod = block->findModule("parent_mod");
  BOOST_ASSERT(parent_mod!=nullptr);
  //dbModule::getName()
  BOOST_ASSERT(string(parent_mod->getName())=="parent_mod");
  //dbModInst::create() Succeed
  BOOST_ASSERT(dbModInst::create(parent_mod,master_mod,"i1")!=nullptr);
  //dbModInst::create() rejected
  BOOST_ASSERT(dbModInst::create(parent_mod,master_mod,"i1")==nullptr);
  //dbModule::findModInst()
  dbModInst* modInst = parent_mod->findModInst("i1");
  //dbModInst::getName()
  BOOST_ASSERT(string(modInst->getName())=="i1");
  //dbModule::getModInsts()
  BOOST_ASSERT(parent_mod->getModInsts().size()==1);
  //dbBlock::getModInsts()
  BOOST_ASSERT(block->getModInsts().size()==1);
  //dbInst <--> dbModule
  auto inst1 = dbInst::create(block,lib->findMaster("and2"),"inst1");
  auto inst2 = dbInst::create(block,lib->findMaster("and2"),"inst2");
  //dbModule::addInst()
  parent_mod->addInst(inst1);
  parent_mod->addInst(inst2);
  //dbModule::getInsts()
  BOOST_ASSERT(parent_mod->getInsts().size()==2);
  //dbInst::getModule()
  BOOST_ASSERT(std::string(inst1->getModule()->getName())=="parent_mod");
  //dbModule::removeInst()
  parent_mod->removeInst(inst2);
  BOOST_ASSERT(parent_mod->getInsts().size()==1);
  BOOST_ASSERT(inst2->getModule()==nullptr);
  //dbInst::destroy -> dbModule insts
  dbInst::destroy(inst1);
  BOOST_ASSERT(parent_mod->getInsts().size()==0);
}
BOOST_AUTO_TEST_CASE(test_destroy)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  lib       = db->findLib("lib1");
  auto master_mod = dbModule::create(block,"master_mod");
  auto parent_mod = dbModule::create(block,"parent_mod");
  auto i1 = dbModInst::create(parent_mod,master_mod,"i1");
  auto i2 = dbModInst::create(parent_mod,master_mod,"i2");
  BOOST_ASSERT(block->getModInsts().size()==2);
  //dbModInst::destroy()
  dbModInst::destroy(i1);
  BOOST_ASSERT(parent_mod->findModInst("i1")==nullptr);
  //dbModule::destroy()
  dbModule::destroy(parent_mod);
  BOOST_ASSERT(block->findModule("parent_mod")==nullptr);
  BOOST_ASSERT(block->getModInsts().size()==0);
}

BOOST_AUTO_TEST_CASE(test_heirarchy)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  lib       = db->findLib("lib1");
  auto master_mod = dbModule::create(block,"master_mod");
  auto parent_mod = dbModule::create(block,"parent_mod");
  auto child_mod1 = dbModule::create(parent_mod,"child_mod1");

  BOOST_ASSERT(parent_mod->getChildren().size()==1);
  BOOST_ASSERT(parent_mod->findModule("child_mod1")!=nullptr);
  BOOST_ASSERT(dbModule::create(parent_mod,"child_mod1")==nullptr);
  BOOST_ASSERT(dbModule::create(block,"child_mod1")!=nullptr);
  
  dbModule::destroy(child_mod1);
  
  BOOST_ASSERT(parent_mod->getChildren().size()==0);
  BOOST_ASSERT(parent_mod->findModule("child_mod1")==nullptr);
  
}


BOOST_AUTO_TEST_SUITE_END()