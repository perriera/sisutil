// YUCont.hpp  - type safe homogeneous by-ref container class  -*- c++ -*-
// $Id: YUCont.hpp,v 1.24 1999/07/30 12:04:21 ed Exp $

#ifndef _YUCONT_H
#define _YUCONT_H 1

#include <algorithm>
//#include <slist>
#include <ext/slist>       // singly linked list, SGI/GNU extension, not part of C++ standard lib

using namespace std;

//#define CONCAT(a, b) a ## b
#define DECLARE_YUCONTAINER(className,compType) typedef SVList_of_p<compType> className

// use the GNU (SGI) extension namespace to pick up the slist (singly linked list) implementation
typedef __gnu_cxx::slist<void*> list_impl;

template <class T>
class SVList_of_p : protected list_impl {
public:
   inline SVList_of_p() : myPos(0), myPosCache(begin()) {}
	SVList_of_p(const SVList_of_p<T>&);
	~SVList_of_p() { deepDelete(); }               // non-virtual: do not derive // was inline

	long	      Count() const;
	T*          GetObject(long nPos) const;
	T*          GetNth(long nPos) const;
	void        Insert(T* pItem, int atEnd = 0);
	T*          Remove(T* pItem);
	void        Clear();                                  // empty and discard contents
	T*          Replace(T* pNew, T* pOld);
   T*          Replace(T* pNew, unsigned long nPos);

	// StarView uses an intrusive iterator, which we will emulate
	T*          First();
	inline T*   GetFirst() { return First(); }
	T*          Next();
	inline T*   GetNext()  { return Next(); }
	typedef enum SetSizeEnum {DEFSIZE_SETCOUNT = 5} SetSize;
	// number of bytes required to hold maximum set item count in ASCII
	inline int  maxSizeASCIIBytes() const {return DEFSIZE_SETCOUNT;} //was inline

private:
	SVList_of_p<T>& operator = (const SVList_of_p<T>&);   // disable op=
	void        deepDelete();
	T*          getAt(long nPos) const;                   // mutable: changes current position
	void        resetCache();
	void        incrementCache();
	int         myPos;                                    // mutable: changed by iterator mutators
	list_impl::iterator myPosCache;
	inline void Latch() const {}                          // Lock and unlock mutex hooks
	inline void Unlatch() const {}
};

template <class T>
SVList_of_p<T>::SVList_of_p(const SVList_of_p<T>& rhs)
   : myPos(0), myPosCache(begin()) {
   T* thisMember;
   SVList_of_p<T>& mutableRhs = (SVList_of_p<T>&)rhs;    // cast away const;
   for (list_impl::iterator i = mutableRhs.begin(); i != mutableRhs.end(); i++)
      Insert(new T(*(T*)*i));                            // TODO: static_cast
}

template <class T>
long SVList_of_p<T>::Count() const {
   Latch();
   long retVal = size();
   Unlatch();
   return retVal;
}

template <class T>
T* SVList_of_p<T>::GetNth(long nPos) const {
   Latch();
   T* retVal = GetObject(nPos);
   Unlatch();                                            // slist<>::find; is O(N^2) slow; use (multi)set?
   return retVal;                                        // user should use iterator directly
}

template <class T>
T* SVList_of_p<T>::GetObject(long nPos) const {
   Latch();
   T* retVal = getAt(nPos);
   Unlatch();
   return retVal;
}

template <class T>
void SVList_of_p<T>::Insert(T* pItem, int /*ignored*/)   {
   Latch();
   if (size() == 0) {
      push_front(pItem);
      resetCache();
   } else {
      insert_after(myPosCache, pItem);
      incrementCache();
   }
   Unlatch();
}

template <class T>
T* SVList_of_p<T>::Remove(T* pItem) {
   Latch();
   T* retVal = (T*)0;
   list_impl::iterator i = find(begin(), end(), pItem);
   if (i != end()) {
      resetCache();
      retVal = (T*)*i;
      erase(i);
      
      // PINGLI July 3, 2020      
      resetCache();

   }
   Unlatch();
   return retVal;
}

template <class T>
T* SVList_of_p<T>::First() {
   Latch();                                              // protect myPos =
   resetCache();
   T* retVal = getAt(myPos);
   Unlatch();
   return retVal;
}

template <class T>
T* SVList_of_p<T>::Next() {
   Latch();                                              // protect myPos++
   incrementCache();
   T* retVal = getAt(myPos);
   Unlatch();
   return retVal;                                        // slist<>::find; is O(N^2) slow; use vector?
}  // user should use iterator directly

template <class T>
T* SVList_of_p<T>::getAt(long nPos) const {
   T* retVal = (T*)0;
   if (( (size_t)nPos >= size()) || (nPos < 0)) return retVal;
   Latch();
   SVList_of_p<T>& mutableThis = (SVList_of_p<T>&)*this; // cast away const;
   if (nPos < myPos)                                     // lose the cache
      mutableThis.resetCache();

   // myPos <= nPos < size()-> search ahead, hopefully no more than one
   for (int i = myPos; i < nPos; ++i)
      mutableThis.incrementCache();

   retVal = (T*)*myPosCache;
   Unlatch();
   return retVal;
}

// be sure you own all these pointers
template <class T>
void SVList_of_p<T>::deepDelete() {
   Latch();
   //T* e; unused
   for (list_impl::iterator i = begin(); i != end(); i++) {
      delete ((T*)*i);
   }
   Clear();
   Unlatch();
}

template <class T>
void SVList_of_p<T>::Clear() {
   Latch();
   clear();
   resetCache();
   Unlatch();
}

template <class T>
T* SVList_of_p<T>::Replace(T* pNew, T* pOld) {
   Latch();
   T* retVal = (T*)0;
   // if pOld points to an element in the list with
   // pNew, and return pOld, else return NULL
   list_impl::iterator i = find(begin(), end(), pOld);
   if (i != end()) {
      retVal = pOld;
      incrementCache();
      insert_after(i, pNew);
      erase(i);
   }
   Unlatch();
   return retVal;
}

template <class T>
T* SVList_of_p<T>::Replace(T* pNew, unsigned long nPos) {
   Latch();
   T* pOld = GetNth(nPos);
   if (pOld != NULL)
      Replace(pNew, pOld);

   Unlatch();
   return pOld;
}

template <class T>
void SVList_of_p<T>::resetCache() {
   myPos = 0;
   myPosCache = begin();
}

template <class T>
void SVList_of_p<T>::incrementCache() {
   if (myPosCache != end()) {
      myPosCache++;
      myPos++;
   }
}

//#define DECLARE_YUCONTAINER(className,compType) typedef SVList_of_p<compType> className

// Name: DECLARE_YUCONTAINER(className, elementType);
// Documentation:  type safe homogeneous by-ref container class
// N.B. this is a poor design for multi-threaded environments; if you can, 
//      use a List class with an external iterator class directly.
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
// Persistence: static | dynamic

#endif
