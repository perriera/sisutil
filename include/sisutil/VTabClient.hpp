// VTabClient.hpp  - Valid Table service
// $Id: VTabClient.hpp,v 1.7 2000/01/29 16:44:09 ed Exp $

#ifndef _VTABCLIENT_H
#define _VTABCLIENT_H 1

#ifndef _YU_YUCONT_H_INCLUDED
#include <sisutil/YUCont.hpp>
#define _YU_YUCONT_H_INCLUDED
#endif

class VTab;

#include <string>

DECLARE_YUCONTAINER(ValidTableSET, VTab);

class VTabClient {
public:
	inline VTabClient() {}
	virtual ~VTabClient();

	inline const std::string& getTableDir() const { return sTableDir; }
	inline void setTableDir(const std::string& theDir) { sTableDir = theDir; }
	// what is a DatExt anyway?
	inline const std::string& getDatExt() const { return sDatExt; }
	inline void setDatExt(const std::string& theDatExt) { sDatExt = theDatExt; }

	int   cacheVTab(VTab*);
	VTab* locateVTab(const std::string& rFName);
private:
	VTabClient(const VTabClient&);              // disable copy ctor
	VTabClient& operator = (const VTabClient&);	// disable op=

	std::string sTableDir;
	std::string sDatExt;
	ValidTableSET myTableSet;
};

// Name: VTabClient
// Documentation: Valid Table service
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
