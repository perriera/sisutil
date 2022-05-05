// YUDate.cpp - YUTime,YUDate,YUDateTime - self marshalling dates
// $Id: YUDate.cpp,v 1.9 1999/12/22 05:39:38 ed Exp $

#include <sisutil/YUDate.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sisutil/delimfs.hpp>
#include <sstream> 	  	 // for subStringtoInt
#include <cstring>       // for strlen

// daysMax[0] is actually a leap year February
static int daysMax[13] = { 29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static std::string shortMonthText[13] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static std::string monthText[13] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

YUDate::Julian YUDate::cvtJulian(int idd, int imm, int iyy) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::cvtJulian(): called with idd = " << idd << ", imm = " << imm << ", iyy = " << iyy << std::endl;
   long dd = (long)idd;
   long mm = (long)imm;
   long yy = (long)iyy;

   long ctmp, dtmp, mtmp, ytmp;
   long result1, result2, result3;

   // basic sanity
   if (!dd || !mm || !yy) return(0L); // exception?

   if (mm < 0 || mm > 12) mm = 12;    // exception

   int maxDD = daysMax[mm];
   if (mm == 2 && yy % 4 == 0) {
      maxDD = daysMax[0];             // leap year February
   }

   if (dd < 0 || dd > maxDD) dd = maxDD; // exception
   if (mm > 2) {
      mtmp = mm - 3;
      ytmp = yy;
   }
   else {
      mtmp = mm + 9;
      ytmp = yy - 1;
   }
   ctmp = (ytmp / 100);
   dtmp = ytmp - (100 * ctmp);
   result1 = 146097L * ctmp / 4;
   result2 = (1461 * dtmp) / 4;
   result3 = (153 * mtmp + 2) / 5;
   return((result1 + dd + result2 + 1721119L + result3));
}

void YUDate::splitJulian(Julian j, int& idd, int& imm, int& iyy) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::splitJulian(): called with Julian = " << j << ", idd = " << idd << ", imm = " << imm << ", iyy = " << iyy << std::endl;

   long mtmp;
   long dd = (long)idd;
   long mm = (long)imm;
   long yy = (long)iyy;

   if (!j || j < 0) {
      idd = 0;
      imm = 0;
      iyy = 0;
      return;
   }

   mtmp = j - 1721119L;
   yy = (4 * mtmp - 1) / 146097L;
   mtmp = 4 * mtmp - 1 - 146097L * yy;
   dd = mtmp / 4;
   mtmp = (4 * dd + 3) / 1461;
   dd = 4 * dd + 3 - 1461 * mtmp;
   dd = (dd + 4) / 4;
   mm = (5 * dd - 3) / 153;
   dd = 5 * dd - 3 - 153 * mm;
   dd = (dd + 5) / 5;
   yy = 100 * yy + mtmp;

   if (mm < 10) {
      mm += 3;
   }
   else {
      mm -= 9;
      yy++;
   }

   idd = (int)dd;
   imm = (int)mm;
   iyy = (int)yy;
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::splitJulian(): leaving with Julian = " << j << ", idd = " << idd << ", imm = " << imm << ", iyy = " << iyy << std::endl;
}

YUDate::Seconds YUDate::cvtSeconds(int hh, int mm, int ss) {
   if (hh < 0) return(-1L);
   if (mm < 0) return(-1L);
   if (ss < 0) return(-1L);

   hh = (hh % 24);
   mm = (mm % 60);
   ss = (ss % 60);

   return((Seconds)(hh * 3600L + mm * 60L + ss));
}

void YUDate::splitSeconds(YUDate::Seconds nsec, int& ihh, int& imm, int& iss) {
   long hh = nsec / 3600L;
   long mm = (nsec - (hh * 3600L)) / 60L;
   long ss = nsec - (hh * 3600L + mm * 60L);

   if (nsec < 0) {
      ihh = -1;
      imm = -1;
      iss = -1;
   }
   else {
      ihh = (int)hh;
      imm = (int)mm;
      iss = (int)ss;
   }
}

YUDate::Julian YUDate::sysDate() {
   time_t now;
   time(&now);
   //tm *d  = localtime(&now);   // deprecated - unsafe
   struct tm d;
#ifdef WIN32
   localtime_s(&d, &now);
#else
   localtime_r(&now, &d);
#endif
   int yy = d.tm_year + 1900;
   int dd = d.tm_mday;
   int mm = d.tm_mon + 1;
   return(cvtJulian(dd, mm, yy));
}

YUDate::Seconds YUDate::sysTime() {
   time_t now;
   time(&now);
   //tm *d  = localtime(&now);   // deprecated - unsafe
   struct tm d;
#ifdef WIN32
   localtime_s(&d, &now);
#else
   localtime_r(&now, &d);
#endif
   int hh = d.tm_hour;
   int mm = d.tm_min;
   int ss = d.tm_sec;
   return(cvtSeconds(hh, mm, ss));
}

// N.B. returns -1 if no valid numeric string is found
int YUDate::subStringToInt(const char* c, int start, int length) {
   // no built-in atoi conversion for string
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::subStringToInt(): entering with c = " << c << ", start = " << start << ", length = " << length << std::endl;
   std::string aStr = c;
   aStr = aStr.substr(start, length);
   //std::istringstream tmpStream(c + start + '\0');
   std::istringstream tmpStream(aStr);
   int retVal = -1;
   tmpStream >> std::dec >> retVal;

   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::subStringToInt(): leaving with retVal = " << retVal << std::endl;
   return retVal;
}

YUDate::YUDate() {
   myJulValue = sysDate();
}

YUDate::YUDate(int dd, int mm, int yy) {
   myJulValue = cvtJulian(dd, mm, yy);
}

YUDate::YUDate(const SISDate& rD) {
   myJulValue = cvtJulian(rD.day(), rD.month(), rD.year());
}

YUDate::YUDate(Julian j) {
   myJulValue = j;
   if (myJulValue < 0) myJulValue = 0;
}

YUDate::YUDate(const YUDate& rD) {
   myJulValue = rD.myJulValue;
}

YUDate& YUDate::operator = (const YUDate& rD) {
   myJulValue = rD.myJulValue;
   return(*this);
}

YUDate& YUDate::operator = (long jD) {
   myJulValue = (Julian)jD;
   return(*this);
}

YUDate& YUDate::operator = (const SISDate& rD) {
   myJulValue = cvtJulian(rD.day(), rD.month(), rD.year());
   return(*this);
}

int YUDate::operator == (const YUDate& rD) const {
   return(myJulValue == rD.myJulValue);
}

int YUDate::operator > (const YUDate& rD) const {
   return(myJulValue > rD.myJulValue);
}

int YUDate::operator < (const YUDate& rD) const {
   return(myJulValue < rD.myJulValue);
}

int YUDate::operator >= (const YUDate& rD) const {
   return(myJulValue >= rD.myJulValue);
}

int YUDate::operator <= (const YUDate& rD) const {
   return(myJulValue <= rD.myJulValue);
}

int YUDate::getDay() const {
   int dd, mm, yy;
   splitJulian(myJulValue, dd, mm, yy);
   return(dd);
}

int YUDate::getMonth() const {
   int dd, mm, yy;
   splitJulian(myJulValue, dd, mm, yy);
   return(mm);
}

int YUDate::getYear() const {
   int dd, mm, yy;
   splitJulian(myJulValue, dd, mm, yy);
   return(yy);
}

int YUDate::isWeekDay() const {
   return(!(getWeekDay() == 6 || !getWeekDay()));
}

std::string YUDate::getMonthShortText() {
   std::string month = shortMonthText[getMonth()];
   return month;
}

std::string YUDate::getMonthText() {
   std::string month = monthText[getMonth()];
   return month;
}

YUDate& YUDate::setToday() {
   myJulValue = sysDate();
   return(*this);
}

YUDate::operator SISDate() const {
   int dd, mm, yy;
   splitJulian(myJulValue, dd, mm, yy);
   return(SISDate(yy, mm, dd));
}

Marshall& operator << (Marshall& m, const YUDate& yu) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::operator << (Marshall& m, YUDate& yu): called with: " << yu << std::endl;
   char cStream[9];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      int dd, mm, yy;
      YUDate::splitJulian(yu.getJulian(), dd, mm, yy);
      snprintf(cStream, 9, "%04d%02d%02d", yy, mm, dd);
   }
   m << Setmw(8) << cStream;
   return(m);
}

Marshall& operator >> (Marshall& m, YUDate& yu) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDate::operator >> (Marshall& m, YUDate& yu): called with: " << yu << std::endl;
   char cStream[9];
   m >> Setmw(8) >> cStream;
   cStream[8] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 8) {
      int yy = YUDate::subStringToInt(cStream, 0, 4);
      int mm = YUDate::subStringToInt(cStream, 4, 2);
      int dd = YUDate::subStringToInt(cStream, 6, 2);
      if (yy != -1 && mm != -1 && dd != -1) yu = YUDate(dd, mm, yy);
   }
   return(m);
}

std::ostream& operator << (std::ostream& os, const YUDate& yu) {
   char cStream[9];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      int dd, mm, yy;
      YUDate::splitJulian(yu.getJulian(), dd, mm, yy);
      snprintf(cStream, 9, "%04d%02d%02d", yy, mm, dd);
   }
   os << cStream;
   return(os);
}

delim_ofstream& operator << (delim_ofstream& rS, const YUDate& yu) {
   char cStream[9];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      int dd, mm, yy;
      YUDate::splitJulian(yu.getJulian(), dd, mm, yy);
      snprintf(cStream, 9, "%04d%02d%02d", yy, mm, dd);
   }
   rS << cStream;
   return(rS);
}

delim_ifstream& operator >> (delim_ifstream& rS, YUDate& yu) {
   char cStream[9];
   rS >> cStream;
   cStream[8] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 8) {
      int yy = YUDate::subStringToInt(cStream, 0, 4);
      int mm = YUDate::subStringToInt(cStream, 4, 2);
      int dd = YUDate::subStringToInt(cStream, 6, 2);
      if (yy != -1 && mm != -1 && dd != -1) yu = YUDate(dd, mm, yy);
   }
   return(rS);
}

YUTime::YUTime() {
   myTicker = YUDate::sysTime();
}

YUTime::YUTime(int hh, int mm, int ss) {
   myTicker = YUDate::cvtSeconds(hh, mm, ss);
}

YUTime::YUTime(YUDate::Seconds theTicker) {
   myTicker = theTicker;
}

YUTime::YUTime(const YUTime& rT) {
   myTicker = rT.myTicker;
}

YUTime& YUTime::operator = (const YUTime& rT) {
   myTicker = rT.myTicker;
   return(*this);
}

YUTime& YUTime::operator = (long jT) {
   myTicker = (YUDate::Seconds)jT;
   return(*this);
}

int YUTime::operator == (const YUTime& rT) const {
   return(myTicker == rT.myTicker);
}

int YUTime::operator > (const YUTime& rT) const {
   return(myTicker > rT.myTicker);
}

int YUTime::operator < (const YUTime& rT) const {
   return(myTicker < rT.myTicker);
}

int YUTime::operator >= (const YUTime& rT) const {
   return(myTicker >= rT.myTicker);
}

int YUTime::operator <= (const YUTime& rT) const {
   return(myTicker <= rT.myTicker);
}

int YUTime::getHour() const {
   int hh;
   int mm;
   int ss;

   YUDate::splitSeconds(myTicker, hh, mm, ss);
   return(hh);
}

int YUTime::getMinute() const {
   int hh;
   int mm;
   int ss;

   YUDate::splitSeconds(myTicker, hh, mm, ss);
   return(mm);
}

int YUTime::getSecond() const {
   int hh;
   int mm;
   int ss;

   YUDate::splitSeconds(myTicker, hh, mm, ss);
   return(ss);
}

Marshall& operator << (Marshall& m, const YUTime& yu) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUTime::operator << (Marshall& m, YUTime& yu): called with: " << yu << std::endl;
   char cStream[6];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      snprintf(cStream, 6, "%05ld", (long)yu.getSECS());
   }
   m << Setmw(5) << cStream;
   return(m);
}

Marshall& operator >> (Marshall& m, YUTime& yu) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUTime::operator >> (Marshall& m, YUTime& yu): called with: " << yu << std::endl;
   char cStream[6];
   m >> Setmw(5) >> cStream;
   cStream[5] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 5) {
      long theTicker = -1;
      sscanf(cStream, "%ld", &theTicker);
      yu = YUTime(theTicker);
   }
   return(m);
}

std::ostream& operator << (std::ostream& os, const YUTime& yu) {
   char cStream[6];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      snprintf(cStream, 6, "%05ld", (long)yu.getSECS());
   }
   os << cStream;
   return(os);
}

delim_ofstream& operator << (delim_ofstream& rS, const YUTime& yu) {
   char cStream[6];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      snprintf(cStream, 6, "%05ld", (long)yu.getSECS());
   }
   rS << cStream;
   return(rS);
}

delim_ifstream& operator >> (delim_ifstream& rS, YUTime& yu) {
   char cStream[6];
   rS >> cStream;
   cStream[5] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 5) {
      long theTicker = -1;
      sscanf(cStream, "%ld", &theTicker);
      yu = YUTime(theTicker);
   }
   return(rS);
}

YUDateTime::YUDateTime() {
}

YUDateTime::YUDateTime(const YUDate& rD, const YUTime& rT) {
   myDate = rD;
   myTime = rT;
}

YUDateTime::YUDateTime(YUDate::Julian j, YUDate::Seconds s) {
   myDate = YUDate(j);
   myTime = YUTime(s);
}

YUDateTime::YUDateTime(const YUDateTime& rDT) {
   myDate = rDT.getDate();
   myTime = rDT.getTime();
}

YUDateTime& YUDateTime::operator = (const YUDateTime& rDT) {
   myDate = rDT.getDate();
   myTime = rDT.getTime();
   return(*this);
}

int YUDateTime::operator == (const YUDateTime& rDT) const {
   return(myDate == rDT.getDate() && myTime == rDT.getTime());
}

int YUDateTime::operator > (const YUDateTime& rDT) const {
   return(myDate > rDT.getDate() || (myDate == rDT.getDate() && myTime > rDT.getTime()));
}

int YUDateTime::operator < (const YUDateTime& rDT) const {
   return(myDate < rDT.getDate() || (myDate == rDT.getDate() && myTime < rDT.getTime()));
}

int YUDateTime::operator >= (const YUDateTime& rDT) const {
   return(myDate >= rDT.getDate() || (myDate == rDT.getDate() && myTime >= rDT.getTime()));
}

int YUDateTime::operator <= (const YUDateTime& rDT) const {
   return(myDate <= rDT.getDate() || (myDate == rDT.getDate() && myTime <= rDT.getTime()));
}

YUDate YUDateTime::getDate() const {
   return(myDate);
}

YUTime YUDateTime::getTime() const {
   return(myTime);
}

YUDateTime& YUDateTime::setDate(const YUDate& rD) {
   myDate = rD;
   return(*this);
}

YUDateTime& YUDateTime::setTime(const YUTime& rT) {
   myTime = rT;
   return(*this);
}

Marshall& operator << (Marshall& m, const YUDateTime& yu) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDateTime::operator << (Marshall& m, YUDateTime& yu): called with: " << yu << std::endl;
   char cStream[15];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      YUDate aD(yu.getDate());
      YUTime aT(yu.getTime());
      snprintf(cStream, 15, "%04d%02d%02d%02d%02d%02d", aD.getYear(), aD.getMonth(), aD.getDay(), aT.getHour(), aT.getMinute(), aT.getSecond());
   }
   m << Setmw(14) << cStream;
   return(m);
}

Marshall& operator >> (Marshall& m, YUDateTime& yu) {
   //if (getenv("SIS_OCI_DEBUG")) std::cerr << "YUDateTime::operator << (Marshall& m, YUDateTime& yu): called with: " << yu << std::endl;
   char cStream[15];
   m >> Setmw(14) >> cStream;
   cStream[14] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 14) {
      int yy = YUDate::subStringToInt(cStream, 0, 4);
      int mo = YUDate::subStringToInt(cStream, 4, 2);
      int dd = YUDate::subStringToInt(cStream, 6, 2);
      int hh = YUDate::subStringToInt(cStream, 8, 2);
      int mm = YUDate::subStringToInt(cStream, 10, 2);
      int ss = YUDate::subStringToInt(cStream, 12, 2);
      YUDate aDT(dd, mo, yy);
      YUTime aTM(hh, mm, ss);
      if (yy != -1 && mo != -1 && dd != -1) yu.setDate(aDT);
      yu.setTime(aTM);
   }
   return(m);
}

std::ostream& operator << (std::ostream& os, const YUDateTime& yu) {
   YUDate aD(yu.getDate());
   os << aD;
   if (!yu.isNULL()) {
      char cStream[7];
      YUTime aT(yu.getTime());
      snprintf(cStream, 7, "%02d%02d%02d", aT.getHour(), aT.getMinute(), aT.getSecond());
      os << cStream;
   }
   return(os);
}

delim_ofstream& operator << (delim_ofstream& rS, const YUDateTime& yu) {
   char cStream[15];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      YUDate aD(yu.getDate());
      YUTime aT(yu.getTime());
      snprintf(cStream, 15, "%04d%02d%02d%02d%02d%02d", aD.getYear(), aD.getMonth(), aD.getDay(), aT.getHour(), aT.getMinute(), aT.getSecond());
   }
   rS << cStream;
   return(rS);
}

delim_ifstream& operator >> (delim_ifstream& rS, YUDateTime& yu) {
   char cStream[15];
   rS >> cStream;
   cStream[14] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 14) {
      int yy = YUDate::subStringToInt(cStream, 0, 4);
      int mo = YUDate::subStringToInt(cStream, 4, 2);
      int dd = YUDate::subStringToInt(cStream, 6, 2);
      int hh = YUDate::subStringToInt(cStream, 8, 2);
      int mm = YUDate::subStringToInt(cStream, 10, 2);
      int ss = YUDate::subStringToInt(cStream, 12, 2);
      YUDate aDT(dd, mo, yy);
      YUTime aTM(hh, mm, ss);
      if (yy != -1 && mo != -1 && dd != -1) yu.setDate(aDT);
      yu.setTime(aTM);
   }
   return(rS);
}

YUTXNDate::YUTXNDate()
{
   myTime = YUTime(0);
}

YUTXNDate::YUTXNDate(const YUDate& rD, const YUTime& rT)
{
   myDate = rD;
   if (rT.isNULL()) {
      myTime = YUTime(0);
   }
   else {
      myTime = rT;
   }
}

YUTXNDate::YUTXNDate(Julian j, Seconds s)
{
   myDate = YUDate(j);
   myTime = YUTime(s);
}

YUTXNDate::YUTXNDate(const YUTXNDate& rDT)
{
   myDate = rDT.getDate();
   myTime = rDT.getTime();
}

YUTXNDate::YUTXNDate(const YUDateTime& rDT)
{
   myDate = rDT.getDate();
   myTime = rDT.getTime();
   if (myTime.isNULL()) myTime = YUTime(0);
}

YUTXNDate& YUTXNDate::operator = (const YUTXNDate& rDT)
{
   myDate = rDT.getDate();
   myTime = rDT.getTime();
   return(*this);
}

YUTXNDate& YUTXNDate::operator = (const YUDateTime& rDT)
{
   myDate = rDT.getDate();
   myTime = rDT.getTime();
   if (myTime.isNULL()) myTime = YUTime(0);
   return(*this);
}

bool YUTXNDate::operator == (const YUTXNDate& rDT) const
{
   return(myDate == rDT.getDate() && myTime == rDT.getTime());
}

bool YUTXNDate::operator > (const YUTXNDate& rDT) const
{
   return(myDate > rDT.getDate() || (myDate == rDT.getDate() && myTime > rDT.getTime()));
}

