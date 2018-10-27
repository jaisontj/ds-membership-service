#ifndef REQUEST_ID_PROVIDER
#define REQUEST_ID_PROVIDER

#include <atomic>

class RequestIdProvider {
	private:
		RequestIdProvider();
		std::atomic<int> id;
	public:
		RequestIdProvider(RequestIdProvider const&) = delete;
		void operator=(RequestIdProvider const&) = delete;
		static RequestIdProvider& get_instance();
		int get_req_id();
		int get_new_req_id();
};

#endif
