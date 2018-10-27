#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H

#include "SocketImpl.h"

typedef void (*TcpMessageHandler)(void *);
class TcpListener: public SocketImpl {
	public:
		TcpListener(std::string port);
		void start_listening(TcpMessageHandler handler, void *m, int msg_length);
};

#endif
