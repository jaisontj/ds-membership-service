#include "MembershipList.h"

#include <iostream>

MembershipList::MembershipList() {}

MembershipList& MembershipList::get_instance() {
	static MembershipList instance;
	return	instance;
}

void MembershipList::increment_view_id() {
	this->view_id.store(this->view_id.load() + 1);
}

void MembershipList::add_new_peer(uint32_t p_id) {
	std::lock_guard<std::mutex> lk(m);
	this->peers.push_back(p_id);
}

void MembershipList::remove_peer(uint32_t p_id) {
	std::lock_guard<std::mutex> lk(m);
	for (uint32_t i = 0; i < this->peers.size(); i++) {
		if (peers[i] == p_id) {
			this->peers.erase(this->peers.begin() + i);
			break;
		}
	}
}

std::vector<uint32_t> MembershipList::get_peer_list() {
	std::lock_guard<std::mutex> lk(m);
	return peers;
}

void MembershipList::set_peer_list(std::vector<uint32_t> p_list) {
	std::lock_guard<std::mutex> lk(m);
	this->peers = p_list;
}

void MembershipList::set_view_id(uint32_t v_id) {
	this->view_id.store(v_id);
}

uint32_t MembershipList::get_view_id() {
	return this->view_id.load();
}

void MembershipList::print() {
	std::cout<<"-----------------------------------------------------------------------------------"<<std::endl;
	std::cout<<"ViewId: "<<this->view_id.load()<<std::endl;
	std::cout<<"Membership List:"<<std::endl;
	for (auto const &peer: this->peers) {
		std::cout<<"Peer Id: "<<peer<<std::endl;
	}
	std::cout<<"-----------------------------------------------------------------------------------"<<std::endl;
}

uint32_t MembershipList::get_next_leader_id() {
	std::vector<uint32_t> p_list = this->get_peer_list();
	if (p_list.size() == 1) {
		throw std::string("There can be no next leader");
	}
	return p_list[1];
}

