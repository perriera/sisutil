// YUBase.hpp  - class YUDBOBase - persistent object base class
// $Id: YUBase.hpp,v 1.7 1998/11/27 22:31:03 ed Exp $

#ifndef _YUDBOBASE_H
#define _YUDBOBASE_H 1

#include <string>

// enum enumLanguage : int {langENGLISH, langFRENCH} ;

class YUDBOBase {
public:
   typedef enum DBOTypeEnum {
        dboTUnknown, dboTCRUD, dboTVALID, dboTVIEW, dboTLOCAL} DBOType;
   typedef enum DBOSubTypeEnum {
        dboSTUnknown, dboSTREF, dboSTFULL, dboSTSUM, dboSTSET} DBOSubType;

	inline YUDBOBase(const std::string& cName,
                    const DBOType aDBOType = dboTUnknown,
                    const DBOSubType aSubType = dboSTUnknown)
        : myClassName(cName), myType(aDBOType), mySubType(aSubType) {}

   inline virtual ~YUDBOBase() {}

   inline const std::string& getClassName() const {return myClassName;}
   inline const DBOType&     getClassType() const {return myType;}
   inline const DBOSubType&  getSubType()   const {return mySubType;}

private:
   YUDBOBase();                      // disable default ctor
   YUDBOBase(const YUDBOBase&);      // disable copy ctor
   YUDBOBase&                operator = (const YUDBOBase&);     // disable op=

   const std::string         myClassName;
   // use of RTTI suggests something wrong here; why no virtual functions?
   const DBOType             myType;
   const DBOSubType          mySubType;
};
#endif
