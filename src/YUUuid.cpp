// YUUuid.cpp - self marshalling DCE uuid
// $Id: YUUuid.cpp,v 1.6 1998/12/02 19:25:49 ed Exp $

#include <sisutil/YUUuid.hpp>
#include <sisutil/OCIMarsh.hpp>
#include <sisutil/RPCMarsh.hpp>
#include <sisutil/delimfs.hpp>  // for delim_ofstream and delim_ifstream
#include <string>

RPCMarshall& operator << (RPCMarshall& m, const YUUuid& yu) {
   std::string c;
   yu.toString(c);
   m << c.c_str();
   return m;
}

RPCMarshall& operator >> (RPCMarshall& m, YUUuid& yu) {
   char t[40];
   m >> t;
   std::string tc(t);
   Uuid u(tc);
   yu = u;
   return m;
}

OCIMarshall& operator << (OCIMarshall& m, const YUUuid& yu) {
   uuid_t u; // = yu;
   uuid_copy(u, yu.operator const uuid_t & ());

   const char* c = (const char*)&u;
   for (int i = 0; i < 16; i++, c++)
      m << *c;
   return m;
}

OCIMarshall& operator >> (OCIMarshall& m, YUUuid& yu) {
   uuid_t* u = yu;
   char* c = (char*)u;
   for (int i = 0; i < 16; i++, c++)
      m >> *c;
   return m;
}

std::ostream& operator << (std::ostream& os, const YUUuid& yu) {
   std::string c;
   yu.toString(c);
   os << c.c_str();
   return os;
}

delim_ofstream& operator << (delim_ofstream& rS, const YUUuid& yu) {
   std::string c;
   yu.toString(c);
   rS << c.c_str();
   return rS;
}

delim_ifstream& operator >> (delim_ifstream& rS, YUUuid& yu) {
   char aUuid[40];
   rS >> aUuid;
   yu = Uuid(std::string(aUuid));
   return rS;
}
