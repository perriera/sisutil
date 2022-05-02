// YUString.hpp  - Marshalling ASCII string 
// $Id: YUString.hpp,v 1.10 2000/01/29 16:44:12 ed Exp $

#ifndef _YUSTRING_H
#define _YUSTRING_H 1

#include <sisutil/Marshall.hpp>
#include <iostream>
#include <string>

class delim_ofstream;
class delim_ifstream;

class YUString : public YUStreamable {
public:
	YUString(int length);
	YUString(const YUString&);
	YUString(const char* cs);
	YUString(const std::string& s);
	virtual ~YUString();

	int length() const;

	YUString& operator = (const char*);
	YUString& operator = (const YUString&);
	YUString& operator = (const std::string&);

	int       operator == (const char*) const;
	int       operator == (const std::string&) const;
	int       operator == (const YUString&) const;

	operator const std::string() const;
	virtual long getLenRPCMarshall() const { return((long)myLength + 1L); }

	friend  int operator == (const char*, const YUString&);

	friend  Marshall& operator << (Marshall& m, const YUString& fc);
	friend  Marshall& operator >> (Marshall& m, YUString& fc);
	friend  delim_ofstream& operator << (delim_ofstream& rS, const YUString& fc);
	friend  delim_ifstream& operator >> (delim_ifstream& rS, YUString& fc);
	friend  std::ostream& operator << (std::ostream& m, const YUString& fc);

private:
	YUString();						// disable default ctor

protected:
	int   myLength;
	char* myData;
};

// Name: YUString
// Documentation: ASCII string
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
