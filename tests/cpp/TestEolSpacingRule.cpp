#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include "db.h"
#include <libgen.h>
#include "lefin.h"
#include "lefout.h"
#include "defin.h"
#include "defout.h"

#include <iostream>
using namespace odb;

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( test_default )
{
    dbDatabase*  db   = dbDatabase::create();
    lefin lefParser(db, false);
    const char *libname = "gscl45nm.lef";
    
    std::string path = std::string(std::getenv("BASE_DIR")) + "/data/gscl45nm.lef";

    lefParser.createTechAndLib(libname, path.c_str());
    
    auto dbTech = db->getTech();
    auto layers = dbTech->getLayers();

    for (auto &&layer : layers)
    {
        if(layer->getName() == "metal1"){
            auto rules = layer->getEolSpacingRules();
            BOOST_ASSERT(rules.size() == 1);
            for (auto &&rule : rules)
            {
                BOOST_ASSERT(rule->geteolSpace() == 1.3);
                BOOST_ASSERT(rule->getEolwidth() == 1.5);
                BOOST_ASSERT(rule->isWITHIN() == 1);
                BOOST_ASSERT(rule->geteolWithin() == 1.9);
                BOOST_ASSERT(rule->isSAMEMASK() == 1);
                BOOST_ASSERT(rule->isEXCEPTEXACTWIDTH() == 1);
                BOOST_ASSERT(rule->getexactWidth() == 0.5);
                BOOST_ASSERT(rule->getotherWidth() == 0.3);
                BOOST_ASSERT(rule->isPARALLELEDGE() == 1);
                BOOST_ASSERT(rule->getparSpace() == 0.2);
                BOOST_ASSERT(rule->getparWithin() == 9.1);
                BOOST_ASSERT(rule->isPARPRL() == 1);
                BOOST_ASSERT(rule->getparPrl() == 81);
                BOOST_ASSERT(rule->isPARMINLENGTH() == 1);
                BOOST_ASSERT(rule->getparMinLength() == -0.1);
                BOOST_ASSERT(rule->isTWOEDGES() == 1);
                BOOST_ASSERT(rule->isTOCONCAVECORNER() == 0);
           }
            
        }
    }
    
}

BOOST_AUTO_TEST_SUITE_END()
