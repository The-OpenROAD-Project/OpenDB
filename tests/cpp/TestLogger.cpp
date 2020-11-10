#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "Logger.h"
using namespace std;
BOOST_AUTO_TEST_SUITE(test_suite)
BOOST_AUTO_TEST_CASE(test_default)
{
    BOOST_ASSERT(ordlog::initDefault()==0);
    BOOST_ASSERT(ordlog::initDefault()==-1);
    BOOST_ASSERT(ordlog::error(ordlog::OPENDB,1,"Hello {}",1)==0);
    BOOST_ASSERT(ordlog::crit(ordlog::OPENDB,12345,"Hello {}",1)==-1);
    BOOST_ASSERT(ordlog::drop()==0);
    BOOST_ASSERT(ordlog::drop()==-1);
}
BOOST_AUTO_TEST_CASE(test_special_log)
{
    ordlog::initDefault();
    ordlog::info(ordlog::OPENDB,20,"default logger");
    ordlog::init("opendb_special.log","opendb_logger");
    ordlog::info("opendb_logger",ordlog::OPENDB,1,"opendb logger");
    ordlog::drop("opendb_logger");
}
BOOST_AUTO_TEST_SUITE_END()