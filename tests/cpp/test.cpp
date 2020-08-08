#include "db.h"
#include <iostream>
using namespace odb;
int main(int argc, char *argv[])
{
    dbDatabase* db = dbDatabase::create();
    std::cout<<"2"<<std::endl;
    return 0;
}