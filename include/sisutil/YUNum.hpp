// YUNum.hpp  - self marshalling number
// $Id: YUNum.hpp,v 1.13 2000/01/29 16:44:11 ed Exp $

#ifndef _YUNUMBER_H_
#define _YUNUMBER_H_ 1

#include <iostream>
#include <string>
#include <sisutil/Marshall.hpp>

class delim_ofstream;
class delim_ifstream;

class YUNumProps {
public:
	inline YUNumProps(int precision = 9, int scale = 0) :
		myPrecision(precision - scale), myScale(scale) {}
	inline int getPrecision() const { return myPrecision; }
	inline int getScale() const { return myScale; }

protected:
	int myPrecision;
	int myScale;
};

class YUNumber : public YUStreamable {
public:
	inline YUNumber(YUNumProps props)
		: myProps(props), myLHS(0), myRHS(0), nullflag(1), overflowflag(0) {}
	YUNumber(long);
	YUNumber(double);
	YUNumber(const YUNumber&);
	inline virtual ~YUNumber() {}

	const YUNumProps getProps() const { return myProps; }

	YUNumber& nullify();
	int isNull() const;
	int isOverflow() const;
	inline long LHS() const { return myLHS; }
	inline int  RHS() const { return myRHS; }

	YUNumber& operator = (int i) { return operator = ((long)i); }
	YUNumber& operator = (long);
	YUNumber& operator = (double);
	YUNumber& operator = (const YUNumber&);

	int operator == (int i) const { return operator == ((long)i); }
	int operator == (long) const;
	int operator == (double) const;
	int operator == (const YUNumber&) const;
	int operator != (const YUNumber&) const;

	operator int() const { return operator long(); }
	operator long() const;
	operator double() const;

	friend  int operator == (long, const YUNumber&);
	friend  int operator == (double, const YUNumber&);

	friend Marshall& operator << (Marshall& m, const YUNumber& sn);

	virtual long getLenRPCMarshall() const;

	virtual Marshall& Marsh(Marshall& m) const;

	virtual Marshall& UnMarsh(Marshall& m);
	friend  Marshall& operator >> (Marshall& m, YUNumber& sn);

	virtual std::ostream& DisplayOn(std::ostream& m) const;
	friend  std::ostream& operator << (std::ostream& m, const YUNumber& sn);

	friend delim_ofstream& operator << (delim_ofstream& rS, const YUNumber& rO);
	friend delim_ifstream& operator >> (delim_ifstream& rS, YUNumber& rO);

private:
	YUNumber(); 											// disable default ctor

protected:
	YUNumProps myProps;
	long       myLHS;
	long       myRHS;
	int        nullflag;
	int        overflowflag;
};

// Name: YUNumProps, YUNumber
// Documentation: streamable number class
// Visibility: exported
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
// Persistence: static | dynamic
#endif
