#ifndef ADS_SHAPES_H
#define ADS_SHAPES_H

#ifndef ADS_H
//#include "ads.h"
#include <ads.h>
#endif

#include "dbShape.h"
#include "db.h"
// #include <db.h>
BEGIN_NAMESPACE_ADS

class SHAPES
{
private:
	const char * _test_input;
	int _test_val;

	dbDatabase *_db;
	FILE *_out;
	// Other obhect reference 
	//
	double _dist_factor;
	
public:
	SHAPES(dbDatabase *db);

	dbBlock* GetTopBlock();

	void printMacros(const char *file_name);

	// ----------------------------------- DB basic object traversal functions
	void writeTopBlock(const char *file_name);
	void selectNet(dbNet* net);
	void selectInst(dbInst* inst);
	bool writeBlock(dbBlock* block, const char* def_file);
	void writeRows(dbBlock* block);
	void writeInsts(dbBlock* block);
	void writeInst(dbInst* inst);
	void writeIterm(dbITerm* iterm, char *tab);
	void writeIterms(dbInst* inst);
	void writeBTerms(dbNet* net);
	void writeBTerm(dbBTerm* bterm);
	void writeBlockages(dbBlock* block);
	void writeBPin(dbBPin* bpin, int cnt);
	int defdist( int value ) 
    	{
        	return (int) (((double) value) * _dist_factor);
    	}
	int defdist( uint value ) 
    	{
        	return (uint) (((double) value) * _dist_factor);
    	}
	uint getItermShapesNoVias(dbITerm*);

};

END_NAMESPACE_ADS
#endif
