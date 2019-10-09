#ifndef ADS_RPL_H
#define ADS_RPL_H

#ifndef ADS_TM_H
#include <tm.h>
#endif
#include <util.h>
// #include "extRCap.h"
// #include "exttree.h"

#include <logger.h>

#include "../SHAPES.h"

#define TCL_METHOD(name) int name( ZArgs * in, ZArgs * out )

BEGIN_NAMESPACE_ADS

class PDN : public ZTechModule<PDN>
{
public:
	PDN(ZArgs* in, ZArgs* out);
	~PDN();

	TCL_METHOD( print_macros );
	

private:
	SHAPES* _main;

};

END_NAMESPACE_ADS
#endif
