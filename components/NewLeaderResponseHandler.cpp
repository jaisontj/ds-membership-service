#include "NewLeaderResponseHandler.h"
#include "../utils/Log.h"

void NewLeaderResponseHandler::setup(uint32_t req_id, PendingOpsHandler handler) {
	this->req_id = req_id;
	this->handler = handler;
}

void NewLeaderResponseHandler::handle_response(ReqMessage message) {
	if (message.op_type == OP_PENDING) {
		Log::i("NewLeaderResponseHandler:: Received PENDING in NEW_LEADER_RESPONSE. Weird. Should not happen. Ignoring..");
		return;
	}
	if (message.req_id != this->req_id) {
		Log::i("NewLeaderResponseHandler:: Received NEW_LEADER_RESPONSE with different Req_id. Ignoring this message.");
		return;
	}
	std::string m = "Type: " + std::to_string(message.type) + " Req_id: " + std::to_string(message.req_id) + " View_id: " + std::to_string(message.view_id) + " OP_Type: " + std::to_string(message.op_type) + " Peer_id: " + std::to_string(message.peer_id);
	Log::i("Received NewLeaderResponse: " + m);
	//if op type is NOTHING, ignore
	if (message.op_type == OP_NOTHING) return;
	bool copy_exists = false;
	//if op has already been handled ignore.
	for (auto const &op: pending_ops) {
		if (op.op_type == message.op_type && op.peer_id == message.peer_id) {
			//If op type and peer id is same, deemed equal. Ignoring view id.
			copy_exists = true;
		}
	}
	if (!copy_exists) {
		this->pending_ops.push_back(message);
		this->handler(message);
	}
}
