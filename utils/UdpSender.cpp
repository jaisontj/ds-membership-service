#include<netdb.h>
#include<string>
#include<iostream>

#include"UdpSender.h"
#include "Log.h"

UdpSender::UdpSender(std::string hostname, std::string port): SocketImpl(SOCK_DGRAM, -1, hostname, port, 1, false) {
	Log::v("Created Sender Socket: Hostname->" + hostname + " Port->" + port);
}

int UdpSender::send(void *message, size_t message_size) {
	return sendto(this->fd, message, message_size, 0, p->ai_addr, p->ai_addrlen);
}

