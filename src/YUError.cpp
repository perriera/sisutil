// YUError.cpp - smart pointer class for errors
// $Id: YUError.cpp,v 1.8 1999/01/14 22:26:03 ed Exp $

#include <sisutil/YUError.hpp>
#include <sisutil/Error.hpp>
#include <string>

const std::string YUError::YUError_REP::NoError::text() const { return ""; }

Error* YUError::YUError_REP::NoError::clone() const { return new NoError(*this); }

YUError& YUError::operator = (const YUError& e) {
	if (e.rep) e.rep->count++;
	if (rep && (--rep->count <= 0)) delete rep;
	rep = e.rep;
	return *this;
}

std::ostream& YUError::displayOn(std::ostream& os) const {
	if (rep || rep->err) os << *(rep->err);
	return os;
}

const std::string YUError::text() const {
	return rep ? rep->err->text() : std::string("");
}

YUError::YUError(const YUError& e) : rep(NULL) {
	rep = e.rep;
	if (rep) rep->count++;
}

YUError::~YUError() {
	if (rep && (--rep->count <= 0)) delete rep;
}

int YUError::isSuccess() const {
	return rep ? rep->err->isSuccess() : 1; // default to OK
}

Error::SeverityLevel YUError::severity() const {
	return rep ? rep->err->severity() : Error::SEV_UNSPECIFIED;
}

Error::ErrorStatus YUError::value() const {
	if (rep)
		return rep->err->value();
	return Error::OK;	// default to OK
}

YUError::YUError_REP::YUError_REP(const Error* e)
	: err(e ? e->clone() : new NoError()), count(1) {
}
