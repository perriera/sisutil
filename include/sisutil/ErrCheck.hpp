// ErrCheck.hpp  -*- C++ -*-
// class ErrorChecked -  DCE error status abstract base class 
// $Id: ErrCheck.hpp,v 1.3 1993/09/21 01:24:08 ed Exp $

#ifndef _ERRORCHECKED_H
#define _ERRORCHECKED_H 1

#include <sisutil/DCEError.hpp>

class ErrorChecked {
public:
	inline ErrorChecked();
	inline ErrorChecked(const ErrorChecked&);
	inline ErrorChecked& operator=(const ErrorChecked&);
	inline virtual ~ErrorChecked();

	inline int isValid() const;
	inline DCEError lastError() const;
	// set the error status
	inline void error(const error_status_t);
	inline error_status_t error() const;
private:
	error_status_t myStatus;
};

inline
ErrorChecked::ErrorChecked() : myStatus((error_status_t)-1) {}

inline
ErrorChecked::ErrorChecked(const ErrorChecked& e) : myStatus(e.error()) {}

inline
ErrorChecked& ErrorChecked::operator=(const ErrorChecked& e) {
	if (this != &e) error(e.error());
	return *this;
}

inline
ErrorChecked::~ErrorChecked() {}

inline int
ErrorChecked::isValid() const { return error() == error_status_ok; }

inline DCEError
ErrorChecked::lastError() const { return DCEError(error()); }

inline void
ErrorChecked::error(const error_status_t e) { myStatus = e; }

inline error_status_t
ErrorChecked::error() const { return myStatus; }

// Name: ErrorChecked
// Documentation: DCE error status base class
//  Any class that uses calls to the DCE runtime in constructors 
//  can inherit from this class to manage the storage and access to the
//  status value returned from the runtime. 
// Hierarchy:
//  Superclasses: none
// Public Interface:
// Protected Interface: none
// Private Interface:
//  Fields: 	error_status_t myStatus;
// Implementation:
//  Uses: DCEError
// Concurrency: sequential
// Persistance: dynamic
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// fill-prefix: "// "
// End:
