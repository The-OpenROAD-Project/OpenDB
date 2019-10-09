#include "SHAPES.h"

//#include "logger.h"
#include <logger.h>

BEGIN_NAMESPACE_ADS

SHAPES::SHAPES(dbDatabase *db)
{
	_db= db;
};
dbBlock* SHAPES::GetTopBlock()
{
        if (_db->getChip()==NULL)
                return NULL;
        return _db->getChip()->getBlock();
};

void SHAPES::printMacros(const char * out_file)
{
	dbBlock* block= GetTopBlock();
	
	if (block==NULL)
		warning(0, "No design was read in; use db load_lef, load_def commands ...\n");
	notice(0, "Dumping block %s ...\n", block->getConstName());
	writeBlock(block, out_file);
};
END_NAMESPACE_ADS
