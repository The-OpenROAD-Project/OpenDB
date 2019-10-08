/* ATHENA DESIGN SYSTEMS, 2005.*/

/*=============================================================================
 * FILE: memobj.cpp
 *=============================================================================
 * Implementation of the infamous Memory Object.
 *=============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Debug.h"
#include "MemObj.h"
#include "Md5.h"

// NBUFFERS_STEP: how many buffers to allocate next time round
// BUFFER_SIZE  : buffer size is always a multiple of this
#define	NBUFFERS_STEP	100
#define BUFFER_SIZE_STEP (1024*1024)

#define DEBUG_LEVEL 3		// This is our "normal" debugging level

/*-----------------------------------------------------------------------------
 * MemObj::MemObj -- CONSTRUCTOR
 *-----------------------------------------------------------------------------
 * Construct the object
 *-----------------------------------------------------------------------------
 */
MemObj::MemObj()
{
    Debug(DEBUG_LEVEL, ">MemObj::CONSTRUCTOR this=0x%lx\n", (long) this);
    _nbytes = 0;
    _mux = NULL;

    _buffers = NULL;
    _nbuffers = 0;
    _maxbuffers = 0;
    Debug(DEBUG_LEVEL, "<MemObj::CONSTRUCTOR this=0x%lx\n", (long) this);
}

/*-----------------------------------------------------------------------------
 * MemObj::~MemObj -- DESTRUCTOR
 *-----------------------------------------------------------------------------
 */
MemObj::~MemObj()
{
    Debug(DEBUG_LEVEL, "<MemObj::DESTRUCTOR this=0x%lx\n", (long) this);

    // Free buffers
    this->clear();

    // free mux
    Tcl_MutexFinalize(&_mux);

    Debug(DEBUG_LEVEL, "<MemObj::DESTRUCTOR this=0x%lx\n", (long) this);
}

/*-----------------------------------------------------------------------------
 * MemObj::lock
 * MemObj::unlock
 *-----------------------------------------------------------------------------
 * Used to lock/unlock the object
 *-----------------------------------------------------------------------------
 */
void MemObj::lock(void)
{
    Debug(DEBUG_LEVEL, ">MemObj::lock this=0x%lx\n", (long)this);
    Tcl_MutexLock(&_mux);
    Debug(DEBUG_LEVEL, "<MemObj::lock this=0x%lx\n", (long)this);
}
void MemObj::unlock(void)
{
    Debug(DEBUG_LEVEL, ">MemObj::unlock this=0x%lx\n", (long)this);
    Tcl_MutexUnlock(&_mux);
    Debug(DEBUG_LEVEL, "<MemObj::unlock this=0x%lx\n", (long)this);
}

/*-----------------------------------------------------------------------------
 * MemObj::size
 *-----------------------------------------------------------------------------
 * Return the size of the object (number of bytes)
 *-----------------------------------------------------------------------------
 */
size_t MemObj::size()
{
    Debug(DEBUG_LEVEL, ">MemObj::size this=0x%lx\n", (long)this);
    Debug(DEBUG_LEVEL,
	    "<MemObj::size this=0x%lx size=%ld\n", (long)this, (long)_nbytes);
    return _nbytes;
}

/*-----------------------------------------------------------------------------
 * MemObj::clear
 *-----------------------------------------------------------------------------
 * Clear everything...
 * Free memory and reset the object to zero length
 *-----------------------------------------------------------------------------
 */
void MemObj::clear(void)
{
    Debug(DEBUG_LEVEL, ">MemObj::clear this=0x%lx\n", (long)this);

    // Free all allocated buffers
    //
    int i;
    for (i=0; i<_nbuffers; i++) {
	if (_buffers[i].bytes != NULL) {
	    free(_buffers[i].bytes);
	}
    }

    // Free array of buffers
    //
    if (_buffers != NULL) {
	free(_buffers);
    }
    _nbytes = 0;
    _nbuffers = 0;
    _maxbuffers = 0;
    _buffers = NULL;
    Debug(DEBUG_LEVEL, "<MemObj::clear this=0x%lx\n", (long)this);
}

