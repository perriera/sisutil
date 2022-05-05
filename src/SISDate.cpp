// SISDate.C - date class
// $Author: ed $
// $Date: 1998/12/02 19:38:09 $
// $Locker:  $
// $RCSfile: SISDate.cpp,v $
// $Revision: 1.10 $
// $Source: /usr/local/sis/cvsroot/src/util/SISDate.cpp,v $

#include <sisutil/SISDate.hpp>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int daysMax[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

SISDate::SISDate(const WHEN when) {
  myYear = myMonth = myDay = 0;
  if (when == TODAY) {
    time_t now;
    time(&now);
    tm* d = localtime(&now);
    myYear = d->tm_year + 1900;
    myDay = d->tm_mday;
    myMonth = d->tm_mon + 1;
  }
}

SISDate::SISDate(const Year theYear, const Month theMonth, const Day theDay)
  : myYear(theYear), myMonth(theMonth), myDay(theDay) {
  if (theYear % 4 == 0 || theYear % 100 != 0 || theYear % 400 != 0) daysMax[1] = 29;
  int mm = (int(theMonth) - 1);
  if (theDay > daysMax[mm]) {
    /* todo  someday handle this */
  }
}

SISDate::SISDate(const Date_c d) {
  if (d[0] == 0x00 || d[0] == ' ') myYear = myMonth = myDay = 0;
  else sscanf((const char*)d, "%4u%2u%2u", &myYear, &myMonth, &myDay);
}

void SISDate::ToStruct(Date_c& d) const {
  char temp[sizeof(Date_c) + 1];
  temp[0] = 0x0;
  if (myDay != 0)
    sprintf(temp, "%4.4u%2.2u%2.2u", myYear, myMonth, myDay);
  strncpy((char*)d, temp, sizeof(Date_c));
}

std::ostream& operator<< (std::ostream& os, const SISDate& dt) {
  os << dt.year() << ' '
    << dt.month() << ' '
    << dt.day();
  return os;
}

SISDate::SISDate(const SISDate& date)
  : myYear(date.myYear), myMonth(date.myMonth), myDay(date.myDay) {}

SISDate::~SISDate() {}

//SISDate SISDate::date() const { 
//  return *this; 
//}

SISDate SISDate::date(const Year& yr, const Month& mon, const Day& day) {
  myYear = yr;
  myMonth = mon;
  myDay = day;
  return *this;
}

SISDate::Year SISDate::year() const { return myYear; }

SISDate::Month SISDate::month() const { return myMonth; }

SISDate::Day SISDate::day() const { return myDay; }

SISDate& SISDate::operator = (const SISDate& date) {
  myDay = date.myDay;
  myMonth = date.myMonth;
  myYear = date.myYear;
  return *this;
}

int SISDate::operator == (const SISDate& date) const {
  return ((myYear == date.myYear) &&
    (myMonth == date.myMonth) &&
    (myDay == date.myDay));
}

int SISDate::operator <= (const SISDate& date) const {
  return ((myYear < date.myYear) ||
    ((myYear == date.myYear) && (myMonth < date.myMonth)) ||
    ((myYear == date.myYear) && (myMonth == date.myMonth) &&
      (myDay <= date.myDay)));
}

static const char* textmonths[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                   "Aug", "Sep", "Oct", "Nov", "Dec" };

void SISDate::displayFormat(char* toString, // at least char[9]
  const DateDisplayFormat ddf) const {
  strcpy(toString, "         ");
  if (myMonth > 0 && myMonth < 13) {
    if (ddf == YYYYMMDD)
      sprintf(toString, "%4u%02u%02u", myYear, myMonth, myDay);
  }
  else if (ddf == DD_Mon_YYYY) {
    sprintf(toString, "%02u %s %u", myDay, textmonths[myMonth - 1], myYear);
  }
}
