#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/*-----------------------------------------------------------------------------
 * CONSTANTS / DEFINITIONS
 *-----------------------------------------------------------------------------
 */
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define MEGABYTE (1024*1024)		/* # of bytes in a Mbyte! */

#define DEFAULT_PORT		1234		/* default port to listen to */
#define DEFAULT_BUFFER_SIZE	((long)(1024 * 1024))
#define DEFAULT_MIN_MBYTES	4.0
#define DEFAULT_MAX_MBYTES	-1
#define DEFAULT_BACKLOG		100

/*-----------------------------------------------------------------------------
 * TYPE DEFINITIONS
 *-----------------------------------------------------------------------------
 */
/*
 * ConfData
 */
typedef struct {
    int port;
    long buffersize;
    float min_mbytes;
    float max_mbytes;
    int backlog;
} ConfData;

/*
 * ConnectionData:
 *	data about a connection. This struct is passed to the thread
 *	that will service it.
 */
typedef struct {
    int sock;			/* socket to write data to */
    int clientid;		/* unique client number */
    char clientname[100];	/* client name in "address:port" form */
    unsigned long nbytes;	/* bytes to write to the socket */
    size_t bufsize;		/* buffer size to use */
} ConnectionData;

/*-----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *-----------------------------------------------------------------------------
 */
ConfData MyConf;

/*-----------------------------------------------------------------------------
 * STATIC FUNCTIONS
 *-----------------------------------------------------------------------------
 */
static int check_int(const char *s, int *val);
static float check_float(const char *s, float *val);
static void usage(const char *progname);
static void parse_options(int argc, char *argv[]);
static int create_socket();
static void accept_connections();
static void *thread_main(void *arg);

/*-----------------------------------------------------------------------------
 * usage
 *-----------------------------------------------------------------------------
 * Print usage message
 *-----------------------------------------------------------------------------
 */
static void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [options]\n", progname);
    fprintf(stderr, "Valid options are:\n\
\t-port : port to listen to. It must be a positive integer.\n\
	  Default is %d.\n\
\t-bs   : buffer size to use (in bytes). It must be a positive\n\
          integer. Default is %ld.\n\
\t-len  : number of *megabytes* to send to each client. Default is %.3f.\n\
\t        It must be a positive float.\n\
\t        NOTE: THIS IS MEGABYTES, NOT BYTES!\n\
\t-len2 : If present, then we send a random number of megabytes in the\n\
\t	  range specified by \"-len\" and \"-len2\".\n\
\t        It must be greater or equal to the value specified by \"-len\",\n\
\t	  otherwise it is ignored.\n\
\t-help : Print this message.\n\
",
    		DEFAULT_PORT,
    		(long) DEFAULT_BUFFER_SIZE,
		DEFAULT_MIN_MBYTES);

    exit(1);
}

/*-----------------------------------------------------------------------------
 * check_int
 *-----------------------------------------------------------------------------
 * Check that the given string is an integer.
 * ARGUMENTS:
 *	s   : the string to check
 *	val : this is where we return the value
 * RETURNS:
 *	1 if this is a valid integer, 0 otherwise
 *-----------------------------------------------------------------------------
 */
static int check_int(const char *s, int *val)
{
    long int v;
    char *endptr;

    v = strtol(s, &endptr, 0);
    if (*endptr != '\0') {
	return(0);
    }
    *val = v;	/* check for overflow ??? */
    return(1);
}

/*-----------------------------------------------------------------------------
 * check_float
 *-----------------------------------------------------------------------------
 * Check that the given string is an float
 * ARGUMENTS:
 *	s   : the string to check
 *	val : this is where we return the value
 * RETURNS:
 *	1 if this is a valid float, 0 otherwise
 *-----------------------------------------------------------------------------
 */
static float check_float(const char *s, float *val)
{
    float v;
    char *endptr;

    /* NOTE: for some reason 'strtof' does not work! So we use 'strtod' */
    v = strtod(s, &endptr);
    if (*endptr != '\0') {
	return(0);
    }
    *val = v;
    return(1);
}

/*-----------------------------------------------------------------------------
 * parse_options
 *-----------------------------------------------------------------------------
 * Parse command line arguments and populate "MyConf"
 *-----------------------------------------------------------------------------
 */
static void parse_options(int argc, char *argv[])
{
    int i;
    int ival;
    float fval;
    float min_mbytes;
    float max_mbytes;

    MyConf.port = DEFAULT_PORT;
    MyConf.buffersize = DEFAULT_BUFFER_SIZE;
    MyConf.min_mbytes = DEFAULT_MIN_MBYTES;
    MyConf.max_mbytes = DEFAULT_MAX_MBYTES;
    MyConf.backlog = DEFAULT_BACKLOG;
    for (i=1; i<argc; i++) {
	if (!strcmp(argv[i], "-port")) {
	    i++;
	    if (i >= argc || !check_int(argv[i], &ival) || ival <= 0) {
		usage(argv[0]);
	    }
	    MyConf.port = ival;
	} else if (!strcmp(argv[i], "-bs")) {
	    i++;
	    if (i >= argc || !check_int(argv[i], &ival) || ival <= 0) {
		usage(argv[0]);
	    }
	    MyConf.buffersize= ival;
	} else if (!strcmp(argv[i], "-len")) {
	    i++;
	    if (i >= argc || !check_float(argv[i], &fval) || fval <= 0) {
		usage(argv[0]);
	    }
	    MyConf.min_mbytes= fval;
	} else if (!strcmp(argv[i], "-len2")) {
	    i++;
	    if (i >= argc || !check_float(argv[i], &fval) || fval <= 0) {
		usage(argv[0]);
	    }
	    MyConf.max_mbytes = fval;
	} else if (!strcmp(argv[i], "-help")) {
	    usage(argv[0]);
	} else {
	    fprintf(stderr, "Invalid option \"%s\".\n\n", argv[i]);
	    usage(argv[0]);
	}
    }
    if (MyConf.max_mbytes < MyConf.min_mbytes) {
	MyConf.max_mbytes = MyConf.min_mbytes;
    }
}

/*-----------------------------------------------------------------------------
 * create_socket
 *-----------------------------------------------------------------------------
 * Create a socket, bind it to the given port, listen to it and
 * return it.
 * RETURNS:
 *	A socket, ready to used in "accept"
 *-----------------------------------------------------------------------------
 */
static int create_socket()
{
    int sock, status, val;
    struct sockaddr_in my_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	perror("socket");
	exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MyConf.port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    status = bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (status < 0) {
	close(sock);
	perror("bind");
	exit(1);
    }

    status = listen(sock, MyConf.backlog);
    if (status < 0) {
	close(sock);
	perror("listen");
	exit(1);
    }

    /*
     * Make address reusable sooner than TCP requires...
     */
#ifdef DOES_NOT_WORK_AND_BREAKS_THE_PROGRAM
    val = 1;
    status = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
    		(void *)&val, (socklen_t)sizeof(val));
    if (status < 0) {
	perror("setsockopt");
	exit(1);
    }
#endif

    return(sock);
}

/*-----------------------------------------------------------------------------
 * accept_connections
 *-----------------------------------------------------------------------------
 * This is the main loop of the main thread.
 * We just accept connections from a socket, and we create a new thread
 * to handle it.
 *-----------------------------------------------------------------------------
 */
static void accept_connections(int sock)
{
    int status, s;
    struct sockaddr_in client_addr;
    socklen_t dummy;
    pthread_t thrid;
    pthread_attr_t attr;
    ConnectionData *cdata;
    int client_id;
    long min_nbytes, max_nbytes;


    /* 
     * Prepare thread attributes...
     */
    pthread_attr_init(&attr);

    client_id = 0;
    for (;;) {
	errno = 0;
	s = accept(sock, (struct sockaddr *)&client_addr, &dummy);
	if (s < 0) {
	    perror("accept");
	    close(sock);
	    exit(1);
	}
	/*
	 * Create new client data
	 */
	cdata = (ConnectionData *)malloc(sizeof(ConnectionData));
	if (cdata == NULL) {
	    fprintf(stderr, "OUT OF MEMORY IN MALLOC!");
	    exit(1);
	}
	cdata->sock = s;
	cdata->bufsize = MyConf.buffersize;
	cdata->clientid = client_id++;
	sprintf(cdata->clientname, "%s:%d", 
	    inet_ntoa(client_addr.sin_addr), (int) client_addr.sin_port);

	min_nbytes = MyConf.min_mbytes * 1024 * 1024;
	max_nbytes = MyConf.max_mbytes * 1024 * 1024;
	if (min_nbytes == max_nbytes) {
	    cdata->nbytes = min_nbytes;
	} else {
	    cdata->nbytes = min_nbytes + (rand() % (max_nbytes - min_nbytes));
	}

	/*
	 * Create new thread
	 */
	status = pthread_create(&thrid, &attr, thread_main, cdata);
    }
}

/*-----------------------------------------------------------------------------
 * thread_main
 *-----------------------------------------------------------------------------
 * This is the main thread routine that serves a client.
 *-----------------------------------------------------------------------------
 */
static void *thread_main(void *arg)
{
    ConnectionData *cdata;
    pthread_t tid;
    char *buf;
    size_t nleft, n;
    ssize_t nwritten;
    struct timeval t1, t2;
    float t, rate;
    int i;

    cdata = (ConnectionData *)arg;
    tid = pthread_self();

    printf("CONNECT thread %d socket %d clientid %d clientname %s nbytes %ld\n",
	(int)tid, cdata->sock, cdata->clientid,
	cdata->clientname, cdata->nbytes);

    /*
     * Allocate buffer
     */
    buf = (char *)malloc(cdata->bufsize);
    memset(buf, (int)'.', cdata->bufsize);

    if (gettimeofday(&t1, NULL) < 0) {
	perror("gettimeofday t1");
	exit(1);
    }
    nleft = cdata->nbytes;
    while (nleft > 0) {
	nwritten = write(cdata->sock, buf, MIN(nleft, cdata->bufsize));
	if (nwritten == ((ssize_t)-1)) {
	    /*
	     * Ooops! write error
	     */
	    printf("WRITEERROR clientid %d nleft %ld error '%s'\n",
		    cdata->clientid, nleft, strerror(errno));
	    close(cdata->sock);
	    free(cdata);
	    free(buf);
	    pthread_exit(NULL);
	}
	nleft -= nwritten;
    }
    if (gettimeofday(&t2, NULL) < 0) {
	perror("gettimeofday t1");
	exit(1);
    }
    t =t2.tv_sec - t1.tv_sec;
    t += ((float)(t2.tv_usec - t1.tv_usec)) / 1000000.0;
    if (t == 0) {
	t = 0.001;
    }
    rate = cdata->nbytes / (1024*1024) / t;

    printf("END clientid %d nbytes %ld time %.3f rate %.3f\n",
	cdata->clientid, cdata->nbytes, t, rate);

    close(cdata->sock);
    free(cdata);
    free(buf);
    pthread_exit(NULL);
}

/*-----------------------------------------------------------------------------
 * MAIN
 *-----------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
    int sock;

    parse_options(argc, argv);

    sock = create_socket();
    accept_connections(sock);
    return(0);
}
