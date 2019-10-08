
/*=============================================================================
 * FILE: memobj.h
 *=============================================================================
 * Declarations of the infamous Memory Object.
 *=============================================================================
 */
#ifndef MEMOBJ_DOT_H
#define MEMOBJ_DOT_H

#include <string>

#include <stdio.h>
#include <tcl.h>

/*-----------------------------------------------------------------------------
 * MemObjException
 *-----------------------------------------------------------------------------
 */
/*-----------------------------------------------------------------------------
 * CLASS: MemObj
 *-----------------------------------------------------------------------------
 */
class MemObj  {
    public:

	//---------------------------------------------------------------------
	// MemObjException
	//---------------------------------------------------------------------
	// This exception is thrown when something goes wrong
	//---------------------------------------------------------------------
	class MemObjException {
	    public:
		MemObjException(const std::string &msg) {
		    fprintf(stderr, "MemObjException: %s\n", msg.c_str());
		}
	    private:
		std::string _msg;	// the error message
	};

	MemObj();
	~MemObj();
	size_t size();
	size_t get(size_t start, size_t nbytes, unsigned char *buf);
	void append(const unsigned char *buf, size_t nbytes);
	void clear();
	void lock();
	void unlock();

	// misc utility functions
	//
	int load(const char *fname, bool appendflag = false);
	int load(FILE *fp, bool appendflag = false);
	int store(const char *fname);
	int store(FILE *fp);
	std::string md5sum();

	void makerandom(size_t size);
	void makerandom(size_t size, unsigned int seed);

	unsigned char *getpointer(size_t offset, size_t *nbytesptr);

    private:
	size_t _nbytes;		// total number of bytes in object
	Tcl_Mutex _mux;		// used to lock/unlock object

	// _Buffer: This is a private class used to hold
	// one "buffer" of data
	typedef struct {
	    size_t startoffset;		// offset in memobj of first byte
	    size_t endoffset;		// offset of last byte in buffer
	    size_t maxbytes;		// max number of bytes
	    size_t nbytes;		// actual number of bytes stored
	    unsigned char *bytes;	// the bytes!
	} _BufferStruct;
	void _buffer_add(size_t minsize);
	std::string _debug(void);

	_BufferStruct *_buffers;
	int _nbuffers;			// number of buffers
	int _maxbuffers;		// size of "_buffers" array
};

#endif /* MEMOBJ_DOT_H */
