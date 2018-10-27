#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H

#include "SocketImpl.h"

class TcpListener: public SocketImpl {
	public:
		TcpListener(std::string port);
		void start_listening(SocketMessageHandler handler, void *m, int msg_length);
};

#endif
