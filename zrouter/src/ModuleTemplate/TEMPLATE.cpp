#include "TEMPLATE.h"

//#include "logger.h"
#include <logger.h>

BEGIN_NAMESPACE_ADS

TEMPLATE::TEMPLATE(dbDatabase *db)
{
	_db= db;
};
void TEMPLATE::test_option(const char *v)
//void TEMPLATE::test_option(const char *v, int num)
{
	if (v==NULL)
		return;

	_test_input= strdup(v);
	printf("test_option: %s\n", _test_input);
	// printf("test_option: num= %d\n", _test_input);
};

/*********************************  TEMPLATE Functions ****************************/
dbBlock* TEMPLATE::GetTopBlock()
{
	if (_db->getChip()==NULL)
		return NULL;
	return _db->getChip()->getBlock();
};


void TEMPLATE::init()
{
	printf("TEMPLATE:Init val: \n");
};
void TEMPLATE::writeTopBlock(const char * out_file)
{
	dbBlock* block= GetTopBlock();
	
	if (block==NULL)
		warning(0, "No design was read in; use db load_lef, load_def commands ...\n");
	notice(0, "Dumping block %s ...\n", block->getConstName());
	writeBlock(block, out_file);
};
END_NAMESPACE_ADS
