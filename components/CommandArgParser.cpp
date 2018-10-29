#include "CommandArgParser.h"

#include <iostream>
#include <unistd.h>
#include <cstring>

#include "../utils/Log.h"

void show_usage_and_exit() {
	Log::f("Usage: -p port -h hostfile -c count");
}

CommandArgs parse_cmg_args(int argc, char* argv[]) {
	int opt, temp_port;
	std::string port = "";
	std::string filepath;
	std::string test = "";
	while((opt = getopt(argc, argv, "p:h:v:t:")) != -1) {
		switch(opt) {
			case 'p':
				temp_port = atoi(optarg);
				if (temp_port < 1024 || temp_port > 65535) {
					std::cout<<"The value for port (p) must be a valid number between 1024 and 65535"<<std::endl;
					exit(0);
				}
				port = optarg;
				break;
			case 'h':
				filepath = optarg;
				break;
			case 'v':
				if (strcmp(optarg, "debug") == 0) Log::LOG_LEVEL = DEBUG;
				if (strcmp(optarg, "verbose") == 0) Log::LOG_LEVEL = VERBOSE;
				if (strcmp(optarg, "error") == 0) Log::LOG_LEVEL = ERROR;
				if (strcmp(optarg, "info") == 0) Log::LOG_LEVEL = INFO;
				break;
			case 't':
				test = optarg;
				break;
			default:
				show_usage_and_exit();
				break;
		}
	}

	if(port == "" || filepath == "")
		show_usage_and_exit();
	return (CommandArgs) { port, filepath, test };
}


