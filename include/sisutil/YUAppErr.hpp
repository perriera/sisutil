// YUAppError.hpp - York Univeristy Application error messages
// $Id: YUAppErr.hpp,v 1.25 1998/11/18 06:04:01 ed Exp $

#ifndef _YUAPPERROR_
#define _YUAPPERROR_ 1
#include <sisutil/Error.hpp>

class YUAppError : public Error {
public:
	typedef enum { YUAPP_BASE = 0 } YUAppErrorBase;
	typedef enum {
		YUAPP_OK = YUAPP_BASE,
		YUAPP_AUTHEN_FAILURE, YUAPP_AUTHOR_FAILURE,
		YUAPP_AUDIT_FAILURE,
		YUAPP_NO_SUCH_PERSON,
		YUAPP_NO_SUCH_COURSE, YUAPP_NO_SUCH_MEET, YUAPP_NO_PROGRAMME,
		YUAPP_INTERNAL_ERROR,
		YUAPP_INVALID_SIN, YUAPP_DUPLICATE_SIN, // add new ones here
		YUAPP_LIST_END	
} ErrorStatus;
	YUAppError(const ErrorStatus s, const std::string& moreInfo = "");
	inline YUAppError(const YUAppError& rhs);
	inline YUAppError& operator = (const YUAppError& rhs);
	inline virtual ~YUAppError() {}
	inline YUAppError() {};
	inline virtual const std::string text() const { return myText; }
	inline virtual Error* clone()  const { return new YUAppError(*this); }
protected:
	std::string myText;
	static const char* YUApp_err_msg_cat[];
};

inline  YUAppError::YUAppError(const YUAppError& rhs)
	: Error(rhs), myText(rhs.text()) {}

inline YUAppError& YUAppError::operator = (const YUAppError& rhs) {
	if (this != &rhs) {
		Error::operator = (rhs);
	}
	return *this;
}

// Name: YUAppError
// Documentation: Oracle YUApp error messages
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
