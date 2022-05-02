// Uuid.cpp
// $Id: Uuid.cpp,v 1.19 1999/11/09 03:23:43 ed Exp $
// stripped out DCE dependencies and used Linux libuuid

//#include <dce/id_base.h>
//#include <dce/secidmap.h>
#include <sisutil/ErrLog.hpp>
#include <sisutil/DCEError.hpp>
//#include <GMLock.hpp>
#include <sisutil/Uuid.hpp>
#include <uuid/uuid.h>

//SecRgyBindingHandle* Uuid::mySecRgyContext = NULL;

Uuid::Uuid() {
	//unsigned32 ignoredStatus;
	//uuid_create_nil(&myId, &ignoredStatus);
	uuid_clear(myId);
}

Uuid::Uuid(const std::string& s) {
	uuid_clear(myId); // need this ??? <-- maybe, s might be an empty string so this is a good default
	/*
	unsigned32 status;
	uuid_create_nil(&myId, &status);
	// uuid_from_string lacks const for input string, hence cast away const
	uuid_from_string((idl_char*)s.c_str(), &myId, &status);
	if (status != rpc_s_ok)
		getTheErrLog().logErrMsg("Uuid", "Uuid(const C&)", "uuid_from_string", status);
	*/
	if (s.compare("") != 0) { // s is not empty so we might have a real uuid
		int stat = uuid_parse(s.c_str(), myId);
		if (stat != 0) {
			//std::cerr << "Uuid::Uuid(const string&) error: input string is: " << s.c_str() << std::endl;
			getTheErrLog().logErrMsg("Uuid", "Uuid(const string&)", "uuid_parse error", stat);
		}
	}
}

int Uuid::compare(const Uuid& rhs) const {
	unsigned32 status = 0; // = ~rpc_s_ok;
	 /*
	 const uuid_t& rhsRep = rhs;
	 // uuid_compare lacks const for input uuid_t's, hence cast away const
	int retVal = uuid_compare((uuid_t*)&myId, (uuid_t*)&rhsRep, &status);
	 //cerr << "myId=" << ((uuid_t&)myId).time_low << ", rhs=" << ((uuid_t&)rhsRep).time_low << endl;
	 if (status != rpc_s_ok)
		 getTheErrLog().logErrMsg("Uuid", "compare", "uuid_compare", status);
	 */

	int retVal = uuid_compare(myId, rhs);
	if (retVal != 0)
		getTheErrLog().logErrMsg("Uuid", "compare", "uuid_compare", status);

	return retVal;
}

// modify arg to return string containing string form and return
// a success DCEError, or modify arg to contain "", and return non-success
DCEError
Uuid::toString(std::string& s) const {
	//unsigned_char_t* stringUUID;
	char stringUUID[37];
	unsigned32 status = 0;
	//s.erase();
	/*
	// uuid_to_string lacks const for input uuit_t, hence cast away const
	uuid_to_string((uuid_t*)&myId, &stringUUID, &status);
	if (status != rpc_s_ok)
		getTheErrLog().logErrMsg("Uuid", "toString", "uuid_to_string", status);
	if (status == rpc_s_ok) {
		s = (const char*)stringUUID;
		rpc_string_free(&stringUUID, &status);
	}
	*/
	// TODO: check for a valid uuid before passing to uuid_unparse() and return an error
	uuid_unparse(myId, stringUUID);
	s = stringUUID;
	return status;  // dummy return
}

/*
const SecRgyBindingHandle&
Uuid::getSecRgyBH(const SecRgyBindingHandle* sbh) const {
	ErrLog& theErrLog = getTheErrLog();
	// use sbh (if supplied) in preference to mySecRgyContext
  if (!sbh) {										// shared SecRgyBindingHandle
	GlobalMutexLock serializeThisScope;
		if (!mySecRgyContext){
			if (mySecRgyContext = new SecRgyBindingHandle())
				theErrLog.logErrMsg("Uuid", "getSecRgyBH", "SecRgyBindingHandle()",
														mySecRgyContext->lastError());
		}
		else if (!mySecRgyContext->isValid()) {
			theErrLog.logErrMsg("Uuid", "getSecRgyBH", "isValid()",	mySecRgyContext->lastError());
			DCEError bindResult = mySecRgyContext->bind();
			theErrLog.logErrMsg("Uuid", "getSecRgyBH", "bind()", bindResult);
		}

		return *mySecRgyContext;
	} else return *sbh;
}
*/

/*
// modify arg to return string containing Security Registry
// principal name form and return a success DCEError, or modify
// arg to contain "", and return non-success
DCEError
Uuid::toPrincipalName(std::string& s, const SecRgyBindingHandle* sbh) const {
	sec_rgy_name_t principalName;
	unsigned32 status;
	const SecRgyBindingHandle& theSecRgyBH = getSecRgyBH(sbh);

	s.erase();

	sec_id_gen_name(theSecRgyBH,
									// assume no foreign uuids
									// cast away const (this is an input only arg)
									(uuid_t*)&theSecRgyBH.theCellUuid(),
									(Uuid&) *this, // cast away const (this is an only input arg)
									NULL,
									NULL,
									principalName,
									&status);
	if (status == rpc_s_ok) s = (const char*)principalName;
	return status;
}
*/

// modify arg to return string containing Security Registry
// principal name form and return a success DCEError, or modify
// arg to contain "", and return non-success
/*
DCEError
Uuid::toGroupName(string& s, const SecRgyBindingHandle* sbh) const {
	sec_rgy_name_t principalName;
	unsigned32 status;
	const SecRgyBindingHandle& theSecRgyBH = getSecRgyBH(sbh);

	s.erase();

	sec_id_gen_group(theSecRgyBH,
									 // cast away const (this is an only input arg)
									 (uuid_t*)&theSecRgyBH.theCellUuid(), // no foreign uuids
									 (Uuid&) *this,// cast away const (this is an only input arg)
									 NULL,
									 NULL,
									 principalName,
									 &status);
	if (status == rpc_s_ok) s = (const char*)principalName;
	return status;
}
*/

/*
DCEError Uuid::bindRgy() {
	return mySecRgyContext->bind();
}
*/
