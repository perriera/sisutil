// Mutex.cpp
// $Id: Mutex.cpp,v 1.2 1995/04/06 01:32:07 ed Exp $

#include <sisutil/Mutex.hpp>
#include <sisutil/ErrLog.hpp>

Mutex::Mutex() {
	ErrLog& theErrLog = getTheErrLog();
	// Implement recursive mutexes (overkill? but original design - OJ)
	//if (pthread_mutexattr_create(&myMutexAttr))
	if (pthread_mutexattr_init(&myMutexAttr))
		theErrLog.putLine("Mutex", "Mutex", "pthread_mutexattr_create failed.");
	//if (pthread_mutexattr_setkind_np(&myMutexAttr, MUTEX_RECURSIVE_NP))
	if (pthread_mutexattr_settype(&myMutexAttr, PTHREAD_MUTEX_RECURSIVE))
		theErrLog.putLine("Mutex", "Mutex", "pthread_mutexattr_settype failed.");
	//										         "pthread_mutexattr_setkind_np failed.");
   //if (pthread_mutex_init(&myMutex, myMutexAttr))
	if (pthread_mutex_init(&myMutex, &myMutexAttr))
		theErrLog.putLine("Mutex", "Mutex", "pthread_mutex_init failed.");
}

void Mutex::lock() {
	ErrLog& theErrLog = getTheErrLog();
	if (pthread_mutex_lock(&myMutex))
		theErrLog.putLine("Mutex", "Unlock", "pthread_mutex_lock failed.");
}

void Mutex::unlock() {
	ErrLog& theErrLog = getTheErrLog();
	if (pthread_mutex_unlock(&myMutex))
		theErrLog.putLine("Mutex", "Unlock", "pthread_mutex_unlock failed.");
}

Mutex::~Mutex() {
	ErrLog& theErrLog = getTheErrLog();
	if (pthread_mutex_destroy(&myMutex))
		theErrLog.putLine("Mutex", "Mutex", "pthread_mutex_destroy failed.");
	//if (pthread_mutexattr_delete(&myMutexAttr))
	 //	theErrLog.putLine("Mutex", "Mutex", "pthread_mutexattr_delete failed.");
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57)
// fill-column: 70
// End:
