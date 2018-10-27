#include<iostream>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<thread>

#include "utils/Log.h"
#include "utils/TcpListener.h"
#include "utils/TcpSender.h"

#include "components/CommandArgParser.h"
#include "components/ProcessInfoHelper.h"
#include "components/MessageHandler.h"
#include "components/NetworkDataTypes.h"
#include "components/MessageSender.h"
#include "components/MembershipList.h"

using namespace std;

LogLevel Log::LOG_LEVEL = NONE;
vector<ProcessInfo> ProcessInfoHelper::PROCESS_LIST;
ProcessInfo ProcessInfoHelper::SELF;
ProcessInfo ProcessInfoHelper::LEADER;

void start_tcp_listener(CommandArgs c_args) {
	try {
		TcpListener listener = TcpListener(c_args.port);
		listener.start_listening(&tcp_message_handler, new NetworkMessage(), sizeof (NetworkMessage));
		listener.close_socket();
	} catch (string m) {
		Log::e("MAIN:: Error listening on tcp: " + m);
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
		Log::e("MAIN:: Could not send join to leader. Message-> " + m);
	}
	tcp_listener.join();
	return 0;
}

