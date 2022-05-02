// YUAppErr.cpp -  Oracle error messages
// $Id: YUAppErr.cpp,v 1.25 1998/12/02 19:47:21 ed Exp $

#include <sisutil/YUAppErr.hpp>
#include <stdio.h>							// for sprintf

const char* YUAppError::YUApp_err_msg_cat[] = {
	"successful completion",  // leave 0 as "ok"
	"Authentication failure",
	"Authorization failure",
	"Audit failure",
	"No Person \"%s\".",
	"No Course \"%s\".",
	"No Meeting Group \"%s\".",
	"No programme available.",
	"Internal Error %s",
	"Invalid SIN value",
	"SIN value already in use",		// add new ones here only (Audit DB encoding)
	"**end of array fencepost**"
};

YUAppError::YUAppError(const ErrorStatus s, const std::string& moreInfo)
	: Error(s) {
	char tmpBuf[1024];
	const char* morePtr = moreInfo.c_str();
	const char* formatPtr = "???";
	if (s < YUAPP_LIST_END) formatPtr = YUApp_err_msg_cat[s];
	(void)sprintf(tmpBuf, formatPtr, morePtr);
	myText = tmpBuf;
	// myText =  YUApp_err_msg_cat[s] + ": " + moreInfo;
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:

