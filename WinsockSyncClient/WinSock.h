#pragma once
#include <iostream>
#include <winsock2.h>
#include <string>
#include "TypeDefs.h"

#define BUF_LEN 32

namespace Cetinkaya {
	class WinSock
	{
	public:
		WinSock();
		~WinSock();
		//int send_all(int socket, const void *buffer, size_t length, int flags);
		void receive_msg();
		void send_msg();
		bool is_connected();
		void handle_msg();
		void PSU_rx_handler();
		void SNP_rx_handler();
		void MCU_rx_handler();
		void PSU_tx_handler();
		void SNP_tx_handler();
		void MCU_tx_handler();
		void append_command(command_type command);
		void begin_loop();

	private:
		SOCKET Socket;
		WSADATA WsaDat;
		struct hostent *host;
		SOCKADDR_IN SockAddr;
		char buffer_rx[BUF_LEN];
		char buffer_rx_history[BUF_LEN];
		char buffer_tx[BUF_LEN];
		bool connected;
		packet data_packet;
		command_type command;

	};
}
