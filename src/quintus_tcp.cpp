/*  File   : 5/5/94 @(#)tcp.c	73.1
    Author : Tom Howland
    Defines: various calls to the operating system supporting tcp.pl
    SeeAlso: tcp.pl, man select, man tcp

    Copyright (C) 1990, Quintus Computer Systems, Inc.  All rights reserved.
*/

#ifndef	lint
// This will cause an "unused variable warning" with gcc so I have put "-Wno-unused-variable" in the compile settings
static char* SCCS_ID = "@(#)tcp.c	73.1 5/5/94 Copyright (C) 1990 Quintus";
#endif

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>

// happify eclipse
#include <bits/signum.h>

#ifdef PROLOG

#include "quintus.h"
#include "socket_io.h"

#endif

#include "quintus_tcp.h"

static struct {
  int     width;             /* used to limit search in tcp_select */
  int     startfd;
  int     fds[FD_SETSIZE];
  int	    altered;

#ifdef PROLOG

  struct socket_input_stream* stream[FD_SETSIZE];
  QP_atom last_error_msg;
  int last_error_number;
  QP_pred_ref timer_callback, input_callback;

#endif

} tcp;

#ifdef PROLOG

/* error handling */

static void report_error(char* s)
{
  if (tcp.last_error_number == 0) {
    tcp.last_error_number = errno;
    tcp.last_error_msg = QP_atom_from_string(s);
  }
}

tcp_fetch_error(QP_atom* msg)
{
  int n;

  *msg = tcp.last_error_msg;
  n = tcp.last_error_number;
  tcp.last_error_number = 0;
  return n;
}

/* add Seconds to absolute time. */

void
tcp_time_plus(long is, long iu,   /* input time */
  long* os, long* ou, /* output time */
  double d)           /* delta time in seconds */
{
  register double x;
  register long s;
  register long u;

  x = d + is + iu / 1.0e6;
  s = x;
  u = (x - s) * 1.0e6 + 0.5;
  if (u == 1000000) {
    *os = ++s;
    *ou = 0;
  }
  else {
    *os = s;
    *ou = u;
  }
}

/* return the current time. */

void tcp_now(long* S, long* U)
{
  struct timeval  t;
  struct timezone tz;

  if (gettimeofday(&t, &tz))
    report_error("tcp_now.gettimeofday");

  *U = t.tv_usec;
  *S = t.tv_sec;
}

void tcp_check_stream(int fd, struct socket_input_stream* u)
{
  tcp.stream[fd] = u;
}

static void tcp_timer_callback(long dummy, long cookie)
{
  (void)QP_query(tcp.timer_callback, cookie);
}

tcp_create_timer_callback(long int s, long int u,
  long int* cookie, long int* timer_id)
{
  double milliseconds;
  long   ns, nu;
  int    status;

  static long count;

  if (tcp.timer_callback == (QP_pred_ref)0) {
    tcp.timer_callback = QP_predicate("timer_callback", 1, "tcp");
    if (tcp.timer_callback == QP_BAD_PREDREF) {
      report_error("tcp_timer_callback() could not find tcp:timer_callback/0");
      tcp.timer_callback = (QP_pred_ref)0;
      return -1;
    }
  }
  tcp_now(&ns, &nu);
  milliseconds = 1000.0 * (s - ns) + 0.001 * (u - nu) + 0.5;
  status = QP_add_timer((int)milliseconds, tcp_timer_callback, (char*)count);
  if (status == QP_ERROR) {
    report_error("tcp_create_timer_callback.QP_add_timer");
    return -1;
  }
  *timer_id = status;
  *cookie = count++;
  return 0;
}

static void tcp_input_callback(int fd)
{
  (void)QP_query(tcp.input_callback, fd);
}

static void tcp_input_check_buffer(int fd, QP_stream* stream)
{
  struct socket_input_stream* sock_stream = (struct socket_input_stream*)stream;
  while (stream->n_char > 0)
    tcp_input_callback(fd);
}

tcp_create_input_callback(int fd, QP_stream* stream)
{
  if (tcp.input_callback == (QP_pred_ref)0) {
    tcp.input_callback = QP_predicate("input_callback", 1, "tcp");
    if (tcp.input_callback == QP_BAD_PREDREF) {
      tcp.input_callback = (QP_pred_ref)0;
      report_error("tcp_create_input_callback() could not find tcp:input_callback/1");
      return -1;
    }
  }

  (void)QP_add_input(fd, tcp_input_callback, NULL, tcp_input_check_buffer, (char*)stream);
  return tcp_setmask(fd, tcp_OFF);
}

void tcp_destroy_input_callback(int fd)
{
  (void)QP_remove_input(fd);
  (void)tcp_setmask(fd, tcp_ON);
}

QP_atom tcp_current_mask(int Socket)
{
  static first = 1;
  static QP_atom on, off;
  if (first) {
    first = 0;
    on = QP_atom_from_string("on");
    off = QP_atom_from_string("off");
  }
  if (tcp.fds[Socket]) return on;
  else return off;
}

#else

#define report_error(s) perror(s)

#endif  /* PROLOG */

/* Given a ServerFile, tcp_address_from_file() returns the Port and
 * Host that was written there by tcp_listen().  It returns 0 if
 * successful.
 */
long int tcp_address_from_file(char* ServerFile, int* Port, char** Host)
{
  //puts("tcp_address_from_file() ==>");
  long int r;
  FILE* h;
  static char hostname[tcp_MAXHOST];
  if ((h = fopen(ServerFile, "r")) == NULL) {
    report_error("tcp_address_from_file.fopen");
    return -1;
  }
  if (fscanf(h, "port %d\nhost %s\n", Port, hostname) == 2) r = 0;
  else {
    r = -1;
    report_error("tcp_address_from_file.fscanf");
  }
  (void)fclose(h);
  *Host = hostname;
  //printf("tcp_address_from_file() Port = %d, Host=%s\n", *Port, *Host);
  return r;
}

/* Given a remote Host1 and ServerFile, tcp_address_from_shell()
 * returns the Port and Host that was written there by tcp_listen().
 * It returns 0 if successful.
 */
long int tcp_address_from_shell(char* Host1,
  char* UserId,
  char* ServerFile,
  int* Port,
  char** Host)
{
  //puts("tcp_address_from_shell() ==>");
  long int r;
  FILE* stream;
  char Command[512];
  char user_spec[512];
  static char hostname[tcp_MAXHOST];
  if (strlen(UserId)) (void) sprintf(user_spec, "-l %s", UserId);
  else user_spec[0] = 0;
  (void)sprintf(Command, "rsh %s -n %s cat %s", Host1, user_spec, ServerFile);
  if (!(stream = popen(Command, "r"))) {
    report_error("tcp_address_from_shell.popen");
    return -1;
  }
  if (fscanf(stream, "port %d\nhost %s\n,", Port, hostname) == 2) {
    r = 0;
  }
  else {
    r = -1;
    report_error("tcp_address_from_shell.fscanf");
  }
  (void)pclose(stream);
  *Host = hostname;
  return r;
}

/* tcp_setmask updates the fd_set's used in tcp_select. */
long int tcp_setmask(long int fd, long int On)
{
  if (On) {
    static int ignore_sigpipe = 1;

    if (ignore_sigpipe) {
      typedef void (*sigfunc)(int);
      sigfunc old_sigpipe_handler;

      old_sigpipe_handler = signal(SIGPIPE, SIG_IGN);
      if (old_sigpipe_handler == SIG_ERR) {
        report_error("tcp_setmask.signal1");
        return -1;
      }
      ignore_sigpipe = 0;
    }

    tcp.fds[fd] = 1;

    if (fd >= tcp.width) tcp.width = fd + 1;
  }
  else {
    tcp.fds[fd] = 0;

    if (fd + 1 == tcp.width) {
      long int i;

      for (i = fd - 1; i != -1 && tcp.fds[i] == 0; i--);
      if (tcp.startfd > i) tcp.startfd = 0;
      tcp.width = i + 1;
    }
  }

  /* if this was reached from a callback, make sure tcp_select() knows. */
  tcp.altered = 1;

  return 0;
}

#ifndef PROLOG
#define QP_select select
/* Return values defined by select */
#define SELECT_TIMEOUT 0
#define SELECT_FAILURE -1
#define SELECT_INTERRUPTED -3
#else
/* Return values defined by QP_select */
#define SELECT_TIMEOUT QP_SUCCESS
#define SELECT_FAILURE QP_ERROR
#define SELECT_INTERRUPTED -3
#endif

static int handle_timeouts(long int Block, double* Timeout,
  int w, int poll, fd_set* readfds)
{
  struct timeval timeout, entry_time, * timeoutp;
  struct timezone tz;
  int r;
  double Timeout0;

  if (!Block) (void) gettimeofday(&entry_time, &tz);
  Timeout0 = *Timeout;

  if ((poll || !Block) && (Timeout0 <= 0.0)) {
    timeoutp = &timeout;
    timerclear(&timeout);
  }
  else if (Block) {
    timeoutp = 0;
  }
  else {
    long x;

    timeoutp = &timeout;
    timeout.tv_sec = x = Timeout0;
    timeout.tv_usec = 0.5 + (Timeout0 - x) * 1e6;
  }

  r = QP_select(w, readfds, 0, 0, timeoutp);

  /* As of delta 67.1 QP_select() will not return with SELECT_FAILURE
     and errno EINTR. But when it is fixed to do that, we should
     check for it here */
  if (tcp.altered || (r == SELECT_FAILURE && errno == EINTR)) {
    if (!Block) {
      struct timeval  t;

      (void)gettimeofday(&t, &tz);
      *Timeout = Timeout0 - t.tv_sec + entry_time.tv_sec -
        (t.tv_usec - entry_time.tv_usec) / 1e6;
    }
    tcp.altered = 1;
    return SELECT_INTERRUPTED;
  }
  else {
    return r;
  }
}

/* tcp_select(long int Block, double Timeout, long int *selectedfd)

 determine the ready file descriptors in a round-robin fashion.

 return value is one of:
   tcp_ERROR
   tcp_TIMEOUT
   tcp_SUCCESS data available

 *selectedfd is the file descriptor that has input data available.
*/
long int tcp_select(long int Block,
  double   Timeout,
  long int* selectedfd)
{
  long int fd, w, start, poll;
  fd_set readfds;	/* indicates which fd's have input available */

  do {
    tcp.altered = 0;
    poll = 0;
    if ((w = tcp.width)) {
      start = tcp.startfd;
      fd = start;
      do {
        if (tcp.fds[fd]) {
#ifdef PROLOG
          long int ready;
          if (!poll) {
            /* check for any unconsumed characters on
               the individual streams. */
            register struct socket_input_stream* s;
            s = tcp.stream[fd];
            if (s && (s->qpinfo.n_char > 0)) {
              poll = 1;
              ready = fd;
            }
          }
#endif
          FD_SET(fd, &readfds);
        }
        else {
          FD_CLR(fd, &readfds);
        }
        //fd = ++fd % w;  // warning: operation on ‘fd’ may be undefined [-Wsequence-point]
        fd = (fd + 1) % w;
      } while (fd != start);
    }

    switch (handle_timeouts(Block, &Timeout, w, poll, &readfds)) {
    case SELECT_TIMEOUT:
      if (!poll) return tcp_TIMEOUT;
      break;
    case SELECT_FAILURE:
      report_error("tcp_select");
      return tcp_ERROR;
    }
  } while (tcp.altered);

#ifdef PROLOG
  if (poll) FD_SET(ready, &readfds);
#endif
  fd = start;
  //for (; ! FD_ISSET(fd, &readfds); fd = ++fd % w);  // warning: operation on ‘fd’ may be undefined [-Wsequence-point]
  for (; !FD_ISSET(fd, &readfds); fd = (fd + 1) % w);
  *selectedfd = fd;
  tcp.startfd = ++fd % w;
  return tcp_SUCCESS;
}

/* tcp_shutdown is called whenever prolog closes an input stream to a socket.
 * It is also called directly to kill the server socket
 */
long int tcp_shutdown(int s)
{
  int q, r;
  r = 0;
  q = tcp_setmask(s, tcp_OFF);
  if (q) r = -1;

  /* The error code EINVAL is set when shutdown() is called on a
     broken socket.  But we don't care if it is broken at this point.
  */

  q = shutdown(s, 2);
  if (q && !r && errno != EINVAL) {
    report_error("tcp_shutdown.shutdown");
    r = -1;
  }

  q = close(s);
  if (q && !r) {
    report_error("tcp_shutdown.close");
    r = -1;
  }

#ifdef PROLOG
  tcp.stream[s] = CoerceSocketInputStream(0);
#endif

  return r;
}

/** connect to some server
 *  changed to use getipnodebyname instead of gethostbyname (ipv6 refactor)
 *  also, gethostbyname is not multithreaded (OJ)
 *  TODO: change AF_INET for ipv6 when the time comes
 */
long int tcp_connect(const char* HostName, int Port)
{
  if (getenv("PrologServer_DEBUG")) printf("tcp_connect(): HostName = %s, Port=%d\n", HostName, Port); // need to know what Hostname looks like on Linux

  int                connection, rv;
  //struct hostent    *hp;
  //struct sockaddr_in sin;
  char               emsg[99];
  struct addrinfo    hints, * servinfo = NULL;
  char               port[15];
  /* given the simple host name, get the internal host identification */
  // TODO: getipnodebyname is deprecated and has been replaced by getaddrinfo
  //       see http://beej.us/guide/bgnet/output/html/multipage/getaddrinfoman.html for an example
  // int getaddrinfo(const char *node, const char *service,
  //                     const struct addrinfo *hints,
  //                     struct addrinfo **res);

  //hp = gethostbyname(HostName);
  //int error_num;
  //hp = getipnodebyname(HostName, AF_INET, AI_DEFAULT, &error_num);
  //hp = getipnodebyname(HostName, AF_INET, (AI_ADDRCONFIG | AI_V4MAPPED), &error_num);
  //fprintf(stderr, "tcp_connect(): going to getaddrinfo()\n");

  snprintf(port, 15, "%d", Port);
  fprintf(stderr, "tcp_connect(): port: %s\n", port);
  memset(&hints, 0, sizeof hints); // clear the struct
  hints.ai_family = AF_INET;       // IP4 for now, change to AF_UNSPEC for either or use AF_INET6 to force IPv6
  //hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if (getenv("PrologServer_DEBUG")) puts("tcp_connect(): calling getaddrinfo"); // need to know what Hostname looks like on Linux
  rv = getaddrinfo(HostName, port, &hints, &servinfo);
  //if (hp == NULL) {
  if (rv != 0) {
    // TODO: interpret any error messages passed in error_num and add to message
    //sprintf(emsg, "tcp_connect.getipnodebyname host(%s), port(%d), error(%d)",
    //        HostName, Port, error_num);
    //snprintf(emsg, sizeof(emsg), "tcp_connect.getaddrinfo host(%s), port(%d), error(%s)",
    //        HostName, Port, gai_strerror(rv));
    //report_error(emsg);
    fprintf(stderr, "tcp_connect.getaddrinfo() error: %s\n", gai_strerror(rv));
    return -1;
  }

  // loop through all the results and connect to the first we can (we trust the sorting function given in RFC 3484)
  struct addrinfo* p;
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if (getenv("PrologServer_DEBUG")) {
      struct sockaddr_in* h = (struct sockaddr_in*)p->ai_addr;
      fprintf(stderr, "getaddrinfo:\n");
      //fprintf(stderr, "            %s (canonname)\n", p->ai_canonname);
      fprintf(stderr, "            %d (family)\n", p->ai_family);
      fprintf(stderr, "            %d (flags)\n", p->ai_flags);
      fprintf(stderr, "            %d (protocol)\n", p->ai_protocol);
      fprintf(stderr, "            %d (socktype)\n", p->ai_socktype);
      //fprintf(stderr, "            %s (sockaddr)\n", servinfo->ai_addr->sa_data);
      fprintf(stderr, "            %u, %s (addr)\n", h->sin_addr.s_addr, inet_ntoa(h->sin_addr));  // TODO: convert inet_ntoa() to inet_ntop()
    }
    //strcpy(ip , inet_ntoa( h->sin_addr ) );
    break;  // first one - temporary until we set up a test for best one
  }
  //fprintf(stderr, "using address: %u with length: %d\n", p->ai_addr, p->ai_addrlen);
  //fprintf(stderr, "tcp_connect(): continuing to socket()\n"); fflush(stderr);

  /* get a socket */
  //connection = socket(AF_INET, SOCK_STREAM, 0);
  connection = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
  //fprintf(stderr, "socket() returns: %d\n", connection);
  if (connection == -1) {
    int err = errno;
    sprintf(emsg, "tcp_connect.socket failed for host(%s), port(%d) with error: %d", HostName, Port, err);
    report_error(emsg);
    //freehostent(hp);
    freeaddrinfo(servinfo); // release resource before return
    return -1;
  }
  //fprintf(stderr, "tcp_connect(): continuing to connect()\n"); fflush(stderr);
  /* fill in the name structure */
  /*
  memset((char *) &sin, 0, sizeof(sin));
  //memcpy((char *) &sin.sin_addr, hp->h_addr, hp->h_length);
  //memcpy((char *) &sin.sin_addr, servinfo->ai_addr, servinfo->ai_addrlen);  // TODO: NOT YET TESTED!!
  // sin is a struct sockaddr_in
  //struct sockaddr *ai_addr;
  //sin.sin_family = hp->h_addrtype;
  sin.sin_family = p->ai_family;
  sin.sin_port = htons( (u_short)Port );
  //sin.sin_addr.s_addr = inet_ntoa( p->ai_addr );
  inet_pton(p->ai_family, p->ai_addr->sa_data, &(sin.sin_addr));  // int af, const char *src, void *dst
  //inet_pton(AF_INET, p->ai_addr, &sin.sin_addr); //p->ai_addr
  //freehostent(hp);         // finished with hostent structure

  //fprintf(stderr, "sin: %d\n", sin.sin_family);
  //fprintf(stderr, "sin: %d\n", sin.sin_port);
  //fprintf(stderr, "sin: \n", sin.sin_family);
  //fprintf(stderr, "sin: %d\n", sin.sin_addr.s_addr);
  */

  /* connect to the server */
  //if (connect(connection, (struct sockaddr *) &sin, sizeof(sin))) {
  if (connect(connection, p->ai_addr, p->ai_addrlen) == -1) {
    snprintf(emsg, sizeof(emsg), "tcp_connect.connect failed for host(%s, s), port(%d)", HostName, Port);
    report_error(emsg);
    freeaddrinfo(servinfo);  // finished with addrinfo structure
    (void)shutdown(connection, 2);
    (void)close(connection);
    return -1;
  }
  freeaddrinfo(servinfo);  // finished with addrinfo structure

  if (tcp_setmask(connection, tcp_ON)) {
    fputs("tcp_connect.connect() closing connection on tcp_setmask() return\n", stderr);
    (void)shutdown(connection, 2);
    (void)close(connection);
    return -1;
  }
  fprintf(stderr, "tcp_connect(): finished\n");

  return connection;
}

