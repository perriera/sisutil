// Uuid.hpp  -*- C++ -*- DCE Security Registry Principal and Group uuid class
// $Id: Uuid.hpp,v 1.16 1999/01/14 22:26:03 ed Exp $

#ifndef _UUID_H
#define _UUID_H 1

#include <uuid/uuid.h>
//#include <linux/uuid.h>
#include <string>
#include <sisutil/DCEError.hpp>

class Uuid {
public:
	Uuid();
	inline Uuid(const uuid_t&);
	Uuid(const std::string&);
	inline Uuid(const Uuid&);
	inline Uuid& operator =  (const Uuid&);
	inline bool  operator == (const Uuid&) const;
	inline bool  operator != (const Uuid&) const;
	inline bool  operator <  (const Uuid&) const;

	inline virtual ~Uuid();

	// modify arg to return string containing string form and return
	// a success DCEError, or modify arg to contain "", and return non-success
	DCEError toString(std::string&) const;

	// modify arg to return string containing Security Registry
	// principal name form and return a success DCEError, or modify
	// arg to contain "", and return non-success. Optional sec_rgy handle 
  // directs the lookup.
	//DCEError toPrincipalName(std::string&, const SecRgyBindingHandle* = NULL) const;

	// modify arg to return string containing Security Registry
	// group name form and return a success DCEError, or modify
	// arg to contain "", and return non-success. Optional sec_rgy handle 
   // directs the lookup.
	//DCEError toGroupName(std::string&, const SecRgyBindingHandle* = NULL) const;

	// conversion for input arg to DCE primitive
	inline operator const uuid_t& () const;
	// conversion for in/out arg to DCE primitive
	inline operator uuid_t* ();
private:
	//inline const SecRgyBindingHandle* secRgyContext() const;
   //const SecRgyBindingHandle& getSecRgyBH(const SecRgyBindingHandle*) const;
   //DCEError bindRgy();
	int compare(const Uuid& rhs) const;
	//static SecRgyBindingHandle* mySecRgyContext;
	uuid_t myId;
};

//inline Uuid::Uuid(const uuid_t& u) : myId(u) {}
inline Uuid::Uuid(const uuid_t& u) { uuid_copy(myId, u); }

//inline Uuid::Uuid(const Uuid& u) : myId(u.operator const uuid_t&()) {}
inline Uuid::Uuid(const Uuid& u) { uuid_copy(myId, u.operator const uuid_t & ()); }

inline Uuid& Uuid::operator = (const Uuid& u) {
	//if (this != &u) myId = u.operator const uuid_t&();
	if (this != &u) uuid_copy(myId, u.operator const uuid_t & ());
	return *this;
}

// Comparison operators
inline bool Uuid::operator == (const Uuid& rhs) const { return (compare(rhs) == 0); }
inline bool Uuid::operator <  (const Uuid& rhs) const { return (compare(rhs) < 0); }
inline bool Uuid::operator != (const Uuid& rhs) const { return !(*this == rhs); }

inline Uuid::~Uuid() {}

inline Uuid::operator const uuid_t& () const { return myId; }

inline Uuid::operator uuid_t* () { return &myId; }

//inline const SecRgyBindingHandle* Uuid::secRgyContext() const {return mySecRgyContext;}

// Name: Uuid
// Documentation: DCE uuid_t class
// Hierarchy:
//  Superclasses: none
// Public Interface: hex-dump string form, principal name form
// Protected Interface: none
// Private Interface:
//  Fields: 	uuid_t myId;
// Implementation:
//  Uses: DCEError
// Concurrency: sequential
// Persistance: dynamic
#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// fill-prefix: "// "
// End:
