/*=============================================================================
 * FILE: client.c
 *=============================================================================
 * Implements a client.
 * We just connect (using many threads) to a server and we read data
 * from it (until EOF).
 *=============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

/*-----------------------------------------------------------------------------
 * CONSTANTS / DEFINITIONS
 *-----------------------------------------------------------------------------
 */
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define DEFAULT_PORT		1234
#define DEFAULT_SERVER		"localhost"
#define DEFAULT_BUFFER_SIZE	(1024*1024)
#define DEFAULT_NCLIENTS	10

/*-----------------------------------------------------------------------------
 * TYPE DEFINITIONS
 *-----------------------------------------------------------------------------
 */

/*
 * ConfData:
 *	Information about this program's configuration
 */
typedef struct {
    char *server;		/* name of server to connect to */
    struct in_addr serveraddr;	/* server address (internal representation) */
    int port;			/* port number to connect to */
    int buffersize;		/* buffersize to use for reads */
    int nclients;		/* number of clients to fire up! */
} ConfData;

/*
 * StatsData:
 *	Statistical data for each client thread
 */
typedef struct {
    int finished;		/* have we finished? */
    long nbytes;		/* number of bytes read */
    float t;			/* time (in seconds) */
} StatsData;

/*-----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *-----------------------------------------------------------------------------
 */
ConfData MyConf;		/* all out configuration stored here */
StatsData *MyStats;		/* my statistical information */

/*-----------------------------------------------------------------------------
 * STATIC FUNCTIONS
 *-----------------------------------------------------------------------------
 */
static int check_int(const char *s, int *val);
static float check_float(const char *s, float *val);
static void usage(const char *progname);
static void parse_options(int argc, char *argv[]);
static void *thread_client(void *arg);

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
\t-server: server to connect to. Default is \"%s\".\n\
\t-port  : port to connect to. It must be a positive integer.\n\
\t         Default is %d.\n\
\t-bs    : buffer size to use (in bytes). It must be a positive\n\
\t         integer. Default is %ld.\n\
\t-n     : number of concurrent clients (threads) to use.\n\
\t         It must be a positive integer. Default is %d.\n\
\t-help  : print this message\n\
",
		DEFAULT_SERVER, DEFAULT_PORT,
    		(long) DEFAULT_BUFFER_SIZE, DEFAULT_NCLIENTS);

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

    v = strtof(s, &endptr);
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
    int min_mbytes;
    int max_mbytes;

    MyConf.server = DEFAULT_SERVER;
    MyConf.port = DEFAULT_PORT;
    MyConf.buffersize = DEFAULT_BUFFER_SIZE;
    MyConf.nclients = DEFAULT_NCLIENTS;
    for (i=1; i<argc; i++) {
	if (!strcmp(argv[i], "-server")) {
	    i++;
	    if (i >= argc) {
		usage(argv[0]);
	    }
	    MyConf.server = argv[i];
	} else if (!strcmp(argv[i], "-port")) {
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
	} else if (!strcmp(argv[i], "-n")) {
	    i++;
	    if (i >= argc || !check_int(argv[i], &ival) || ival <= 0) {
		usage(argv[0]);
	    }
	    MyConf.nclients = ival;
	} else if (!strcmp(argv[i], "-help")) {
	    usage(argv[0]);
	} else {
	    fprintf(stderr, "Invalid option \"%s\".\n\n", argv[i]);
	    usage(argv[0]);
	}
    }
}

/*-----------------------------------------------------------------------------
 * thread_client
 *-----------------------------------------------------------------------------
 * This is the main routine that a client thread runs.
 * ARGUMENTS:
 * RETURNS:
 *-----------------------------------------------------------------------------
 */
static void *thread_client(void *arg)
{
    int clientid;
    int tid;
    int sock, status;
    struct sockaddr_in addr;
    char *buf;
    long nbytes;
    ssize_t n;
    struct timeval t1, t2;
    float t, rate;

    clientid = (int)arg;
    tid = (int)pthread_self();

    /*
     * Connect to server
     */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	printf("ERROR type socket_error thread %d errormsg \"%s\"\n",
		tid, strerror(errno));
	pthread_exit(NULL);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(MyConf.port);
    addr.sin_addr = MyConf.serveraddr;

    status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (status < 0) {
	printf("ERROR type connection_error thread %d errormsg \"%s\"\n",
		tid, strerror(errno));
	close(sock);
	pthread_exit(NULL);
    }

    /*
     * create buffer
     */
    buf = (char *)malloc(MyConf.buffersize);
    if (buf == NULL) {
	printf("ERROR type outofmemory thread %d\n", tid);
	close(sock);
	pthread_exit(NULL);
    }

    /*
     * read!
     */
    if (gettimeofday(&t1, NULL) < 0) {
	perror("gettimeofday t1");
	exit(1);
    }
    for (;;) {
	n = read(sock, buf, MyConf.buffersize);
	if (n < 0) {
	    printf("ERROR type read_error thread %d errormsg\n",
			tid, strerror(errno));
	    close(sock);
	    free(buf);
	    pthread_exit(NULL);
	} else if (n == 0) {
	    /*
	     * EOF!
	     */
	    break;
	}
	nbytes += n;
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
    rate = nbytes / (1024*1024) / t;

    printf("END tid %d nbytes %ld time %.3f rate %.3f\n",
            tid, nbytes, t, rate);

    close(sock);
    free(buf);

    /*
     * Update our statistics
     * No need for synchronization, we are the only ones to update
     * this record in the table
     */
    MyStats[clientid].nbytes = nbytes;
    MyStats[clientid].t = t;
    MyStats[clientid].finished = 1;

    pthread_exit(NULL);
}

/*-----------------------------------------------------------------------------
 * print_stats
 *-----------------------------------------------------------------------------
 */
static void print_stats()
{
    int i;
    long nbytes;
    float t, rate, mbytes;
    int nfinished;

    nfinished = 0;
    nbytes = 0;
    t = 0.0;
    for (i=0; i<MyConf.nclients; i++) {
	if (!MyStats[i].finished) {
	    continue;
	}
	nfinished++;
	nbytes += MyStats[i].nbytes;
	t += MyStats[i].t;
    }

    if (t == 0) {
	t = 0.001;
    }
    mbytes = nbytes / (1024.0 * 1024.0);
    rate = mbytes / t;

    printf("#--- STATISTICS ---\n");
    printf("STAT nclients %d\n", MyConf.nclients);
    printf("STAT nfinished %d\n", nfinished);
    printf("STAT server %s\n", MyConf.server);
    printf("STAT port %d\n", MyConf.port);
    printf("STAT buffersize %ld\n", MyConf.buffersize);

    printf("STAT mbytes %.3f\n", mbytes);
    printf("STAT totaltime %.3f\n", t);
    printf("STAT mbytes_per_client %.3f\n",
    	nfinished == 0 ? 0.0 : mbytes / nfinished);
    printf("STAT rate %.3f\n", rate);

    printf(
	"STATTOTAL nfinished %d totalmbytes %.3f totaltime %.3f rate %.3f\n", 
    	nfinished, mbytes, t, rate);

}

/*-----------------------------------------------------------------------------
 * MAIN
 *-----------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
    int i, status;
    pthread_t *thrid;
    pthread_attr_t attr;
    int client_id;
    char *server;
    struct hostent *h;
    size_t sz;

    parse_options(argc, argv);

    /*
     * Resolve server's name
     */
    h = gethostbyname(MyConf.server);
    if (h == NULL) {
	fprintf(stderr, "Can not find IP address of host \"%s\".\n",
		MyConf.server);
	exit(1);
    }
    memcpy(&(MyConf.serveraddr), h->h_addr, sizeof(MyConf.serveraddr));

    /*
     * Create the array where we will store stat info
     */
    sz = MyConf.nclients * sizeof(StatsData);
    MyStats = (StatsData *)malloc(sz);
    if (MyStats == NULL) {
	fprintf(stderr, "Out of memory!?!?\n");
	exit(1);
    }
    for (i=0; i<MyConf.nclients; i++) {
	MyStats[i].finished = 0;
	MyStats[i].nbytes = 0;
	MyStats[i].t = 0;
    }

    /*
     * Initialize thread stuff...
     * Make threads joinable...
     */
    pthread_attr_init(&attr);
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) < 0) {
	perror("pthread_attr_setdetachstate");
	exit(1);
    }

    sz = MyConf.nclients * sizeof(pthread_t);
    thrid = (pthread_t *)malloc(sz);
    if (thrid == NULL) {
	fprintf(stderr, "Out of memory!?!?\n");
	exit(1);
    }

    for (client_id=0; client_id < MyConf.nclients; client_id ++) {
	status = pthread_create(&(thrid[client_id]), &attr, thread_client,
			(void *)client_id);
    }

    /*
     * Now wait for all threads to finish
     */
    for (client_id=0; client_id < MyConf.nclients; client_id ++) {
	status = pthread_join(thrid[client_id], NULL);
	if (status < 0) {
	    perror("pthread_join");
	    exit(1);
	}
	printf("DEBUG: Client %d finished\n", client_id);
    }

    /*
     * All threads finished!
     */
    print_stats();
}
