#ifndef FAILURE_DETECTOR
#define FAILURE_DETECTOR

#include<iostream>
#include<mutex>
#include<vector>
#include "ProcessInfoHelper.h"

struct HeartBeat {
	uint32_t type;
	uint32_t id;
};

struct PeerHBTrack{
	ProcessInfo info;
	time_t next_hb_at;
};

typedef void (*FailedPeerHandler)(uint32_t peer_id);

class FailureDetector {
	private:
		std::mutex m;
		std::vector<PeerHBTrack> peer_list;
		FailedPeerHandler handler;
		FailureDetector();
		void start_heartbeat();
		void start_detection();
		PeerHBTrack* get_failed_peer();
		void send_heartbeat_to_all(HeartBeat hb);
		void print_failure(uint32_t peer_id);
	public:
		static time_t TIMEOUT;
		static FailureDetector& get_instance();
		FailureDetector(FailureDetector const&) = delete;
		void operator=(FailureDetector const&) = delete;
		void add_peer(ProcessInfo p_info);
		void replace_peers(std::vector<uint32_t> peers);
		void remove_peer(ProcessInfo p_info);
		void handle_heartbeat(HeartBeat hb);
		void start(FailedPeerHandler handler);
};

#endif
