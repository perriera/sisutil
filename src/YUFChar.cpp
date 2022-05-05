// YUFChar.cpp - class YUFixedChar - self marshalling fixed length char string
// $Id: YUFChar.cpp,v 1.8 1998/11/18 02:13:52 ed Exp $

#include <sisutil/YUFChar.hpp>
#include <sisutil/delimfs.hpp>
#include <string>

YUFixedChar::YUFixedChar(int length) : myLength(length)
{
  myData = new char[length + 1];
  memset(myData, 0x00, length + 1);
  myData[0] = 0x0;
}

YUFixedChar::YUFixedChar(const YUFixedChar& fc) : myLength(fc.myLength)
{
  myData = new char[fc.myLength + 1];
  memset(myData, 0x00, myLength + 1);
  strcpy(myData, fc.myData);
}
YUFixedChar::YUFixedChar(const char* cs) : myLength(strlen(cs))
{
  myData = new char[myLength + 1];
  memset(myData, 0x00, myLength + 1);
  strcpy(myData, cs);
}

YUFixedChar::YUFixedChar(const std::string& s) : myLength(s.length())
{
  myData = new char[myLength + 1];
  memset(myData, 0x00, myLength + 1);
  strcpy(myData, s.c_str());
}

int YUFixedChar::length() const { return myLength; }

YUFixedChar::~YUFixedChar()
{
  delete[] myData;
}

YUFixedChar& YUFixedChar::operator = (const char* c)
{
  strncpy(myData, c, myLength);
  myData[myLength] = 0x00;
  return *this;
}

YUFixedChar& YUFixedChar::operator = (const YUFixedChar& rhs)
{
  if (this != &rhs) {
    strncpy(myData, rhs.myData, myLength);
    myData[myLength] = 0x00;
  }
  return *this;
}

YUFixedChar& YUFixedChar::operator = (const std::string& s)
{
  strncpy(myData, s.c_str(), myLength);
  myData[myLength] = 0x00;
  return *this;
}

int YUFixedChar::operator == (const char* c) const
{
  return (!strcmp(myData, c));
}

int YUFixedChar::operator == (const YUFixedChar& fc) const
{
  return (!strcmp(myData, fc.myData) && (myLength == fc.myLength));
}

YUFixedChar::operator const char* () const
{
  return myData;
}


int operator == (const char* c, const YUFixedChar& fc)
{
  return (fc == c);
}

Marshall& operator << (Marshall& m, const YUFixedChar& fc)
{
  m << Setmw(fc.myLength) << fc.myData;
  return m;
}

Marshall& operator >> (Marshall& m, YUFixedChar& fc)
{
  m >> Setmw(fc.myLength) >> fc.myData;
  return m;
}

std::ostream& operator << (std::ostream& m, const YUFixedChar& fc)
{
  m << fc.myData;
  return m;
}

delim_ofstream& operator << (delim_ofstream& rS, const YUFixedChar& fc)
{
  rS << fc.myData;
  return rS;
}

delim_ifstream& operator >> (delim_ifstream& rS, YUFixedChar& fc)
{
  rS >> fc.myData;
  return rS;
}


// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
