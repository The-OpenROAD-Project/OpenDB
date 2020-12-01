#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "db.h"
#include "helper.cpp"
#include <string>

using namespace odb;
using namespace std;
struct F_DETAILED {
  F_DETAILED()
  {
    db        = createSimpleDB();
    block     = db->getChip()->getBlock();
    lib       = db->findLib("lib1");
    master_mod1 = dbModule::create(block,"master_mod1");
    master_mod2 = dbModule::create(block,"master_mod2");
    master_mod3 = dbModule::create(block,"master_mod3");
    parent_mod = dbModule::create(block,"parent_mod");
    i1 = dbModInst::create(parent_mod,master_mod1,"i1");
    i2 = dbModInst::create(parent_mod,master_mod2,"i2");
    i3 = dbModInst::create(parent_mod,master_mod3,"i3");
    inst1 = dbInst::create(block,lib->findMaster("and2"),"inst1");
    inst2 = dbInst::create(block,lib->findMaster("and2"),"inst2");
    inst3 = dbInst::create(block,lib->findMaster("and2"),"inst3");
  }
  ~F_DETAILED()
  {
    dbDatabase::destroy(db);
  }
  dbDatabase* db;
  dbLib*      lib;
  dbBlock*    block;
  dbModule*   master_mod1;
  dbModule*   master_mod2;
  dbModule*   master_mod3;
  dbModule*   parent_mod;
  dbModInst*  i1;
  dbModInst*  i2;
  dbModInst*  i3;
  dbInst*     inst1;
  dbInst*     inst2;
  dbInst*     inst3;
};
BOOST_FIXTURE_TEST_SUITE(test_suite,F_DETAILED)
BOOST_AUTO_TEST_CASE(test_group_default)
{
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
  auto group = dbGroup::create(block,"group");
  group->addInst(inst1);
  group->addInst(inst2);
  group->addInst(inst3);
  auto insts = group->getInsts();
  BOOST_ASSERT(insts.size()==3);
  BOOST_ASSERT(*insts.begin()==inst3);
  BOOST_ASSERT(inst3->getGroup()==group);
  group->removeInst(inst3);
  BOOST_ASSERT(insts.size()==2);
  BOOST_ASSERT(inst3->getGroup()==nullptr);
  BOOST_ASSERT(*insts.begin()==inst2); 
  dbInst::destroy(inst2);
  BOOST_ASSERT(insts.size()==1);
  BOOST_ASSERT(*insts.begin()==inst1); 
  dbGroup::destroy(group);
  BOOST_ASSERT(inst1->getGroup()==nullptr); 
}

BOOST_AUTO_TEST_CASE(test_group_group)
{
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