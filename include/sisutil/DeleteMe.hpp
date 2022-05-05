// DeleteMe.hpp - template class for deleting new'd objects
// $Id: DeleteMe.hpp,v 1.2 1995/06/07 16:22:08 ed Exp $
#ifndef _CLASS_H
#define _CLASS_H 1

template <class T>
class DeleteMe {
public:
		inline DeleteMe(T* an0bject) : myPtr(an0bject) {}
		inline virtual ~DeleteMe() {delete myPtr;}

private:
		DeleteMe();																	// disable default ctor
		DeleteMe(const DeleteMe<T>&);								// disable copy ctor
		DeleteMe<T>& operator=(const DeleteMe<T>&);	// disable op=
		T* myPtr;
};

// Name: DeleteMe
// Documentation: template class for deleting new'd objects
// This is mainly for cleaning up control structures
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
