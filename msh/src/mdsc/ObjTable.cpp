/* ATHENA DESIGN SYSTEMS, 2005.*/

/*=============================================================================
 * FILE: ObjTable.cpp
 *=============================================================================
 * Definition of the "ObjTable" class.
 *=============================================================================
 */
#include "Debug.h"
#include "ObjTable.h"

#define DEBUG_LEVEL 4		// Our "Normal" debugging level

/*-----------------------------------------------------------------------------
 * TYPE DECLARATION
 *-----------------------------------------------------------------------------
 */

/*
 * ObjectInfo: this is a class holding information about each object
 * that we insert in our hash table.
 * Fields:
 *	object : the object we are dealing with (this is a "black box"
 *		for us.
 *	isdeleted: Normally this is 0 (false). When we delete
 *		the object we set this field to 1 to mark the object
 *		as "deleted". The actual removal of the hash entry and
 *		destruction of the object can happen later if the object
 *		is already in use by some thread. However new threads
 *		that try to access this object will assume that it is deleted.
 *	refcount: 
 *		This is used to count how many threads are using the object.
 *		In the beginning is zero. Every time we "pin" the object,
 *		it is incremented by one, and every time we "unpin" the
 *		object is decremented by one.
 *		When this counter is zero *and* isdeleted is true, then
 *		it is time to actually delete the object.
 *	cond:
 *		This is a thread "condition" where a thread that
 *		wants to synchronously delete the object can wait
 *		until the object is unpinned by whoever is using it.
 *	cback:
 *		The callback to be called when the object is to be
 *		be deleted. If NULL then we do not have to call any
 *		callback.
 *	extraargs:
 *		Extra args to be passed to "delete callback"
 */
class ObjectInfo {
    public:
	ClientData object;	// the object we are dealing with
	int isdeleted;		// the object has been marked as "deleted"
	int refcount;		// reference count for this object
	Tcl_Condition cond;	// used to wake up the thread that deletes
    				// the object
	ObjTable::DeleteCallback cback;	// The callback to call to delete the
				// object (or NULL)
	ClientData extraargs;	// extra args to be passed to "cback"


	ObjectInfo();		// Constructor
	~ObjectInfo();		// Destructor
};

/*-----------------------------------------------------------------------------
 * ObjectInfo::ObjectInfo -- CONSTRUCTOR
 *-----------------------------------------------------------------------------
 */
ObjectInfo::ObjectInfo()
{
    object = (ClientData) NULL;
    isdeleted = 0;
    refcount = 0;
    cond = (Tcl_Condition) NULL;
    cback = NULL;
}

/*-----------------------------------------------------------------------------
 * ObjectInfo::~ObjectInfo -- DESTRUCTOR
 *-----------------------------------------------------------------------------
 */
ObjectInfo::~ObjectInfo()
{
    Tcl_ConditionFinalize(&(this->cond));
}


/*-----------------------------------------------------------------------------
 * ObjTable::ObjTable -- CONSTRUCTOR
 *-----------------------------------------------------------------------------
 * This is the constructor of the "ObjTable" class
 *-----------------------------------------------------------------------------
 */
ObjTable::ObjTable()
{
    this->_next_id = 0;
    this->_mux = NULL;

    // NOTE: the keys in the hash table are integers, so we
    // use a key type of "TCL_ONE_WORD_KEYS"
    //
    Tcl_InitHashTable(&(this->_tbl), TCL_ONE_WORD_KEYS);

}

/*-----------------------------------------------------------------------------
 * ObjTable::~ObjTable -- DESTRUCTOR
 *-----------------------------------------------------------------------------
 * Free all used resources...
 *-----------------------------------------------------------------------------
 */
ObjTable::~ObjTable()
{
    // We must delete all hash table entries.
    // Of course "Tcl_DeleteHashTable" will free all resources used
    // by the hash table and delete all entries, but we must also
    // free all resources used by the data contained in the entries.
    //
    Tcl_HashSearch hsearch;
    Tcl_HashEntry *he = Tcl_FirstHashEntry(&(this->_tbl), &hsearch);
    while (he != NULL) {
	ObjectInfo *objinfo = (ObjectInfo *) Tcl_GetHashValue(he);
	ClientData obj = objinfo->object;

	// Delete the "ObjectInfo" thing.
	delete objinfo;

	// Shall we also delete the actual object?
	// XXXXXXXXXXXXXXXXXXXXXXX

	he = Tcl_NextHashEntry(&hsearch);
    }

    // Delete the hash table and its mutex.
    //
    Tcl_DeleteHashTable(&(this->_tbl));
    Tcl_MutexFinalize(&(this->_mux));
}

/*-----------------------------------------------------------------------------
 * ObjTable::create
 *-----------------------------------------------------------------------------
 * Create a new entry for the given object, and store it in the table.
 * We return an automatically generated key (handle) for this obbject.
 *
 * ARGUMENTS:
 *	object : the object we want to store. It is declared as
 *		 ClientData and it is a "black box" as far as we are
 *		 concerned.
 * RETURNS:
 *	A key/handle for this object, that can be subsequently used
 *	in the "find", "pin" etc. methods.
 *-----------------------------------------------------------------------------
 */
int ObjTable::create(ClientData object)
{
    int key;

    Debug(DEBUG_LEVEL, ">ObjTable::create 0x%lx: object=0x%lx\n",
    	(long)this, (long)object);

    // Lock the whole table so that we can insert the entry.
    Tcl_MutexLock(&(this->_mux));

    // Get a unique key for this new object
    this->_next_id += 1;
    key = this->_next_id;

    // Create a new hash entry (with this key) and add it to the
    //  hash table
    //
    int newflag;
    Tcl_HashEntry *he;
    he = Tcl_CreateHashEntry(&(this->_tbl), (char *)key, &newflag);
    if (!newflag) {
	// This means that an entry with this key already existed
	// in the table.
	// This is not supposed to happen... ever!
	Tcl_MutexUnlock(&(this->_mux));
	fprintf(stderr, "ObjTable::create : id already exists??!!!\n");
	exit(1);
    }

    // Create the struct that will hold the object information
    // and add it to the hash entry
    //
    ObjectInfo *objinfo = new(ObjectInfo);
    objinfo->object = object;
    Tcl_SetHashValue(he, (ClientData) objinfo);

    // Unlock & return
    Tcl_MutexUnlock(&(this->_mux));

    Debug(DEBUG_LEVEL, "<ObjTable::create 0x%lx: object=0x%lx key=%d\n",
    	(long)this, (long)object, key);

    return(key);
}

/*-----------------------------------------------------------------------------
 * ObjTable::find
 *-----------------------------------------------------------------------------
 * Find and return the object with the given key.
 * Return NULL if the object does not exist
 *
 * NOTE: the object returned is not necessarilly safe to use!
 * If another thread destroys the object you will end up having
 * a dangling pointer. Use this function only iff you are sure that no one
 * else is going to delete the object while you are using it!
 *
 * ARGUMENTS:
 *	key : the key of the object we are looking for
 * RETURNS:
 *	A pointer to the object if found, or NULL (see NOTE above).
 *-----------------------------------------------------------------------------
 */
ClientData ObjTable::find(int key)
{
    Debug(DEBUG_LEVEL, ">ObjTable::find 0x%lx: key=%d\n", (long)this, key);

    ClientData obj;

    Tcl_MutexLock(&(this->_mux));
    Tcl_HashEntry *he = Tcl_FindHashEntry(&(this->_tbl), (char *)key);
    obj = NULL;
    if (he != NULL) {
	ObjectInfo *objinfo = (ObjectInfo *) Tcl_GetHashValue(he);
	// Use the object only if not marked as deleted.
	// Do not modify the reference count of the object.
	if (!objinfo->isdeleted) {
	    obj = objinfo->object;
	}
    }
    Tcl_MutexUnlock(&(this->_mux));
    Debug(DEBUG_LEVEL, "<ObjTable::find 0x%lx: key=%d obj=0x%lx\n",
    	(long)this, key, (long)obj);
    return(obj);
}

/*-----------------------------------------------------------------------------
 * ObjTable::pin
 *-----------------------------------------------------------------------------
 * Find and pin the object.
 * This is similar to the "find" method but we increment the
 * reference count of this object, so that it is not deleted while
 * we are using it. However you *MUST* unpin the object after you
 * are done using the object, otherwise the object could never be
 * deleted!
 *
 * ARGUMENTS:
 *	key : the key of the object we are looking for
 * RETURNS:
 *	A pointer to the object if found, or NULL if the object is not
 *	found. The pointer of the object is guaranteed to remain valid
 *	until you call the "unpin" method (which you *must* call eventually!)
 *-----------------------------------------------------------------------------
 */
ClientData ObjTable::pin (int key)
{
    Debug(DEBUG_LEVEL, ">ObjTable::pin 0x%lx: key=%d\n", (long)this, key);

    ClientData obj;

    Tcl_MutexLock(&(this->_mux));
    Tcl_HashEntry *he = Tcl_FindHashEntry(&(this->_tbl), (char *)key);
    obj = NULL;
    if (he != NULL) {
	ObjectInfo *objinfo = (ObjectInfo *) Tcl_GetHashValue(he);
	// Use the object only if not marked as deleted.
	if (!objinfo->isdeleted) {
	    obj = objinfo->object;
	    // Increment its reference count...
	    objinfo->refcount += 1;
	    Debug(DEBUG_LEVEL, "=ObjTable::pin 0x%lx: key=%d: refcount=%d\n",
			(long)this, key, objinfo->refcount);
	}
    }
    Tcl_MutexUnlock(&(this->_mux));
    Debug(DEBUG_LEVEL, "<ObjTable::pin 0x%lx: key=%d obj=0x%lx\n",
    	(long)this, key, (long)obj);
    return(obj);
}

/*-----------------------------------------------------------------------------
 * ObjTable::unpin
 *-----------------------------------------------------------------------------
 * Unpin a previously pinned object.
 *
 * ARGUMENTS:
 *	key : the key of the object we are looking for
 *-----------------------------------------------------------------------------
 */
void ObjTable::unpin(int key)
{
    Debug(DEBUG_LEVEL, ">ObjTable::unpin 0x%lx: key=%d\n", (long)this, key);

    ClientData obj = NULL;
    DeleteCallback cback = NULL;
    ClientData extraargs = NULL;
    int must_exec_callback = 0;

    Tcl_MutexLock(&(this->_mux));
    Tcl_HashEntry *he = Tcl_FindHashEntry(&(this->_tbl), (char *)key);
    if (he == NULL) {
	// What shall we do here? The object did not exist!
	// ???
    } else {
	ObjectInfo *objinfo = (ObjectInfo *) Tcl_GetHashValue(he);
	objinfo->refcount -= 1;
	Debug(DEBUG_LEVEL,
	"=ObjTable::unpin 0x%lx: key=%d: refcount=%d isdel=%d cback=0x%lx\n",
	    (long)this, key, objinfo->refcount,
	    objinfo->isdeleted, objinfo->cback);

	// Ok, if the object is marked as deleted and the reference
	// count is zero, then it is time to delete the object!!!
	// NOTE: wait until we unlock the hash table and call the callback
	// afterwards to avoid deadlocks.
	//
	if (objinfo->refcount == 0 && objinfo->isdeleted) {
	    if (objinfo->cback == NULL) {
		// The thread which delete the object is waiting
		// to do the deletion. Notify it
		// Don't do anything else, the other thread will do it
		//
		Tcl_ConditionNotify(&(objinfo->cond));
		must_exec_callback = 0;
	    } else {
		// There is a callback waiting to be executed
		// (the thread that "deleted" the object did an async
		// deletion) so *we* have to execute the callback
		must_exec_callback = 1;
		cback = objinfo->cback;
		extraargs = objinfo->extraargs;
		ClientData obj = objinfo->object;
		Tcl_DeleteHashEntry(he);	
		delete objinfo;
	    }
	}
    }
    Tcl_MutexUnlock(&(this->_mux));

    if (must_exec_callback && cback != NULL) {
	Debug(DEBUG_LEVEL,
		"=ObjTable::unpin 0x%lx: key=%d: executing_cback=0x%lx\n",
		(long)this, key, cback);
	(*cback)(key, obj, extraargs);
    }

    Debug(DEBUG_LEVEL, "<ObjTable::unpin 0x%lx: key=%d\n", (long)this, key);
}

/*-----------------------------------------------------------------------------
 * ObjTable::list
 *-----------------------------------------------------------------------------
 * Return a vector with the keys of all the obects stored in the database
 * (in no particular order)
 *-----------------------------------------------------------------------------
 */
std::vector<int> ObjTable::list()
{
    Debug(DEBUG_LEVEL, ">ObjTable::list 0x%lx\n", (long)this);

    Tcl_MutexLock(&(this->_mux));

    std::vector<int> res; 
    Tcl_HashSearch hsearch;
    Tcl_HashEntry *he = Tcl_FirstHashEntry(&(this->_tbl), &hsearch);

    while (he != NULL) {
	int key = (int) Tcl_GetHashKey(&(this->_tbl), he);
	res.push_back(key);
	he = Tcl_NextHashEntry(&hsearch);
    }
    Tcl_MutexUnlock(&(this->_mux));
    Debug(DEBUG_LEVEL, "<ObjTable::list 0x%lx : return %d elements\n",
    	(long)this, res.size());
    return(res);
}

/*-----------------------------------------------------------------------------
 * ObjTable::delete_sync
 *-----------------------------------------------------------------------------
 * Delete the object with the given id
 * This is a synchronous operation. The call will block until
 * we can safely remove the object from the table (no one else is using it)
 * and the object will be returned.
 * NOTE: we just remove the object from the table.
 * It is the responsibility of the user to actually delete the object
 * (e.g. by calling its destructor)
 *
 * ARGUMENTS:
 *	key : the key (handle) for the object we want to delete
 * RETURNS:
 *	The object to be deleted, or NULL if the object does not exist.
 *-----------------------------------------------------------------------------
 */
ClientData ObjTable::delete_sync(int key)
{
    Debug(DEBUG_LEVEL,
	    ">ObjTable::delete_sync 0x%lx key=%d\n", (long)this, key);

    ClientData obj;

    Tcl_MutexLock(&(this->_mux));
    Tcl_HashEntry *he = Tcl_FindHashEntry(&(this->_tbl), (char *)key);
    obj = NULL;
    if (he != NULL) {
	ObjectInfo *objinfo = (ObjectInfo *) Tcl_GetHashValue(he);
	// Delete the object only if not already marked as deleted.
	if (!objinfo->isdeleted) {
	    // Mark the object as deleted
	    objinfo->isdeleted = 1;

	    // Wait until the reference count of the object drops to zero
	    //
	    while (objinfo->refcount > 0) {
		Debug(DEBUG_LEVEL,
			"=ObjTable::delete_sync 0x%lx key=%d refcount=%d\n",
		    (long)this, key, objinfo->refcount);
		Tcl_ConditionWait(&(objinfo->cond), &(this->_mux), NULL);
	    }

	    Debug(DEBUG_LEVEL,
		    "=ObjTable::delete_sync 0x%lx key=%d refcount=%d\n",
		(long)this, key, objinfo->refcount);

	    // Ok, delete the hash entry and the object information
	    obj = objinfo->object;
	    Tcl_DeleteHashEntry(he);
	    delete objinfo;
	}
    }
    Tcl_MutexUnlock(&(this->_mux));

    Debug(DEBUG_LEVEL, "<ObjTable::delete_sync 0x%lx key=%d obj=0x%lx\n",
    		(long)this, key, obj);
    return(obj);
}

/*-----------------------------------------------------------------------------
 * ObjTable::delete_async
 *-----------------------------------------------------------------------------
 * Delete the object with the given id asynchronously
 * ARGUMENTS:
 *	key : the key (handle) for the object we want to delete
 *	cback: a callback to be called to delete/cleanup the object
 *		If NULL then nothing is called.
 *	extraargs: extra argumens to pass to the callback specified.
 *-----------------------------------------------------------------------------
 */
void ObjTable::delete_async(int key, DeleteCallback cback, ClientData extraargs)
{
    Debug(DEBUG_LEVEL,
	    ">ObjTable::delete_async 0x%lx key=%d cback=0x%lx args=0x%lx\n",
    		(long)this, key, (long)cback, (long)extraargs);

    ClientData obj = NULL;
    int must_exec_callback = 0;

    Tcl_MutexLock(&(this->_mux));
    Tcl_HashEntry *he = Tcl_FindHashEntry(&(this->_tbl), (char *)key);
    if (he != NULL) {
	ObjectInfo *objinfo = (ObjectInfo *) Tcl_GetHashValue(he);
	// Delete the object only if not already marked as deleted.
	if (!objinfo->isdeleted) {
	    // Mark the object as deleted and set the callback to
	    objinfo->isdeleted = 1;

	    // If the reference count is zero, then we can call
	    // the "delete callback" now.
	    // Else set the "delete callback" to whatever was passed
	    // to us as argument, and let the last "unpin" method called
	    // to do the job.
	    //
	    if (objinfo->refcount == 0) {
		must_exec_callback = 1;
		obj = objinfo->object;
		Tcl_DeleteHashEntry(he);	
		delete objinfo;
	    } else {
		must_exec_callback = 0;
		objinfo->cback = cback;
		objinfo->extraargs = extraargs;
	    }
	}
    }
    Tcl_MutexUnlock(&(this->_mux));

    // If we must delete the object now (the refcount was zero)
    // we should call the callback now...
    //
    if (must_exec_callback && cback != NULL) {
	(*cback)(key, obj, extraargs);
    }

    Debug(DEBUG_LEVEL,
	    "<ObjTable::delete_async 0x%lx key=%d cback=0x%lx args=0x%lx\n",
    		(long)this, key, (long)cback, (long)extraargs);
}

