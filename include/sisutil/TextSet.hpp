/*
 *               File:   TextSet.hpp
 *
 *        Description:   utility classes for strings
 *
 * Copyright 2018 YORK UNIVERSITY
 *
 *  Revision History
 *
 *  When      Who        What
 *  --------- ---------- -----------------------------------------------------
 *  29-Aug-96 larry      textset copied from client base lib's
 *                       - this could/should be implemented as a base class
 *                         on the server side
 *  16-Jan-18 ojones     conversion to STL & Linux
 *
 */
 // $Id:

#ifndef TEXTSET_HPP_
#define TEXTSET_HPP_
#include <sisutil/YUCont.hpp>
#include <string>

//using std::string;

typedef unsigned long  ULONG;
typedef unsigned short USHORT;

//DECLARE_LIST(LstTextStrings, String*);
DECLARE_YUCONTAINER(LstTextStrings, std::string);

class TextSet {
public:
   TextSet();
   virtual ~TextSet();
   TextSet(const TextSet&);
   TextSet& operator  = (const TextSet&);
   TextSet& operator += (const TextSet&);

   void          Clear();
   bool          Insert(string* pS);
   ULONG         GetCount() const { return(theSet.Count()); }
   std::string* GetNth(ULONG nPos) const { return(theSet.GetObject(nPos)); }

protected:
   LstTextStrings theSet;
};

TextSet      BreakAtLF(const TextSet&, long nTab = 0);
TextSet      BreakAtLF(const std::string&, long nTab = 0);
TextSet      WrapText(const TextSet&, ULONG cpl, long nTab = 0);
TextSet      WrapText(const std::string&, ULONG cpl, long nTab = 0);
std::string  EOPage(long cpl);
std::string  EODocument(long cpl);
std::string  EOPgHdr(long cpl);
std::string  SODocument(long cpl);
std::string  NULLField(long width);
std::string  MarkBreakLine(long cpl);
std::string  MarkEmptyLine(long cpl);
std::string  FixedString(const std::string& rStr, USHORT nChars, bool trailSpace = true, bool rJustify = false);
std::string  RemoveLineEnd(const std::string&, char cReplace = ' ');

#endif /* TEXTSET_HPP_ */
