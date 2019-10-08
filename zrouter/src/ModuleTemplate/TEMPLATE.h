#ifndef ADS_TEMPLATE_H
#define ADS_TEMPLATE_H

#ifndef ADS_H
//#include "ads.h"
#include <ads.h>
#endif

#include "db.h"
// #include <db.h>
BEGIN_NAMESPACE_ADS

class TEMPLATE
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
	TEMPLATE(dbDatabase *db);

	dbBlock* GetTopBlock();

	//void test_option(const char *v, int num);
	void test_option(const char *v);
	void dump_db();
	void init();


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

};

END_NAMESPACE_ADS
#endif
