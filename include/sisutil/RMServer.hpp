// RMServer.hpp  - abstract Resourse Manager initialization interface
// $Id: RMServer.hpp,v 1.4 1998/11/18 20:12:57 ed Exp $

#ifndef _RMSERVER_H
#define _RMSERVER_H 1
#include <string>

class RMServer  {
public:
	inline RMServer() {}
	virtual ~RMServer() {}
	virtual int
	addRM(const std::string& optionsString,
		  const std::string& userName,
		  std::string& password,
		  long serverId = 0) = 0;
	virtual int reopenAll() const = 0;
private:
	RMServer(const RMServer&);	            // disable copy ctor
	RMServer& operator=(const RMServer&);   // disable op=
};

// Name: RMServer
// Documentation: abstract RM interface
// Cardinality:
#endif
