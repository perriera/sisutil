/*
 * SisUtilTests.cpp
 *
 * These tests depend on the environment variables LC_NUMERIC and LC_TIME being
 * set up in the runtime settings. The usual value is "en_CA.UTF-8" for both.
 *
 *  Created on: Jan 23, 2018
 *      Author: ojones
 */

#include <iostream>
#include <string.h>
#include <sisutil/toString.hpp>

#include "../vendor/catch.hpp"

using namespace std;
using namespace sv;

#define EXPECT_EQ(x,y) (x)==(y)
#define EXPECT_NE(x,y) (x)!=(y)
#define EXPECT_FALSE(x) (x)==false
#define EXPECT_TRUE(x) (x)==true
#define EXPECT_STREQ(a,b) strcmp(a,b)==0

SCENARIO("TEST(ToString, toString)", "[sv::toString]")
{
   int i = 0;   // middle integer
   EXPECT_EQ(0, atoi(toString(i).c_str()));

   i = 1;       // positive
   EXPECT_EQ(1, atoi(toString(i).c_str()));

   i = -1;      // negative
   EXPECT_EQ(-1, atoi(toString(i).c_str()));

   i = 3456;    // a normal positive integer
   EXPECT_EQ(3456, atoi(toString(i).c_str()));

   i = -6789;   // a normal negative integer
   EXPECT_EQ(-6789, atoi(toString(i).c_str()));

   i = 1000000; // a large positive integer
   //EXPECT_EQ(1000000, atoi(toString(i).c_str()));
   EXPECT_EQ(0, strcmp(toString(i).c_str(), "1000000"));
}

SCENARIO("TEST(ToUpper, upperCase)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "foobar   ";  // lower case + spaces - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(toUpper(str).c_str(), str_orig.c_str()));

   str = "FOOBAR";  // upper case - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(toUpper(str).c_str(), str_orig.c_str()));

   str = "FooBar";  // mixed case - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(toUpper(str).c_str(), str_orig.c_str()));

   str = "`FOOBAR{";  // edge case - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(toUpper(str).c_str(), str_orig.c_str()));
}

SCENARIO("TEST(ToLower, lowerCase)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "foobar   ";  // lower case + spaces - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(toLower(str).c_str(), str_orig.c_str()));

   str = "FOOBAR";  // upper case - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(toLower(str).c_str(), str_orig.c_str()));

   str = "FooBar";  // mixed case - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(toLower(str).c_str(), str_orig.c_str()));

   str = "@foobar[";  // edge case - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(toLower(str).c_str(), str_orig.c_str()));
}

SCENARIO("TEST(EraseTrailingChars, rightSpaces)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "";  // empty string - no difference, no crash
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseTrailingChars(str).c_str(), str_orig.c_str()));

   str = "foobar   ";  // trailing spaces - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(EraseTrailingChars(str).c_str(), str_orig.c_str()));

   str = "foobar_too"; // no spaces - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseTrailingChars(str).c_str(), str_orig.c_str()));

   str = "      ";    // all spaces - should be truncated
   EXPECT_EQ(0, strcmp(EraseTrailingChars(str).c_str(), ""));

   str = "   foo";    // leading spaces - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseTrailingChars(str).c_str(), str_orig.c_str()));

   str = "foo   bar"; // spaces in the middle - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseTrailingChars(str).c_str(), str_orig.c_str()));

   str = "foobarrrr  "; // trailing r, r specified but trailing spaces  - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseTrailingChars(str, 'r').c_str(), str_orig.c_str()));

   str = "foobarrrr"; // trailing r, r specified  - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(EraseTrailingChars(str, 'r').c_str(), str_orig.c_str()));
}

SCENARIO("TEST(EraseLeadingChars, leftSpaces", "[sv::toString]")
{
   std::string str, str_orig;

   str = "";  // empty string - no difference, no crash
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseLeadingChars(str).c_str(), str_orig.c_str()));

   str = "foobar   ";    // trailing spaces - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseLeadingChars(str).c_str(), str_orig.c_str()));

   str = "foobar_too";   // no spaces - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseLeadingChars(str).c_str(), str_orig.c_str()));

   str = "      ";       // all spaces - should be truncated
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseLeadingChars(str).c_str(), ""));

   str = "   foo";       // leading spaces - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(EraseLeadingChars(str).c_str(), str_orig.c_str()));

   str = "foo   bar";    // spaces in the middle - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseLeadingChars(str).c_str(), str_orig.c_str()));

   str = "  ffffoobar";  // leading f, f specified but leading spaces  - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(EraseLeadingChars(str, 'f').c_str(), str_orig.c_str()));

   str = "ffffoobar";    // leading f, f specified  - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(EraseLeadingChars(str, 'f').c_str(), str_orig.c_str()));
}

SCENARIO("TEST(Trim, leftAndRightSpaces)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "";  // empty string - no difference, no crash
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str).c_str(), str_orig.c_str()));

   str = "foobar   ";    // trailing spaces - should be truncated
   str_orig = str;
   EXPECT_NE(0, strcmp(Trim(str).c_str(), str_orig.c_str()));

   str = "foobar_too";   // no spaces - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str).c_str(), str_orig.c_str()));

   str = "      ";       // all spaces - should be truncated
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str).c_str(), ""));

   str = "   foo";       // leading spaces - should be truncated
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str).c_str(), "foo"));

   str = "   foo    ";       // leading and trailing spaces - should be truncated
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str).c_str(), "foo"));

   str = "foo   bar";    // spaces in the middle - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str).c_str(), str_orig.c_str()));

   str = "  ffffoobar";  // leading f, f specified but leading spaces  - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(Trim(str, 'f').c_str(), str_orig.c_str()));

   str = "ffffoobar";    // leading f, f specified  - should be different
   str_orig = str;
   EXPECT_NE(0, strcmp(Trim(str, 'f').c_str(), str_orig.c_str()));
}

