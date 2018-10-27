#ifndef LISTENER_SOCKET_H
#define LISTENER_SOCKET_H

#include "SocketImpl.h"

class UdpListener: public SocketImpl {
	public:
		UdpListener(std::string port);
		void start_listening(SocketMessageHandler handler, void *message, size_t msg_size);
};


#endif
