#ifndef COMMAND_ARG_PARSER
#define COMMAND_ARG_PARSER

#include <string>

struct CommandArgs {
	std::string port;
	std::string filename;
	std::string testcase;
};

CommandArgs parse_cmg_args(int, char* []);
void show_usage_and_exit();

#endif
