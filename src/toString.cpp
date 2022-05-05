/*
 * toString.cpp
 *
 *
 * Created on: Nov 11, 2016
 * Author: ojones
 */

 //#include "gtest/gtest.h"
#include <sisutil/toString.hpp>
#include <string>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <locale>


/**
 * An awkward way to do an int to string conversion - C++11 has std::to_string which is potentially better.
 * Note: this method appends a null terminator to the string (C style) so it may not be suitable for C++
 *       concatenation operations (the embedded terminator remains embedded!)
 * @param a long int (or an autoboxed short int)
 * @return a string representation of the input integer
 */
extern "C"
std::string sv::toString(long l) {
   //std::cout << "toString() entered with l = |" << l << "|" << std::endl;
   std::ostringstream ostr;
   ostr << l; // << std::ends;
   //std::cout << "toString() leaving with |" << ostr.str() << "|" << std::endl;
   return ostr.str();
}

/**
 * The same as sv::toString() but does NOT apply a null terminator to the end.
 */
extern "C"
std::string sv::toString2(long l) {
   //std::cout << "toString() entered with l = |" << l << "|" << std::endl;
   std::ostringstream ostr;
   ostr << l;
   //std::cout << "toString() leaving with |" << ostr.str() << "|" << std::endl;
   return ostr.str();
}

/**
 * A function to set all characters in a string to upper case
 * ASCII characters are assumed (normal with STL string class).
 * @param str the string to be upper cased
 * @return the input string, now modified to upper case
 */
extern "C"
std::string sv::toUpper(std::string & str) {
   //std::cout << "toUpper() entered with str = |" << str << "|" << std::endl;
   // forward iterator
   for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
      char ch = *it;
      //if (ch > 96 && ch < 123) { // lower case character?
      if (ch > '`' && ch < '{') { // lower case character?
         ch &= 0x5F;    // clear b6
         *it = ch;      // put it back
      }
   }
   //std::cout << "toUpper() returning |" << str << "|" << std::endl;
   return str;
}

/**
 * A function to set all characters in a string to lower case
 * ASCII characters are assumed (normal with STL string class).
 * @param str the string to be upper cased
 * @return the input string, now modified to lower case
 */
extern "C"
std::string sv::toLower(std::string & str) {
   //std::cout << "toLower() entered with str = |" << str << "|" << std::endl;
   // forward iterator
   for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
      char ch = *it;
      //if (ch < 91 && ch > 64) { // upper case character?
      if (ch < '[' && ch > '@') { // upper case character?
         ch |= 0x60;    // set b6
         *it = ch;      // put it back
      }
   }
   //std::cout << "toLower() returning |" << str << "|" << std::endl;
   return str;
}

/**
 * A function to erase all specified characters in a string.
 * ASCII characters are assumed (normal with STL string class).
 * @param str the string to be processed
 * @param ch the (optional) character to be erased (defaults to space)
 * @return the input string, now modified to lower case
 */
extern "C"
std::string sv::EraseAllChars(std::string & str, char ch) {
   //std::cout << "EraseAllChars() entered with str = |" << str << "| and ch = |" << ch << "|" << std::endl;
   // forward iterator
   for (std::string::iterator it = str.begin(); it != str.end(); it++) {
      char c = *it;
      if (c == ch) str.erase(it);
   }
   //std::cout << "EraseAllChars() returning |" << str << "|" << std::endl;
   return str;
}

/**
 * A simple right trim method
 * @param str the string to be trimmed
 * @param ch the (optional) character to be removed - defaults to space
 * @return the input string, now right trimmed
 */
extern "C"
std::string sv::EraseTrailingChars(std::string str, char ch) {
   //std::cout << "EraseTrailingChars() entered with str = |" << str << "| and ch = |" << ch << "|" << std::endl;
   while (str.length() > 0 && str[str.length() - 1] == ch) {
      str.erase(str.length() - 1, 1);
   }
   //std::cout << "EraseTrailingChars() returning |" << str << "|" << std::endl;
   return str;
}

/**
 * A simple left trim method
 * @param str the string to be trimmed
 * @param ch the (optional) character to be removed - defaults to space
 * @return the input string, now left trimmed
 */
extern "C"
std::string sv::EraseLeadingChars(std::string & str, char ch) {
   //std::cout << "EraseLeadingChars() entered with str = |" << str << "| and ch = |" << ch << "|" << std::endl;
   str.erase(0, str.find_first_not_of(ch));
   //std::cout << "EraseLeadingChars() returning |" << str << "|" << std::endl;
   return str;
}

/**
 * A simple left trim method
 * @param str the string to be trimmed
 * @param ch the (optional) character to be removed - defaults to space
 * @return the input string, now left trimmed
 */
extern "C"
std::string sv::Trim(std::string & str, char ch) {
   //std::cout << "Trim() entered with str = |" << str << "| and ch = |" << ch << "|" << std::endl;
   str = EraseLeadingChars(str, ch);
   str = EraseTrailingChars(str, ch);
   //std::cout << "Trim() returning |" << str << "|" << std::endl;
   return str;
}

extern "C"
std::string sv::Copy(const std::string & str, unsigned int nPos, unsigned long nCount) {
   //std::cout << "EraseLeadingChars() entered with str = |" << str << "| and nPos = |" << nPos << "| and nCount = |" << nCount << "|" << std::endl;
   std::string copy = std::string(str, nPos, nCount); // copy substring c'tor
   //std::cout << "EraseLeadingChars() returning |" << copy << "|" << std::endl;
   return copy;
}

/**
 * A function to examine all characters in a string and determine if they are all numeric.
 * ASCII characters are assumed (normal with STL string class).
 * Note: C++ 2011 has some built in regex methods but we're only on C++98
 * TODO: make param str const (but the iterator potentially changes the contents so need to redo that)
 * @param str the string to be processed
 * @return true if all characters are numeric, false otherwise
 */
extern "C"
bool sv::IsNumeric(std::string & str) {
   //std::cout << "IsNumeric() entered with str = |" << str << "|" << std::endl;
   if (str.length() == 0) return false; // empty string has no digits so it is not numeric
   bool flag = true;  // default to all numeric
   // forward iterator
   for (std::string::iterator it = str.begin(); it != str.end(); it++) {
      char c = *it;
      if (!isdigit(c)) {
         flag = false;
         break; // early exit - a single non-numeric character is enough to fail
      }
   }
   //std::cout << "IsNumeric() returning |" << flag << "|" << std::endl;
   return flag;
}

/**
 * A function to convert a number to a string formatted for the local numeric locale.
 * relies on "C" locale implementation to get appropriate separators.
 * TODO: C++ locales do not seem to be supported in C++98 - implement if we go to C++2011.
 * @see toString for a non-localized version
 * @param num the number to be converted
 * @return a formatted string with appropriate separator
 */
extern "C"
std::string sv::GetNum(long num) {
   //std::cout << "GetNum() entered with num = |" << num << "|" << std::endl;

   char* loc = setlocale(LC_NUMERIC, NULL); // save the value for restoration
   setlocale(LC_NUMERIC, "");                // force locale to be set from environment variable
   //loc = (loc == NULL) ? (char *)"NULL" : loc;
   //std::cout << "Locale: " << loc << std::endl;

   char buf[20] = { '\0' };
   snprintf(buf, 20, "%'ld", num);          // relies on C language locale implementation to get appropriate separators
   std::string str = buf;

   setlocale(LC_NUMERIC, loc);              // reset locale to the original value

   //std::cout << "GetNum() leaving with |" << str << "|" << std::endl;
   return str;
}

/* A Convenient reference for the subsequent date and time functions
struct tm {
    int tm_sec;         // seconds
    int tm_min;         // minutes
    int tm_hour;        // hours
    int tm_mday;        // day of the month
    int tm_mon;         // month
    int tm_year;        // year
    int tm_wday;        // day of the week
    int tm_yday;        // day in the year
    int tm_isdst;       // daylight saving time
};
*/

/**
 * A function to convert a YUDate to a string formatted for the local numeric locale.
 * Relies on "C" locale implementation to get appropriate separators.
 * TODO: C++ locales do not seem to be supported in C++98 - implement if we go to C++2011.
 * @param date the date to be converted
 * @return a formatted string of the form dd-mmm-yy, i.e. 30-Aug-18
 */
extern "C"
std::string sv::GetDate(const YUDate & date) {
   //std::cout << "GetDate() entered with date = |" << date << "|" << std::endl;
   if (date.isNULL()) return "";
   std::string str;

   char* loc = setlocale(LC_TIME, NULL); // save the value for restoration
   setlocale(LC_TIME, "");                // force locale to be set from environment variable

   struct tm myDate;
   char buf[20];
   memset(&myDate, 0, sizeof(struct tm));
   int year = date.getYear();
   if (year >= 1900) year -= 1900;        // y2k: is 1900 the best assumption?
   myDate.tm_year = year;                // ... it is for unix
   myDate.tm_mon = date.getMonth() - 1; // 0-11 for Unix, 1-12 for SIS
   myDate.tm_mday = date.getDay();
   myDate.tm_isdst = -1;                  // don't know if DST was in effect or not
   //std::cout << "GetDate() tm set to |" << myDate.tm_year << "|" << myDate.tm_mon << "|" << myDate.tm_mday << "|" << std::endl;
   strftime(buf, 20, "%e-%b-%y", &myDate);
   str = buf;

   setlocale(LC_TIME, loc);               // reset locale
   //std::cout << "GetDate() leaving with |" << str << "|" << std::endl;
   return str;
}

/**
 * A function to convert a YUDate to a string formatted for the local numeric locale.
 * Relies on "C" locale implementation to get appropriate separators.
 * TODO: C++ locales do not seem to be supported in C++98 - implement if we go to C++2011.
 * Currently same output as GetDate(). TODO: what is the difference as used in STAC?
 * @param date the date to be converted
 * @return a formatted string of the form dd-mmm-yy, i.e. 30-Aug-18
 */
extern "C"
std::string sv::GetLongDate(const YUDate & date) {
   //std::cout << "GetLongDate() entered with date = |" << date << "|" << std::endl;
   if (date.isNULL()) return "";
   std::string str;

   char* loc = setlocale(LC_TIME, NULL); // save the value for restoration
   setlocale(LC_TIME, "");                // force locale to be set from environment variable

   struct tm myDate;
   char buf[20];
   memset(&myDate, 0, sizeof(struct tm));
   int year = date.getYear();
   if (year >= 1900) year -= 1900;        // y2k: is 1900 the best assumption?
   myDate.tm_year = year;
   myDate.tm_mon = date.getMonth() - 1;
   myDate.tm_mday = date.getDay();
   myDate.tm_isdst = -1;                  // don't know if DST was in effect or not
   strftime(buf, 20, "%B %e, %Y", &myDate);
   str = buf;
   setlocale(LC_TIME, loc);               // reset locale
   //std::cout << "GetLongDate() leaving with |" << str << "|" << std::endl;
   return str;
}

/**
 * A function to convert a YUTime to a string formatted for the local numeric locale.
 * Relies on "C" locale implementation to get appropriate separators.
 * TODO: C++ locales do not seem to be supported in C++98 - implement if we go to C++2011.
 * @param time the time to be converted
 * @return a formatted string in the form "hh:mm:ss"
 */
extern "C"
std::string sv::GetTime(const YUTime & aTime) {
   //std::cout << "GetTime() entered with aTime = |" << aTime << "|" << std::endl;
   if (aTime.isNULL()) return "";
   std::string str;

   char* loc = setlocale(LC_TIME, NULL); // save the value for restoration
   setlocale(LC_TIME, "");                // force locale to be set from environment variable

   struct tm myTime;
   char buf[20];
   memset(&myTime, 0, sizeof(struct tm));
   myTime.tm_hour = aTime.getHour();
   myTime.tm_min = aTime.getMinute();
   myTime.tm_sec = aTime.getSecond();
   myTime.tm_isdst = -1;                  // don't know if DST was in effect or not
   strftime(buf, 20, "%k:%M:%S", &myTime);
   str = buf;

   setlocale(LC_TIME, loc);               // reset locale
   //std::cout << "GetTime() leaving with |" << str << "|" << std::endl;
   return str;
}

extern "C"
std::string Expand(std::string & str, const int nCount, const char ch = ' ') {
   if (str.length() > (size_t)nCount) return str;

   //std::cout << "Expand: " << str << "nCount: " << nCount << std::endl;
   int count = nCount - str.length();
   std::string result = str.append(count, ch);
   //std::cout << "Expand: result is " << result << std::endl;
   return result;
}
