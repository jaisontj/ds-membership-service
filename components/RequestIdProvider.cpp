#include "RequestIdProvider.h"

RequestIdProvider::RequestIdProvider() {
	this->id.store(0);
}

RequestIdProvider& RequestIdProvider::get_instance() {
	static RequestIdProvider instance;
	return instance;
}

int RequestIdProvider::get_req_id() {
	return this->id.load();
}

int RequestIdProvider::get_new_req_id() {
	this->id.store(this->id.load() + 1);
	return this->id.load();
}


