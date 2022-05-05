// YUNum.cpp - Number class
// $Id: YUNum.cpp,v 1.21 2011/06/02 15:40:40 ojones Exp $

#include <sisutil/YUNum.hpp>
#include <sstream>
#include <sisutil/delimfs.hpp>
#include <stdio.h>  // for sprintf
#include <stdlib.h> // for labs
#include <math.h>   // modf is useful
#include <string>
#include <cstring>

YUNumber::YUNumber(const YUNumber& rhs)
   : myProps(rhs.getProps()), myLHS(rhs.LHS()), myRHS(rhs.RHS()),
   nullflag(rhs.isNull()), overflowflag(rhs.isOverflow()) {}

YUNumber& YUNumber::nullify() { nullflag = 1; return *this; }

int YUNumber::isNull() const { return nullflag ? 1 : 0; }

int YUNumber::isOverflow() const { return overflowflag ? 1 : 0; }

YUNumber::YUNumber(long l) : myLHS(l), myRHS(0), nullflag(0), overflowflag(0) {
   int i;
   for (i = 1; (l > pow(10, i)); i++);
   myProps = YUNumProps(i, 0);
}

YUNumber::YUNumber(double f) : nullflag(0), overflowflag(0) {
   // scale to get non-fractional component
   double dVal = (fabs(f) + (5.0 / (pow(10, (getProps().getScale() + 1.0)))));
   int negative = false;
   if (f < 0.0) {
      negative = true;
   }
   myLHS = (long)dVal;
   double fraction, integral;
   fraction = modf(dVal, &integral);
   myRHS = 0L;
   if (getProps().getScale()) {
      double scaled_fraction = fraction * pow(10, getProps().getScale());
      if (negative)
         myRHS = scaled_fraction - 0.5;
      else
         myRHS = scaled_fraction + 0.5;
   }

   if (myLHS >= pow(10, getProps().getPrecision())) {
      overflowflag = 1;
   }
   if (negative) {
      if (myLHS > 0) myLHS *= -1;
      else myRHS *= -1;
   }
}

YUNumber& YUNumber::operator = (long i) {
   nullflag = 0;
   myLHS = i;
   myRHS = 0;
   if (myLHS >= pow(10, getProps().getPrecision())) {
      overflowflag = 1;
   }
   else {
      overflowflag = 0;
   }
   return *this;
}

YUNumber& YUNumber::operator = (double f) {
   nullflag = 0;
   int negative = false;
   if (f < 0.0) {
      negative = true;
   }
   double dVal = (fabs(f) + (5.0 / (pow(10, (myProps.getScale() + 1.0)))));
   myLHS = (long)dVal;

   double fraction, integral;
   //fraction = modf(f, &integral);
   fraction = modf(dVal, &integral);
   myRHS = 0L;
   if (getProps().getScale()) {
      double scaled_fraction = fraction * pow(10, getProps().getScale());
      if (negative)
         myRHS = scaled_fraction - 0.5;
      else
         myRHS = scaled_fraction + 0.5;
   }
   if (myLHS >= pow(10, getProps().getPrecision())) {
      overflowflag = 1;
   }
   else {
      overflowflag = 0;
   }
   if (negative) {
      if (myLHS > 0) myLHS *= -1;
      else myRHS *= -1;
   }
   return *this;
}

YUNumber& YUNumber::operator = (const YUNumber& rhs) {
   // Note: possible bug here.
   // This method does not copy over the YUNumProps which does not seem
   // correct if it is to be an assignment operator.
   // It also does not crash & burn on overflow as do other methods (OJ)
   if (this != &rhs) {
      nullflag = rhs.isNull();
      overflowflag = rhs.isOverflow();
      myLHS = rhs.LHS();
      if (getProps().getScale()) {
         if (getProps().getScale() > rhs.getProps().getScale())
            myRHS = long(rhs.myRHS * pow(10, getProps().getScale() - rhs.getProps().getScale()));
         else if (getProps().getScale() < rhs.getProps().getScale())
            myRHS = long(rhs.myRHS / pow(10, rhs.getProps().getScale() - getProps().getScale()));
         else
            myRHS = rhs.RHS();
      }
      if (myLHS >= pow(10, getProps().getPrecision())) {
         overflowflag = 1;
      }
      else overflowflag = 0;
   }
   return *this;
}

int YUNumber::operator == (long i) const {
   YUNumber t(myProps);
   t = i;
   return ((t == *this) && !isNull());
}

int YUNumber::operator == (double f) const {
   YUNumber t(myProps);
   t = f;
   return ((t == *this) && !isNull());
}

YUNumber::operator long() const {
   return myLHS;
}

