#include "ProcessInfoHelper.h"

#include <fstream>
#include <vector>
#include <unistd.h>

void ProcessInfoHelper::init_from_file(std::string filepath, std::string port) {
	char name[255];

	if (gethostname(name, 255) != 0) {
		perror("Unable to get hostname");
		exit(0);
	}

	std::string line_content;
	uint32_t line_num = 0;
	std::vector<ProcessInfo> file_content;
	std::ifstream file (filepath);
	if (file.is_open()) {
		while(getline(file, line_content)) {
			ProcessInfo pi = {
				.hostname = line_content,
				.port = port,
				.id = ++line_num
			};
			if (line_num == 1) ProcessInfoHelper::LEADER = pi;
			if (line_content == name) {
				ProcessInfoHelper::SELF = pi;
			}
			file_content.push_back(pi);
		}
		file.close();
	} else {
		std::cout<<"Unable to open file: "<<filepath<<std::endl;
		exit(0);
	}

	if (ProcessInfoHelper::SELF.id == 0) {
		std::cout<<"Unable to find hostname in provided file at "<<filepath<<". Please ensure that the hostname is provided in file."<<std::endl;
		exit(0);
	}

	ProcessInfoHelper::PROCESS_LIST = file_content;
}

ProcessInfo ProcessInfoHelper::get_process_info(uint32_t id) {
	for (auto const &process: ProcessInfoHelper::PROCESS_LIST) {
		if (process.id == id) return process;
	}
	throw std::string("Could not find process");
}

bool ProcessInfoHelper::is_leader() {
	return SELF.id == 1;
}

