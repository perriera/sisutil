// YUFChar.hpp - class YUFixedChar - self marshalling fixed length char string
// $Id: YUFChar.hpp,v 1.8 2000/01/29 16:44:11 ed Exp $

#ifndef YUFCHAR_HXX
#define YUFCHAR_HXX 1

#include <sisutil/Marshall.hpp>
#include <iostream>
#include <string>
#include <cstring>

class delim_ofstream;
class delim_ifstream;

class YUFixedChar : public YUStreamable {
public:
	YUFixedChar(int length);
	YUFixedChar(const YUFixedChar&);
	YUFixedChar(const char* cs);
	YUFixedChar(const std::string& s);
	virtual ~YUFixedChar();
	int length() const;

	YUFixedChar& operator = (const char*);
	YUFixedChar& operator = (const YUFixedChar&);
	YUFixedChar& operator = (const std::string&);

	int operator == (const char*) const;
	int operator == (const YUFixedChar&) const;

	operator const char* () const;

	friend  int operator == (const char*, const YUFixedChar&);

	friend  Marshall& operator << (Marshall& m, const YUFixedChar& fc);
	friend  Marshall& operator >> (Marshall& m, YUFixedChar& fc);

	friend  delim_ofstream& operator << (delim_ofstream& rS, const YUFixedChar& fc);
	friend  delim_ifstream& operator >> (delim_ifstream& rS, YUFixedChar& fc);

	friend  std::ostream& operator << (std::ostream& m, const YUFixedChar& fc);
	virtual long getLenRPCMarshall() const { return(myLength + 1); }

private:
	YUFixedChar();							// disable default ctor
protected:
	int   myLength;
	char* myData;
};

// Name: Class
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

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
