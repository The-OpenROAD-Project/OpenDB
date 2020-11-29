#pragma once

#include <string>
#include "db.h"


namespace odb{

    class lefTechLayerSpacingEolParser
    {
        
    public:
        static dbTechLayerSpacingEolRule* parse(std::string, dbTechLayer*);
    };


}

