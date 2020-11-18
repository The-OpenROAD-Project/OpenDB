#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "db.h"
#include "helper.cpp"
#include <cstring>

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
  BOOST_ASSERT(strcmp(block->findModule("mod1")->getName(),"mod1")==0);
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
  BOOST_ASSERT(strcmp(modInst->getName(),"i1")==0);
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
BOOST_AUTO_TEST_SUITE_END()