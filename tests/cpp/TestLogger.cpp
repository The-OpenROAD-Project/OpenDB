#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "Logger.h"
#include <string>
#include <unistd.h>
#include <limits.h>
using namespace std;
BOOST_AUTO_TEST_SUITE(test_suite)
BOOST_AUTO_TEST_CASE(test_default)
{
    char cwd[PATH_MAX];
    string sink_path = "sink.log";
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if(string(cwd)=="/")
            sink_path = "/OpenDB/build/"+sink_path;//in Docker
    }
    ordlog::init();
    BOOST_ASSERT(ordlog::info(ordlog::OPENDB,1,"Fresh start")==0);
    BOOST_ASSERT(ordlog::addSinkFile(sink_path.c_str())==0);
    BOOST_ASSERT(ordlog::warn(ordlog::OPENDB,2,"Added Sink")==0);
    BOOST_ASSERT(ordlog::addSinkFile(sink_path.c_str())==-2);
    BOOST_ASSERT(ordlog::removeSinkFile(sink_path.c_str())==0);
    BOOST_ASSERT(ordlog::crit(ordlog::OPENDB,3,"Removed Sink")==0);
    BOOST_ASSERT(ordlog::error(ordlog::OPENDB,10000,"Invalid Id")==-1);
    BOOST_ASSERT(ordlog::error(ordlog::OPENDB,9999,"valid error Id")==0);
    BOOST_ASSERT(ordlog::removeSinkFile(sink_path.c_str())==-2);
}
BOOST_AUTO_TEST_CASE(test_init_file)
{
    char cwd[PATH_MAX];
    string sink_path = "init_sink.log";
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if(string(cwd)=="/")
            sink_path = "/OpenDB/build/"+sink_path;//in Docker
    }
    ordlog::init(sink_path.c_str());
    BOOST_ASSERT(ordlog::info(ordlog::OPENDB,1,"Fresh start in file")==0);
    BOOST_ASSERT(ordlog::addSinkStdout()==0);
    BOOST_ASSERT(ordlog::info(ordlog::OPENDB,2,"Added stdout")==0);
    BOOST_ASSERT(ordlog::removeSinkStdout()==0);
    BOOST_ASSERT(ordlog::info(ordlog::OPENDB,3,"Removed stdout")==0);
}
BOOST_AUTO_TEST_SUITE_END()