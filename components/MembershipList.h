#ifndef MEMBERSHIP_LIST
#define MEMBERSHIP_LIST

#include <atomic>
#include <vector>
#include <mutex>

class MembershipList {
	private:
		MembershipList();
		std::atomic<uint32_t> view_id;
		std::vector<uint32_t> peers;
		std::mutex m;
	public:
		static MembershipList& get_instance();
		MembershipList(MembershipList const&) = delete;
		void operator=(MembershipList const&) = delete;
		void increment_view_id();
		void add_new_peer(uint32_t peer_id);
		void set_view_id(uint32_t view_id);
		void set_peer_list(std::vector<uint32_t> peer_list);
		uint32_t get_view_id();
		std::vector<uint32_t> get_peer_list();
		void print();
		uint32_t get_next_leader_id();
};

#endif
