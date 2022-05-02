// YUString.C - ASCII String
// $Id: YUString.cpp,v 1.8 1999/04/27 16:17:07 ed Exp $

#include <sisutil/YUString.hpp>
#include <sisutil/delimfs.hpp>
#include <string>
#include <cstring>      // for memset

YUString::YUString(int length) : myLength(length) {
  myData = new char[length + 1];
  memset(myData, 0x00, length + 1);
}

YUString::YUString(const YUString& ys) : myLength(ys.myLength) {
  myData = new char[ys.myLength + 1];
  memset(myData, 0x00, myLength + 1);
  strcpy(myData, ys.myData);
}

YUString::YUString(const char* cs) : myLength(strlen(cs)) {
  myData = new char[myLength + 1];
  memset(myData, 0x00, myLength + 1);
  strcpy(myData, cs);
}

YUString::YUString(const std::string& s) : myLength(s.length()) {
  myData = new char[myLength + 1];
  memset(myData, 0x00, myLength + 1);
  strcpy(myData, s.c_str());
}

YUString::~YUString() { delete[] myData; }

int YUString::length() const { return myLength; }

YUString& YUString::operator = (const char* c) {
  strncpy(myData, c, myLength);
  myData[myLength] = 0x00;
  return *this;
}

YUString& YUString::operator = (const YUString& rhs) {
  if (this != &rhs) {
    strncpy(myData, rhs.myData, myLength);
    myData[myLength] = 0x00;
  }
  return *this;
}
YUString& YUString::operator = (const std::string& s) {
  strncpy(myData, s.c_str(), myLength);
  myData[myLength] = 0x00;
  return *this;
}

int YUString::operator == (const char* c) const {
  return (!strcmp(myData, c));
}

int YUString::operator == (const std::string& s) const {
  return s == myData;
}

int YUString::operator == (const YUString& fc) const {
  return (!strcmp(myData, fc.myData));
}

YUString::operator const std::string() const {
  return std::string(myData);
}

int operator==(const char* c, const YUString& fc) {
  return (fc == c);
}

Marshall& operator << (Marshall& m, const YUString& fc) {
  m << Setmw(fc.myLength) << fc.myData;
  return m;
}

Marshall& operator >> (Marshall& m, YUString& fc) {
  m >> Setmw(fc.myLength) >> fc.myData;
  return m;
}

std::ostream& operator << (std::ostream& m, const YUString& fc) {
  m << fc.myData;
  return m;
}

delim_ofstream& operator << (delim_ofstream& rS, const YUString& fc) {
  rS << fc.myData;
  return rS;
}

delim_ifstream& operator >> (delim_ifstream& rS, YUString& fc) {
  rS >> fc.myData;
  return rS;
}
