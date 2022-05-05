/*  File   : 2/28/91 @(#)tcp.h	63.1
    Purpose: Header file for the C interface to the tcp package.
    Author : Tom Howland
    Updated: 7/10/89
    SCCS   : @(#)89/07/10 tcp.h	1.2
    SeeAlso: 

    Copyright (C) 1989, Quintus Computer Systems, Inc.  All rights reserved.
*/
#ifdef __cplusplus
    extern "C" {
#endif

#ifndef tcp_ON

#define tcp_ON 1
#define tcp_OFF 0

#define tcp_ERROR -1
#define tcp_TIMEOUT 0
#define tcp_SUCCESS 1

#define tcp_BLOCK 1
#define tcp_POLL 0

long int
    tcp_accept(int),
    tcp_connect(const char*, int),
    tcp_listen(char *, int *, char **),
    tcp_select(long int, double, long int *),
    tcp_setmask(long int, long int),
    tcp_shutdown(int),
    tcp_address_from_file(char *, int *, char **),
    tcp_address_from_shell(char *, char *, char *, int *, char **);
//    tcp_listen_at_port(),

/* MAXHOST is how many bytes a hostname can be */

#define tcp_MAXHOST 34

#endif
#ifdef __cplusplus
    }

#endif
