// YUApp.H  -*- C++ -*- Application anchor base class
// $Id: YUApp.hpp,v 1.14 1999/04/22 23:57:18 ed Exp $

#ifndef _YUAPP_H
#define _YUAPP_H 1

#include <sisutil/YUAppErr.hpp>
#include <iostream>

class YUError;
class Error;
class ErrLog;

class YUApp {
public:
   YUApp(std::ostream& errStream = std::cerr); // was ostream_withassign but that has been removed from standard C++
   YUApp(const std::string& errStreamFileName);
   virtual ~YUApp();

   static YUApp* theApp(); // Singleton, the one and only
   static ErrLog& errLog();
   static int     checkStatus(const Error& e, const std::string& className,
      const std::string& routine,
      const std::string& calledRoutine);
   static YUError internalError(const std::string& className,
      const std::string& callingRoutine,
      const std::string& calledRoutine,
      const Error& err);
   static pid_t   pid();
   enum { MINLEVEL = 0, DEFAULTLEVEL = 1, MAXLEVEL = 10 } TraceLevels;
   // control error log detail, 0 == minimal, x|x>=10 == maximal
   static long    getTraceLevel();
   static void    setTraceLevel(long traceLevel);

   class NewHandler {        // inner class
   public:
      NewHandler();
      static void new_error_handler();
   };

   class LastChanceHandler { // inner class
   public:
      LastChanceHandler();
      //static void LastChanceHandler::last_exception_handler();
      static void last_exception_handler();
   };

protected:
   static YUApp* theAppAnchor;

private:
   YUApp(const YUApp&);              // disable copying
   YUApp& operator = (const YUApp&); // disable assigning
   static ErrLog* theErrLog;
   static NewHandler        theNewHandlerInit;
   static LastChanceHandler theLastChanceHandlerInit;
   static long              myTraceLevel;
};

// Name: YUApp
// Documentation: Application anchor for application wide services such as
//  error logging
// Cardinality: 1
// Hierarchy:
// Public Interface:
/// Concurrency: sequential
#endif
