/*
 * TextSet.cpp
 *
 * Copyright 2018 YORK UNIVERSITY
 *
 */
 // $Id:

#include <sisutil/YUBase.hpp>
#include <sisutil/TextSet.hpp>
#include <iostream>
#include <sisutil/toString.hpp>

using std::cerr;
using std::endl;
using std::string;

TextSet::TextSet() {
   // TODO Auto-generated constructor stub

}

TextSet::~TextSet() {
   Clear();
}

void TextSet::Clear() {
   string* pS = theSet.First();
   while (pS) {
      delete pS;
      pS = theSet.Next();
   }
   theSet.Clear();
}

TextSet::TextSet(const TextSet& rSet) {
   ULONG rSetCount = rSet.GetCount();
   for (ULONG curItem = 0; curItem < rSetCount; curItem++) {
      string* pSR = rSet.GetNth(curItem);
      if (pSR) {
         string* pNS = new string(*pSR);
         if (!Insert(pNS)) delete pNS;
      }
   }
}

TextSet& TextSet::operator = (const TextSet& rSet) {
   Clear();
   ULONG rSetCount = rSet.GetCount();
   for (ULONG curItem = 0; curItem < rSetCount; curItem++) {
      string* pSR = rSet.GetNth(curItem);
      if (pSR) {
         string* pNS = new string(*pSR);
         if (!Insert(pNS)) delete pNS;
      }
   }
   return(*this);
}

TextSet& TextSet::operator += (const TextSet& rSet) {
   ULONG rSetCount = rSet.GetCount();
   for (ULONG curItem = 0; curItem < rSetCount; curItem++) {
      string* pSR = rSet.GetNth(curItem);
      if (pSR) {
         string* pNS = new string(*pSR);
         if (!Insert(pNS)) delete pNS;
      }
   }
   return(*this);
}

bool TextSet::Insert(string* pS) {
   //theSet.Insert(pS, LIST_APPEND);
   theSet.Insert(pS, 1);
   return(true);
}

// implementation of FUNCTIONS
TextSet BreakAtLF(const TextSet& rTS, long nTab) {
   TextSet tsResult;
   long nItem = 0;
   string* pS = rTS.GetNth(nItem++);
   while (pS) {
      tsResult += BreakAtLF(*pS, nTab);
      pS = rTS.GetNth(nItem++);
   }
   return(tsResult);
}

TextSet BreakAtLF(const string& rS, long nTab) {
   TextSet tsResult;
   string myCopy(rS);

   // TODO: this method call is from Starview and it converts any line ending to the requested on.
   //       In this case, the line ending is for Unix which is what we would expect to have anyways by default
   //       however, the client may be using a Windows line ending (CRLF).
   //       See the Starview supplemental documentation for more (C:\sv32\DOC\NEW.TXT).
   //myCopy.ConvertLineEnd(LINEEND_LF);

   // There is no difference between \r\n and \015\012. 
   // \0XX escape sequence denotes a literal octal representation of a char.//
   string testLF("\012");
   //string sTab;
   //sTab.Expand((USHORT)nTab);
   string sTab(nTab, ' ');

   //USHORT nlPos = myCopy.Search(testLF);
   //USHORT nlPos = myCopy.find(testLF);
   ULONG nlPos = myCopy.find(testLF);
   while (nlPos != string::npos) {
      //string remainder = myCopy.Cut(nlPos);
      //string remainder = myCopy.erase(nlPos);
      string remainder = myCopy.substr(nlPos);
      myCopy.erase(nlPos);
      tsResult.Insert(new string(myCopy));
      //remainder.Cut(0, testLF.length());
      remainder.erase(0, testLF.length());
      myCopy = sTab + remainder;
      //nlPos = myCopy.Search(testLF);
      nlPos = myCopy.find(testLF);
   }
   tsResult.Insert(new string(myCopy));
   return(tsResult);
}

TextSet WrapText(const TextSet& rTS, ULONG cpl, long nTab) {
   TextSet tsResult;
   long nItem = 0;
   string* pS = rTS.GetNth(nItem++);
   while (pS) {
      tsResult += WrapText(*pS, cpl, nTab);
      pS = rTS.GetNth(nItem++);
   }
   return(tsResult);
}

TextSet WrapText(const string& rS, ULONG cpl, long nTab) {
   TextSet tsResult;
   string myCopy(rS);
   //string sTab;
   //sTab.Expand((USHORT)nTab);
   string sTab(nTab, ' ');

   while (myCopy.length() > cpl) {
      // look for the last space @position <=cpl
      ULONG breakPos = string::npos;
      //USHORT nextCandidate = myCopy.Search(' ', (breakPos == string::npos ? 0 : breakPos + 1));
      ULONG nextCandidate = myCopy.find(' ', (breakPos == string::npos ? 0 : breakPos + 1));
      while (nextCandidate <= cpl && nextCandidate != string::npos) {
         breakPos = nextCandidate;
         //nextCandidate = myCopy.Search(' ', (breakPos == string::npos ? 0 : breakPos + 1));
         nextCandidate = myCopy.find(' ', (breakPos == string::npos ? 0 : breakPos + 1));
      }
      if (breakPos == string::npos) {
         cerr << "[textset.cpp:wraptext] - could not break string:" << myCopy.c_str() << endl;
         break;
      }
      //string remainder = myCopy.Cut(breakPos);
      //string remainder = myCopy.erase(breakPos);
      string remainder = myCopy.substr(breakPos);
      myCopy.erase(breakPos);
      tsResult.Insert(new string(myCopy));
      //remainder.Cut(0, 1);
      remainder.erase(0, 1);
      myCopy = sTab + remainder;
   }
   tsResult.Insert(new string(myCopy));
   return(tsResult);
}

string EOPage(long cpl) {
   //string result;
   //result.Expand((USHORT)cpl, '*');
   string result(cpl, '*');

   string sText("END OF PAGE, NO ENTRIES BELOW THIS LINE");

   USHORT insertAt = (result.length() / 2) - (sText.length() / 2);
   result.replace(insertAt, string::npos, sText);

   int restLen = cpl - result.length();
   string endStars(restLen, '*');
   result = result + endStars;

   return(result);
}

string EODocument(long cpl) {
   //string result;
   //result.Expand((USHORT)cpl, '*');
   string result(cpl, '*');

   string sText("END OF TRANSCRIPT");

   USHORT insertAt = (result.length() / 2) - (sText.length() / 2);
   result.replace(insertAt, string::npos, sText);

   int restLen = cpl - result.length();
   string endStars(restLen, '*');
   result = result + endStars;

   return(result);
}

string SODocument(long cpl) {
   //string result;
   //result.Expand((USHORT)cpl,'*');
   string result(cpl, '*');

   string sText("YORK UNIVERSITY TRANSCRIPT BEGINS");

   USHORT insertAt = (result.length() / 2) - (sText.length() / 2);
   result.replace(insertAt, string::npos, sText);

   int restLen = cpl - result.length();
   string endStars(restLen, '*');
   result = result + endStars;

   return(result);
}

string EOPgHdr(long cpl) {
   //string result;
   //result.Expand((USHORT)cpl, '*');
   string result(cpl, '*');

   return(result);
}

string NULLField(long width) {
   //string result;
   //result.Expand((USHORT)width, '-');
   string result(width, '-');

   return(result);
}

string MarkBreakLine(long cpl) {
   //string result;
   //result.Expand(cpl, '-');
   string result(cpl, '-');

   return(result);
}

string MarkEmptyLine(long /* cpl */) {
   string result("***");
   //result.Expand(cpl,'*');
   return(result);
}

string FixedString(const string& rStr, USHORT nChars, bool trailSpace, bool rJustify) {
   string result = rStr;
   if (result.length() < nChars) {
      //result.Expand(nChars);
      result.insert(result.length(), nChars - result.length(), ' '); // fill insert mode
   }
   else {
      //result.Cut(nChars);
      result.erase(nChars);
   }

   if (rJustify) {
      string sT1 = result;
      //sT1.EraseTrailingChars();
      sT1 = sv::EraseTrailingChars(sT1);
      unsigned int rhp = nChars;
      if (trailSpace) rhp--;
      if (sT1.length() < rhp) {
         int leading = (rhp - sT1.length());
         //string sT2;
         //sT2.Expand(leading);
         string sT2(leading, ' ');
         result = sT2 + sT1;
         //if (trailSpace) result.Expand(nChars);
         //if (trailSpace) result.insert(result.length() - 1, nChars, ' ');
         if (trailSpace && (nChars > result.length())) result.append(nChars - result.length(), ' ');
      }
   }

   if (trailSpace) {
      result.replace(result.length() - 1, string::npos, " ");
   }
   return(result);
}

string RemoveLineEnd(const string& rSrc, char cReplace) {
   string result(rSrc);

   // TODO: this method call is from Starview and it converts any line ending to the requested one.
   //       In this case, the line ending is for Unix which is what we would expect to have anyways by default
   //       however, the client may be using a Windows line ending (CRLF).
   //       See the Starview supplemental documentation for more (C:\sv32\DOC\NEW.TXT).
   //result.ConvertLineEnd(LINEEND_LF);

   string newline("\012");
   string replace(1, cReplace); // use fill c'tor - string has no c'tor that will take a char by itself!

   //USHORT nlPos  = result.Search(newline);
   ULONG nlPos = result.find(newline);
   while (nlPos != string::npos) {
      //result.Cut(nlPos, newline.length());
      result.erase(nlPos, newline.length());
      //result.Insert(replace, nlPos);
      result.insert(nlPos, replace);
      //nlPos = result.Search(newline);
      nlPos = result.find(newline);
   }

   return(result);
}
