// RPCMarshall.hpp  - RPC data value marshaling abstract base class
// $Id: RPCMarsh.hpp,v 1.3 2000/01/29 16:44:09 ed Exp $
#ifndef _RPCMARSHALL_HPP_
#define _RPCMARSHALL_HPP_ 1

#ifndef _YU_MARSHALL_H_INCLUDED
#include <sisutil/Marshall.hpp>
#define _YU_MARSHALL_H_INCLUDED
#endif

#include <sisutil/YUFlatArray.h>

class RPCMarshall : public Marshall {
public:
	typedef YUFlatArray** ppYUFlatArray;

	inline RPCMarshall(const int& DefaultSize) : Marshall(DefaultSize) {}
	inline RPCMarshall(YUFlatArray* sfa) : Marshall(sfa) {}
	inline virtual ~RPCMarshall() {}

	inline operator ppYUFlatArray();
	inline operator YUFlatArray* ();

private:
	RPCMarshall();											// disable default ctor
	RPCMarshall(const RPCMarshall&);			      // disable copy ctor
	RPCMarshall& operator=(const RPCMarshall&);	// disable op=
};

inline RPCMarshall::operator RPCMarshall::ppYUFlatArray() {
	return &myArray;
}

inline RPCMarshall::operator YUFlatArray* () {
	return myArray;
}

// Name: RPCMarshall
// Documentation: RPC data value marshaling abstract base class
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
