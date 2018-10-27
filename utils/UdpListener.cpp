#include "UdpListener.h"
#include "Log.h"

#include <unistd.h>
#include <netdb.h>
#include <string>

UdpListener::UdpListener(std::string port) : SocketImpl(SOCK_DGRAM, AI_PASSIVE, "", port, 1, true) {
	free_serve_info();
}

void UdpListener::start_listening(SocketMessageHandler handler, void *message, size_t msg_size) {
	while (true) {
		Log::v("Listening for messages.....");
		struct sockaddr_storage recv_addr;
		socklen_t recv_addr_len = sizeof(recv_addr);
		int recv_bytes = recvfrom(this->fd, message, msg_size, 0, (struct sockaddr *)&recv_addr, &recv_addr_len);
		if (recv_bytes == -1) {
			perror("Listener: Error in receiving message");
			Log::e("Listener: Error in receiving message");
		}
		Log::v("Listener: packet is " + std::to_string(recv_bytes) + " bytes long");
		handler(message);
	}
}
