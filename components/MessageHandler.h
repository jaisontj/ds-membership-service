#ifndef MESSAGE_HANDLER
#define MESSAGE_HANDLER

#include <iostream>

void tcp_message_handler(void *message);
void handle_peer_crash(uint32_t peer_id);
void simulate_test(std::string testcase);

#endif
