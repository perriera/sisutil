// YUUuid.hpp  - marshalling DCE uuid_t
// $Id: YUUuid.hpp,v 1.9 2000/01/29 16:44:12 ed Exp $

#ifndef _YUUUID_H_
#define _YUUUID_H_ 1

#include <sisutil/Uuid.hpp>
#include <sisutil/Marshall.hpp>

#include <string>

class RPCMarshall;
class OCIMarshall;							// XXX todo: factor this out to ociutil
class delim_ofstream;
class delim_ifstream;

class YUUuid : public Uuid, public YUStreamable {
public:
	// todo: design bug; this should be disabled
	inline YUUuid() : Uuid("") {}
	inline YUUuid(const uuid_t& u) : Uuid(u) {};
	inline YUUuid(const std::string& c) : Uuid(c) {};
	inline YUUuid(const Uuid& u) : Uuid(u) {};

	inline YUUuid& operator = (const YUUuid&);
	inline virtual long    getLenRPCMarshall() const { return(36L); }
	// canonical form over an RPC
	friend RPCMarshall& operator << (RPCMarshall& m, const YUUuid& fc);
	friend RPCMarshall& operator >> (RPCMarshall& m, YUUuid& fc);
	// binary form into the DB
	friend OCIMarshall& operator << (OCIMarshall& m, const YUUuid& fc);
	friend OCIMarshall& operator >> (OCIMarshall& m, YUUuid& fc);

	friend delim_ofstream& operator << (delim_ofstream& rS, const YUUuid& fc);
	friend delim_ifstream& operator >> (delim_ifstream& rS, YUUuid& fc);

	friend std::ostream& operator << (std::ostream& m, const YUUuid& fc);

private:
};

inline YUUuid& YUUuid::operator = (const YUUuid& rhs) {
	if (this != &rhs) Uuid::operator = (rhs);
	return *this;
}


// Name: YUUuid
// Documentation: self marshalling DCE uuid_t
// Visibility: exported
// Cardinality: n
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
