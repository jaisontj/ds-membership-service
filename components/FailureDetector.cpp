#include "FailureDetector.h"
#include "../utils/UdpSender.h"
#include "../utils/UdpListener.h"
#include "../utils/Log.h"
#include "ProcessInfoHelper.h"

#include <thread>
#include <unistd.h>

FailureDetector::FailureDetector() {}

FailureDetector& FailureDetector::get_instance() {
	static FailureDetector instance;
	return instance;
}

void FailureDetector::send_heartbeat_to_all(HeartBeat hb) {
	std::lock_guard<std::mutex> lk(m);
	for (auto const &p : peer_list) {
		UdpSender sender = UdpSender(p.info.hostname, p.info.port);
		int sent_bytes = sender.send((void *) &hb, sizeof hb);
		if (sent_bytes == -1) {
			Log::e("Failed to send hearbeat to " + p.info.hostname);
		} else {
			Log::v("Sent heartbeat to " + p.info.hostname);
		}
	}
}

void FailureDetector::print_failure(uint32_t peer_id) {
	Log::p("Peer " + std::to_string(peer_id) + " not reachable");
}

void FailureDetector::add_peer(ProcessInfo p_info) {
	std::lock_guard<std::mutex> lk(m);
	time_t f = time(NULL) + 2*FailureDetector::TIMEOUT;
	this->peer_list.push_back({
			.info = p_info,
			.next_hb_at = f
			});
}

void FailureDetector::replace_peers(std::vector<uint32_t> peers) {
	std::lock_guard<std::mutex> lk(m);
	peer_list.clear();
	time_t f = time(NULL) + 2 * FailureDetector::TIMEOUT;
	for (auto const &p_id: peers) {
		ProcessInfo p_info = ProcessInfoHelper::get_process_info(p_id);
		peer_list.push_back({
				.info = p_info,
				.next_hb_at = f
				});
	}
}

void FailureDetector::remove_peer(ProcessInfo p_info) {
	std::lock_guard<std::mutex> lk(m);
	uint32_t initial_size = peer_list.size();
	for (uint32_t i = 0; i<peer_list.size(); i++) {
		ProcessInfo peer = peer_list[i].info;
		if (peer.id == p_info.id) {
			peer_list.erase(peer_list.begin() + i);
			break;
		}
	}
	if (initial_size == peer_list.size()) {
		throw std::string("No such peer exists to remove.");
	}
}

void FailureDetector::handle_heartbeat(HeartBeat hb) {
	ProcessInfo p = ProcessInfoHelper::get_process_info(hb.id);
	remove_peer(p);
	add_peer(p);
}

PeerHBTrack* FailureDetector::get_failed_peer() {
	std::lock_guard<std::mutex> lk(m);
	time_t now = time(NULL);
	for (auto const &p: peer_list) {
		if (p.next_hb_at < now) {
			return (PeerHBTrack *) &p;
		}
	}
	return NULL;
}

void FailureDetector::start_heartbeat() {
	HeartBeat hb = { .type = 1, .id = ProcessInfoHelper::SELF.id };
	while (true) {
		send_heartbeat_to_all(hb);
		sleep(TIMEOUT);
	}
}

void FailureDetector::start_detection() {
	while(true) {
		PeerHBTrack* failed_peer = get_failed_peer();
		if (failed_peer != NULL) {
			print_failure(failed_peer->info.id);
			remove_peer(failed_peer->info);
		}
	}
}

void FailureDetector::start() {
	//start sending heartbeats
	std::thread t1(&FailureDetector::start_heartbeat, this);
	t1.detach();
	//start detecting failures
	std::thread t2(&FailureDetector::start_detection, this);
	t2.detach();
}