YUNumber::operator double() const {
   int  negative = false;
   long tmpLHS = myLHS;
   long tmpRHS = myRHS;
   if (myLHS < 0 || myRHS < 0) {
      negative = true;
      tmpLHS = labs(myLHS);
      tmpRHS = labs(myRHS);
   }
   double result = (double)tmpLHS + (double)(tmpRHS) / pow(10, myProps.getScale());
   if (negative) result *= -1.0;
   return result;
}

int YUNumber::operator == (const YUNumber& sn) const {
   return ((isNull() && sn.isNull()) ||
      ((myLHS == sn.LHS()) && (myRHS == sn.RHS())));
}

int YUNumber::operator != (const YUNumber& sn) const {
   return !operator == (sn);
}

int operator == (long i, const YUNumber& sn) {
   return (sn == i);
}

int operator == (double d, const YUNumber& sn) {
   return (sn == d);
}

// free friend inserter function overloads
Marshall& operator << (Marshall& m, const YUNumber& sn) {
   return sn.Marsh(m);
}

Marshall& operator >> (Marshall& m, YUNumber& sn) {
   return sn.UnMarsh(m);
}

std::ostream& operator << (std::ostream& m, const YUNumber& sn) {
   return sn.DisplayOn(m);
}

long YUNumber::getLenRPCMarshall() const {
   long result = (long)(getProps().getPrecision() + getProps().getScale()) + 1L;
   if (getProps().getScale()) result++;
   return(result);
}

Marshall& YUNumber::Marsh(Marshall& m) const {
   // convert to char array
   //char temp[20];
   //memset(temp, 0x00, sizeof(temp));
   //ostrstream sb1(temp, 20);
   std::ostringstream sb1;
   if (!isNull() && !isOverflow()) {
      int i;
      //int sign = 0;
      int negative = false;
      long tmpRHS = myRHS;
      long tmpLHS = myLHS;
      if (myRHS < 0 || myLHS < 0) {
         negative = true;
         tmpLHS = labs(myLHS);
         tmpRHS = labs(myRHS);
         //sign = 1;
      }

      long o = tmpLHS;
      //	for (i = getProps().getPrecision() - 1; (i > sign) && (o < pow(10, (i - sign))); i--)
      //     sb1 << ' ';
      if (negative) {
         sb1 << '-';
      }
      sb1 << tmpLHS;
      if (getProps().getScale()) {
         sb1 << '.';
         o = tmpRHS;
         for (i = getProps().getScale() - 1; (i) && (o < pow(10, i)); i--)
            sb1 << '0';
         sb1 << tmpRHS;
      }
      //sb1 << std::ends;
   }

   // some prospect for handling a generic exception here would be REAL NICE.
   // For now, we'll simply attract attention if we're forced out of bounds
   /// 28-apr-98 [ld] - DIE ON OVERFLOW
   if (isOverflow()) {          // XXX todo: throw
      std::cerr << "[FATAL]YUNumber::Marsh:"
         << " (@202) overflow condition on marshall operation"
         << ", myLHS: " << myLHS
         << ", myRHS: " << myRHS
         << ", myProps.prec: " << myProps.getPrecision()
         << ", myProps.scale: " << myProps.getScale()
         << std::endl;
      abort();
   }

   int width = getProps().getPrecision();
   if (getProps().getScale()) width += (getProps().getScale() + 1);

   //std::cerr << "Marsh: temp:\"" << temp << "\"" << std::endl;
   //m << Setmw(width) << temp;
   //std::cerr << "Marsh: sb1:\"" << sb1 << "\"" << std::endl;
   m << Setmw(width) << sb1.str().c_str();
   return m;
}

Marshall& YUNumber::UnMarsh(Marshall& m) {
   char  temp[128];
   memset(temp, 0x00, sizeof(temp));
   nullflag = 0;
   int width = getProps().getPrecision();
   if (getProps().getScale()) width += (getProps().getScale() + 1);

   if (width > 127) {
      m.inValidate();
      return(m);
   }
   m >> Setmw(width) >> temp;
   //std::cerr << "YUNumber::UnMarsh: (" << getProps().getPrecision() << "," << getProps().getScale()<< "), temp:\"" << temp << "\"" << std::endl;
   if (temp[0]) {
      int negative = false;
      double dVal = 0.0;
      sscanf(temp, "%lf", &dVal);
      if (dVal < 0.0) {
         negative = true;
         dVal = fabs(dVal);
      }
      double fraction, integral;
      fraction = modf(dVal, &integral);
      myLHS = (long)integral;
      myRHS = 0L;
      if (getProps().getScale()) {
         double scaled_fraction = fraction * pow(10, getProps().getScale());
         myRHS = scaled_fraction + 0.5;
         if (negative) {
            if (myLHS == 0) myRHS *= -1;
            else myLHS *= -1;
         }
      }
      else {
         if (negative) myLHS *= -1;
      }
      // Potential bug here: some seq numbers have more than 9 digits.
      // Example: seqtranshandle in stars_transaction table.
      // A better approach is to use INT_MAX from limits.h as the upper limit.
      // There should be no immediate problem since we're not yet close to this value.
      if (labs(myLHS) > 999999999L
         || (labs(myLHS) > pow(10, getProps().getPrecision()))
         || labs(myRHS) > 999999999L) {
         // this would be nice, but we're not symmetric:
         overflowflag = 1;
         // so....  some prospect for handling a generic exception here
         // would be REAL NICE. For now, we'll simply attract attention
         // if we're forced out of bounds
         /// 28-apr-98 [ld] - DIE ON OVERFLOW

         std::cerr << "[FATAL]YUNumber::UnMarsh:"
            << "(@254) overflow condition on marshall operation"
            << ", myLHS: " << myLHS
            << ", myRHS: " << myRHS
            << ", myProps.prec: " << myProps.getPrecision()
            << ", myProps.scale: " << myProps.getScale() << std::endl;
         abort();
      }
      else {
         overflowflag = 0;
      }
      nullflag = 0;
   }
   else {
      nullflag = 1;
   }
   return m;
}

std::ostream& YUNumber::DisplayOn(std::ostream& os) const {
   //std::cerr << "YUNumber::DisplayOn(std::ostream &): myLHS = " << myLHS << ", myRHS = " << myRHS << std::endl;
   if (!isNull() && !isOverflow()) {
      if (myLHS == 0 && myRHS < 0) os << '-';
      os << myLHS;
      if (getProps().getScale()) {
         os << '.';
         int i;
         long o = labs(myRHS);
         for (i = getProps().getScale() - 1; (i) && (o < pow(10, i)); i--) {
            os << '0';
         }
         os << labs(myRHS);
      }
   } // else do nothing for overflow or null
   return os;
}

delim_ofstream& operator << (delim_ofstream& rS, const YUNumber& rO) {
   //char temp[20];
   //memset(temp, 0x00, sizeof(temp));
   //ostrstream sb1(temp, 20);
   //std::ostringstream sb1(temp, std::ios_base::app);
   std::ostringstream sb1;
   if (!rO.isNull() && !rO.isOverflow()) {
      int  i;
      int  sign = 0;
      int  negative = false;
      long tmpRHS = rO.myRHS;
      long tmpLHS = rO.myLHS;
      if (rO.myRHS < 0 || rO.myLHS < 0) {
         negative = true;
         tmpLHS = labs(rO.myLHS);
         tmpRHS = labs(rO.myRHS);
         sign = 1;
      }
      long o = tmpLHS;
      for (i = rO.myProps.getPrecision() - 1; (i > sign) && (o < pow(10, (i - sign))); i--) {
         sb1 << ' ';
      }
      if (negative) {
         sb1 << '-';
      }
      sb1 << tmpLHS;
      if (rO.myProps.getScale()) {
         sb1 << '.';
         o = tmpRHS;
         for (i = rO.myProps.getScale() - 1; (i) && (o < pow(10, i)); i--) {
            sb1 << '0';
         }
         sb1 << tmpRHS;
      }
      sb1 << std::ends;
   }
   //std::cerr << "op<<: temp:\"" << temp << "\"" << std::endl;
  //std::cerr << "op<<: sb1:\"" << sb1.str().c_str() << "\"" << std::endl;
   //rS << temp;
   rS << sb1.str();
   return rS;
}

delim_ifstream& operator >> (delim_ifstream& rS, YUNumber& rO) {
   char temp[50];
   memset(temp, 0x00, sizeof(temp));
   rS >> temp;
   //std::cerr << "op>>: temp:\"" << temp << "\"" << std::endl;
   if (temp[0]) {
      double dVal = 0.0;
      int negative = false;
      sscanf(temp, "%lf", &dVal);
      if (dVal < 0.0) {
         negative = true;
         dVal = fabs(dVal);
      }

      double fraction, integral;
      fraction = modf(dVal, &integral);
      rO.myLHS = (long)integral;
      rO.myRHS = 0L;

      if (rO.getProps().getScale()) {
         rO.myRHS = (long)(fraction * pow(10, rO.myProps.getScale()) + 0.5);
         if (negative) {
            if (rO.myLHS == 0) rO.myRHS *= -1;
            else               rO.myLHS *= -1;
         }
      }
      else {
         if (negative) rO.myLHS *= -1;
      }
      rO.nullflag = 0;
      rO.overflowflag = 0;
   }
   else {
      rO.nullflag = 1;
   }

   return rS;
}
