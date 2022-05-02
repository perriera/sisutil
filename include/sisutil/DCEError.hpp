// DCEError.hpp	 -*- C++ -*-	DCE error_status_t utility class
// $Id: DCEError.hpp,v 1.26 2000/01/29 16:44:08 ed Exp $

#ifndef _DCEERROR_H
#define _DCEERROR_H 1

#ifndef _YU_ERROR_H_INCLUDED
#include <sisutil/Error.hpp>
#define _YU_ERROR_H_INCLUDED
#endif

#include <string>

// deprecated - use RPCError in future (OJ)
class DCEError : public Error {
public:
   inline DCEError();
   DCEError(const Error::ErrorStatus,
      const Error::SeverityLevel s = SEV_UNSPECIFIED);
   // DCE specific
   //DCEError(const EXCEPTION e,
   //         const Error::SeverityLevel s = SEV_UNSPECIFIED);
   DCEError(const DCEError&);
   inline virtual ~DCEError();

   DCEError& operator = (const DCEError&);
   inline virtual const std::string text() const;
   inline virtual Error* clone()  const { return new DCEError(*this); }
   inline int isTranslated() const;

protected:
   //virtual void translate(const error_status_t, dce_error_string_t&, int&);
   //virtual void translate(const error_status_t, std::string&, int&);
   //dce_error_string_t myText;
   char myText[160];
   int  myLookupStatus;
};

inline DCEError::DCEError() : myLookupStatus(-1) { myText[0] = '\0'; }

inline const std::string
DCEError::text() const { return (const char*)myText; }

inline int DCEError::isTranslated() const { return myLookupStatus == 0; }

inline DCEError::~DCEError() {}

// Name: DCEError
// Documentation: DCE error_status_t utility class
// Visibility: exported | private | imported
// Cardinality: 0 | 1 | n
// Hierarchy:
//	Superclasses: list of class names
// Generic parameters: list of parameters
// Public Interface:
//	Uses: list of class names
//	Operations: list of operation declarations
// Protected Interface:
//	Uses: list of class names
//	Fields: list of field declarations
//	Operations: list of operation declarations
// Private Interface:
//	Uses: list of class names
//	Fields: list of field declarations
//	Operations: list of operation declarations
// Implementation:
//	Uses: list of class names
//	Fields: list of field declarations
//	Operations: list of operation declarations
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
