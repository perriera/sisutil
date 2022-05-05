// InCoreACLRep.C - in-core RPC ACL repository 
// $Id: InCoreACLRep.cpp,v 1.16 1999/04/24 00:36:06 ed Exp $

#include <sisutil/InCoreACLRep.hpp>
#include <sisutil/ACL.hpp>
#include <sisutil/ErrLog.hpp>
#include <iostream>

InCoreACLRep::InCoreACLRep(const InCoreACLRep::ACLEntry*& contents) {
	unsigned int index = 0;
	// contents array must be terminated with a "" element
	while (contents[index].RPCName[0]) {
		// XXX TODO should return from insert be interpreted?
		myACLMap[contents[index].RPCName].insert(Uuid(contents[index].uuid));
		index++;
	}
}

InCoreACLRep::InCoreACLRep(const std::string& fileName) {
	std::ifstream aclSource(fileName.c_str());
	if (!aclSource)
		getTheErrLog().logErrMsg("InCoreACLRep", "InCoreACLRep(const string&)",
			"ifstream", std::string("cannot open file \""
				+ fileName + "\""));
	while (aclSource) {
#if ( defined (__xlC__) && (__xlC__ < 0x400) )
		// XXX TODO: get string::op>>(istream&)
		// istream_iterator<string>(aclSource) 
		char thisRPCNameBuf[1024], thisUuidStrBuf[1024];
		aclSource >> thisRPCNameBuf >> thisUuidStrBuf;
		const std::string thisRPCName = thisRPCNameBuf;
		const std::string thisUuidStr = thisUuidStrBuf;
#else
		std::string thisRPCName;
		std::string thisUuidStr;
		aclSource >> thisRPCName >> thisUuidStr;
#endif
		if (aclSource.fail() && !aclSource.eof())
			getTheErrLog().logErrMsg("InCoreACLRep", "InCoreACLRep(const string&)",
				"ifstream::op>>", std::string("error reading file \""
					+ fileName + "\""));
		if (thisRPCName.length() && thisUuidStr.length())
			myACLMap[thisRPCName].insert(Uuid(thisUuidStr));
	}
}

ACL*
InCoreACLRep::lookup(const std::string& key) const {
	ACL* theACL = NULL;
	const  ACLMap::const_iterator foundACL = myACLMap.find(key);
	if (foundACL != myACLMap.end()) { // an matching ACL exists
		const ACL::UuidSet& allowedUuids = (*foundACL).second;
		for (ACL::UuidSet::const_iterator i = allowedUuids.begin();
			i != allowedUuids.end(); ++i) {
			const Uuid& thisUuid = *i;     // copy each Uuid into return value
			if (!theACL) theACL = new ACL;
			theACL->addGroup(thisUuid);
		}
	}
	return theACL;
}

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 3
// tab-stop-list: (3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57) // fill-column: 70
// fill-column: 70
// End:
