// ErrLog.cpp - shared Error log
// $Id: ErrLog.cpp,v 1.9 1999/04/26 03:11:01 ed Exp $

#include <sisutil/ErrLog.hpp>
#include <time.h>
//#include <strstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <sisutil/GMLock.hpp>
#include <fstream>
#include <sisutil/DCEError.hpp>

#include <unistd.h>                // for syscall()
#include <sys/syscall.h>           // for syscall() to call gettid()

ErrLog::ErrLog(const std::string& fileName) : myStream(std::cerr) {
   std::ofstream f(fileName.c_str());
   if (f.good()) {
      //myStream = f;
      myStream.rdbuf(f.rdbuf()); // replace ostream_withassign
   }
   else {
      std::string errMsg = "cannot open " + fileName;
      logErrMsg("ErrLog", "ErrLog", "ofstream::open()", errMsg);
   }
}

void ErrLog::logErrMsg(const char* theClass,
   const char* callingRoutine,
   const char* calledRoutine,
   const std::string& theMessage) {
   GlobalMutexLock serializeThisScope;
   putWho(theClass, callingRoutine);
   myStream << calledRoutine << " returns " << theMessage << std::endl;
   myStream.flush();
}

void ErrLog::putLine(const char* theClass,
   const char* callingRoutine,
   const char* what) {
   GlobalMutexLock serializeThisScope;
   putWho(theClass, callingRoutine);
   myStream << what << std::endl;
   myStream.flush();
}

void ErrLog::putStream(const char* theClass,
   const char* callingRoutine,
   std::stringstream& what) {
   GlobalMutexLock serializeThisScope;
   putWho(theClass, callingRoutine);
   //const char* whatStr = what.str().c_str();
   //myStream << whatStr << std::endl;
   std::string whatStr = what.str();
   myStream << whatStr << std::endl;
   myStream.flush();
   //delete whatStr;
}

std::string ErrLog::currentDateTime() {
   time_t now;
   if (time(&now) != -1) {
      const int BUF_SIZE = 26;
      char buf[BUF_SIZE];
      (void)ctime_r(&now, buf);
      buf[BUF_SIZE - 2] = '\0'; // axe trailing \n
      return buf;
   }
   return "time unavailable";
}

// _DECTHREADS_ pthread_t type
// typedef struct CMA_T_HANDLE {
//     cma_t_address  field1;
//     short int    field2; // this one appears to be a useful id
//     short int    field3;
//     } cma_t_handle;

void
ErrLog::putWho(const char* theClass, const char* callingRoutine) {
   //pthread_t thisThread = pthread_self();
   pid_t thisThread = syscall(SYS_gettid);  // no glibc wrapper for gettid() so do it this way; the thread id may be reused
   myStream << "[" << currentDateTime() << "] ";
   myStream << theClass << "::" << callingRoutine << "["
      << std::setw(2) << std::setfill('0')
      << std::hex << (unsigned short)thisThread << std::dec
      << "]: ";
}

/* does not appear to be used anywhere
// hash pointer value to small value for logging
static unsigned char fingerprint(void* p) {
   const int len = sizeof(p);
   register unsigned char hash = 0;
   register unsigned char g = 0;
   for (int i = 0; i < len; i++) {
      hash = (hash << 4) + ((const char* const)p)[i];
      if ((g = hash & 0xf0)) {
         hash = hash ^ (g >> 4);
         hash = hash ^ g;
      }
   }
   return hash;
}
*/
