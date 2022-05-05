// YUBool.hpp  - self marshalling boolean
// $Id: YUBool.hpp,v 1.5 2000/01/29 16:44:10 ed Exp $

#ifndef _YUBOOL_H
#define _YUBOOL_H 1

#ifndef _YU_YUFCHAR_H_INCLUDED
#include <sisutil/YUFChar.hpp>
#include <string>
#define _YU_YUFCHAR_H_INCLUDED
#endif

class YUBool : public YUFixedChar {
public:
	inline YUBool() : YUFixedChar(1) {};
	YUBool(const YUBool& rSrc);
	YUBool& operator=(const YUBool& rSrc);
	inline virtual ~YUBool() {}
	YUBool& operator = (const int b);
	YUBool& operator = (const char c);
	void Set(int b);
	inline int isSet() const { return YUFixedChar::operator == ("Y"); }
};

// Name: YUBool
// Documentation: self marshalling boolean
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
