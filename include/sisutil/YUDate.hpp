// YUDate.hpp - class YUTime,YUDate,YUDateTime - self marshalling dates
// $Id: YUDate.hpp,v 1.4 2000/01/29 16:44:10 ed Exp $
#ifndef _YUDATE_H_
#define _YUDATE_H_ 1

#include <iostream>
#include <sisutil/SISDate.hpp>
#include <sisutil/Marshall.hpp>

class delim_ofstream;
class delim_ifstream;

/**
 * Months start at 1
 * Days start at 1
 */
class YUDate : public YUStreamable {
public:
   typedef long Julian;
   typedef long Seconds; // seconds since midnight

   YUDate(); // defaults to TODAY
   YUDate(int dd, int mm, int yy);
   YUDate(const SISDate&);
   YUDate(Julian);
   YUDate(const YUDate&);
   inline virtual ~YUDate() {}

   YUDate& operator = (const YUDate&);
   YUDate& operator = (const SISDate&);
   YUDate& operator = (long);

   int         operator == (const YUDate&) const;
   int         operator >  (const YUDate&) const;
   int         operator <  (const YUDate&) const;
   int         operator >= (const YUDate&) const;
   int         operator <= (const YUDate&) const;

   int         getDay() const;
   int         getMonth() const;
   int         getYear() const;
   inline int  getWeekDay() const { return((int)(myJulValue % 7L)); } // 0=Sunday
   Julian      getJulian() const { return (myJulValue); }
   int         isWeekDay() const; // true iff !{saturday || sunday}
   int         isNULL() const { return(!myJulValue); }
   std::string getMonthShortText();
   std::string getMonthText();

   // note: most mutation should be done via conversion (eg Date or SISDate)
   // (enough wheels re-invented already)
   void        nullify() { myJulValue = 0L; }
   YUDate& setToday();

   operator SISDate() const;

   friend Marshall& operator << (Marshall&, const YUDate&);
   friend Marshall& operator >> (Marshall&, YUDate&);

   friend delim_ofstream& operator << (delim_ofstream&, const YUDate&);
   friend delim_ifstream& operator >> (delim_ifstream&, YUDate&);

   friend std::ostream& operator << (std::ostream&, const YUDate&);

   inline virtual long getLenRPCMarshall() const { return(9); }

private:
   Julian  myJulValue;

public:													// or private: friend class YUTime?
  // helpers
   static Julian  cvtJulian(int dd, int mm, int yy);
   static void    splitJulian(Julian, int& dd, int& mm, int& yy);
   static Seconds cvtSeconds(int hh, int mm, int sec = 0);
   static void    splitSeconds(Seconds, int& hh, int& mm, int& ss);
   static Julian  sysDate();
   static Seconds sysTime();
   static int     subStringToInt(const char* c, int start, int length);
};

class YUTime : public YUStreamable
{
public:
   YUTime(); // defaults to NOW
   YUTime(int hh, int mm = 0, int sec = 0);
   YUTime(YUDate::Seconds);
   YUTime(const YUTime&);
   inline virtual ~YUTime() {};

   YUTime& operator = (const YUTime&);
   YUTime& operator = (long);

   int     operator == (const YUTime&) const;
   int     operator >  (const YUTime&) const;
   int     operator <  (const YUTime&) const;
   int     operator >= (const YUTime&) const;
   int     operator <= (const YUTime&) const;

   int     getHour() const;
   int     getMinute() const;
   int     getSecond() const;

   YUDate::Seconds getSECS() const { return(myTicker); }

   int     isAM() const { return(getHour() < 12); }
   int     isNULL() const { return(myTicker < 0); }
   void    nullify() { myTicker = -1; }

   friend Marshall& operator << (Marshall&, const YUTime&);
   friend Marshall& operator >> (Marshall&, YUTime&);

   friend delim_ofstream& operator << (delim_ofstream&, const YUTime&);
   friend delim_ifstream& operator >> (delim_ifstream&, YUTime&);

   friend std::ostream& operator << (std::ostream&, const YUTime&);

   virtual long getLenRPCMarshall() const { return(6); } // number(5)

private:
   YUDate::Seconds myTicker;
};

class YUDateTime : public YUStreamable
{
public:
   YUDateTime(); // defaults to NOW
   YUDateTime(const YUDate&, const YUTime&);
   YUDateTime(YUDate::Julian, YUDate::Seconds);
   YUDateTime(const YUDateTime&);
   inline virtual ~YUDateTime() {};

   YUDateTime& operator = (const YUDateTime&);

   int         operator == (const YUDateTime&) const;
   int         operator >  (const YUDateTime&) const;
   int         operator <  (const YUDateTime&) const;
   int         operator >= (const YUDateTime&) const;
   int         operator <= (const YUDateTime&) const;

   YUDate      getDate() const;
   YUTime      getTime() const;

   inline int  isNULL() const { return(myDate.isNULL() || myTime.isNULL()); }
   inline void nullify() { myDate.nullify(); myTime.nullify(); }

   YUDateTime& setDate(const YUDate&);
   YUDateTime& setTime(const YUTime&);

   friend Marshall& operator << (Marshall&, const YUDateTime&);
   friend Marshall& operator >> (Marshall&, YUDateTime&);

   friend delim_ofstream& operator << (delim_ofstream&, const YUDateTime&);
   friend delim_ifstream& operator >> (delim_ifstream&, YUDateTime&);

   friend std::ostream& operator << (std::ostream&, const YUDateTime&);

   virtual long getLenRPCMarshall() const { return(15); } // yyyyddmmhhmmss

private:
   YUDate myDate;
   YUTime myTime;
};

// 28-apr-98 [ld]
// YUTXNDate - used by STAC
// - a YUDateTime with default time=12:00 am (vs defTime=NOW)
// - nullification of time is not allowed
class YUTXNDate : public YUStreamable
{
public:
   typedef long Julian;
   typedef long Seconds; // seconds since midnight

   YUTXNDate(); // defaults to Date=NOW, Time=12:00am
   YUTXNDate(const YUDate&, const YUTime&);
   YUTXNDate(Julian, Seconds);
   YUTXNDate(const YUDateTime&); //+
   YUTXNDate(const YUTXNDate&);
   ~YUTXNDate() {};

   YUTXNDate& operator = (const YUTXNDate&);
   YUTXNDate& operator = (const YUDateTime&); //+

   bool       operator == (const YUTXNDate&) const;
   bool       operator >  (const YUTXNDate&) const;
   bool       operator <  (const YUTXNDate&) const;
   bool       operator >= (const YUTXNDate&) const;
   bool       operator <= (const YUTXNDate&) const;

   YUDate     getDate() const;
   YUTime     getTime() const;

   bool       isNULL() const { return(myDate.isNULL() || myTime.isNULL()); }
   void       nullify() { myDate.nullify(); myTime = 0; }

   YUTXNDate& setDate(const YUDate&);
   YUTXNDate& setTime(const YUTime&);

   friend Marshall& operator << (Marshall&, const YUTXNDate&);
   friend Marshall& operator >> (Marshall&, YUTXNDate&);

   friend delim_ofstream& operator << (delim_ofstream&, const YUTXNDate&);
   friend delim_ifstream& operator >> (delim_ifstream&, YUTXNDate&);

   friend std::ostream& operator << (std::ostream&, const YUTXNDate&);

   virtual long getLenRPCMarshall() const { return(15); } // yyyyddmmhhmmss

private:
   YUDate myDate;
   YUTime myTime;
};


// Name: YUTime,YUDate,YUDateTime 
// Documentation: self marshalling dates
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
