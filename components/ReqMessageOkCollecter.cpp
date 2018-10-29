#include "ReqMessageOkCollecter.h"
#include "../utils/Log.h"

ReqMessageOkCollecter::ReqMessageOkCollecter() {}

void ReqMessageOkCollecter::set_req_message(ReqMessage rm, uint32_t expected_ok_count, ReqMessageConfirmationHandler handler) {
	this->m = rm;
	this->expected_ok_count = expected_ok_count;
	this->handler = handler;
	this->count.store(0);
}

void ReqMessageOkCollecter::handle_ok_message(OkMessage om) {
	if (om.req_id != m.req_id && om.view_id == m.view_id) {
		Log::e("Wrong okmessage for request. Crashing program.");
		exit(1);
	}

	this->count.store(this->count.load() + 1);
	if (this->count.load() == expected_ok_count) {
		this->handler(this->m);
	}
}
