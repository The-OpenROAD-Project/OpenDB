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

BOOST_AUTO_TEST_CASE(test_group_default)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  BOOST_ASSERT(dbGroup::create(block,"group")!=nullptr);
  BOOST_ASSERT(dbGroup::create(block,"group")==nullptr);
  BOOST_ASSERT(block->getGroups().size()==1);
  dbGroup* group = block->findGroup("group");
  BOOST_ASSERT(group!=nullptr);
  BOOST_ASSERT(std::string(group->getName())=="group");
  dbGroup::destroy(group);
  BOOST_ASSERT(block->getGroups().size()==0);
  BOOST_ASSERT(block->findGroup("group")==nullptr);
}
BOOST_AUTO_TEST_CASE(test_group_modinst)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  auto master_mod = dbModule::create(block,"master_mod");
  auto parent_mod = dbModule::create(block,"parent_mod");
  auto i1 = dbModInst::create(parent_mod,master_mod,"i1");
  auto i2 = dbModInst::create(parent_mod,master_mod,"i2");
  auto i3 = dbModInst::create(parent_mod,master_mod,"i3");
  auto group = dbGroup::create(block,"group");
  group->addModInst(i1);
  group->addModInst(i2);
  group->addModInst(i3);
  auto insts = group->getModInsts();
  BOOST_ASSERT(insts.size()==3);
  BOOST_ASSERT(*insts.begin()==i3);
  BOOST_ASSERT(i3->getGroup()==group);
  group->removeModInst(i3);
  BOOST_ASSERT(insts.size()==2);
  BOOST_ASSERT(i3->getGroup()==nullptr);
  BOOST_ASSERT(*insts.begin()==i2); 
  dbModInst::destroy(i2);
  BOOST_ASSERT(insts.size()==1);
  BOOST_ASSERT(*insts.begin()==i1); 
  dbGroup::destroy(group);
  BOOST_ASSERT(i1->getGroup()==nullptr); 
}
BOOST_AUTO_TEST_CASE(test_group_inst)
{
db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  lib       = db->findLib("lib1");
  auto i1 = dbInst::create(block,lib->findMaster("and2"),"i1");
  auto i2 = dbInst::create(block,lib->findMaster("and2"),"i2");
  auto i3 = dbInst::create(block,lib->findMaster("and2"),"i3");
  auto group = dbGroup::create(block,"group");
  group->addInst(i1);
  group->addInst(i2);
  group->addInst(i3);
  auto insts = group->getInsts();
  BOOST_ASSERT(insts.size()==3);
  BOOST_ASSERT(*insts.begin()==i3);
  BOOST_ASSERT(i3->getGroup()==group);
  group->removeInst(i3);
  BOOST_ASSERT(insts.size()==2);
  BOOST_ASSERT(i3->getGroup()==nullptr);
  BOOST_ASSERT(*insts.begin()==i2); 
  dbInst::destroy(i2);
  BOOST_ASSERT(insts.size()==1);
  BOOST_ASSERT(*insts.begin()==i1); 
  dbGroup::destroy(group);
  BOOST_ASSERT(i1->getGroup()==nullptr); 
}

BOOST_AUTO_TEST_CASE(test_group_group)
{
  db        = createSimpleDB();
  block     = db->getChip()->getBlock();
  auto group1 = dbGroup::create(block,"group1");
  auto group2 = dbGroup::create(block,"group2");
  auto group3 = dbGroup::create(block,"group3");
  auto parent = dbGroup::create(block,"parent");
  parent->addGroup(group1);
  parent->addGroup(group2);
  parent->addGroup(group3);
  auto groups = parent->getGroups();
  BOOST_ASSERT(groups.size()==3);
  BOOST_ASSERT(*groups.begin()==group3);
  BOOST_ASSERT(group3->getParentGroup()==parent);
  parent->removeGroup(group3);
  BOOST_ASSERT(groups.size()==2);
  BOOST_ASSERT(group3->getParentGroup()==nullptr);
  BOOST_ASSERT(*groups.begin()==group2); 
  dbGroup::destroy(group2);
  BOOST_ASSERT(groups.size()==1);
  BOOST_ASSERT(*groups.begin()==group1); 
  dbGroup::destroy(parent);
  BOOST_ASSERT(group1->getParentGroup()==nullptr); 
}

BOOST_AUTO_TEST_SUITE_END()