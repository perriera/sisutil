// YUBool.cpp - self marshalling booleans
// $Id: YUBool.cpp,v 1.1 1996/06/24 15:51:51 ed Exp $

#include <sisutil/YUBool.hpp>

YUBool::YUBool(const YUBool& rSrc) : YUFixedChar(1) {
	rSrc.isSet() ? YUFixedChar::operator = ("Y") : YUFixedChar::operator = ("N");
}

YUBool& YUBool::operator = (const YUBool& rhs) {
	if (this != &rhs)
		rhs.isSet() ? YUFixedChar::operator = ("Y") : YUFixedChar::operator = ("N");
	return(*this);
}

YUBool& YUBool::operator = (const int b) {
	b ? YUFixedChar::operator = ("Y") : YUFixedChar::operator = ("N");
	return(*this);
}

YUBool& YUBool::operator=(const char c) {
	c == 'Y' ? YUFixedChar::operator = ("Y") : YUFixedChar::operator = ("N");
	return(*this);
}

void YUBool::Set(int b) {
	b ? YUFixedChar::operator=("Y") : YUFixedChar::operator = ("N");
}
