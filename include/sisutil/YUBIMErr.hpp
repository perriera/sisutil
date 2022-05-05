// YUBIM_Error.hpp - York University BIM server error messages
// $Id: YUBIMErr.hpp,v 1.5 2000/01/29 16:44:09 ed Exp $

#ifndef _YUBIM_ERROR_
#define _YUBIM_ERROR_ 1
#ifndef _YU_ERROR_H_INCLUDED
#include <sisutil/Error.hpp>
#define _YU_ERROR_H_INCLUDED
#endif

class YUBIMError : public Error {
public:
	inline YUBIMError(const YUBIMError& rhs)
		: Error(rhs), mytext(rhs.text()) {}
	inline YUBIMError(const long num, const std::string& cs)
		: Error(num), mytext(cs) {};
	inline virtual const std::string text() const { return mytext; }
	inline virtual Error* clone() const { return new YUBIMError(*this); }
protected:
	std::string mytext;
private:
	YUBIMError();								// disable
};

// Name: YUAppError
// Documentation: YU BIM server error messages
// Cardinality: 1
// Hierarchy:
//  Superclasses: Error
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

