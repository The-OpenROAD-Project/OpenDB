/* ATHENA DESIGN SYSTEMS, 2005.*/

/*============================================================================
 * FILE: memobj_tcl.cpp
 *============================================================================
 * This is the wrapper for our memory object routines around the
 * MemObj class...
 *============================================================================
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <tcl.h>
#include <string>

#include "MemObj.h"
#include "ObjTable.h"
#include "Debug.h"


extern "C" {
/*----------------------------------------------------------------------------
 * DECLARATIONS OF TCL CALLED FUNCTIONS
 *----------------------------------------------------------------------------
 * These are the functions known to Tcl.
 * They are (and should be) wrapped in 'extern "C"' declaration
 *----------------------------------------------------------------------------
 */
int Memobj_Init(Tcl_Interp *interp);
int Memobj_Cmd(ClientData data, Tcl_Interp *interp,
    				int objc, Tcl_Obj *CONST objv[]);
}
int MdscDebug_Cmd(ClientData data, Tcl_Interp *interp,
    				int objc, Tcl_Obj *CONST objv[]);
// END OF EXTERN "C" DECLARATIONS

/*-----------------------------------------------------------------------------
 * MISC CONSTANTS
 *-----------------------------------------------------------------------------
 */

/*
 * Memory objects have names of the form "memobjNNN" where "NNN" is an
 * integer.
 * The exact prefix (i.e. "memobj") is defined by MEMOBJ_NAME_PREFIX
 * and the max name length is MEMOBJ_MAX_NAME_LEN which should be enough
 * to accomodate the MEMOBJ_NAME_PREFIX + number of digits for an integer
 * + the '\0' at the end)
 * NOTE: 64 bit integers have at most 20 digits...
 */
#define MEMOBJ_NAME_PREFIX	"memobj"
#define MEMOBJ_MAX_NAME_LEN	(32)

/*-----------------------------------------------------------------------------
 * TYPE DEFINITIONS
 *-----------------------------------------------------------------------------
 */
// DebugConfStruct: Used for the "mdsc::debug" implementation
typedef struct {
    char *fname;		// file name where we write debug info
    int level;			// debug level
    FILE *fp;			// file descriptor
    int must_close;		// should we close file descriptor?
} DebugConfStruct;

/*-----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *-----------------------------------------------------------------------------
 */

// NOTE: debug info is global for all interpeters. Maybe we should change
// this in the future, but for now it should be ok...
static DebugConfStruct _DebugConf = {NULL, 0, NULL, 0};

static ObjTable MemObjTable;


/*-----------------------------------------------------------------------------
 * STATIC FUNCTIONS (LOCAL TO THIS FILE)
 *-----------------------------------------------------------------------------
 */
static void _memobj_id_to_name(int id, char *buf);
static int _memobj_name_to_id(const char *name);
static int _memobj_create(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_clear(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_size(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_get(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_append(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_delete(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_exists(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_md5sum(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_list(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_load(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_store(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_makerandom(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_copytofile(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _memobj_copyfromfile(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);

/*-----------------------------------------------------------------------------
 * Memobj_Init
 *-----------------------------------------------------------------------------
 * Main initialization function
 *
 * We only register two commands: "mdsc::memobj" and "mdsc::debug"
 *  and we provide package Memobj
 *-----------------------------------------------------------------------------
 */
int Memobj_Init(Tcl_Interp *interp) {

    if (Tcl_InitStubs(interp, "8.4", 0) == NULL) {
	return TCL_ERROR;
    }

    /*
     * Register all commands
     */
    Tcl_CreateObjCommand(interp, "mdsc::memobj",  Memobj_Cmd,
	    (ClientData) NULL, NULL);

    Tcl_CreateObjCommand(interp, "mdsc::debug",  MdscDebug_Cmd,
	    (ClientData) NULL, NULL);

    Tcl_PkgProvide(interp, "Mdsc", "1.0");

    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * Memobj_Cmd
 *-----------------------------------------------------------------------------
 * This is the main entry point for the "memobj" command.
 * The first argument specifies the subcommand, optionally followed
 * by more arguments.
 *
 * Usage:
 *	memobj create				-> a new object
 *	memobj clear <handle>
 *	memobj size <handle>			-> size of object
 *	memobj get <handle> <start> <nbytes>	-> return bytes
 *	memobj append <handle> <bytes>
 *	memobj delete <handle>
 *	memobj exists <handle>
 *	memobj md5sum <handle>
 *	memobj list
 *	memobj load <handle> <fname>
 *	memobj store <handle> <fname>
 *	memobj random <handle> <size> ?<seed>?
 *-----------------------------------------------------------------------------
 */
int Memobj_Cmd(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    // We must have at least one argument (the (sub)command)
    //
    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "cmd ?arg ...");
	return(TCL_ERROR);
    }

    // Get the command
    char *cmd = Tcl_GetString(objv[1]);
    if (!strcmp(cmd, "create")) {
	return _memobj_create(data, interp, objc, objv);
    } else if (!strcmp(cmd, "delete")) {
	return _memobj_delete(data, interp, objc, objv);
    } else if (!strcmp(cmd, "clear")) {
	return _memobj_clear(data, interp, objc, objv);
    } else if (!strcmp(cmd, "size")) {
	return _memobj_size(data, interp, objc, objv);
    } else if (!strcmp(cmd, "get")) {
	return _memobj_get(data, interp, objc, objv);
    } else if (!strcmp(cmd, "append")) {
	return _memobj_append(data, interp, objc, objv);
    } else if (!strcmp(cmd, "md5sum")) {
	return _memobj_md5sum(data, interp, objc, objv);
    } else if (!strcmp(cmd, "exists")) {
	return _memobj_exists(data, interp, objc, objv);
    } else if (!strcmp(cmd, "list")) {
	return _memobj_list(data, interp, objc, objv);
    } else if (!strcmp(cmd, "load")) {
	return _memobj_load(data, interp, objc, objv);
    } else if (!strcmp(cmd, "store")) {
	return _memobj_store(data, interp, objc, objv);
    } else if (!strcmp(cmd, "makerandom")) {
	return _memobj_makerandom(data, interp, objc, objv);
    } else if (!strcmp(cmd, "copytofile")) {
	return _memobj_copytofile(data, interp, objc, objv);
    } else if (!strcmp(cmd, "copyfromfile")) {
	return _memobj_copyfromfile(data, interp, objc, objv);
    } else {
	Tcl_AppendResult(interp, "Invalid command \"", cmd, "\".",
		" Valid commands are: create, delete, clear, size,",
		" exists, md5sum, list, load, store,",
		" get, append,"
		" makerandom, copyfromfile or copytofile.",
		NULL);
	return TCL_ERROR;
    }
}

/*-----------------------------------------------------------------------------
 * _memobj_name_to_id
 * _memobj_id_to_name
 *-----------------------------------------------------------------------------
 * Internally all memory objects are associated with a "key" which is an
 * integer (> 0). This is the API provided by the "ObjTable".
 * However we provide a string to our callers, which is nothing more
 * than the string "memobj" prepended to the id.
 * So for example if the internal representation is "123" in the tclsh
 * the object is known as "memobj123"
 *
 *-----------------------------------------------------------------------------
 * _memobj_id_to_name
 *
 * ARGUMENTS:
 *	id : the (internal) id of the object
 * 	buf: the buffer where we will store the name. It must be
 *		big enough to hold the name (i.e. MEMOBJ_MAX_NAME_LEN bytes
 *		long)
 *-----------------------------------------------------------------------------
 * _memobj_name_to_id
 * ARGUMENTS:
 *	name : the name of the memory object
 * RETURNS:
 *	the internal id if this is a valid name, else -1
 *-----------------------------------------------------------------------------
 */
static void _memobj_id_to_name(int id, char *buf)
{
    sprintf(buf, "%s%d", MEMOBJ_NAME_PREFIX, id);
}

static int _memobj_name_to_id(const char *name)
{
    // the name must start with "MEMOBJ_NAME_PREFIX"
    //
    int l = strlen(MEMOBJ_NAME_PREFIX);
    if (strncmp(name, MEMOBJ_NAME_PREFIX, l)) {
	return -1;
    }
    const char *s = name + l;
    if (*s == '\0') {
	return -1;
    }
    int id = 0;
    while ((*s) >= '0' && (*s) <= '9') {
	id = 10*id + (*s) - '0';
	s++;
    }
    if (*s != '\0') {
	return -1;
    }
    return id;
}

/*-----------------------------------------------------------------------------
 * _memobj_create
 *-----------------------------------------------------------------------------
 * Usage  : memobj create
 * Returns: the name of the new object (a handle)
 *-----------------------------------------------------------------------------
 */
static int _memobj_create(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    // no extra args...
    if (objc != 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "create");
	return TCL_ERROR;
    }

    // Create the new memory object.
    //
    MemObj *obj = new(MemObj);

    // Store it in the hash table and get a "key" (handle)
    // for it (which is an int)
    int key = MemObjTable.create((ClientData)obj);

    // Now construct the handle from the key by prepending
    // the string "memobj" and return it to the caller.
    //
    char name[MEMOBJ_MAX_NAME_LEN];
    _memobj_id_to_name(key, name);

    Tcl_AppendResult(interp, name, NULL);
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_clear
 *-----------------------------------------------------------------------------
 * Clear all data from of an object
 *
 * Usage  : memobj clear <handle>
 * Returns: nothing
 *-----------------------------------------------------------------------------
 */
static int _memobj_clear(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc != 3) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle");
	return TCL_ERROR;
    }


    // Get and pin the object
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock, clear and unlock
    o->lock();
    o->clear();
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_size
 *-----------------------------------------------------------------------------
 * Return the size (number of bytes) of the object
 *
 * Usage  : memobj size <handle>
 * Returns: number of bytes
 *-----------------------------------------------------------------------------
 */
static int _memobj_size(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc != 3) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle");
	return TCL_ERROR;
    }


    // Get and pin the object
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock, get size and unlock
    o->lock();
    size_t sz = (int) o->size();
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    Tcl_Obj *res = Tcl_NewIntObj((int)sz);
    Tcl_SetObjResult(interp, res);
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_get
 *-----------------------------------------------------------------------------
 * Get some bytes from an object
 *
 * Usage  : memobj get <handle> ?<start> ?<nbytes>??
 * Returns: a (binary) string
 *
 * NOTE: if the optional argument <nbytes> is not specified, then
 * we return everything up to and including the last byte.
 * Same thing if equal to ""
 * If the optional argument <start> is not specified, then we start
 * from the first byte.
 * The number of bytes could be less than <nbytes> if we ask for too
 * many bytes. Use [string bytelength ...] to findout how many bytes
 * were returned.
 *-----------------------------------------------------------------------------
 */
static int _memobj_get(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc < 3 || objc > 5) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle ?start ?nbytes??");
	return TCL_ERROR;
    }


    // Parse arguments
    // NOTE: the fourth & fifth args (if present) should be integers
    //
    const char *name = Tcl_GetString(objv[2]);
    int start = 0;
    int nbytes = -1;
    if (objc > 3) {
	if (Tcl_GetIntFromObj(interp, objv[3], &start) != TCL_OK || start < 0) {
	    Tcl_AppendResult(interp, "invalid start offset \"",
	    		Tcl_GetString(objv[3]),
			"\". Must be a valid non-negative integer.",
			NULL);
	    return TCL_ERROR;
	}
    }
    if (objc > 4) {
	// Allow for empty string....
	// Else it must be a non-negative integer.
	const char *tmp = Tcl_GetString(objv[4]);
	if (tmp == NULL || *tmp == '\0') {
	    // allow that (the default value -1 will be used)
	} else if (Tcl_GetIntFromObj(interp, objv[4], &nbytes) != TCL_OK 
			|| nbytes < 0) {
	    Tcl_AppendResult(interp, "invalid number of bytes \"",
	    		Tcl_GetString(objv[3]),
			"\". Must be a valid non-negative integer or ",
			"\"\".", NULL);
	    return TCL_ERROR;
	}
    }

    // Get and pin the object
    //
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock, get and unlock
    // NOTE: if "nbytes == -1" then return all the remaining bytes.
    o->lock();

    // Find how many bytes we will copy
    //
    int n = o->size() - start;
    if (n < 0) {
	n = 0;
    }
    if (nbytes == -1) {
	nbytes = n;
    } else if (nbytes > n) {
	nbytes = n;
    }

    // Allocate a buffer big enough to hold "n" bytes
    //
    unsigned char *buf = (unsigned char *)ckalloc((int)nbytes);
    //printf("XXX ckalloc 1 0x%lx\n", (long)buf);
    if (buf == NULL) {
	// Normally the interpreter should have already panicked!
	// but better safe than sorry....
	o->unlock();
	Tcl_AppendResult(interp, "out of memory", NULL);
	return TCL_ERROR;
    }
    n = (int) o->get(start, nbytes, buf);
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    // Ok, construct the result. Don't forget to free our buffer
    // afterwards!
    //
    Tcl_Obj *res = Tcl_NewByteArrayObj(buf, n);
    Tcl_SetObjResult(interp, res);
    ckfree((char *)buf);
    //printf("XXX ckfree 1 0x%lx\n", (long)buf);

    return TCL_OK;
}

#ifdef OBSOLETE
/*-----------------------------------------------------------------------------
 * _memobj_get
 *-----------------------------------------------------------------------------
 * Get some bytes from an object
 *
 * Usage  : memobj get <handle> <varname> ?<start> ?<nbytes>??
 * Returns: nothing
 *
 * NOTE: if the optional argument <nbytes> is not specified, then
 * we return everything up to and including the last byte.
 * Same thing if equal to ""
 * If the optional argument <start> is not specified, then we start
 * from the first byte.
 * The number of bytes could be less than <nbytes> if we ask for too
 * many bytes. Use [string bytelength ...] to findout how many bytes
 * were returned.
 *-----------------------------------------------------------------------------
 */
static int _memobj_get(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc < 4 || objc > 6) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle varname ?start ?nbytes??");
	return TCL_ERROR;
    }


    // Parse arguments
    // NOTE: the fourth & fifth args (if present) should be integers
    //
    const char *name = Tcl_GetString(objv[2]);
    const char *varname = Tcl_GetString(objv[3]);
    int start = 0;
    int nbytes = -1;
    if (objc > 4) {
	if (Tcl_GetIntFromObj(interp, objv[4], &start) != TCL_OK || start < 0) {
	    Tcl_AppendResult(interp, "invalid start offset \"",
	    		Tcl_GetString(objv[4]),
			"\". Must be a valid non-negative integer.",
			NULL);
	    return TCL_ERROR;
	}
    }
    if (objc > 5) {
	// Allow for empty string....
	// Else it must be a non-negative integer.
	const char *tmp = Tcl_GetString(objv[5]);
	if (tmp == NULL || *tmp == '\0') {
	    // allow that (the default value -1 will be used)
	} else if (Tcl_GetIntFromObj(interp, objv[5], &nbytes) != TCL_OK 
			|| nbytes < 0) {
	    Tcl_AppendResult(interp, "invalid number of bytes \"",
	    		Tcl_GetString(objv[5]),
			"\". Must be a valid non-negative integer or ",
			"\"\".", NULL);
	    return TCL_ERROR;
	}
    }

    // Get and pin the object
    //
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock, get and unlock
    // NOTE: if "nbytes == -1" then return all the remaining bytes.
    o->lock();

    // Find how many bytes we will copy
    //
    int n = o->size() - start;
    if (n < 0) {
	n = 0;
    }
    if (nbytes == -1) {
	nbytes = n;
    } else if (nbytes > n) {
	nbytes = n;
    }

    // Allocate a buffer big enough to hold "n" bytes
    //
    unsigned char *buf = (unsigned char *)ckalloc((int)nbytes);
    //printf("XXX ckalloc 2 0x%lx\n", (long)buf);
    if (buf == NULL) {
	// Normally the interpreter should have already panicked!
	// but better safe than sorry....
	o->unlock();
	Tcl_AppendResult(interp, "out of memory", NULL);
	return TCL_ERROR;
    }
    n = (int) o->get(start, nbytes, buf);
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    // Ok, construct the result
    //
    Tcl_Obj *res = Tcl_NewByteArrayObj(buf, n);
    ckfree((char *)buf);
    //printf("XXX ckfree 2 0x%lx\n", (long)buf);
    Tcl_Obj *tmp = Tcl_SetVar2Ex(interp, varname, NULL, res, TCL_LEAVE_ERR_MSG);
    if (tmp == NULL) {
	return TCL_ERROR;
    }
    //Tcl_SetObjResult(interp, res);

    return TCL_OK;
}
#endif /* OBSOLETE */

/*-----------------------------------------------------------------------------
 * _memobj_append
 *-----------------------------------------------------------------------------
 * Append the given bytes (as a binary object) to the memory object
 * If the optional argument "start" is given, then we copy only the bytes
 * after this offset (i.e. we skip that many bytes from the beginning
 * of the string)
 *
 * Usage  : memobj append <handle> <binarystring> ?<start> ?<nbytes>??
 * Returns: number of bytes copied
 *-----------------------------------------------------------------------------
 */
static int _memobj_append(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc < 4 || objc > 6) {
	Tcl_WrongNumArgs(interp, 2, objv,
	    "handle binarystring ?startoffset ?nbytes??");
	return TCL_ERROR;
    }

    // Parse arguments
    const char *name = Tcl_GetString(objv[2]);
    int buflen;
    const unsigned char *buf = Tcl_GetByteArrayFromObj(objv[3], &buflen);

    int startoffset = 0;
    if (objc >= 5) {
	if (Tcl_GetIntFromObj(interp, objv[4], &startoffset) != TCL_OK 
			|| startoffset < 0) {
	    Tcl_AppendResult(interp, "invalid start offset \"",
	    		Tcl_GetString(objv[4]),
			"\". Must be a valid non-negative integer.",
			NULL);
	    return TCL_ERROR;
	}
    }
    int nbytes = -1;	// negative means all the bytes
    if (objc >= 6) {
	if (Tcl_GetIntFromObj(interp, objv[5], &nbytes) != TCL_OK) {
	    Tcl_AppendResult(interp, "invalid number of bytes\"",
	    		Tcl_GetString(objv[5]),
			"\". Must be a valid integer.",
			NULL);
	    return TCL_ERROR;
	}
    }

    // Readjust "startoffset" and "nbytes" depending on the actual
    // number of bytes in the object...
    //
    if (startoffset > buflen) {
	startoffset = buflen;
    }
    if (nbytes < 0) {
	// A negative value means all remaining bytes of string...
	nbytes = buflen - startoffset;
    } else {
	if (startoffset + nbytes > buflen) {
	    nbytes = buflen - startoffset;
	}
    }

    // Get and pin the object
    //
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }
    if (nbytes > 0) {
	// Lock, append and unlock
	o->lock();
	o->append(buf + startoffset, (size_t)nbytes);
	o->unlock();
    }

    // Unpin the object
    MemObjTable.unpin(key);

    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_delete
 *-----------------------------------------------------------------------------
 * Delete the given memory object.
 *
 * Usage  : memobj delete <handle>
 * Returns: nothing
 *-----------------------------------------------------------------------------
 */
static int _memobj_delete(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{

    if (objc != 3) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle");
	return TCL_ERROR;
    }


    // Delete the object.
    // NOTE:we use a synchronous deletion...
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);

    MemObj *o = (MemObj *) MemObjTable.delete_sync(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // At this point, "o" points to the memory object which is
    // not longer used by anyone so it is safe to free it.
    // If someone else later on attempts to use this handle again
    // he/she/it will get a "can not find memory object ...." message.
    //
    delete o;
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_md5sum
 *-----------------------------------------------------------------------------
 * Calculate the md5 sum of the data of the mem object
 *
 * Usage  : memobj md5sum <handle>
 * Returns: Checksum (as a string - 32 hex digits)
 *-----------------------------------------------------------------------------
 */
static int _memobj_md5sum(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc != 3) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle");
	return TCL_ERROR;
    }


    // Get and pin the object
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock, get size and unlock
    std::string md5sum;
    o->lock();
    md5sum = o->md5sum();
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    Tcl_AppendResult(interp, md5sum.c_str(), NULL);
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_exists
 *-----------------------------------------------------------------------------
 * Return 1 if the given object exists, 0 otherwise
 *
 * Usage  : memobj exists <handle>
 * Returns: 0 or 1
 *-----------------------------------------------------------------------------
 */
static int _memobj_exists(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc != 3) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle");
	return TCL_ERROR;
    }


    // Get the object (no need to pin it!)
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    int exists;
    MemObj *o = (MemObj *) MemObjTable.find(key);
    if (o == NULL) {
	exists = 0;
    } else {
	exists = 1;
    }

    Tcl_Obj *res = Tcl_NewIntObj(exists);
    Tcl_SetObjResult(interp, res);
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_list
 *-----------------------------------------------------------------------------
 * Return a list with all the memory objects
 *
 * Usage  : memobj list
 * Returns: A list of all memory object handlers
 *-----------------------------------------------------------------------------
 */
static int _memobj_list(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{

    if (objc != 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "list");
	return TCL_ERROR;
    }

    // Get a list of all object ids.
    std::vector<int> keys = MemObjTable.list();
    int nkeys = keys.size();

    // Construct a list to return
    //
    int i;
    char name[MEMOBJ_MAX_NAME_LEN];
    Tcl_Obj **objlist = NULL;
    if (nkeys > 0) {
	objlist = (Tcl_Obj **)ckalloc(nkeys * sizeof(Tcl_Obj *));
	//printf("XXX ckalloc 3 0x%lx\n", (long)objlist);
	if (objlist == NULL) {
	    // Normally the interpreter should have already panicked!
	    // but better safe than sorry....
	    Tcl_AppendResult(interp, "out of memory", NULL);
	    return TCL_ERROR;
	}
    }
    for (i=0; i<nkeys; i++) {
	_memobj_id_to_name(keys[i], name);
	Tcl_Obj *element = Tcl_NewStringObj(name, -1);
	objlist[i] = element;
    }
    Tcl_Obj *res = Tcl_NewListObj(nkeys, objlist);
    Tcl_SetObjResult(interp, res);
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_load
 *-----------------------------------------------------------------------------
 * Load (synchronoysly) an object from a file. Used mainly for debugging.
 *
 * Usage  : memobj load <handle> <fname>
 * Returns: nothing
 *-----------------------------------------------------------------------------
 */
static int _memobj_load(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc != 4) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle fname");
	return TCL_ERROR;
    }

    // Get fname
    const char *fname = Tcl_GetString(objv[3]);

    // Get and pin the object
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    int retcode = TCL_OK;

    // Lock the object, clear it and load file
    o->lock();
    int status = o->load(fname, false);
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    if (status) {
	// There was an error
	Tcl_AppendResult(interp, "Could not load file \"", fname, "\"", NULL);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_store
 *-----------------------------------------------------------------------------
 * Store (synchronoysly) an object to a file. Used mainly for debugging.
 *
 * Usage  : memobj store <handle> <fname>
 * Returns: nothing
 *-----------------------------------------------------------------------------
 */
static int _memobj_store (ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc != 4) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle fname");
	return TCL_ERROR;
    }

    // Get fname
    const char *fname = Tcl_GetString(objv[3]);

    // Get and pin the object
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    int retcode = TCL_OK;

    // Lock the object, clear it and load file
    o->lock();
    int status = o->store(fname);
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    if (status) {
	// There was an error
	Tcl_AppendResult(interp, "Could not store to file \"",
				fname, "\"", NULL);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_makerandom
 *-----------------------------------------------------------------------------
 * Generate radom data for an object
 *
 * Usage  : memobj random <handle> <size> ?<seed>?
 * Returns: nothing
 *-----------------------------------------------------------------------------
 */
static int _memobj_makerandom(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc < 4 || objc > 5 ) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle size ?seed?");
	return TCL_ERROR;
    }

    // Get size and seed
    int size;
    if (Tcl_GetIntFromObj(interp, objv[3], &size) != TCL_OK || size < 0) {
	Tcl_AppendResult(interp, "invalid object size \"",
		    Tcl_GetString(objv[3]),
		    "\". Must be a valid non-negative integer.",
		    NULL);
	return TCL_ERROR;
    }

    int seed;
    if (objc == 5) {
	if (Tcl_GetIntFromObj(interp, objv[4], &seed) != TCL_OK) {
	    Tcl_AppendResult(interp, "invalid seed \"",
	    		Tcl_GetString(objv[4]),
			"\". Must be a valid integer.",
			NULL);
	    return TCL_ERROR;
	}
    } else {
	seed = 0;
    }
    //
    // Get and pin the object
    //
    const char *name = Tcl_GetString(objv[2]);
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock, randomize and unlock
    o->lock();
    o->makerandom((size_t)size, (unsigned int) seed);
    o->unlock();

    // Unpin the object
    MemObjTable.unpin(key);

    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_copyfromfile
 *-----------------------------------------------------------------------------
 * Copy a chunk of data from an already open channel to our
 * object. This is a synchronous read.
 *
 * Usage  : memobj random <handle> <channel> ?<nbytes>?
 * 	If the optional <nbytes> argument is not given or is negative,
 * 	then we copy until EOF
 * Returns: number of bytes copied (could be less than the requested
 * number of bytes)
 *-----------------------------------------------------------------------------
 */
static int _memobj_copyfromfile(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc < 4 || objc > 5) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle channel ?nbytes?");
	return TCL_ERROR;
    }

    // Parse arguments
    const char *name = Tcl_GetString(objv[2]);
    const char *channelname = Tcl_GetString(objv[3]);

    int nbytes = -1;	// negative means read until EOF
    if (objc >= 5) {
	if (Tcl_GetIntFromObj(interp, objv[4], &nbytes) != TCL_OK) {
	    Tcl_AppendResult(interp, "invalid number of bytes\"",
	    		Tcl_GetString(objv[4]),
			"\". Must be a valid integer.",
			NULL);
	    return TCL_ERROR;
	}
    }

    // Get the channel
    //
    int mode;
    Tcl_Channel chan = Tcl_GetChannel(interp, channelname, &mode);
    if (chan == NULL) {
	return TCL_ERROR;
    }

    // Get and pin the object
    //
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }
    
    // Do the read
    //
    int nread = 0;
    unsigned char buf[1024*1024];
    o->lock();
    int error_found = 0;
    while (1) {
	// break loop if EOF or we have read all the bytes
	//
	if (Tcl_Eof(chan)) {
	    break;
	}
	if (nbytes >= 0 && nread >= nbytes) {
	    break;
	}
	
	// Read a chunk of bytes
	//
	int n_to_read;
	if (nbytes < 0) {
	    n_to_read = sizeof(buf);
	} else {
	    n_to_read = nbytes - nread;
	    if (n_to_read > sizeof(buf)) {
		n_to_read = sizeof(buf);
	    }
	}
	int n1 = Tcl_Read(chan, (char *)buf, n_to_read);
	if (n1 < 0) {
	    // Ooops! I/O error
	    error_found = 1;
	    Tcl_AppendResult(interp, "Error while reading channel \"",
		    	channelname, "\": ", Tcl_ErrnoMsg(Tcl_GetErrno()),
			" (", Tcl_ErrnoId(), ")", NULL);
	    break;
	} else if (n1 == 0) {
	    // either EOF or no data available (if in non blocking mode)
	    // return...
	    break;
	} else {
	    // n1 > 0, some bytes were read
	    o->append(buf, (size_t)n1);
	    nread += n1;
	}
    }
    o->unlock();
    // Unpin the object
    MemObjTable.unpin(key);

    if (error_found) {
	// The error message is already stored in the interpreter...
	return TCL_ERROR;
    }

    // Store number of bytes read in interpreter
    Tcl_Obj *res = Tcl_NewIntObj(nread);
    Tcl_SetObjResult(interp, res);
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _memobj_copytofile
 *-----------------------------------------------------------------------------
 * Copy a chunk of data from a memory object to a channel
 * This is a synchronous write.
 *
 * Usage  : memobj get <handle> <channel> ?<start> ?<nbytes>??
 * Returns: nothing
 * Usage  : memobj random <handle> <channel> ?<nbytes>?
 * 	If the optional <nbytes> argument is not given or is negative,
 * 	then we copy until EOF
 * Returns: number of bytes copied (could be less than the requested
 * number of bytes)
 *-----------------------------------------------------------------------------
 */
static int _memobj_copytofile(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    if (objc < 4 || objc > 6) {
	Tcl_WrongNumArgs(interp, 2, objv, "handle channel ?start ?nbytes??");
	return TCL_ERROR;
    }


    // Parse arguments
    // NOTE: the fourth & fifth args (if present) should be integers
    //
    const char *name = Tcl_GetString(objv[2]);
    const char *channelname = Tcl_GetString(objv[3]);

    int start = 0;
    int nbytes = -1;
    if (objc > 4) {
	if (Tcl_GetIntFromObj(interp, objv[4], &start) != TCL_OK || start < 0) {
	    Tcl_AppendResult(interp, "invalid start offset \"",
	    		Tcl_GetString(objv[4]),
			"\". Must be a valid non-negative integer.",
			NULL);
	    return TCL_ERROR;
	}
    }
    if (objc > 5) {
	// Allow for empty string....
	// Else it must be a non-negative integer.
	const char *tmp = Tcl_GetString(objv[5]);
	if (tmp == NULL || *tmp == '\0') {
	    // allow that (the default value -1 will be used)
	    nbytes = -1;
	} else if (Tcl_GetIntFromObj(interp, objv[5], &nbytes) != TCL_OK 
			|| nbytes < 0) {
	    Tcl_AppendResult(interp, "invalid number of bytes \"",
	    		Tcl_GetString(objv[5]),
			"\". Must be a valid non-negative integer or ",
			"\"\".", NULL);
	    return TCL_ERROR;
	}
    }

    // Get the channel
    //
    int mode;
    Tcl_Channel chan = Tcl_GetChannel(interp, channelname, &mode);
    if (chan == NULL) {
	return TCL_ERROR;
    }

    // Get and pin the object
    //
    int key = _memobj_name_to_id(name);
    MemObj *o = (MemObj *) MemObjTable.pin(key);
    if (o == NULL) {
	Tcl_AppendResult(interp, "can not find memory object \"",
			name, "\"", NULL);
	return TCL_ERROR;
    }

    // Lock object
    o->lock();

    // Find how many bytes we will copy
    // Update 'nbytes' accordingly
    //
    int n = o->size() - start;
    if (n < 0) {
	n = 0;
    }
    if (nbytes == -1) {
	nbytes = n;
    } else if (nbytes > n) {
	nbytes = n;
    }

    // Start writing to channel
    //
    int nwritten = 0;
    int error_found = 0;
    while (nwritten < nbytes) {
	// Get the pointer to the data
	// 'maxbytes' will tell us how many bytes are contained
	// in the returned buffer.
	//
	size_t maxbytes;
	unsigned char *ptr = o->getpointer(start + nwritten, &maxbytes);
	if (ptr == NULL || maxbytes == 0) {
	    // no more bytes in object
	    break;
	}
	int n1 = Tcl_Write(chan, (char *)ptr, maxbytes);
	if (n1 < 0) {
	    // Ooops! I/O error
	    error_found = 1;
	    Tcl_AppendResult(interp, "Error while writing channel \"",
		    	channelname, "\": ", Tcl_ErrnoMsg(Tcl_GetErrno()),
			" (", Tcl_ErrnoId(), ")", NULL);
	    break;
	} else if (n1 == 0) {
	    // Is this supposed to happen?
	    break;
	} else {
	    // n1 > 0, some bytes were written
	    nwritten += n1;
	}
    }
    o->unlock();
    // Unpin the object
    MemObjTable.unpin(key);

    if (error_found) {
	// The error message is already stored in the interpreter...
	return TCL_ERROR;
    }

    // Store number of bytes written in interpreter
    Tcl_Obj *res = Tcl_NewIntObj(nwritten);
    Tcl_SetObjResult(interp, res);
    return TCL_OK;
}

/*============================================================================
 * Implementation of "mdsc::debug"
 *============================================================================
 */

/*-----------------------------------------------------------------------------
 * MISC CONSTANTS
 *-----------------------------------------------------------------------------
 */


/*-----------------------------------------------------------------------------
 * STATIC FUNCTIONS (LOCAL TO THIS FILE)
 *-----------------------------------------------------------------------------
 */
static int _mdscdebug_configure(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[]);
static int _mdscdebug_print(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[], int level);

/*-----------------------------------------------------------------------------
 * MdscDebug_Cmd
 *-----------------------------------------------------------------------------
 * This is the main entry point for the "mdsc::debug" command.
 *
 * Usage:
 * 	mdsc::debug <level> <msg> ?<msg2> ...?
 * 		where "<level>" is an integer. In this case we
 * 		print a debuging message using this "level" of
 * 		debugging verbosity. The remaining arguments are
 * 		concatenated (with a " " between them) to specify
 * 		the message to print.
 *
 * 	mdsc::debug configure
 * 	mdsc::debug configure attr
 * 	mdsc::debug configure attr val ?attr2 val2 ...?
 * 		Set/get the value(s) of one or more attributes
 * 		(see below). If no arguments are specified then
 * 		we return the value of all attributes (as an
 * 		attribute/value list). If a single argument is
 * 		specified then it must be an attribute and we return
 * 		its value. Else we must specify one or more pairs
 * 		of attributes/values
 *
 * 	ATTRIBUTES:
 * 	-level <integer>
 * 		the debugging level
 * 	-file <fname>
 * 		The file where we write the debug messages.
 * 		An empty file name means no debugging.
 * 		The special name "stderr" and "stdout" mean
 * 		std error and output...
 *-----------------------------------------------------------------------------
 */
int MdscDebug_Cmd(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    int status;
    // We must have at least one argument (the (sub)command)
    //
    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "cmd_or_level ?arg ...");
	return(TCL_ERROR);
    }

    // Get the command
    char *cmd = Tcl_GetString(objv[1]);
    if (!strcmp(cmd, "configure")) {
	status = _mdscdebug_configure(data, interp, objc, objv);
	return status;
    } else {
	// It must be an integer (a debugging level)
	//
	int level;
	if (Tcl_GetIntFromObj(NULL, objv[1], &level) != TCL_OK) {
	    Tcl_AppendResult(interp, "Invalid argument \"", cmd, "\".",
		    " It must be a debugging level (an integer) or the"
		    " keyword \"configure\".",
		    NULL);
	    return TCL_ERROR;
	}
	// Print the debugging message
	status = _mdscdebug_print(data, interp, objc, objv, level);
	return status;
    }
}

/*-----------------------------------------------------------------------------
 * _mdscdebug_configure
 *-----------------------------------------------------------------------------
 * Usage  : mdsc::debug configure
 *-----------------------------------------------------------------------------
 */
static int _mdscdebug_configure(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[])
{
    // If no args, then just return all the configuration values
    // as an attribute/value list
    //
    if (objc == 2) {
	Tcl_Obj *objlist[10];
	int n = 0;
	objlist[n++] = Tcl_NewStringObj("-file", -1);
	if (_DebugConf.fname == NULL) {
	    objlist[n++] = Tcl_NewStringObj("", -1);
	} else {
	    objlist[n++] = Tcl_NewStringObj(_DebugConf.fname, -1);
	}
	objlist[n++] = Tcl_NewStringObj("-level", -1);
	objlist[n++] = Tcl_NewIntObj(_DebugConf.level);
	Tcl_Obj *res = Tcl_NewListObj(n, objlist);
	Tcl_SetObjResult(interp, res);
	return TCL_OK;
    }

    // If we have one argument only then we want the value
    // of this attribute
    if (objc == 3) {
	const char *what = Tcl_GetString(objv[2]);
	if (!strcmp(what, "-level")) {
	    Tcl_Obj *res = Tcl_NewIntObj(_DebugConf.level);
	    Tcl_SetObjResult(interp, res);
	    return TCL_OK;
	} else if (!strcmp(what, "-file")) {
	    Tcl_Obj *res;
	    if (_DebugConf.fname == NULL) {
		res = Tcl_NewStringObj("", -1);
	    } else {
		res = Tcl_NewStringObj(_DebugConf.fname, -1);
	    }
	    Tcl_SetObjResult(interp, res);
	    return TCL_OK;
	} else {
	    Tcl_AppendResult(interp, "Invalid option \"", what,
		    "\". Must be one of \"-file\" or \"-level\".",
		    NULL);
	    return TCL_ERROR;
	}
    }
    
    // If we arrive here, then the args must be even in number
    // and of the form "attr1 val1 ?attr2 val2 ...?"
    //
    int i;
    for (i=2; i<objc; i+=2) {
	const char *what = Tcl_GetString(objv[i]);
	if (!strcmp(what, "-level")) {
	    if (i+1 >= objc) {
		Tcl_AppendResult(interp,
			"value for \"", what, "\" missing", NULL);
		return TCL_ERROR;
	    }
	    int level;
	    if (Tcl_GetIntFromObj(interp, objv[i+1], &level) != TCL_OK) {
		const char *tmp = Tcl_GetString(objv[i+1]);
		Tcl_AppendResult(interp, "Invalid value \"", tmp, 
			"\" for the \"-level\" option.",
			" Was expecting an integer.",
			NULL);
		return TCL_ERROR;
	    }
	    _DebugConf.level = level;
	    DebugSetLevel(level);
	} else if (!strcmp(what, "-file")) {
	    if (i+1 >= objc) {
		Tcl_AppendResult(interp,
			"value for \"", what, "\" missing", NULL);
		return TCL_ERROR;
	    }
	    const char *fname = Tcl_GetString(objv[i+1]);
	    FILE *fp;
	    int must_close = 0;
	    if (fname == NULL || *fname == '\0') {
		fname = "";
		fp = NULL;
		must_close = 0;
	    } else if (!strcmp(fname, "stderr")) {
		fp = stderr;
		must_close = 0;
	    } else if (!strcmp(fname, "stdout")) {
		fp = stdout;
		must_close = 0;
	    } else {
		// Open file name
		fp = fopen(fname, "w");
		if (fp == NULL) {
		    Tcl_AppendResult(interp, "Can not open file \"",
			    fname, "\": ", strerror(errno), NULL);
		    return TCL_ERROR;
		}
		must_close = 1;
	    }
	    // Close any previously open debug file
	    // and free the space occupied by the file name
	    if (_DebugConf.must_close && _DebugConf.fp != NULL) {
		fclose(_DebugConf.fp);
	    }
	    if (_DebugConf.fname != NULL) {
		ckfree(_DebugConf.fname);
		_DebugConf.fname = NULL;
	    }
	    // set new values
	    DebugSetFp(fp);
	    _DebugConf.fp = fp;
	    if (fname == NULL) {
		_DebugConf.fname = NULL;
	    } else {
		_DebugConf.fname = ckalloc(strlen(fname) + 1);
		strcpy(_DebugConf.fname, fname);
	    }
	    _DebugConf.must_close = must_close;
	} else {
	    Tcl_AppendResult(interp, "Unknown option \"", what,
		    "\". Must be one of \"-file\" or \"-level\".",
		    NULL);
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*-----------------------------------------------------------------------------
 * _mdscdebug_print
 *-----------------------------------------------------------------------------
 * Usage  : mdsc::debug level msg ?msg2 ...?
 *
 * Print the debuging message (if the level is less or equal to the level
 * specified with a previous "mdsc::configure -level ..." command
 *-----------------------------------------------------------------------------
 */
static int _mdscdebug_print(ClientData data, Tcl_Interp *interp, 
				int objc, Tcl_Obj *CONST objv[],
				int level)
{
    // Bail out if we are not supposed to print the message
    // (Normally "Debug" does the same checks, but thus we avoid
    // constructing the strings if not neccessary
    //
    if (_DebugConf.fp == NULL || level > _DebugConf.level) {
	return TCL_OK;
    }

    // Concatenate the arguments to a string (with a " " in between them
    // 
    std::string msg;
    int i;
    for (i=2; i<objc; i++) {
	if (i > 2) {
	    msg = msg + " ";
	}
	const char *s = Tcl_GetString(objv[i]);
	msg = msg + s;
    }
    Debug(level, "%s\n", msg.c_str());
    return TCL_OK;
}
