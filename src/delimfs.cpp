// delimfs.cpp - delimited file stream
// $Id: delimfs.cpp,v 1.9 1998/11/18 02:25:50 ed Exp $

#include <sisutil/delimfs.hpp>
#include <sisutil/ErrLog.hpp>
#include <sisutil/Marshall.hpp>         // for isTraceDebug

static const std::string className = "delim";

delim_ifstream::delim_ifstream(const std::string& aFileName, const char theDelim)
   : myDelim(theDelim), myFilename(aFileName) {
   myFileBuff = new char[BUFFER_LENGTH];
   LENGTH = BUFFER_LENGTH;  // OJ - not previously present but causes a compiler warning
   if (Marshall::isTraceDebug())
      getTheErrLog().putLine(className, className, std::string("reading: " + filename()).c_str());

   //if (myFileBuff) setbuf(myFileBuff, BUFFER_LENGTH); //old
   if (myFileBuff) rdbuf()->pubsetbuf(myFileBuff, BUFFER_LENGTH);
   open(filename().c_str(), std::ios::in);
   //FILE * myFILE = fopen(aFileName.c_str(), "r");
   if (std::ifstream::fail())               // ??
      getTheErrLog().putLine(std::string(className + "_ifstream"), className,
         std::string("file open failed for: " + filename()));
   //if (myFileBuff) setvbuf(myFILE, myFileBuff, _IOFBF, BUFFER_LENGTH);
}

delim_ifstream::~delim_ifstream() {
   if (myFileBuff) delete[] myFileBuff;
   std::ifstream::close();
}

delim_ifstream& operator >> (delim_ifstream& rS, char* outBuffer) {
   char ch = '\0';
   // todo: if (rS.fail())
   rS.get(outBuffer, delim_ofstream::MIN_BUFFER_LENGTH, rS.delim());
   rS.get(ch); // todo: if (rS.fail())
   if (ch != rS.delim() && (int)ch != 0) {
      //strstream tmpStream;
      std::stringstream tmpStream;
      tmpStream << "unknown delimiter char \"" << ch << "\" found in file : " << rS.filename();
      getTheErrLog().putStream("", std::string("operator >> (" + className + "...)"), tmpStream);
   }
   return rS;
}

delim_ofstream::delim_ofstream(const std::string& theFileName, const char theDelim)
   : std::ofstream(theFileName.c_str(), std::ios::out),
   myDelim(theDelim), myFilename(theFileName) {
   LENGTH = MIN_BUFFER_LENGTH;  // OJ - not previously present but causes a compiler warning
   if (std::ofstream::fail())
      getTheErrLog().putLine(std::string(className + "_ofstream"), className,
         std::string("file open failed for " + theFileName));
}

delim_ofstream::~delim_ofstream() {
   std::ofstream::close();
}

delim_ofstream& operator << (delim_ofstream& rS, const char* inBuffer) {
   //rS.ofstream::operator << (inBuffer); // todo: if (rS.fail())
   //rS.std::ofstream::operator << (inBuffer); // todo: if (rS.fail())
   // https://stackoverflow.com/questions/45085960/why-does-output-of-a-string-to-unnamed-stdofstream-give-me-a-hex-number-instea/45085961
   // TODO: C++11 the code will work as expected  
   // To make the code work in C++03, you can use a member function std::ostream::flush()
   rS.std::ofstream::flush() << (inBuffer);

   //   std::string str = inBuffer;
   //   for(unsigned i=0; i<str.length(); i++)
   //       rS.put(str.at(i)); 
   rS.put(rS.delim());   // todo: if (rS.fail())
   return rS;
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57) // fill-column: 70
// End:
