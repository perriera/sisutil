// Error.hpp	 -*- C++ -*-	 error status utility abstract class
// $Id: Error.hpp,v 1.16 1998/11/18 02:10:21 ed Exp $

#ifndef _ERROR_H
#define _ERROR_H 1
#include <iostream>
#include <string>

class Error {
public:
  typedef unsigned long ErrorStatus;
  // status value 0 is always "success"
  typedef enum ErrorStatusValueEnum {OK = 0} ErrorStatusValue;
  typedef enum ErrorStatusMaskValueEnum {FACILITYMASK = 0xfffff000} ErrorStatusMaskValue;
  typedef enum SeverityLevelEnum {SEV_UNSPECIFIED = 0, SEV_INFORMATIONAL, 
											 SEV_WARNING, SEV_ERROR, SEV_FATAL}	SeverityLevel;
  inline Error();
  inline Error(const ErrorStatus, const SeverityLevel = SEV_UNSPECIFIED);
  inline Error(const Error&);
  Error& operator = (const Error&);
  inline  int     isSuccess() const;
  inline          SeverityLevel severity() const;
  const           std::string severityText() const;
  virtual const   std::string text() const = 0;
  inline          ErrorStatus value() const;
  inline          unsigned long facility() const;
  inline  virtual ~Error();
  virtual Error*  clone() const = 0;

protected:
  ErrorStatus     myErrorStatus;
  SeverityLevel   mySeverity;
  //static char* Error::theSeverityTextArray[];
  static const char* theSeverityTextArray[];
};

inline Error::Error() : myErrorStatus(OK), mySeverity(SEV_UNSPECIFIED) {}

inline Error::Error(const ErrorStatus status, const SeverityLevel sev)
  : myErrorStatus(status), mySeverity(sev) {}

inline Error::Error(const Error& e) 
  : myErrorStatus(e.value()), mySeverity(e.severity()) {}

inline int Error::isSuccess() const {return value() == OK;}

inline Error::SeverityLevel Error::severity() const {return mySeverity;}

inline Error::ErrorStatus Error::value() const {return myErrorStatus;}

inline unsigned long
Error::facility() const {return value() & FACILITYMASK;}

inline Error::~Error() {}

std::ostream& operator << (std::ostream&, const Error&);

// Name: Error
// Documentation:  Error status utility abstract class
// N.B. FACILITYMASK follows the convention set by rpc_s_mod in DCE rpcsts.idl
// Hierarchy:
//	Superclasses: none
// Public Interface:
//	Uses: list of class names
//	Operations: list of operation declarations
// Protected Interface:
//	Uses: list of class names
//	Fields: list of field declarations
//	Operations: list of operation declarations
// Concurrency: sequential
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57) // fill-column: 70
// fill-column: 70
// End:
