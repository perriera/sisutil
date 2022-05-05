// YUApp.cpp - Application anchor base class
// $Id: YUApp.cpp,v 1.17 2000/02/08 16:47:27 ed Exp $

#include <sisutil/YUApp.hpp>
#include <sisutil/ErrLog.hpp>
#include <sisutil/YUError.hpp>
#include <unistd.h>							// for getpid()
#include <stdlib.h>                    // for abort()
#include <new>
#include <cstring>

YUApp* YUApp::theAppAnchor = NULL;
ErrLog* YUApp::theErrLog = NULL;

YUApp::NewHandler        theNewHandlerInit;
YUApp::LastChanceHandler theLastChanceHandlerInit;

long YUApp::myTraceLevel = YUApp::DEFAULTLEVEL;

YUApp::NewHandler::NewHandler() {
   //std::cerr << "YUApp::NewHandler::NewHandler() called" << std::endl;
   std::set_new_handler(NewHandler::new_error_handler);
}

void YUApp::NewHandler::new_error_handler() {
   //std::cerr << "YUApp::NewHandler::new_error_handler() called" << std::endl;
   YUApp::errLog().putLine("", "new_error_handler", "new fails");
   abort();
}

YUApp::LastChanceHandler::LastChanceHandler() {
   //std::cerr << "YUApp::LastChanceHandler::LastChanceHandler() called" << std::endl;
   (void)std::set_terminate(LastChanceHandler::last_exception_handler);
   (void)std::set_unexpected(LastChanceHandler::last_exception_handler);
}

void YUApp::LastChanceHandler::last_exception_handler() {
   //std::cerr << "YUApp::LastChanceHandler::last_exception_handler() called" << std::endl;
   errLog().putLine("", "last_exception_handler", "uncaught exception");
   abort();
}

/*
 * Old code used a ostream_withassign class as the input parameter but ostream_withassign was
 * removed from standard C++ (it's unsafe). Instead, an ostream class with the rdbuf replaced
 * is the preferred approach to switching streams.
 * Expecting a modified (or not) ostream here
 */
YUApp::YUApp(std::ostream& errStream) {  // = std::cerr <- moved to hpp file!
   // last one in wins
   //std::cerr << "YUApp c'tor 1 called" << std::endl;
   theAppAnchor = this;
   if (theErrLog) delete theErrLog;
   theErrLog = new ErrLog(errStream);
}

YUApp::YUApp(const std::string& errStreamFileName) {
   //std::cerr << "YUApp c'tor 2 called" << std::endl;
   // last one in wins
   theAppAnchor = this;
   if (theErrLog) delete theErrLog;
   theErrLog = new ErrLog(errStreamFileName);
}

YUApp::~YUApp() {
   //std::cerr << "YUApp d'tor called" << std::endl;
   if (theErrLog) delete theErrLog;
}

int YUApp::checkStatus(const Error& e,
   const std::string& className,
   const std::string& routine,
   const std::string& calledRoutine) {
   //std::cerr << "YUApp::checkStatus() called" << std::endl;
   if (!e.isSuccess()) {
      errLog().logErrMsg(className, routine, calledRoutine, e);
   }
   return e.isSuccess();
}

// low level errLog access, YUApp clients can use YUApp::checkStatus
// or YUApp::errLog()
ErrLog& getTheErrLog() {
   //std::cerr << "YUApp::getTheErrLog() called" << std::endl;
   return YUApp::errLog();
}

ErrLog& YUApp::errLog() {
   //std::cerr << "YUApp::errLog() called" << std::endl;
   // make an errLog always available
   if (!theErrLog) theErrLog = new ErrLog(std::cerr);  // TODO: new_handler
   return *theErrLog;
}

YUApp* YUApp::theApp() {
   //std::cerr << "YUApp::theApp() called" << std::endl;
   // make an anchor always available, but don't delete it in ~this
   if (!theAppAnchor) theAppAnchor = new YUApp(std::cerr);     // TODO: new_handler
   return theAppAnchor;
}

YUError YUApp::internalError(const std::string& className,
   const std::string& callingRoutine,
   const std::string& calledRoutine,
   const Error& err) {
   std::cerr << "YUApp::internalError() called" << std::endl;
   std::stringstream tmpStrm;
   tmpStrm << className << "::" << callingRoutine << ": " << calledRoutine;
   if (err.isSuccess())
      tmpStrm << " fails";
   else
      tmpStrm << " returns " << err;
   //tmpStrm << std::ends;

// /home/perry/.cache/CPM/sisutil/15422c9bb34f1e12b4c68d2763188f2f850b7f21/src/YUApp.cpp:114:27: 
// warning: object backing the pointer will be destroyed at the end of the full-expression [-Wdangling-gsl]
//    const char* moreInfo = tmpStrm.str().c_str();
//                           ^~~~~~~~~~~~~
// 1 warning generated.

   const char* moreInfo = tmpStrm.str().c_str();
   std::string dup(moreInfo);
   YUAppError retVal(YUAppError::YUAPP_INTERNAL_ERROR, dup);
   // give back dealloc responsiblity for *moreInfo
   //tmpStrm.rdbuf()->freeze(0);  // stringstream has no freeze() method (internal storage is a string object not a char array)
   return YUError(retVal);
}

pid_t YUApp::pid() {
   //std::cerr << "YUApp::pid() called" << std::endl;
   return getpid();
}

// control error log detail, 0 == minimal, x|x>=10 == maximal 
void YUApp::setTraceLevel(long traceLevel) {
   //std::cerr << "YUApp::setTraceLevel() called" << std::endl;
   if (traceLevel > MAXLEVEL) traceLevel = MAXLEVEL;
   if (traceLevel < MINLEVEL) traceLevel = MINLEVEL;
   // N.B. un-synced access; check for partial writes or wild values
   {
      //GlobalMutexLock serializeThisScope;
      YUApp::myTraceLevel = traceLevel;
   }
}

long YUApp::getTraceLevel() {
   //std::cerr << "YUApp::getTraceLevel() called" << std::endl;
   // N.B. un-synced access; check for partial writes or wild values
   if (YUApp::myTraceLevel > MAXLEVEL) return MAXLEVEL;
   if (YUApp::myTraceLevel < MINLEVEL) return MINLEVEL;
   return YUApp::myTraceLevel;
}
