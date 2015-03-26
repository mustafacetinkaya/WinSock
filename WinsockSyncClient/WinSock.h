#pragma once
#include <iostream>
#include <winsock2.h>


namespace Cetinkaya {
	class WinSock
	{
	public:
		WinSock();
		~WinSock();
		//int send_all(int socket, const void *buffer, size_t length, int flags);
		void receive_msg();
		void send_msg();
		bool isConnected();
		void handle_msg();

	private:
		SOCKET Socket;
		WSADATA WsaDat;
		struct hostent *host;
		SOCKADDR_IN SockAddr;
		char buffer_rx[20];
		char buffer_rx_history[20];
		char buffer_tx[20];
		bool connected;

	};
}
