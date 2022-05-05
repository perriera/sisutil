// delimfs.hpp  - delimited streams for marshalling
// class delim_ifstream, class delim_ofstream
// $Id: delimfs.hpp,v 1.7 1998/11/18 02:25:27 ed Exp $

#ifndef DELIMFS_HXX
#define DELIMFS_HXX 1

#include <fstream>           // for setbuf
#include <string>

class delim_ifstream : public std::ifstream {
public:
  delim_ifstream(const std::string& aFileName, const char theDelim);
  virtual ~delim_ifstream();

  friend delim_ifstream& operator >> (delim_ifstream& rhs, char* outBuffer);

protected:
  inline char delim() const { return myDelim; }
  inline const std::string& filename() const { return myFilename; }
  char* myFileBuff;

private:
  delim_ifstream();									               // disable default ctor
  delim_ifstream(const delim_ifstream&);                   // disable copy ctor
  delim_ifstream& operator = (const delim_ifstream&);      // disable op=
  const char myDelim;
  std::string myFilename;
  enum LENGTHENUM { BUFFER_LENGTH = 5120 } LENGTH;
};

class delim_ofstream : public std::ofstream {
public:
  delim_ofstream(const std::string& theFileName, const char theDelim);
  ~delim_ofstream();

  friend delim_ofstream& operator << (delim_ofstream& rhs, const char* inBuffer);
  // inBuffer passed to operator << () min length
  enum LENGTHENUM { MIN_BUFFER_LENGTH = 1024 } LENGTH;

protected:
  inline char delim() const { return myDelim; }
  inline const std::string& filename() const { return myFilename; }

private:
  delim_ofstream();                                     // disable default c'tor
  delim_ofstream(const delim_ofstream&);                // disable copy c'tor
  delim_ofstream& operator = (const delim_ofstream&);   // disable op=
  const char myDelim;
  const std::string myFilename;
};

// Name: class delim_ifstream, class delim_ofstream
// Documentation: delimited stream for marshalling (adds 
//  record delimiter to fstream)
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
// tab-width: 3
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
