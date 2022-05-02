// GMLock.hpp  -*- C++ -*- class GlobalMutexLock - lock a shared mutex
// $Id: GMLock.hpp,v 1.12 1995/04/02 23:48:32 ed Exp $

// This is not the pthread global mutex; locking that mutex for
// any significant section of code caused deadlocks and segv's in
// the krb runtime

#ifndef _GLOBALMUTEXLOCK_H
#define _GLOBALMUTEXLOCK_H 1
#include <sisutil/Mutex.hpp>
#include <iostream>

class GlobalMutexLock {
public:
	inline GlobalMutexLock();
	inline ~GlobalMutexLock();		// non-virtual; don't derive from this class
private:
	GlobalMutexLock(const GlobalMutexLock&);						// disable copy ctor
	GlobalMutexLock& operator=(const GlobalMutexLock&);	// disable op=
	static Mutex myMutex;
};

inline GlobalMutexLock::GlobalMutexLock() {
	//std::cerr << "GlobalMutexLock - locking" << std::endl;
	myMutex.lock();
}

inline GlobalMutexLock::~GlobalMutexLock() {
	//std::cerr << "GlobalMutexLock - unlocking" << std::endl;
	myMutex.unlock();
}

// Name: GlobalMutexLock
// Documentation: This class controls access to a private pthread mutex.
// It can be used to coordinate threads that use this class.
// This is not the pthread global mutex.(locking that mutex for
// any significant section of code caused deadlocks and segv's in
// the krb runtime).
// The ctor locks the mutex and the dtor unlocks it, so be sure
// the object is destroyed.
// Visibility: exported
// Hierarchy: no superclasses or subclasses
// Public Interface:
//  Operations: ctor and dtor only
// Implementation:
//  Uses: Mutex
// Concurrency: blocking
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:


