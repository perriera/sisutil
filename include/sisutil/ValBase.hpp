// ValBase.hpp  - class TabSet, VTab - valid table support
// $Id: ValBase.hpp,v 1.7 1999/04/27 16:16:12 ed Exp $

#ifndef _VALBASE_H
#define _VALBASE_H 1

#include <sisutil/YUBase.hpp>
#include <sisutil/YUCont.hpp>

// common abstract base class for ValidTables
class TabSet : public YUDBOBase {
public:
	typedef enum VPropsEnum { vtNONE, vtDATES, vtDESCRIPS } VProps;
	inline TabSet(const std::string& cName, const VProps aVProps = vtNONE)
		: YUDBOBase(cName, dboTVALID, dboSTSET), myVProps(aVProps) {}
	inline TabSet(const TabSet& rhs)
		: YUDBOBase(rhs.getClassName(), rhs.getClassType(), rhs.getSubType()),
		myVProps(getVProps()) {}
	inline  virtual ~TabSet() {}
	//virtual void     clear() = 0;
	virtual void     Clear() = 0;
	virtual TabSet* clone();
	const   VProps& getVProps() const { return myVProps; }

private:
	TabSet(); 									// disable default ctor
	TabSet& operator=(const TabSet&);	// disable op=
	const  VProps    myVProps;
};

// idString + pointer to a ValidTable
class VTab {
public:
	inline             VTab(const std::string& aFName, TabSet* aTabSet = NULL)
		: myFName(aFName), myTabSet(aTabSet) {}
	inline             VTab(const VTab&)
		: myFName(getFName()), myTabSet(getTabSet()->clone()) {}
	//inline virtual    ~VTab() { if (myTabSet) myTabSet->clear(); }
	inline virtual    ~VTab() { if (myTabSet) myTabSet->Clear(); }

	const std::string& getFName() { return myFName; }
	TabSet* getTabSet() { return myTabSet; }
	//inline virtual void clear() {/* todo */}
	void               clear() {/* todo */ }

private:
	VTab(); 											// disable default ctor
	VTab& operator=(const VTab&);	         // disable op=
	const std::string  myFName;
	TabSet* myTabSet;
};

class Index_Element {
	std::string myKey;
	long        myPos;

public:
	Index_Element(const std::string& sKey, long thePos)
	{
		myKey = sKey; myPos = thePos;
	}

	std::string  Key() const { return(myKey); }
	long         Pos() const { return(myPos); }
};

DECLARE_YUCONTAINER(LstIndex_Elements, Index_Element*);

class TABSET_Index {
public:
	TABSET_Index() { bSorted = true; }
	~TABSET_Index() { Clear(); }

	/* Ed M. made these virtual although the original code was not. Why?
	 * It couldn't be a design choice since he then axed all the valid table code that used it!
	virtual void Clear() = 0;
	virtual void Sort() = 0;
	virtual void Insert(Index_Element*) = 0;
	*/
	void         Clear();
	void         Sort();
	void         Insert(Index_Element*);
	long         Search(const std::string& sKey); // note: returns LIST_ENTRY_NOTFOUND on failure
	long         GetCount() const { return(myList.Count()); }

private:
	bool         bSorted;
	LstIndex_Elements myList;
};

// Name: class TabSet, class VTab - valid table support
// Documentation: 
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

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:

