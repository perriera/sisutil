// OCIMarshall.hpp  - OCI generic data marshalling
// $Id: OCIMarsh.hpp,v 1.3 2000/01/29 16:44:08 ed Exp $

#ifndef _OCIMARSHALL_H
#define _OCIMARSHALL_H 1

#ifndef _YU_MARSHALL_H_INCLUDED
#include <sisutil/Marshall.hpp>
#define _YU_MARSHALL_H_INCLUDED
#endif

class OCICursor;

class OCIMarshall : public Marshall {
public:
	inline OCIMarshall(const int DefaultSize) : Marshall(DefaultSize) {};
	inline virtual ~OCIMarshall() {}
	friend OCICursor;

private:
	OCIMarshall();									     // disable default ctor
	OCIMarshall(const OCIMarshall&);	           // disable copy ctor
	OCIMarshall& operator=(const OCIMarshall&); // disable op=
};

// Name: OCIMarshall
// Documentation: text
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
