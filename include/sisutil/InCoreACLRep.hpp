// InCoreACLRep.hpp  -*- C++ -*- RPC ACL repository for edstest
// $Id: InCoreACLRep.hpp,v 1.7 1999/04/23 05:32:33 ed Exp $

#ifndef _INCOREACLREPHPP
#define _INCOREACLREPHPP 1

//#include <dce/nbase.h>
#include <uuid/uuid.h>
#include <sisutil/ACLRep.hpp>
#include <set>
#include <map>
#include <sisutil/Uuid.hpp>
#include <string>
#include <sisutil/ACL.hpp>

class InCoreACLRep : public ACLRep {
public:
	enum { RPCNAMEMAXLEN = 32 } INCOREACLREPCONSTANTS;
	typedef struct { char RPCName[RPCNAMEMAXLEN]; uuid_t uuid; } ACLEntry;
	// client supplies array of (rpcname,authorizedGroupUuid) pairs
	InCoreACLRep(const ACLEntry*& theRep);
	// client names file of rpcname<white space>authorizedGroupUuid lines
	InCoreACLRep(const std::string& fileName);
	// caller must delete return value
	ACL* lookup(const std::string& key) const;
private:
#if ( defined (__xlC__) && (__xlC__ < 0x400) ) \
	|| ( defined (__SUNPRO_CC) && (__SUNPRO_CC <= 0x420 ) )
	typedef map<string, ACL::UuidSet, less<string> > ACLMap;
#else
	typedef std::map<std::string, ACL::UuidSet> ACLMap;
#endif
	InCoreACLRep();							                    // disable default ctor
	InCoreACLRep(const InCoreACLRep&);              // disable copy ctor
	InCoreACLRep& operator = (const InCoreACLRep&); // disable op=
	ACLMap myACLMap;
};

// Name: InCoreACLRep
// Documentation: DCE Access control list repository.
// This is an in-core table.
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
// End:
