#include "MessageSender.h"
#include "ProcessInfoHelper.h"
#include "NetworkDataTypes.h"
#include "MembershipList.h"

#include "../utils/TcpSender.h"
#include "../utils/Log.h"

void send_message(std::string hostname, std::string port, void *m, size_t m_size, bool crash_on_fail) {
	try {
		TcpSender sender = TcpSender(hostname, port);
		sender.send(m, m_size);
		sender.free_serve_info();
		sender.close_socket();
	} catch(std::string m) {
		Log::e("MessageSender:: failed to send message to " + hostname + " via tcp-> " + m);
		if (crash_on_fail) {
			send_message(hostname, port, &m, m_size, crash_on_fail);
		}
	}
}

void send_message(ProcessInfo pInfo, void *m, size_t m_size) {
	Log::v("MessageSender:: Trying to send message to -> " + pInfo.hostname + "at port-> " + pInfo.port);
	send_message(pInfo.hostname, pInfo.port, m, m_size);
}

void send_message(std::vector<uint32_t> peer_list, void *m, size_t m_size, bool crash_on_fail) {
	for (auto const &peer_id: peer_list) {
		ProcessInfo info = ProcessInfoHelper::get_process_info(peer_id);
		send_message(info.hostname, info.port, m, m_size, crash_on_fail);
	}
}

void send_leader_join() {
	Log::v("Sending join to leader....");
	JoinMessage m = JoinMessage();
	m.type = 1;
	m.process_id = ProcessInfoHelper::SELF.id;
	send_message(ProcessInfoHelper::LEADER, (void *) &m, sizeof m);
}

void send_to_all_members(void *m, size_t m_size) {
	send_message(MembershipList::get_instance().get_peer_list(), m, m_size);
}


