// toString.hpp - string conversions
// This file provides many of the StarView String functions that are not also provided by the STL string class.
// Some of these methods will be superseded by C++ 11
// $Id: toString.hpp

// TODO: rename this file to StringUtil.hpp - ditto for the cpp file

/* Notes:
 * Starview Cut() may be replaced by the STL erase() method with the same arguments
 * Starview Replace() may be replaced by the STL replace() method with the arguments switched from (String, nPos) to (nPos, length, string)
*/

#ifndef _TOSTRING_H
#define _TOSTRING_H
#include <string>
#include <sstream>
#include <sisutil/YUDate.hpp>

namespace sv {
   // works for all types 'T' for which an inserter exists:
   template <class T>
   std::string toString(T const& t);

   // works for all types 'T' for which an inserter exists:
   template <class T>
   std::string toString(T const& t) {
      std::ostringstream ostr;
      ostr << t; // << std::ends;
      //const char* retVal = ostr.str();
      //ostr.rdbuf()->freeze(0);
      //return retVal;
      return ostr.str();
   }

   // Defined in cpp file where it is a simple function
   extern "C" std::string toString(long l);
   extern "C" std::string toString2(long l);

   // Supports a base type (long) without an iterator.
   // Looks like a class template but is really just a normal function but we get to place it here.
   /*
   template <class T>
   std::string toString(long l) {
      std::ostringstream ostr;
      ostr << l << std::ends;
      return ostr.str();
   }
   */
   extern "C" std::string toUpper(std::string & str);

   extern "C" std::string toLower(std::string & str);

   extern "C" std::string EraseAllChars(std::string & str, const char ch = ' ');

   extern "C" std::string EraseTrailingChars(std::string str, const char ch = ' ');

   extern "C" std::string EraseLeadingChars(std::string & str, const char ch = ' ');

   extern "C" std::string Trim(std::string & str, const char ch = ' ');

   extern "C" std::string Copy(const std::string & str, const unsigned int nPos = 0, const unsigned long nCount = std::string::npos);

   extern "C" bool IsNumeric(std::string & str);

   extern "C" std::string GetNum(long num);

   extern "C" std::string GetDate(const YUDate & date);

   extern "C" std::string GetLongDate(const YUDate & date);

   extern "C" std::string GetTime(const YUTime & time);

   extern "C" std::string Expand(std::string & str, const int nCount, const char ch = ' ');
}

#endif
