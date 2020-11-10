#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "Logger.h"
using namespace std;
BOOST_AUTO_TEST_SUITE(test_suite)
BOOST_AUTO_TEST_CASE(test_default)
{
    BOOST_ASSERT(ordlog::info(ordlog::OPENDB,1,"Fresh start")==0);
    BOOST_ASSERT(ordlog::addSinkFile("sink.log")==0);
    BOOST_ASSERT(ordlog::warn(ordlog::OPENDB,2,"Added Sink")==0);
    BOOST_ASSERT(ordlog::addSinkFile("sink.log")==-2);
    BOOST_ASSERT(ordlog::removeSinkFile("sink.log")==0);
    BOOST_ASSERT(ordlog::warn(ordlog::OPENDB,3,"Removed Sink")==0);
    BOOST_ASSERT(ordlog::warn(ordlog::OPENDB,10000,"Invalid Id")==-1);
    BOOST_ASSERT(ordlog::removeSinkFile("sink.log")==-2);
}
BOOST_AUTO_TEST_CASE(test_special_log)
{
    BOOST_ASSERT(ordlog::init("special_log.log","special_logger")==0);
    BOOST_ASSERT(ordlog::info("special_logger",ordlog::OPENDB,4,"Special Logger")==0);
}
BOOST_AUTO_TEST_SUITE_END()