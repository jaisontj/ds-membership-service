#include<iostream>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<thread>

#include "utils/Log.h"
#include "utils/TcpListener.h"
#include "utils/UdpListener.h"

#include "components/CommandArgParser.h"
#include "components/ProcessInfoHelper.h"
#include "components/MessageHandler.h"
#include "components/NetworkDataTypes.h"
#include "components/MessageSender.h"
#include "components/MembershipList.h"
#include "components/FailureDetector.h"

using namespace std;

LogLevel Log::LOG_LEVEL = NONE;
vector<ProcessInfo> ProcessInfoHelper::PROCESS_LIST;
ProcessInfo ProcessInfoHelper::SELF;
ProcessInfo ProcessInfoHelper::LEADER;
time_t FailureDetector::TIMEOUT = 2000;

void start_tcp_listener(CommandArgs c_args) {
	try {
		TcpListener listener = TcpListener(c_args.port);
		listener.start_listening(&tcp_message_handler, new NetworkMessage(), sizeof (NetworkMessage));
		listener.close_socket();
	} catch (string m) {
		Log::p("MAIN:: Error listening on tcp: " + m);
		exit(1);
	}
}

void handle_heartbeat_message(void *message) {
	FailureDetector::get_instance().handle_heartbeat(*(HeartBeat *) message);
}

void start_heartbeat_listener(CommandArgs c_args) {
	Log::i("Starting heartbeat listener");
	try {
		UdpListener listener = UdpListener(c_args.port);
		HeartBeat hb = HeartBeat();
		listener.start_listening(handle_heartbeat_message, (void *) &hb, sizeof(HeartBeat));
	} catch(string m) {
		Log::p("MAIN:: Heartbeat listener failed -> " + m);
		exit(1);
	}
}


int main(int argc, char* argv[]) {
	CommandArgs c_args = parse_cmg_args(argc, argv);
	ProcessInfoHelper::init_from_file(c_args.filename, c_args.port);
	thread tcp_listener(start_tcp_listener, c_args);
	ProcessInfo leader = ProcessInfoHelper::LEADER;
	Log::i("Leader is: " + leader.hostname + " port: " + leader.port);
	try {
		send_leader_join();
	} catch (std::string m) {
		Log::p("MAIN:: Could not send join to leader. Message-> " + m);
		exit(1);
	}
	thread udp_listener(start_heartbeat_listener, c_args);
	udp_listener.detach();
	FailureDetector::get_instance().start();
	tcp_listener.join();
	return 0;
}

