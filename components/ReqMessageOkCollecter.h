#ifndef REQ_MESSAGE_OK_COLLECTER
#define REQ_MESSAGE_OK_COLLECTER

#include <atomic>
#include "NetworkDataTypes.h"

typedef void (*ReqMessageConfirmationHandler)(ReqMessage m);
class ReqMessageOkCollecter {
	private:
		std::atomic<uint32_t> count;
		ReqMessage m;
		uint32_t expected_ok_count;
		ReqMessageConfirmationHandler handler;
	public:
		ReqMessageOkCollecter();
		void set_req_message(ReqMessage m, uint32_t expected_ok_count, ReqMessageConfirmationHandler handler);
		void handle_ok_message(OkMessage om);
};

#endif
