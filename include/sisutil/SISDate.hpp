// SISDate.hpp - date class
// $Author: ed $
// $Date: 1999/04/27 16:15:46 $
// $Locker:  $
// $RCSfile: SISDate.hpp,v $
// $Revision: 1.13 $
// $Source: /usr/local/sis/cvsroot/src/util/SISDate.hpp,v $
//

#ifndef SISDATE_H
#define SISDATE_H 1

#include <iostream>

class SISDate {
public: 
		typedef unsigned char Date_c[8];
		typedef enum DateDisplayFormat 
		{YYYYMMDD, DD_Mon_YYYY /*, DDMMMYY, MMMDDYY, Month_DD_YYYY, Mon_DD_YYYY*/}
		DateDisplayFormat;
		typedef int Day;           // ensure day is reasonable for associated
		typedef int Month;
		typedef int Year;
		typedef enum WHEN { NULLDATE = 0, TODAY = 1} WHEN;

		SISDate(const WHEN w = NULLDATE);
		SISDate( const Year theYear, const Month theMonth, const Day theDay);
		SISDate(const Date_c d);
		void ToStruct(Date_c& d) const;
		SISDate( const SISDate& date );
		virtual ~SISDate();
		SISDate date( const Year& yr, const Month& mon, const Day& day );
		Year year() const;
		Month month() const;
		Day day() const; 
		void displayFormat(char* toString, // at least char[9]
											 const DateDisplayFormat ddf = DD_Mon_YYYY) const;
		SISDate& operator = (const SISDate& date);
		int operator == (const SISDate& date) const; 
		int operator <= (const SISDate& date) const; 
		int operator >= (const SISDate& date) const;
		SISDate& operator + ( int ) const; 
		int operator - (const SISDate&) const;
  
	friend std::ostream& operator<< (std::ostream& os, const SISDate& dt);
private:
		Year myYear;
		Month myMonth;
		Day myDay;
};

#endif

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:
