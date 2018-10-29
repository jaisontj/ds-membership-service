#ifndef NEW_LEADER_RESPONSE_HANDLER
#define NEW_LEADER_RESPONSE_HANDLER

#include "NetworkDataTypes.h"
#include <atomic>
#include <iostream>
#include <vector>

typedef void (*PendingOpsHandler)(ReqMessage m);
class NewLeaderResponseHandler {
	private:
		uint32_t req_id;
		std::vector<ReqMessage> pending_ops;
		PendingOpsHandler handler;
	public:
		void setup(uint32_t req_id, PendingOpsHandler handler);
		void handle_response(ReqMessage message);
};

#endif
