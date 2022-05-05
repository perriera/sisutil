// YURaw.C - self marshalling raw byte string
// $Id: YURaw.cpp,v 1.4 1998/02/04 21:27:49 ed Exp $

#include <sisutil/YURaw.hpp>
#include <sisutil/delimfs.hpp>
#include <ctype.h>				  // for isprint
#include <cstring>              // for memset

YURaw::YURaw(int length) : myLength(length) {
  myData = new char[length];
  memset(myData, 0, length);
}

YURaw::YURaw(const YURaw& fc) : myLength(fc.length()) {
  myData = new char[fc.myLength];
  memcpy(myData, fc.myData, myLength);
}

int YURaw::length() const { return myLength; }

YURaw::~YURaw() { delete[] myData; }

YURaw& YURaw::operator = (const char* c) {
  memcpy(myData, c, myLength);
  return *this;
}

YURaw& YURaw::operator = (const YURaw& rhs) {
  if (this != &rhs) memcpy(myData, rhs.myData, myLength);
  return *this;
}

int YURaw::operator == (const char* c) const {
  return (!memcmp(myData, c, myLength));
}

int YURaw::operator == (const YURaw& fc) const {
  return (!memcmp(myData, fc.myData, myLength) && (myLength == fc.myLength));
}

int YURaw::operator != (const char* c) const {
  return (!YURaw::operator == (c));
}

int YURaw::operator != (const YURaw& fc) const {
  return (!YURaw::operator == (fc));
}

YURaw::operator const char* () const {
  return myData;
}

int operator == (const char* c, const YURaw& fc) {
  return (fc == c);
}

Marshall& operator << (Marshall& m, const YURaw& fc) {
  int i;
  for (i = 0; i < fc.myLength; i++)
    m << fc.myData[i];
  return m;
}

Marshall& operator >> (Marshall& m, YURaw& fc) {
  int i;
  for (i = 0; i < fc.myLength; i++)
    m >> fc.myData[i];
  return m;
}

// be sure this does not get used for marshalling
std::ostream& operator << (std::ostream& m, const YURaw& fc) {
  int i;
  for (i = 0; i < fc.myLength; i++) {
    const char& thisChar = fc.myData[i];
    m << (isprint(thisChar) ? thisChar : '.');
  }
  return m;
}

delim_ofstream& operator << (delim_ofstream& rS, const YURaw& fc) {
  rS << fc.myData;
  return rS;
}

delim_ifstream& operator >> (delim_ifstream& rS, YURaw& fc) {
  rS >> fc.myData;
  return rS;
}
