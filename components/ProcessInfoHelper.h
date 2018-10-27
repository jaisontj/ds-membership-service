#ifndef PROCESS_INFO_HELPER_H
#define PROCESS_INFO_HELPER_H

#include <iostream>
#include <vector>
#include <string>

struct ProcessInfo {
	std::string hostname;
	std::string port;
	uint32_t id;
};

class ProcessInfoHelper {
	public:
		static std::vector<ProcessInfo> PROCESS_LIST;
		static ProcessInfo SELF;
		static ProcessInfo LEADER;
		static void init_from_file(std::string filename, std::string port);
		static ProcessInfo get_process_info(uint32_t id);
		static bool is_leader();
};

#endif