bool YUTXNDate::operator < (const YUTXNDate& rDT) const
{
   return(myDate < rDT.getDate() || (myDate == rDT.getDate() && myTime < rDT.getTime()));
}

bool YUTXNDate::operator >= (const YUTXNDate& rDT) const
{
   return(myDate >= rDT.getDate() || (myDate == rDT.getDate() && myTime >= rDT.getTime()));
}

bool YUTXNDate::operator <= (const YUTXNDate& rDT) const
{
   return(myDate <= rDT.getDate() || (myDate == rDT.getDate() && myTime <= rDT.getTime()));
}

YUDate YUTXNDate::getDate() const
{
   return(myDate);
}

YUTime YUTXNDate::getTime() const
{
   return(myTime);
}

YUTXNDate& YUTXNDate::setDate(const YUDate& rD)
{
   myDate = rD;
   return(*this);
}

YUTXNDate& YUTXNDate::setTime(const YUTime& rT)
{
   myTime = rT;
   if (myTime.isNULL()) myTime = YUTime(0);
   return(*this);
}

Marshall& operator << (Marshall& m, const YUTXNDate& yu)
{
   char cStream[15];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      YUDate aD(yu.getDate());
      YUTime aT(yu.getTime());
      sprintf(cStream, "%04d%02d%02d%02d%02d%02d", aD.getYear(), aD.getMonth(), aD.getDay(), aT.getHour(), aT.getMinute(), aT.getSecond());
   }
   m << Setmw(14) << cStream;
   return(m);
}

Marshall& operator >> (Marshall& m, YUTXNDate& yu)
{
   char cStream[15];
   m >> Setmw(14) >> cStream;
   cStream[14] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 14) {
      int yy = atoi((std::string(cStream).substr(0, 4)).c_str());
      int mo = atoi(std::string(cStream).substr(4, 2).c_str());
      int dd = atoi(std::string(cStream).substr(6, 2).c_str());
      int hh = atoi(std::string(cStream).substr(8, 2).c_str());
      int mm = atoi(std::string(cStream).substr(10, 2).c_str());
      int ss = atoi(std::string(cStream).substr(12, 2).c_str());
      YUDate aDT(dd, mo, yy);
      YUTime aTM(hh, mm, ss);
      yu.setDate(aDT);
      yu.setTime(aTM);
   }
   return(m);
}

std::ostream& operator << (std::ostream& os, const YUTXNDate& yu)
{
   YUDate aD(yu.getDate());
   os << aD;
   if (!yu.isNULL()) {
      char cStream[7];
      YUTime aT(yu.getTime());
      sprintf(cStream, "%02d%02d%02d", aT.getHour(), aT.getMinute(), aT.getSecond());
      os << cStream;
   }
   return(os);
}

delim_ofstream& operator << (delim_ofstream& rS, const YUTXNDate& yu)
{
   char cStream[15];
   cStream[0] = 0x0;
   if (!yu.isNULL()) {
      YUDate aD(yu.getDate());
      YUTime aT(yu.getTime());
      sprintf(cStream, "%04d%02d%02d%02d%02d%02d", aD.getYear(), aD.getMonth(), aD.getDay(), aT.getHour(), aT.getMinute(), aT.getSecond());
   }
   rS << cStream;
   return(rS);
}

delim_ifstream& operator >> (delim_ifstream& rS, YUTXNDate& yu)
{
   char cStream[15];
   rS >> cStream;
   cStream[14] = 0x0;
   yu.nullify();
   if (strlen(cStream) == 14) {
      int yy = atoi(std::string(cStream).substr(0, 4).c_str());
      int mo = atoi(std::string(cStream).substr(4, 2).c_str());
      int dd = atoi(std::string(cStream).substr(6, 2).c_str());
      int hh = atoi(std::string(cStream).substr(8, 2).c_str());
      int mm = atoi(std::string(cStream).substr(10, 2).c_str());
      int ss = atoi(std::string(cStream).substr(12, 2).c_str());
      YUDate aDT(dd, mo, yy);
      YUTime aTM(hh, mm, ss);
      yu.setDate(aDT);
      yu.setTime(aTM);
   }
   return(rS);
}