SCENARIO("TEST(Copy, copySubstring)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "";        // empty string - no difference, no crash
   str_orig = str;
   EXPECT_EQ(0, strcmp(Copy(str).c_str(), str_orig.c_str()));

   str = "foobar";  // full string - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(Copy(str).c_str(), str_orig.c_str()));

   str = "foobar";  // right substring - should be different
   EXPECT_EQ(0, strcmp(Copy(str, 3, 3).c_str(), "bar"));

   str = "foobar";  // left substring - should be different
   EXPECT_EQ(0, strcmp(Copy(str, 0, 3).c_str(), "foo"));

   str = "foobar";  // fully specified - no difference
   str_orig = str;
   EXPECT_EQ(0, strcmp(Copy(str, 0, 6).c_str(), str_orig.c_str()));

   str = "foobar";  // empty substring specified - should be different
   EXPECT_EQ(0, strcmp(Copy(str, 0, 0).c_str(), ""));
}

SCENARIO("TEST(IsNumeric, testDigits)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "";            // empty string
   EXPECT_FALSE(IsNumeric(str));

   str = "4";           // single digit
   EXPECT_TRUE(IsNumeric(str));

   str = "0123456789";  // all digits
   EXPECT_TRUE(IsNumeric(str));

   str = "123a";        // right substring
   EXPECT_FALSE(IsNumeric(str));

   str = "a123";        // left substring
   EXPECT_FALSE(IsNumeric(str));

   str = "123 456";     // space in the middle
   EXPECT_FALSE(IsNumeric(str));

   str = "123.456";     // decimal point in the middle (punctuation)
   EXPECT_FALSE(IsNumeric(str));
}

SCENARIO("TEST(GetNum, localized_toString)", "[sv::toString]")
{
   int i = 0;   // middle integer
   EXPECT_STREQ("0", GetNum(i).c_str());

   i = 1;       // positive
   EXPECT_STREQ("1", GetNum(i).c_str());

   i = -1;      // negative
   EXPECT_STREQ("-1", GetNum(i).c_str());

   i = 3456;    // a normal positive integer
   EXPECT_STREQ("3,456", GetNum(i).c_str());

   i = -6789;   // a normal negative integer
   EXPECT_STREQ("-6,789", GetNum(i).c_str());

   i = 1000000; // a large positive integer
   EXPECT_STREQ("1,000,000", GetNum(i).c_str());
}

SCENARIO("TEST(GetDate, localized_toString)", "[sv::toString]")
{
   YUDate dt = YUDate(1, 1, 2017);   // int dd, int mm, int yy
   EXPECT_STREQ(" 1-Jan-17", GetDate(dt).c_str());

   dt = YUDate(30, 12, 2017);       //
   EXPECT_STREQ("30-Dec-17", GetDate(dt).c_str());

   dt = YUDate(1, 1, 1999);     //
   EXPECT_STREQ(" 1-Jan-99", GetDate(dt).c_str());

   dt = YUDate(1, 1, 17);    //
   EXPECT_STREQ(" 1-Jan-17", GetDate(dt).c_str());

   dt = YUDate(1, 1, 2000);   //
   EXPECT_STREQ(" 1-Jan-00", GetDate(dt).c_str());

   dt = YUDate(1, 1, 1); //
   EXPECT_STREQ(" 1-Jan-01", GetDate(dt).c_str());
}

// Currently same output as GetDate(). TODO: what is the difference as used in STAC?
SCENARIO("TEST(GetLongDate, localized_toString)", "[sv::toString]")
{
   YUDate dt = YUDate(1, 1, 2017);   // int dd, int mm, int yy
   EXPECT_STREQ(" 1-Jan-17", GetLongDate(dt).c_str());

   dt = YUDate(30, 12, 2017);       //
   EXPECT_STREQ("30-Dec-17", GetLongDate(dt).c_str());

   dt = YUDate(1, 1, 1999);     //
   EXPECT_STREQ(" 1-Jan-99", GetLongDate(dt).c_str());

   dt = YUDate(1, 1, 17);    //
   EXPECT_STREQ(" 1-Jan-17", GetLongDate(dt).c_str());

   dt = YUDate(1, 1, 2000);   //
   EXPECT_STREQ(" 1-Jan-00", GetLongDate(dt).c_str());

   dt = YUDate(1, 1, 1); //
   EXPECT_STREQ(" 1-Jan-01", GetLongDate(dt).c_str());
}

SCENARIO("TEST(Expand, expandByNChars)", "[sv::toString]")
{
   std::string str, str_orig;

   str = "";        // empty string - no difference, no crash
   str_orig = str;
   EXPECT_EQ(0, strcmp(Expand(str, 0, 'a').c_str(), str_orig.c_str()));

   str = "foobar";  // full string - no difference
   str_orig = str + "aaaaa";
   EXPECT_NE(0, strcmp(Expand(str, 5, 'a').c_str(), str_orig.c_str()));

   str = "foobar";  // full string - no difference
   str_orig = str + "aaaaa";
   EXPECT_EQ(0, strcmp(Expand(str, 11, 'a').c_str(), str_orig.c_str()));
   //std::cout << "expandByNChars: " << str << ", " << str_orig << std::endl;
}
