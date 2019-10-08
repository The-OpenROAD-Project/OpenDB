#ifndef ADS_RPL_H
#define ADS_RPL_H

#ifndef ADS_TM_H
#include <tm.h>
#endif
#include <util.h>
// #include "extRCap.h"
// #include "exttree.h"

#include <logger.h>

#include "../TEMPLATE.h"

#define TCL_METHOD(name) int name( ZArgs * in, ZArgs * out )

BEGIN_NAMESPACE_ADS

class TMP : public ZTechModule<TMP>
{
public:
	TMP(ZArgs* in, ZArgs* out);
	~TMP();

	TCL_METHOD( test_cmd );
	TCL_METHOD( test_option );

/*************************************** module functions *************/
	TCL_METHOD( init );
	TCL_METHOD( dump_db );
	

private:
	TEMPLATE* _main;

};

END_NAMESPACE_ADS
#endif
