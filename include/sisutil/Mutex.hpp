// Mutex.hpp  -*- C++ -*- DCE mutual exclusion semaphore
// $Id: Mutex.hpp,v 1.1 1994/06/17 18:13:09 ed Exp $

#ifndef _MUTEX_H
#define _MUTEX_H 1
//#include <dce/pthread.h>
#include <pthread.h>

class Mutex {
public:
	Mutex();											// recursive mutex type
	virtual ~Mutex();
	void lock();  
	void unlock();
private: 
	Mutex(const Mutex&);						// disable copy ctor
	Mutex& operator=(const Mutex&);	// disable op=
	//pthread_mutex_t myMutex;
	pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutexattr_t myMutexAttr;
};

// Name: Mutex
// Documentation: DCE mutual exclusion semaphore
// Visibility: exported | private | imported
// Cardinality: 0 | 1 | n
// Hierarchy:
//  Superclasses: list of class names
// Generic parameters: list of parameters
// Public Interface:
//  Uses: list of class names
//  Operations: list of operation declarations
// Protected Interface:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Private Interface:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Implementation:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Finite State Machine: state transition diagram
// Concurrency: sequential | blocking | active
// Space complexity: text
// Persistance: static | dynamic
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
