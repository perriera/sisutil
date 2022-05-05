// YUIDLError.hpp - -*- C++ -*- error message passed over an idl interface
// $Id: YUIDLErr.hpp,v 1.13 2000/01/29 16:44:11 ed Exp $

#ifndef _YUIDL_ERROR_
#define _YUIDL_ERROR_ 1
#ifndef _YU_ERROR_H_INCLUDED
#include <sisutil/Error.hpp>
#define _YU_ERROR_H_INCLUDED
#endif
#include <cstring>
#include <sisutil/YUError.h>    // for YUError_struct   

class YUIDLError : public Error {
public:
  YUIDLError(const YUIDLError& rhs) : Error(rhs), myText(rhs.text()) {}
  inline virtual ~YUIDLError() {}
  // RPC clients use this to accept an error returned by an rpc
  inline YUIDLError(const YUError_struct* ys);
  inline YUIDLError(const Error& e) : Error(e), myText(e.text()) {}
  // RPC manager routines use this to return an error in an rpc
  inline operator YUError_struct* () const;
  virtual inline const std::string text() const {
    return isSuccess() ? std::string("successful completion") : myText;
  }
  virtual inline Error* clone()  const { return new YUIDLError(*this); }
private:
  YUIDLError();                                                                                                                      // disable default ctor
  YUIDLError& operator=(const YUIDLError&);  // disable op=
  std::string myText;
};

// client side
YUIDLError::YUIDLError(const YUError_struct* ys)
  : Error((Error::ErrorStatus)(ys->myStatus),
    (Error::SeverityLevel)(ys->mySeverity)),
  myText((const char*)ys->mytext) {
  //rpc_ss_client_free((const idl_void_p_t)ys);
}

// server side
inline YUIDLError::operator YUError_struct* () const {
  //YUError_struct *ye = (YUError_struct*)rpc_ss_allocate((unsigned)sizeof(YUError_struct));
  YUError_struct* ye = new YUError_struct();
  ye->myStatus = value();
  ye->mySeverity = severity();
  strcpy((char*)ye->mytext, text().c_str());
  return ye;
}

#endif
