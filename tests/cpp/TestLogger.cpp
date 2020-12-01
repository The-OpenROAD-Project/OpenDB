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
    ord::init();
    BOOST_ASSERT(ord::info(ord::OPENDB,1,"Fresh start")==0);
    BOOST_ASSERT(ord::addSinkFile(sink_path.c_str())==0);
    BOOST_ASSERT(ord::warn(ord::OPENDB,2,"Added Sink")==0);
    BOOST_ASSERT(ord::addSinkFile(sink_path.c_str())==-2);
    BOOST_ASSERT(ord::removeSinkFile(sink_path.c_str())==0);
    BOOST_ASSERT(ord::crit(ord::OPENDB,3,"Removed Sink")==0);
    BOOST_ASSERT(ord::error(ord::OPENDB,10000,"Invalid Id")==-1);
    BOOST_ASSERT(ord::error(ord::OPENDB,9999,"valid error Id")==0);
    BOOST_ASSERT(ord::removeSinkFile(sink_path.c_str())==-2);
}
BOOST_AUTO_TEST_CASE(test_init_file)
{
    char cwd[PATH_MAX];
    string sink_path = "init_sink.log";
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if(string(cwd)=="/")
            sink_path = "/OpenDB/build/"+sink_path;//in Docker
    }
    ord::init(sink_path.c_str());
    BOOST_ASSERT(ord::info(ord::OPENDB,1,"Fresh start in file")==0);
    BOOST_ASSERT(ord::addSinkStdout()==0);
    BOOST_ASSERT(ord::info(ord::OPENDB,2,"Added stdout")==0);
    BOOST_ASSERT(ord::removeSinkStdout()==0);
    BOOST_ASSERT(ord::info(ord::OPENDB,3,"Removed stdout")==0);
}
BOOST_AUTO_TEST_SUITE_END()
