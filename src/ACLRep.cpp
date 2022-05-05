// ACLRep.C - DCE Access control list repository ABC
// $Id: ACLRep.cpp,v 1.4 1999/01/14 22:26:03 ed Exp $

#include <sisutil/ACLRep.hpp>
//#include <dce/rpc.h>

// see dce/rpcbase.idl
const char* ACLRep::RPCMgmtFnMap[] = {
	"rpc_mgmt_inq_if_ids",
	"rpc_mgmt_inq_stats",
	"rpc_mgmt_is_server_listening",
	"rpc_mgmt_stop_server_listening",
	"rpc_mgmt_inq_server_princ_name" };

// Return ACL for RPC management given function, or 0 if none exists.
// Caller must delete return value. used by rpc_mgmt_authorization_fn_t see
// rpc_mgmt_set_authorization_fn(3rpc)

ACL* ACLRep::lookupRPCMgmtFn(const unsigned32) const {
	//if ((requestedMgmtOperation < 0) || 
	//		(requestedMgmtOperation > rpc_c_mgmt_stop_server_listen))
	//	return NULL;
	//return lookup(RPCMgmtFnMap[requestedMgmtOperation]);
	return NULL;
}

// see dce/rpcbase.idl
//        rpc_mgmt_inq_if_ids()              rpc_c_mgmt_inq_if_ids
//        rpc_mgmt_inq_server_princ_name()   rpc_c_mgmt_inq_princ_name
//        rpc_mgmt_inq_stats()               rpc_c_mgmt_inq_stats
//        rpc_mgmt_is_server_listening()     rpc_c_mgmt_is_server_listen
//        rpc_mgmt_stop_server_listening()   rpc_c_mgmt_stop_server_listen

// Emacs related attributes only from here on
// Local Variables:
// mode: c++
// tab-width: 2
// tab-stop-list: (2 4 6 8 10 12 14 16 18 20 22 24 26)
// fill-column: 70
// End:

