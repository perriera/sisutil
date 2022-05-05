// YURaw.hpp  - self marshalling raw byte string
// $Id: YURaw.hpp,v 1.8 2000/01/29 16:44:12 ed Exp $
#ifndef _YURAW_H_
#define _YURAW_H_ 1

#include <sisutil/Marshall.hpp>
#include <iostream>

class delim_ofstream;
class delim_ifstream;

class YURaw : public YUStreamable
{
public:
  YURaw(int length);
  YURaw(const YURaw&);
  virtual ~YURaw();
  int length() const;

  YURaw& operator = (const char*);
  YURaw& operator = (const YURaw&);

  int operator == (const char*) const;
  int operator == (const YURaw&) const;
  int operator != (const char*) const;
  int operator != (const YURaw&) const;

  operator const char* () const;
  virtual long getLenRPCMarshall() const { return((long)myLength); }

  friend  int operator == (const char*, const YURaw&);

  friend  Marshall& operator << (Marshall& m, const YURaw& fc);
  friend  Marshall& operator >> (Marshall& m, YURaw& fc);

  friend  delim_ofstream& operator << (delim_ofstream& rS, const YURaw& fc);
  friend  delim_ifstream& operator >> (delim_ifstream& rS, YURaw& fc);

  friend std::ostream& operator << (std::ostream& m, const YURaw& fc);

private:
  YURaw();				// disable default ctor

protected:
  int myLength;
  char* myData;
};

// Name: YURaw
// Documentation: self marshalling raw byte string
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
