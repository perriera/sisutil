// ErrLog.hpp  -*- C++ -*- shared Error log 
// $Id: ErrLog.hpp,v 1.18 1999/04/22 23:57:05 ed Exp $

#ifndef _ERRLOG_H
#define _ERRLOG_H 1
typedef unsigned long   unsigned32;
#include <fstream>
#include <iostream>
//#include <strstream>
#include <sstream>
#include <sisutil/DCEError.hpp>

// forward
class  ErrLog;
extern ErrLog& getTheErrLog();	// defined by YUApp, or main()
extern ErrLog& getTheAuditLog();

class ErrLog {
public:
	inline ErrLog();
	ErrLog(const std::string& fileName);
	//inline ErrLog(ostream_withassign&);
	inline ErrLog(std::ostream&);
	inline virtual ~ErrLog();
	inline void logErrMsg(const char* theClass,
		const char* callingRoutine,
		const char* calledRoutine,
		const unsigned32 status);	// DCE error_status_t
	inline void logErrMsg(const char* theClass,
		const char* callingRoutine,
		const char* calledRoutine,
		const Error& err);
	void logErrMsg(const char* theClass,
		const char* callingRoutine,
		const char* calledRoutine,
		const std::string& theMessage);
	void putLine(const char* theClass,
		const char* callingRoutine,
		const char* what);
	// copy strstream contents and de-allocate
	void putStream(const char* theClass,
		const char* callingRoutine,
		std::stringstream& what); //strstream& what);
	inline void logErrMsg(const std::string& theClass,
		const std::string& callingRoutine,
		const std::string& calledRoutine,
		const unsigned32 status);	// DCE error_status_t
	inline void logErrMsg(const std::string& theClass,
		const std::string& callingRoutine,
		const std::string& calledRoutine,
		const Error& err);
	inline void logErrMsg(const std::string& theClass,
		const std::string& callingRoutine,
		const std::string& calledRoutine,
		const std::string& theMessage);
	inline void putLine(const std::string& theClass,
		const std::string& callingRoutine,
		const std::string& what);
	// copy strstream contents and de-allocate
	inline void putStream(const std::string& theClass,
		const std::string& callingRoutine,
		std::stringstream& what);	//strstream&    what);
	static std::string currentDateTime();

private:
	ErrLog(const ErrLog&);						  // disable copy ctor
	void putWho(const char* theClass, const char* callingRoutine);
	void putThreadId();
	ErrLog& operator = (const ErrLog&); // disable op =
	//ostream_withassign& myStream;
	std::ostream& myStream;
};

inline ErrLog::ErrLog() : myStream(std::cerr) {}
//inline ErrLog::ErrLog(std::ostream_withassign& o) : myStream(o) {}
inline ErrLog::ErrLog(std::ostream& o) : myStream(o) {}
//inline ErrLog::ErrLog(std::ostream& o) {streambuf* sbuf = o.rdbuf(); myStream = new ostream(); myStream.rdbuf(sbuf);}

inline ErrLog::~ErrLog() {}

inline void ErrLog::logErrMsg(const char* theClass,
	const char* callingRoutine,
	const char* calledRoutine,
	const unsigned32 status) {
	DCEError theError(status);
	//std::string theText(theError.text());
	//logErrMsg(theClass, callingRoutine, calledRoutine, theText.c_str());
	logErrMsg(theClass, callingRoutine, calledRoutine, theError.text());
}

inline void ErrLog::logErrMsg(const char* theClass,
	const char* callingRoutine,
	const char* calledRoutine,
	const Error& err) {
	logErrMsg(theClass, callingRoutine, calledRoutine, err.text());
}

inline void ErrLog::logErrMsg(const std::string& theClass,
	const std::string& callingRoutine,
	const std::string& calledRoutine,
	const unsigned32 status) {
	logErrMsg(theClass.c_str(), calledRoutine.c_str(), callingRoutine.c_str(), status);
}

inline void ErrLog::logErrMsg(const std::string& theClass,
	const std::string& callingRoutine,
	const std::string& calledRoutine,
	const Error& err) {
	logErrMsg(theClass.c_str(), callingRoutine.c_str(), calledRoutine.c_str(), err);
}

inline void ErrLog::logErrMsg(const std::string& theClass,
	const std::string& callingRoutine,
	const std::string& calledRoutine,
	const std::string& theMessage) {
	logErrMsg(theClass.c_str(), callingRoutine.c_str(), calledRoutine.c_str(), theMessage.c_str());
}

inline void ErrLog::putLine(const std::string& theClass,
	const std::string& callingRoutine,
	const std::string& what) {
	putLine(theClass.c_str(), callingRoutine.c_str(), what.c_str());
}

// copy strstream contents and de-allocate
inline void ErrLog::putStream(const std::string& theClass,
	const std::string& callingRoutine,
	std::stringstream& what) {  //strstream& what)
	putStream(theClass.c_str(), callingRoutine.c_str(), what);
}

// Name: ErrLog
// Documentation: thread-safe error logging facility
// Cardinality: 1
// Hierarchy:
//  Superclasses: none
// Public Interface:
//  Uses: list of class names
//  Operations: list of operation declarations
// Private Interface:
//  Uses: list of class names
//  Fields: list of field declarations
//  Operations: list of operation declarations
// Concurrency: blocking
// Persistance: dynamic
#endif
