// ACL.cpp - DCE Access control list (minimal: groups only)
// $Id: ACL.cpp,v 1.12 1999/01/14 22:26:03 ed Exp $

#include <sisutil/ACL.hpp>
#include <sisutil/ACLRep.hpp>
#include <sisutil/ErrLog.hpp>

const ACLRep* ACL::myACLRep = NULL;

void ACL::setRepository(const ACLRep& a) { myACLRep = &a; }

// static singleton
const ACLRep* ACL::aclRep() {
	if (!myACLRep)
		getTheErrLog().putLine("ACLRep", "aclRep",
			"no setRepository call made yet.");
	return myACLRep;
}

ACL* ACL::lookup(const std::string& key) {
	const ACLRep* theACLRep = aclRep();
	if (theACLRep) return theACLRep->lookup(key);
	return NULL;
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// fill-prefix: "// "
// End:
