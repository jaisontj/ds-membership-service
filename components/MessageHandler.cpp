#include <vector>
#include<bitset>

#include "MessageHandler.h"
#include "MessageSender.h"
#include "NetworkDataTypes.h"
#include "ProcessInfoHelper.h"
#include "RequestIdProvider.h"
#include "MembershipList.h"
#include "ReqMessageOkCollecter.h"

#include "../utils/Log.h"

uint32_t convert_to_int_list(std::vector<uint32_t> peer_list) {
	std::bitset<32> p_list = 0;
	for (auto const &peer: peer_list) {
		p_list.flip(peer);
	}
	return static_cast<uint32_t>(p_list.to_ulong());
}

std::vector<uint32_t> convert_to_vector(uint32_t peer_list) {
	std::vector<uint32_t> p_list;
	for (int i = 0; i< 32; i++)
		if (std::bitset<32>(peer_list).test(i))
			p_list.push_back(i);
	return p_list;
}

void update_membership_and_send_to_all(ReqMessage req_m) {
	uint32_t new_peer_id = req_m.peer_id;
	MembershipList::get_instance().increment_view_id();
	MembershipList::get_instance().add_new_peer(new_peer_id);
	std::vector<uint32_t> peer_list = MembershipList::get_instance().get_peer_list();
	NewViewMessage nm = NewViewMessage();
	nm.type = 4;
	nm.view_id = MembershipList::get_instance().get_view_id();
	nm.peer_list = convert_to_int_list(peer_list);
	Log::i("Sending NEWVIEW message to all");
	send_message(peer_list, (void *) &nm, sizeof nm);
}

ReqMessageOkCollecter collecter;
void handle_ok_message(OkMessage om) {
	Log::i("Received OKMessage-> ReqId: " + std::to_string(om.req_id) + " ViewId: " + std::to_string(om.view_id));
	collecter.handle_ok_message(om);
}

void handle_join_request(JoinMessage jm) {
	uint32_t new_peer_id = jm.process_id;
	Log::i("Process " + std::to_string(new_peer_id) + " wants to join");
	ReqMessage req_m = ReqMessage();
	req_m.type = 2;
	req_m.req_id = RequestIdProvider::get_instance().get_new_req_id();
	req_m.view_id = MembershipList::get_instance().get_view_id();
	req_m.op_type = 1;
	req_m.peer_id = new_peer_id;
	if (MembershipList::get_instance().get_peer_list().size() == 0) {
		//First join.
		update_membership_and_send_to_all(req_m);
		return;
	}
	send_to_all_members((void *) &req_m, sizeof req_m);
	uint32_t size = MembershipList::get_instance().get_peer_list().size();
	collecter.set_req_message(req_m, size, update_membership_and_send_to_all);
}

ReqMessage *saved_req_message;

void handle_req_message(ReqMessage rm) {
	Log::i("Received ReqMessage-> ReqId: " + std::to_string(rm.req_id) + " ViewId: " + std::to_string(rm.view_id) + "OP Type: " + std::to_string(rm.op_type) + " PeerId: " + std::to_string(rm.peer_id));
	//save operation.
	if (saved_req_message != NULL) {
		Log::e("MessageHandler:: Cannot save more than one operation.");
		throw std::string("Cannot save more than one operation.");
	}
	saved_req_message = &rm;
	//Send OK to leader
	OkMessage m = OkMessage();
	m.type = 3;
	m.req_id = rm.req_id;
	m.view_id = rm.view_id;
	send_message(ProcessInfoHelper::LEADER, (void *) &m, sizeof m);
}

void handle_newview_message(NewViewMessage nvm) {
	//Update membership
	std::vector<uint32_t> peer_list = convert_to_vector(nvm.peer_list);
	MembershipList::get_instance().set_view_id(nvm.view_id);
	MembershipList::get_instance().set_peer_list(peer_list);
	MembershipList::get_instance().print();
	//Delete saved data
	saved_req_message = NULL;
}

void tcp_message_handler(void *message) {
	NetworkMessage *m = (NetworkMessage *) message;
	if (m->msg_type == 1) {
		if (!ProcessInfoHelper::is_leader()) {
			Log::e("MessageHandler:: Process who is NOT a leader is receiving a join");
			throw std::string("Process who is NOT a leader is receiving a join");
		}
		JoinMessage jm = *(JoinMessage *) message;
		handle_join_request(jm);
		return;
	}
	if (m->msg_type == 2) {
		ReqMessage rm = *(ReqMessage *) message;
		handle_req_message(rm);
		return;
	}
	if (m->msg_type == 3) {
		if (!ProcessInfoHelper::is_leader()) {
			Log::e("MessageHandler:: Process who is NOT a leader is receiving an OK message.");
			throw std::string("Process who is NOT a leader is receiving OK message.");
		}
		OkMessage om = *(OkMessage *) message;
		handle_ok_message(om);
		return;
	}
	if (m->msg_type == 4) {
		NewViewMessage nvm = *(NewViewMessage *) message;
		handle_newview_message(nvm);
		return;
	}
}
