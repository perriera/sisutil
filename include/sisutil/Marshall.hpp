// Marshall.hpp  - generic data value marshaling
// $Id: Marshall.hpp,v 1.11 1998/12/17 20:38:34 ed Exp $

#ifndef _MARSHALL_H_
#define _MARSHALL_H_ 1

#include <sisutil/YUFlatArray.h>
#include <ostream>

class Marshall {
public:
	Marshall(const int DefaultSize);
	Marshall(YUFlatArray* yfa);
	virtual ~Marshall();

	Marshall& clear();
	Marshall& reset();
	inline int       isValid() const;
	inline Marshall& inValidate();
	inline Marshall& width(const int);
	Marshall& operator << (const char* cs);
	Marshall& operator << (const char c);

	Marshall& operator >> (char* cs);
	Marshall& operator >> (char& c);
	static int       isTraceDebug();
	static int       isTraceRaw();
	friend std::ostream& operator << (std::ostream&, const Marshall&);

protected:
	Marshall();                             // disable default ctor
	Marshall(const Marshall&);              // disable copy ctor
	Marshall& operator = (const Marshall&); // disable op=
	int error;
	int incurrPos;
	int nextWidth;
	int outcurrPos;
	int donotdelete;
	YUFlatArray* myArray;                   // shared
	int maxsize;

private:
	static const char* Debug_VAR;
	static const char* Raw_VAR;
};


class Setmw {
	friend Marshall& operator << (Marshall& m, const Setmw& s);
	friend Marshall& operator >> (Marshall& m, const Setmw& s);

public:
	Setmw(int w) : width(w) {};

private:
	int width;
};

inline int Marshall::isValid() const { return !error; }

inline Marshall& Marshall::inValidate() {
	error = 1;
	return *this;
}

inline Marshall& Marshall::width(const int i) {
	nextWidth = i;
	return *this;
}

class YUStreamable {
public:
	YUStreamable() {}
	virtual ~YUStreamable() {}
	virtual long getLenRPCMarshall() const = 0;
};

// Name: Marshall
// Documentation: generic data value marshalling
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
