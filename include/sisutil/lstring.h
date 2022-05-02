// lstring.hpp - unsigned char overloads for string functions
// $Id: lstring.h,v 1.3 1996/06/05 16:22:01 ed Exp $

#ifndef _LSTRING_H
#define _LSTRING_H
//#include <string.h>
#include <cstring>

inline char *strcpy(unsigned char *s1, const char *s2)
	{ return strcpy ((char *)s1, s2);}
inline char *strncpy(unsigned char *s1, const  char *s2,size_t i)
	{ return strncpy ((char *)s1, s2, i);}
inline size_t strlen(const unsigned char *s1)
        {return strlen((const char*)s1);}

#endif
