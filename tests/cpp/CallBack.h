#include "dbBlockCallBackObj.h"
#include "db.h"
#include <vector>
#include <string>
using namespace odb;
using namespace std;
class CallBack : public dbBlockCallBackObj
{
private:
    /* data */
    bool _pause = false;
public:
  vector<string> events;
  //dbBTerm Start
  void inDbBTermCreate(dbBTerm* bterm) override {
    if(!_pause)
      events.push_back("Create "+bterm->getName());
  }
  void inDbBTermDestroy(dbBTerm* bterm) override 
  {
    if(!_pause)
      events.push_back("Destroy "+bterm->getName());
  }
  void inDbBTermPreConnect(dbBTerm* bterm,dbNet* net) override
  {
    if(!_pause)
      events.push_back("Preconnect "+bterm->getName()+" to "+net->getName());
  }
  void inDbBTermPostConnect(dbBTerm* bterm) override
  {
    if(!_pause)
      events.push_back("Postconnect "+bterm->getName());
  }
  void inDbBTermPreDisconnect(dbBTerm* bterm) override
  {
    if(!_pause)
      events.push_back("Predisconnect "+bterm->getName());
  }
  void inDbBTermPostDisConnect(dbBTerm* bterm, dbNet* net) override 
  {
    if(!_pause)
      events.push_back("Postdisconnect "+bterm->getName()+" from "+net->getName());
  }
  void pause()
  {
    _pause = true;
  }
  void unpause()
  {
    _pause = false;
  }
  void clearEvents()
  {
    events.clear();
  }
  //dbBTerm End
};
