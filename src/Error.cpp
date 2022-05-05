// Error.cpp -  error status utility class
// $Id: Error.cpp,v 1.11 1998/11/18 02:10:18 ed Exp $

#include <sisutil/Error.hpp>

const char* Error::theSeverityTextArray[]
= { "Unspecified", "Informational", "Warning", "Error", "Fatal" };

const std::string Error::severityText() const {
	if ((severity() < SEV_UNSPECIFIED) || (severity() > SEV_FATAL))
		return "Unknown";
	return theSeverityTextArray[severity()];
}

Error& Error::operator = (const Error& d) {
	if (this != &d) {
		// int temp;
		//std::cerr << "Error::operator = : setting myErrorStatus = " << d.value() << std::endl;
		//temp = d.value();myErrorStatus = temp;
		myErrorStatus = d.value();
		//std::cerr << "Error::operator = : setting mySeverity = " << d.severity() << std::endl;
		//SeverityLevel temp2 = d.severity();mySeverity = temp2;
		mySeverity = d.severity();
	}
	return *this;
}

std::ostream& operator << (std::ostream& o, const Error& d) {
	const std::string& theText = d.text();
	if (theText.length()) o << d.text();
	else o << "status value:" << d.value() << " (no translation)";
	return o;
}
