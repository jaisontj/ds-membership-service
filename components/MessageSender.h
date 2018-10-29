#ifndef MESSAGE_SENDER
#define MESSAGE_SENDER

#include "ProcessInfoHelper.h"
#include <string>
#include <vector>

void send_leader_join();
void send_message(std::string hostname, std::string port, void *m, size_t m_size, bool crash_on_fail = true);
void send_message(ProcessInfo pInfo, void *m, size_t m_size);
void send_message(std::vector<uint32_t> peer_list, void *m, size_t m_size, bool crash_on_fail = true);
void send_to_all_members(void *m, size_t m_size);

#endif
