// Marshall.cpp - generic data value marshalling
// $Id: Marshall.cpp,v 1.15 2000/02/01 00:37:55 ed Exp $

#include <sisutil/Marshall.hpp>
#include <sisutil/YUFlatArray.h>
#include <sisutil/lstring.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <sisutil/ErrLog.hpp>
#include <sisutil/YUApp.hpp>

// "setenv MARSH_DEBUG" to turn on debug output
const char* Marshall::Debug_VAR = "MARSH_DEBUG";

// "setenv RAW" to turn on raw debug output
const char* Marshall::Raw_VAR = "RAW";

Marshall::Marshall(const int defaultSize)
   : error(0), incurrPos(0), nextWidth(0), outcurrPos(0),
   donotdelete(0), maxsize(defaultSize) {
   int size = sizeof(YUFlatArray) + defaultSize;
   myArray = (YUFlatArray*)new char[size];
   memset(myArray->data, 0x00, defaultSize);
   myArray->size = 0;
}

Marshall::Marshall(YUFlatArray* yfa)
   : error(0), incurrPos(0), nextWidth(0), outcurrPos(0), donotdelete(1) {
   //myArray(yfa), maxsize(yfa->size) {
   if (yfa == NULL) {
      maxsize = 0;
      donotdelete = 1;
      std::cerr << "Marshall c'tor: yfa is null!" << std::endl;
      // wish we could throw an exception here
   }
   else {
      myArray = yfa;
      maxsize = yfa->size;
   }
}

Marshall::~Marshall() {
   if (!donotdelete)
      delete[] myArray;
}

Marshall& Marshall::clear() {
   incurrPos = outcurrPos = 0;
   myArray->size = 0;
   memset(myArray->data, 0x00, maxsize);
   myArray->size = 0;
   return *this;
}

Marshall& Marshall::operator << (const char* cs) {
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "Marshall::operator << (const char* cs) " << this << ": called with: " << cs << std::endl;
   char* myPos = (char*)&myArray->data[incurrPos];
   if (isTraceDebug()) {
      //strstream tmpStream;
      std::stringstream tmpStream;
      tmpStream << "M:\"";
      for (int i = 0; i < nextWidth && cs[i]; i++) {
         tmpStream << cs[i];
      }
      tmpStream << "\""; // << std::ends;
      getTheErrLog().putStream("Marshall", "op<<", tmpStream);
      if (isTraceRaw()) {
         //strstream tmpRawStream;
         std::stringstream tmpRawStream;
         tmpRawStream << std::setw(4) << nextWidth << " Raw bytes "
            << std::hex << (const void*)cs << "->"
            << (void*)myPos << ": \"";
         tmpRawStream.fill('0');
         for (int i = 0; i < nextWidth; i++) { // print each byte as hex
            const unsigned char charValue = cs[i];
            tmpRawStream << std::setw(2) << std::setfill('0') << (unsigned)charValue;
         }
         tmpRawStream << "\""; // << std::ends;
         getTheErrLog().putStream("Marshall", "op<<", tmpRawStream);
      }
   }
   if (nextWidth) {
      strncpy(myPos, cs, nextWidth);
      myArray->data[incurrPos + nextWidth] = 0x00;  // null terminate
      incurrPos += (nextWidth + 1);
      nextWidth = 0;
   }
   else {
      strcpy(myPos, cs);
      int len = strlen(cs);
      incurrPos += (len + 1);
   }
   myArray->size = incurrPos;
   return *this;
}

Marshall& Marshall::reset() {
   incurrPos = outcurrPos = 0;
   return *this;
}

Marshall& Marshall::operator << (const char c) {
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "Marshall::<< (const char c): called with " << this << ": " << c << std::endl;
   myArray->data[incurrPos] = c;
   incurrPos++;
   myArray->size = incurrPos;
   return *this;
}

Marshall& Marshall::operator >> (char* cs) {
   if (getenv("SIS_OCI_DEBUG")) std::cerr << "Marshall::>> (char* cs): called with " << this << ": " << cs << std::endl;
   const char* myPos = (const char*)&myArray->data[outcurrPos];
   if (isTraceDebug()) {
      //strstream tmpStream;
      std::stringstream tmpStream;
      tmpStream << "U:\"" << myPos << "\""; // << std::ends;
      getTheErrLog().putStream("Marshall", "op>>", tmpStream);
      if (isTraceRaw()) {
         //strstream tmpRawStream;
         std::stringstream tmpRawStream;
         tmpRawStream << std::setw(4) << nextWidth << " Raw bytes "
            << std::hex << (const void*)myPos
            << "->" << (void*)cs << ": \"";
         tmpRawStream.fill('0');
         for (int i = 0; i < nextWidth; i++) { // print each byte as hex
            const unsigned char charValue = myPos[i];
            tmpRawStream << std::setw(2) << (unsigned)charValue;
         }
         tmpRawStream << "\""; // << std::ends;
         getTheErrLog().putStream("Marshall", "op>>", tmpRawStream);
      }
   }
   if (getenv("SIS_OCI_DEBUG")) {
      for (int j = 0;j < nextWidth;j++) {
         printf("%02X", myPos[j]);
      }
      printf("---- unmarshall got something --- \n");
   }
   if (nextWidth) {
      strncpy(cs, myPos, nextWidth);
      cs[nextWidth] = 0x00;
      outcurrPos += (nextWidth + 1);
      nextWidth = 0;
   }
   else {
      strcpy(cs, myPos);
      int len = strlen(myPos);
      outcurrPos += (len + 1);
   }
   return *this;
}

Marshall& Marshall::operator >> (char& c) {
   c = myArray->data[outcurrPos];
   outcurrPos++;
   return *this;
}

Marshall& operator << (Marshall& m, const Setmw& s) {
   m.width(s.width);
   return m;
}

Marshall& operator >> (Marshall& m, const Setmw& s) {
   m.width(s.width);
   return m;
}

int Marshall::isTraceDebug() {
   return (YUApp::getTraceLevel() > YUApp::MAXLEVEL - 2) || getenv(Debug_VAR);
}

int Marshall::isTraceRaw() {
   const char* debugValue = getenv(Debug_VAR);
   return (YUApp::getTraceLevel() > YUApp::MAXLEVEL - 1)
      || (debugValue && strcmp(debugValue, Raw_VAR) == 0);
}

/** A diagnostic used to dump the contents of the YUFlatArray in the Marshall class
 */
std::ostream& operator << (std::ostream& os, const Marshall& m) {
   //std::cerr << "YUFlatArray data: size = " << yf->size << std::endl;
   struct YUFlatArray* yf = m.myArray;
   char* data = yf->data;
   for (int k = 0; k < yf->size; k++) {
      char ch = data[k];
      //std::cerr << (isprint(ch) ? ch : '.');
      std::cerr << std::setw(2) << std::hex << ch;
   }
   std::cerr << std::endl << std::endl;
   return os;
}
