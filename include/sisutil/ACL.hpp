// ACL.hpp  -*- C++ -*- DCE Access control list (minimal: groups only)
// $Id: ACL.hpp,v 1.14 1999/04/23 05:32:07 ed Exp $

#ifndef _ACL_HPP
#define _ACL_HPP 1
#include <set>
#include <sisutil/Uuid.hpp>
#include <string>
//#include <initializer_list>

class ACLRep;

class ACL {
public:
#if ( defined (__xlC__) && (__xlC__ < 0x400) ) \
	|| ( defined (__SUNPRO_CC) && (__SUNPRO_CC <= 0x420 ) )
	typedef set<Uuid, less<Uuid> > UuidSet;
#else
	typedef std::set<Uuid> UuidSet;
#endif
	inline ACL();
	inline virtual ~ACL();
	inline void addGroup(const Uuid&);

	// Return ACL for object named by key, or 0 if none exists.
	// Caller must delete return value.
	static ACL* lookup(const std::string& key);

	inline const UuidSet& groupEntries() const;
	static void setRepository(const ACLRep&);
private:
	ACL(const ACL&);								// disable default ctor
	ACL& operator=(const ACL&);			// disable op=
	UuidSet myGroupEntries;
	static const ACLRep* aclRep(); 	// singleton
	static const ACLRep* myACLRep;
};

inline ACL::ACL() {}
inline ACL::~ACL() {}

inline void ACL::addGroup(const Uuid& u) { myGroupEntries.insert(u); }

inline const ACL::UuidSet& ACL::groupEntries() const { return myGroupEntries; }

// Name: ACL
// Documentation: DCE Access control list
// This class is not intended to support rdacl. It stores group entries only.
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