/*-----------------------------------------------------------------------------
 * MemObj::get
 *-----------------------------------------------------------------------------
 * Return a chunk of bytes into the given buffer
 * ARGUMENTS:
 *	start : start address (address of first byte to be returned)
 *	nbytes: how many bytes to return (must be >= 0)
 *	buf   : a buffer where the bytes will be copied. Must be big
 *		enough to accomodate for the number of bytes requested
 * RETURNS:
 *	Number of bytes copied. This can be less than the number
 *	of bytes requested, if (and only if) the arguments "start"
 *	and "nbytes" specify a range outside the current object's size
 *-----------------------------------------------------------------------------
 */
size_t MemObj::get(size_t start, size_t nbytes, unsigned char *buf)
{
    Debug(DEBUG_LEVEL,
	    ">MemObj::get this=0x%lx start=%ld nbytes=%ld  buf=%0xlx\n",
    	(long)this, (long)start, (long)nbytes, (long)buf);

    size_t my_nbytes;	// number of bytes that we will actually copy

    // "Crop" any requests that are outside the available bytes
    if (start >= _nbytes) {
	my_nbytes = 0;
    } else if (start + nbytes > _nbytes) {
	my_nbytes = _nbytes - start;
    } else {
	my_nbytes = nbytes;
    }

    // Do the copy
    unsigned char *dst = buf;	// where to copy next byte
    size_t offset = start;	// offset (in memobj) of next byte to copy
    size_t nleft = my_nbytes;	// number of bytes left to copy
    int i;
    for (i=0; i<_nbuffers && nleft > 0; i++) {
	if (_buffers[i].endoffset < start) {
	    // this buffer is before the first byte we want to copy
	    continue;
	}

	// src: pointer (in this buffer) of the first byte to copy
	unsigned char *src;
	src = _buffers[i].bytes + (offset - _buffers[i].startoffset);

	// n : number of bytes to copy from this buffer
	size_t n = _buffers[i].endoffset - offset + 1;
	if (n > nleft) {
	    n = nleft;
	}

	// do the copy
	memcpy(dst, src, n);
	dst += n;
	nleft -= n;
    }

    Debug(DEBUG_LEVEL,
	"<MemObj::get this=0x%lx start=%ld nbytes=%ld  buf=%0xlx return=%ld\n",
    	(long)this, (long)start, (long)nbytes, (long)buf, (long)my_nbytes);
    return my_nbytes;
}

/*-----------------------------------------------------------------------------
 * MemObj::append
 *-----------------------------------------------------------------------------
 * Append the given bytes to the data already contained in the memory object
 *
 * ARGUMENTS:
 *	buf: the buffer containing the data to be appended
 *	nbytes: number of bytes to append
 * ERRORS:
 * 	If we can not allocate enough memory we raise an exception
 * 	NOTE: in this case the memory object is not modified at all
 *-----------------------------------------------------------------------------
 */
void MemObj::append(const unsigned char *buf, size_t nbytes)
{

    Debug(DEBUG_LEVEL, ">MemObj::append this=0x%lx nbytes=%ld  buf=0x%lx\n",
    	(long)this, (long)nbytes, (long)buf);

    // NOTE: if we need to allocate more memory, do it now,
    // before updating the object. The reason is that if we fail
    // to allocate memory, we want to raise an exception and
    // *leave the object umodified*.
    // This makes the code a little bit more complicated but...

    // first of all: check the last buffer (if any!) and
    // see if there is some space there...
    size_t last_buf_space = 0;
    if (_nbuffers > 0) {
	_BufferStruct *b = &(_buffers[_nbuffers-1]);
	last_buf_space = b->maxbytes - b->nbytes;
    }
    Debug(DEBUG_LEVEL + 1,
	    "=MemObj::append this=0x%lx last_buf_space=%ld\n", last_buf_space);

    // If this space is not enough, allocate an extra buffer
    // big enough to contain the new data
    //
    if (nbytes > last_buf_space) {
	_buffer_add(nbytes - last_buf_space);
    }
    
    // If we arrive here, we have allocated all the memory we need
    // so we don't have the risk to have out of memory exceptions.
    // Go ahead and update the object...
    
    const unsigned char *src = buf;	// next byte to copy
    size_t nleft = nbytes;	// number of bytes left to copy

    // Did we have some space left in the "last" buffer?
    // If so fill it up.
    if (last_buf_space > 0) {
	_BufferStruct *b = &(_buffers[_nbuffers-1]);
	size_t n = nleft;
	if (n > last_buf_space) {
	    n = last_buf_space;
	}
	memcpy(b->bytes + b->nbytes, src, n);
	b->nbytes += n;
	b->endoffset += n;
	nleft -= n;
	src += n;
	_nbytes += n;
    }

    // are there more bytes to copy in the "next" buffer?
    //
    if (nleft > 0) {
	// initialize next buffer
	//
	_nbuffers += 1;
	_BufferStruct *b = &(_buffers[_nbuffers-1]);
	memcpy(b->bytes, src, nleft);
	if (_nbuffers == 1) {
	    b->startoffset = 0;
	} else {
	    b->startoffset = _buffers[_nbuffers-2].endoffset + 1;
	}
	b->nbytes = nleft;
	b->endoffset = b->startoffset + nleft - 1;
	_nbytes += nleft;
	Debug(DEBUG_LEVEL+5,
		"=MemObj::append newbuffer %d off=%ld-%ld nb=%ld/%ld\n",
		_nbuffers, b->startoffset, b->endoffset,
		b->nbytes, b->maxbytes);
    }

    Debug(DEBUG_LEVEL, "<MemObj::append this=0x%lx nbytes=%ld  buf=0x%lx\n",
    	(long)this, (long)nbytes, (long)buf);
}

/*-----------------------------------------------------------------------------
 * MemObj::md5sum
 *-----------------------------------------------------------------------------
 * Calculate the md5 checksum of the bytes of the object
 * Return it as a C++ string (32 hex characters)
 *-----------------------------------------------------------------------------
 */
std::string MemObj::md5sum()
{
    unsigned int result[4];

    void *handle = md5_init();
    int i;
    for (i=0; i < _nbuffers; i++) {
	md5_update(handle, (char *)_buffers[i].bytes, _buffers[i].nbytes);
    }
    md5_end(handle, result);

    char buf[33];
    md5_result_to_string(result, buf);

    std::string cksum = std::string(buf);
    return cksum;
}

/*-----------------------------------------------------------------------------
 * MemObj::load
 *-----------------------------------------------------------------------------
 * Load a memory object from a file
 *
 * ARGUMENTS:
 *	fname: the file name where data will be read from
 *	fp   : FILE* where data will be read from
 *	appendflag: if true then the data will be appended to whatever
 *		data already exists in the object.
 *		If false (the default) any previous data will be lost.
 * RETURNS:
 * 0 if everything was OK, or -1 if there was a problem (e.g. the file
 * did not exist or there was a read error)
 *-----------------------------------------------------------------------------
 */
int MemObj::load(const char *fname, bool appendflag)
{
    FILE *fp;
    fp = fopen(fname, "r");
    if (fp == NULL) {
	return -1;
    }
    int status = this->load(fp, appendflag);
    fclose(fp);
    return status;
}

int MemObj::load(FILE *fp, bool appendflag)
{
    if (!appendflag) {
	this->clear();
    }

    unsigned char buf[64*1024];
    size_t buflen = sizeof(buf);

    size_t n;

    for (;;) {
	n = fread(buf, (size_t)1, buflen, fp);
	if (n < 0) {
	    return -1;
	}
	if (n == 0) {
	    break;
	}
	this->append(buf, n);
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * MemObj::store
 *-----------------------------------------------------------------------------
 * Store the memory object to a file
 * ARGUMENTS:
 * 	fname : the file where we will store the object
 *-----------------------------------------------------------------------------
 */
int MemObj::store(const char *fname)
{
    FILE *fp = fopen(fname, "w");
    if (fp == NULL) {
	return -1;
    }
    int status = this->store(fp);
    fclose(fp);
    return(status);
}

int MemObj::store(FILE *fp)
{
    unsigned char buf[64*1024];
    size_t buflen = sizeof(buf);
    size_t sz = this->size();
    size_t nsofar;

    nsofar = 0;
    while (nsofar < sz) {
	size_t n1 = this->get(nsofar, buflen, buf);
	if (n1 == 0) {
	    break;
	}
	size_t n2 = fwrite(buf, (size_t)1, n1, fp);
	if (n1 != n2) {
	    return -1;
	}
	nsofar += n2;
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * MemObj::makerandom
 *-----------------------------------------------------------------------------
 * Create an object with the given size containing random data
 * NOTE: the previous data of the object are cleared.
 *
 * ARGUMENTS:
 *	size: the size of the new object
 *	seed: a seed to be used when generating random data
 *	      (0 means do not use a seed). Objects generated with
 *	      the same size/seed have always the same data.
 *-----------------------------------------------------------------------------
 */
void MemObj::makerandom(size_t size)
{
    this->makerandom(size, 0);
}

void MemObj::makerandom(size_t size, unsigned int seed)
{
    this->clear();

    if (seed != 0) {
	srand(seed);
    }
    
    // Create a small buffer with random data
    const int N = 250*1024;
    int buffer[N];
    int i;
    for (i=0; i<N; i++) {
	buffer[i] = rand();
    }

    // "repeat" this buffer until we create the object we want...
    //
    size_t nleft = size;
    while (nleft > 0) {
	size_t n = sizeof(buffer);
	if (n > nleft) {
	    n = nleft;
	}
	this->append((unsigned char*)buffer, n);
	nleft -= n;
    }
}

/*-----------------------------------------------------------------------------
 * MemObj::getpointer
 *-----------------------------------------------------------------------------
 * Get a pointer to the actual data bytes.
 * This is a rather dangerous function.
 * ARGUMENTS:
 * 	offset: the offset of the first byte we want
 * 	nbytesptr: This is a pointer to a 'size_t'. If not NULL
 * 		then we store there the number of "valid" bytes
 * 		that you can read/write after the returned pointer.
 * 		For sure this number is at most equal to the total
 * 		number of bytes in the object - "offset".
 * 		However it can be less, if the object's data is not
 * 		stored sequentially.
 * RETURNS:
 * 	A pointer pointing to the byte found at 'offset'.
 * 	If 'offset' is out of range, then we return NULL
 *-----------------------------------------------------------------------------
 */
unsigned char *MemObj::getpointer(size_t offset, size_t *nbytesptr)
{
    Debug(DEBUG_LEVEL, ">MemObj::getpointer this=0x%lx offset=%ld\n", 
	    	(long)this, (long)offset);
    size_t dummy;
    unsigned char *ptr;

    if (nbytesptr == NULL) {
	nbytesptr = &(dummy);
    }

    // Are we out of range?
    if (offset < 0 || offset >= _nbytes) {
	ptr = NULL;
	*nbytesptr = 0;
	Debug(DEBUG_LEVEL,
	    "<MemObj::getpointer this=0x%lx offset=%ld ptr=0x%lx nbytes=%ld\n", 
	    	(long)this, (long)offset, (long)ptr, (long)(*nbytesptr));
	return ptr;
    }

    // Find the buffer where "offset" is
    int i;
    for (i=0; i<_nbuffers; i++) {
	if (_buffers[i].endoffset >= offset) {
	    break;
	}
    }
    if (i >= _nbuffers) {
	ptr = NULL;
	*nbytesptr = 0;
	Debug(DEBUG_LEVEL,
	    "<MemObj::getpointer this=0x%lx offset=%ld ptr=0x%lx nbytes=%ld\n", 
		    (long)this, (long)offset, (long)ptr, (long)(*nbytesptr));
#ifdef NO
	Debug(DEBUG_LEVEL, ">>>XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	//std::string s = this->_debug();
	//Debug(DEBUG_LEVEL, "%s\n", s.c_str());
	for (i=0; i<_nbuffers; i++) {
	    Debug(DEBUG_LEVEL, "   i=%d off=%ld-%ld nb=%ld/%ld\n",
	    i, _buffers[i].startoffset,
	    _buffers[i].endoffset,
	    _buffers[i].nbytes,
	    _buffers[i].maxbytes);
	}
	Debug(DEBUG_LEVEL, "<<<XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
#endif

	return ptr;
    }

    ptr = _buffers[i].bytes + (offset - _buffers[i].startoffset);
    *nbytesptr = _buffers[i].endoffset - offset + 1;
    Debug(DEBUG_LEVEL,
	"<MemObj::getpointer this=0x%lx offset=%ld ptr=0x%lx nbytes=%ld\n", 
	    	(long)this, (long)offset, (long)ptr, (long)(*nbytesptr));
    return(ptr);
}

/*-----------------------------------------------------------------------------
 * MemObj::_buffer_add
 *-----------------------------------------------------------------------------
 * Apart from the buffers already allocated, add a new buffer that
 * has at least that many bytes
 * NOTE: make sure that if we run out of memory we raise an exception
 * but the object remains in consistent state.
 *-----------------------------------------------------------------------------
 */
void MemObj::_buffer_add(size_t minbufsz)
{
    if (minbufsz == 0) {
	return;
    }

    size_t sz;

    // We need to add a new buffer. Do we have enough space
    // in the "_buffers" array?
    //
    if (_nbuffers >= _maxbuffers) {
	// Hm... we need to allocate more space for the array
	int new_maxbuffers = _maxbuffers + NBUFFERS_STEP;
	sz = new_maxbuffers * sizeof(_BufferStruct);
	_BufferStruct *tmp = (_BufferStruct *)realloc(_buffers,sz);
	if (tmp == NULL) {
	    // Oooops! out of memory
	    Debug(0,
		"<MemObj::_buffer_add this=0x%lx exception=OUT_OF_MEMORY\n",
		(long)this);
	    throw new MemObjException("Ouf of memory");
	}
	// ok, update the _buffers array
	_maxbuffers = new_maxbuffers;
	_buffers = tmp;
    }

    // ok, we have at least one extra un-initialized buffer
    // (it's index is "_nbuffers")
    // Allocate space to hold at least "minbufsz" bytes

    int n = (minbufsz + BUFFER_SIZE_STEP) / BUFFER_SIZE_STEP;
    sz = n * BUFFER_SIZE_STEP;
    unsigned char *newbuf = (unsigned char *)malloc(sz);
    if (newbuf == NULL) {
	// Oooops! Out of memory!
	Debug(0,
	    "<MemObj::_buffer_add this=0x%lx exception=OUT_OF_MEMORY\n",
	    (long)this);
	throw new MemObjException("Ouf of memory");
    }

    // ok, initialize the "next" buffer
    _BufferStruct *b = &(_buffers[_nbuffers]);
    b->startoffset = 0;
    b->endoffset = 0;
    b->nbytes = 0;
    b->maxbytes = sz;
    b->bytes = newbuf;
}
/*-----------------------------------------------------------------------------
 * MemObj::_debug
 *-----------------------------------------------------------------------------
 */
std::string MemObj::_debug ()
{
    std::string s;

    s += std::string("=== MemObj 0x%lx\n", (long)this);
    s += std::string("    nbuffers=%d\n", this->_nbuffers);
    int i;
    for (i=0; i<this->_nbuffers; i++) {
	char buf[1024];
	sprintf(buf, 
	    "    buf %2d offset=%ld-%ld nbytes=%ld (maxbytes=%ld)\n",
	    i,
	    (long)this->_buffers[i].startoffset,
	    (long)this->_buffers[i].endoffset,
	    (long)this->_buffers[i].nbytes,
	    (long)this->_buffers[i].maxbytes);
	s += std::string(buf);
    }
    return s;
}