/* accept a connection request. */

long int tcp_accept(int Service) /* +Service: integer */
{
  int                connection; /* The new socket */
  struct sockaddr_in sin;	      /* An Internet address record */
#if defined sun
  int sizeof_sin;                /* The number of bytes in struct sockaddr_in */
#else
  //size_t sizeof_sin;             /* The number of bytes in struct sockaddr_in */
  socklen_t sizeof_sin;
#endif

  /* make a connection with some client; block until there is one */

  sizeof_sin = sizeof(sin);
  do {
    connection = accept(Service, (struct sockaddr*)&sin, &sizeof_sin);
  } while (connection == -1 && errno == EINTR);

  if (connection == -1) {
    report_error("tcp_accept");
    return -1;
  }

  if (tcp_setmask(connection, tcp_ON)) {
    (void)shutdown(connection, 2);
    (void)close(connection);
    return -1;
  }

  return connection;
}

/* create a socket on which the server can accept connections. */
long int
tcp_create_listener(int InPort,
  int* OutPort,
  char** Host,
  int* Service) /* The file descriptor of the new socket */
{

  struct sockaddr_in sin;           /* An Internet address record */
#if defined sun
  int sizeof_sin;                 /* The number of bytes in struct sockaddr_in */
#else
  //size_t sizeof_sin;            /* The number of bytes in struct sockaddr_in */
  socklen_t sizeof_sin;
#endif

  int service;

  static char HostName[tcp_MAXHOST];

  /* get this machine's name */
  if (gethostname(HostName, tcp_MAXHOST)) {
    report_error("tcp_listen.gethostname");
    return -1;
  }

  /* create the initial socket */
  service = socket(AF_INET, SOCK_STREAM, 0);
  if (service == -1) {
    report_error("tcp_listen.socket");
    return -1;
  }

  /* set up the name structure for the socket */
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons((u_short)InPort);
  sizeof_sin = sizeof sin;

  /* give the name */
  if (bind(service, (struct sockaddr*)&sin, sizeof_sin)) {
    report_error("tcp_listen.bind");
    (void)shutdown(service, 2);
    (void)close(service);
    return -1;
  }

  /* get the port number assigned */
  if (getsockname(service, (struct sockaddr*)&sin, &sizeof_sin)) {
    report_error("tcp_listen.getsockname");
    (void)shutdown(service, 2);
    (void)close(service);
    return -1;
  }

  /* initialize it so clients can request a connection */
  if (listen(service, SOMAXCONN)) {
    report_error("tcp_listen.listen");
    (void)shutdown(service, 2);
    (void)close(service);
    return -1;
  }

  if (tcp_setmask(service, tcp_ON)) {
    (void)shutdown(service, 2);
    (void)close(service);
    return -1;
  }

  *OutPort = ntohs(sin.sin_port);
  *Host = HostName;
  *Service = service;
  return 0;
}

int tcp_address_to_file(char* FileName, int Port, char* Host)
{
  FILE* f;

  if ((f = fopen(FileName, "w")) == NULL) {
    report_error("tcp_address_to_file.fopen");
    return -1;
  }

  if (fprintf(f, "port %d\nhost %s\nprocess %d\n", Port, Host, getpid()) == EOF) {
    report_error("tcp_address_to_file.fprintf");
    (void)fclose(f);
    return -1;
  }

  if (fclose(f)) {
    report_error("tcp_address_to_file.fclose");
    return -1;
  }

  return 0;
}

long int tcp_listen(char* ServerFile, int* Port, char** Host)
{
  int Service;

  if (tcp_create_listener(0, Port, Host, &Service)) return -1;

  if (tcp_address_to_file(ServerFile, *Port, *Host)) {
    (void)tcp_shutdown(Service);
    return -1;
  }

  return Service;
}
