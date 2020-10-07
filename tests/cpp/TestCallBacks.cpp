#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "db.h"
#include "CallBack.h"
#include <iostream>
#include "helper.cpp"
using namespace odb;
using namespace std;
BOOST_AUTO_TEST_SUITE( test_suite )
dbDatabase* db;
dbLib* lib;
dbBlock* block;
CallBack* cb;
BOOST_AUTO_TEST_CASE( test_bterm )
{
    db = create2LevetDb();
    block = db->getChip()->getBlock();
    cb = new CallBack();
    cb->addOwner(block); 
    auto n1 = block->findNet("n1");   
    dbBTerm* IN1 = dbBTerm::create(n1,"IN1");
    IN1->setIoType(dbIoType::INPUT);
    BOOST_ASSERT(cb->events.size()==3);
    BOOST_ASSERT(cb->events[0]=="Create IN1");
    BOOST_ASSERT(cb->events[1]=="Preconnect IN1 to n1");
    BOOST_ASSERT(cb->events[2]=="Postconnect IN1");
    cb->clearEvents();
    IN1->disconnect();
    BOOST_ASSERT(cb->events.size()==2);
    BOOST_ASSERT(cb->events[0]=="Predisconnect IN1");
    BOOST_ASSERT(cb->events[1]=="Postdisconnect IN1 from n1");
    cb->clearEvents();
    IN1->destroy(IN1);
    BOOST_ASSERT(cb->events.size()==1);
    BOOST_ASSERT(cb->events[0]=="Destroy IN1");
    cb->pause();
}
BOOST_AUTO_TEST_SUITE_END()