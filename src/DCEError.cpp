// DCEError.cpp
// $Id: DCEError.cpp,v 1.19 1998/11/18 06:01:05 ed Exp $

//#include <strstream.h>
#include <sstream>
#include <cstring>       // for strncpy
#include <sisutil/DCEError.hpp>

// TODO: this class handles DCEError's in particular and should be removed entirely.
//       Unfortunately, the class permeates our applications so it is not currently possible.
//       A new class, RPCError, has been created to replace this one and should be used where possible.
DCEError::DCEError(const Error::ErrorStatus status,
	const Error::SeverityLevel serverity)
	: Error(status, serverity), myLookupStatus(-1) {
	myText[0] = '\0';
	myLookupStatus = 0;
	// N.B.: this depends on environment variables LANG and NLSPATH
	  //translate(myErrorStatus, myText, myLookupStatus);
}

// copy c'tor
DCEError::DCEError(const DCEError& d)
	: Error(d.value()), myLookupStatus(d.isTranslated() ? 0 : -1) {
	//strncpy((char*) myText, d.text().c_str(), dce_c_error_string_len);
	strncpy((char*)myText, d.text().c_str(), 160);
}

/* DCE specific
DCEError::DCEError(const Error::ErrorStatus status, const Error::SeverityLevel serverity)
	: Error(OK, serverity), myLookupStatus(-1)  {
	if (exc_get_status(&e, &myErrorStatus)) {
		myText[0] = '\0';
		//ostrstream t((char*) myText, dce_c_error_string_len);
		ostringstream t((char*) myText, std::ios_base::app);
		t << "unknown exception at " << (void*) e.address.address;
	}	else {
		dce_error_inq_text(myErrorStatus, myText, &myLookupStatus);
	}
}
*/

/* DCE specific
void DCEError::translate(const error_status_t aStatus,
							 dce_error_string_t& aString,
							 int& transResult) {
	dce_error_inq_text(aStatus, aString, &transResult);
}
*/

DCEError& DCEError::operator = (const DCEError& d) {
	if (this != &d) {
		this->Error::operator = (d);
		//strncpy((char*) myText, d.text().c_str(), dce_c_error_string_len);
		strncpy((char*)myText, d.text().c_str(), 160);
		myLookupStatus = d.isTranslated() ? 0 : -1;
	}
	return *this;
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// fill-prefix: "// "
// End:
