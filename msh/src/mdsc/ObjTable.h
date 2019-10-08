/* ATHENA DESIGN SYSTEMS, 2005.*/

/*=============================================================================
 * FILE: ObjTable.h
 *=============================================================================
 * Declarations for the "ObjTable" class, a thread safe "object" table
 *=============================================================================
 */
#ifndef OBJTABLE_DOT_H
#define OBJTABLE_DOT_H

#include <vector>
#include <tcl.h>

/*-----------------------------------------------------------------------------
 * CLASS: ObjTable
 *-----------------------------------------------------------------------------
 */
class ObjTable {
    public:
	typedef void (*DeleteCallback)(int key, ClientData object, 
		ClientData extraargs);
	ObjTable();
	~ObjTable();
	int create(ClientData object);
	ClientData find(int key);
	ClientData pin(int key);
	void unpin(int key);
	std::vector<int> list();
	ClientData delete_sync(int key);
	void delete_async(int key, DeleteCallback cback, ClientData extraargs);
    private:
	int _next_id;
	Tcl_HashTable _tbl;
	Tcl_Mutex _mux;
};

#endif /* OBJTABLE_DOT_H */

