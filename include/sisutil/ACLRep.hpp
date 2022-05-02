// ACLRep.hpp  -*- C++ -*- DCE Access control list repository ABC
// $Id: ACLRep.hpp,v 1.8 1998/11/18 05:58:32 ed Exp $

#ifndef _ACLREPHPP
#define _ACLREPHPP 1
//#include <dce/nbase.h>
#include <string>

typedef unsigned long unsigned32;

class ACL;

class ACLRep {
public:
		inline ACLRep() {}
		inline virtual ~ACLRep();

		// Return ACLRep for object named by key, or 0 if none exists.
		// Caller must delete return value.
	  virtual ACL* lookup(const std::string& key) const = 0;

		// Return ACL for RPC management function, or 0 if none exists.
		// Caller must delete return value. used by rpc_mgmt_authorization_fn_t see
		// rpc_mgmt_set_authorization_fn(3rpc)
		ACL* lookupRPCMgmtFn(const unsigned32 requestedMgmtOperation) const;
private:
		ACLRep(const ACLRep&);          	// disable copy ctor
		ACLRep& operator=(const ACLRep&); // disable op=
	  //static char* ACLRep::RPCMgmtFnMap[];
		static const char* RPCMgmtFnMap[];
};
inline ACLRep::~ACLRep() {}

// Name: ACLRep
// Documentation: DCE Access control list repository ABC
// This class is not intended to support rdacl.
// Visibility: exported | private | imported
// Cardinality: 0 | 1 | n
// Hierarchy:
//  Superclasses: list of class names
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
// fill-prefix: "// "
// End:
