#include <vector>
#include <bitset>
#include <unistd.h>

#include "MessageHandler.h"
#include "MessageSender.h"
#include "NetworkDataTypes.h"
#include "ProcessInfoHelper.h"
#include "RequestIdProvider.h"
#include "MembershipList.h"
#include "ReqMessageOkCollecter.h"
#include "FailureDetector.h"
#include "NewLeaderResponseHandler.h"

#include "../utils/Log.h"

std::string should_simulate_test = "";

void simulate_test(std::string testcase) {
	should_simulate_test = testcase;
}

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

std::vector<uint32_t> remove_from_vector(std::vector<uint32_t> peer_list, uint32_t id) {
	for (uint32_t i = 0;i<peer_list.size();i++) {
		if (peer_list[i] == id) {
			peer_list.erase(peer_list.begin() + i);
			return peer_list;
		}
	}
	Log::f("MessageHandler:: Peer to be removed not found in list");
	exit(1);
}

void update_leader(uint32_t new_leader_id) {
	ProcessInfo leader_info = ProcessInfoHelper::get_process_info(new_leader_id);
	ProcessInfoHelper::LEADER = leader_info;
}

void update_membership_and_send_to_all(ReqMessage req_m) {
	uint32_t new_peer_id = req_m.peer_id;
	std::vector<uint32_t> peer_list = MembershipList::get_instance().get_peer_list();
	switch (req_m.op_type) {
		case OP_ADD: peer_list.push_back(new_peer_id);
					 break;
		case OP_DEL: peer_list = remove_from_vector(peer_list, new_peer_id);
					 break;
		default: Log::f("Unhandled op type for req.");
	}
	NewViewMessage nm = NewViewMessage();
	nm.type = NEW_VIEW;
	nm.view_id = MembershipList::get_instance().get_view_id() + 1;
	nm.peer_list = convert_to_int_list(peer_list);
	Log::i("Sending NEWVIEW message to all");
	send_message(peer_list, (void *) &nm, sizeof nm);
}

ReqMessageOkCollecter collecter;
void handle_ok_message(OkMessage om) {
	Log::i("Received OKMessage-> ReqId: " + std::to_string(om.req_id) + " ViewId: " + std::to_string(om.view_id));
	collecter.handle_ok_message(om);
}

void initiate_2pc_add_peer(uint32_t new_peer_id) {
	ReqMessage req_m = ReqMessage();
	req_m.type = REQUEST;
	req_m.req_id = RequestIdProvider::get_instance().get_new_req_id();
	req_m.view_id = MembershipList::get_instance().get_view_id();
	req_m.op_type = OP_ADD;
	req_m.peer_id = new_peer_id;
	size_t membership_size = MembershipList::get_instance().get_peer_list().size();
	if (membership_size == 0) {
		//First join.
		update_membership_and_send_to_all(req_m);
		return;
	}
	//simulate test case if specified
	if (should_simulate_test == "4") {
		//Crash when trying to add 6th peer.
		if (membership_size == 5) {
			//crash after sending to everyone except next leader
			try {
				std::vector<uint32_t> alive_peers = remove_from_vector(MembershipList::get_instance().get_peer_list(), MembershipList::get_instance().get_next_leader_id());
				send_message(alive_peers, (void *) &req_m, sizeof req_m);
				sleep(2);
				Log::f("Executing Test case 4: Crashing leader after sending ADD message to all except next leader, on adding the 6th peer.");
			} catch (std::string m) {
				Log::f("MessageHandler: Failed executing test case 4");
			}
		}
	}
	send_to_all_members((void *) &req_m, sizeof req_m);
	uint32_t size = MembershipList::get_instance().get_peer_list().size();
	collecter.set_req_message(req_m, size, update_membership_and_send_to_all);
}

void initiate_2pc_remove_peer(uint32_t peer_id) {
	ReqMessage m = ReqMessage();
	m.type = REQUEST;
	m.req_id = RequestIdProvider::get_instance().get_new_req_id();
	m.view_id = MembershipList::get_instance().get_view_id();
	m.op_type = OP_DEL;
	m.peer_id = peer_id;
	std::vector<uint32_t> alive_peers = remove_from_vector(MembershipList::get_instance().get_peer_list(), peer_id);
	size_t membership_size = alive_peers.size();
	if (membership_size == 0) {
		Log::f("Trying to remove peer from an empty membership.");
	}
	//simulate test case if specified
	if (should_simulate_test == "4") {
		//crash after sending to everyone except next leader
		try {
			alive_peers = remove_from_vector(alive_peers, MembershipList::get_instance().get_next_leader_id());
			send_message(alive_peers, (void *) &m, sizeof m);
			sleep(2);
			Log::f("Executing Test case 4: Crashing leader after sending DEL message to all peers except next leader");
		} catch (std::string m) {
			Log::f("MessageHandler: Failed executing test case 4");
		}
	}
	send_message(alive_peers, (void *) &m, sizeof m);
	collecter.set_req_message(m, membership_size, update_membership_and_send_to_all);
}


void handle_join_request(JoinMessage jm) {
	uint32_t new_peer_id = jm.process_id;
	Log::i("Process " + std::to_string(new_peer_id) + " wants to join");
	initiate_2pc_add_peer(new_peer_id);
}

NewLeaderResponseHandler nlr_handler;
void handle_newleader_response(ReqMessage message) {
	nlr_handler.handle_response(message);
}

void handle_pending_op(ReqMessage m) {
	if (m.op_type == OP_ADD) {
		initiate_2pc_add_peer(m.peer_id);
	} else if (m.op_type == OP_DEL) {
		initiate_2pc_remove_peer(m.peer_id);
	}
}

void handle_leader_failure(uint32_t leader_id) {
	Log::i("Detected leader failure");
	//check if next in line to be leader
	//Update leader
	uint32_t next_leader_id = MembershipList::get_instance().get_next_leader_id();
	update_leader(next_leader_id);
	Log::i("Updated leader: New Leader-> " + std::to_string(next_leader_id));
	if (ProcessInfoHelper::SELF.id != next_leader_id){
		Log::i("Not next in line to be leader :(.");
		return;
	}
	//Handle if next in line to be leader
	Log::p("Assuming leadership!!!");
	//Delete previous leader from membership
	MembershipList::get_instance().remove_peer(leader_id);
	//send NEWLEADER to all except crashed leader.
	NewLeaderMessage m = NewLeaderMessage();
	m.type = NEW_LEADER;
	m.req_id = RequestIdProvider::get_instance().get_new_req_id();
	m.view_id = MembershipList::get_instance().get_view_id();
	m.op_type = OP_PENDING;
	std::vector<uint32_t> alive_peers = MembershipList::get_instance().get_peer_list();
	//send message to all alive, but do not crash on fail because someother process also may have crashed.
	nlr_handler.setup(m.req_id, &handle_pending_op);
	send_message(alive_peers, (void *) &m, sizeof m, false);
}

void handle_peer_crash(uint32_t peer_id) {
	Log::p("Peer " + std::to_string(peer_id) + " not reachable.");
	//check if crashed peer is leader
	if (ProcessInfoHelper::LEADER.id == peer_id) {
		handle_leader_failure(peer_id);
		return;
	}
	if (!ProcessInfoHelper::is_leader()) {
		Log::p("Expecting leader to handle.");
		return;
	}
	//Only handle if leader
	initiate_2pc_remove_peer(peer_id);
}

ReqMessage saved_req_message;

void handle_req_message(ReqMessage rm) {
	Log::i("Received ReqMessage-> ReqId: " + std::to_string(rm.req_id) + " ViewId: " + std::to_string(rm.view_id) + "OP Type: " + std::to_string(rm.op_type) + " PeerId: " + std::to_string(rm.peer_id));
	//save operation.
	if (saved_req_message.type != 0) {
		Log::f("MessageHandler:: Cannot save more than one operation.");
	}
	saved_req_message = rm;
	//Send OK to leader
	OkMessage m = OkMessage();
	m.type = OK;
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
	saved_req_message.type = 0;
	//Add members to failure detector
	FailureDetector::get_instance().replace_peers(peer_list);
}

void handle_newleader_message(NewLeaderMessage nlm) {
	if (nlm.op_type != OP_PENDING) {
		Log::f("MessageHandler:: The only type of NewLeaderMessage that is handled is PENDING. Received something else.");
	}
	//Check if pending request exists
	ReqMessage m = ReqMessage();
	m.type = NEW_LEADER_RESPONSE;
	m.req_id = nlm.req_id;
	m.view_id = MembershipList::get_instance().get_view_id();
	//Send NOTHING back by default
	m.op_type = OP_NOTHING;
	if (saved_req_message.type != 0) {
		m.op_type = saved_req_message.op_type;
		m.peer_id = saved_req_message.peer_id;
		//Delegated to leader. Expected to happen again.
		saved_req_message.type = 0;
	}
	//send response to leader.
	send_message(ProcessInfoHelper::LEADER, (void *) &m, sizeof m);
}

void tcp_message_handler(void *message) {
	NetworkMessage *m = (NetworkMessage *) message;
	if (m->msg_type == JOIN) {
		if (!ProcessInfoHelper::is_leader()) {
			Log::f("MessageHandler:: Process who is NOT a leader is receiving a join");
		}
		JoinMessage jm = *(JoinMessage *) message;
		handle_join_request(jm);
		return;
	}
	if (m->msg_type == REQUEST) {
		ReqMessage rm = *(ReqMessage *) message;
		handle_req_message(rm);
		return;
	}
	if (m->msg_type == OK) {
		if (!ProcessInfoHelper::is_leader()) {
			Log::f("MessageHandler:: Process who is NOT a leader is receiving an OK message.");
		}
		OkMessage om = *(OkMessage *) message;
		handle_ok_message(om);
		return;
	}
	if (m->msg_type == NEW_VIEW) {
		NewViewMessage nvm = *(NewViewMessage *) message;
		handle_newview_message(nvm);
		return;
	}
	if (m->msg_type == NEW_LEADER) {
		NewLeaderMessage nlm = *(NewLeaderMessage *) message;
		handle_newleader_message(nlm);
		return;
	}
	if (m->msg_type == NEW_LEADER_RESPONSE) {
		ReqMessage rm = *(ReqMessage *) message;
		handle_newleader_response(rm);
		return;
	}
}
