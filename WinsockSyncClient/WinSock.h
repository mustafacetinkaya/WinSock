#pragma once
#include "TypeDefs.h"



#define BUF_LEN 64		// Rx & Tx buffers
#define FIXED_SIZE 5	// Fixed part of the data packet; excluding checksum

namespace Cetinkaya {
	class WinSock
	{
	public:
		WinSock(size_t port);
		~WinSock();
		//int send_all(int socket, const void *buffer, size_t length, int flags);

		void append_header( packet &data_packet, enum_header header );
		void append_checksum( packet &data_packet );

		void receive_msg();
		void send_msg(int length);
		
		bool is_connected();
		int handle_msg();

		void PSU_rx_handler(size_t index);
		void SNP_rx_handler(size_t index);
		void MCU_rx_handler(size_t index);
		void SPU_rx_handler( size_t index );
		
		void PSU_tx_handler();
		void SNP_tx_handler();
		void MCU_tx_handler();
		
		void prepare_packet(enum_header header, enum_command_type command, int command_data);
		void begin_loop();

		void set_command_type( enum_command_type command );
		void test_endian();
		void deprecate(bool deprec);

	private:
		SOCKET			Socket;
		WSADATA			WsaDat;
		struct hostent	*host;
		SOCKADDR_IN		SockAddr;
		size_t			port;

		int bytes_received;

		char*	buffer_rx;
		char*	buffer_tx;
		
		bool				connected;
		//struct packet		data_packet;
		enum_command_type	command_type;
		

		bool deprecate_after_use;	// deprecate command after using and return to loop
		bool new_command;			// flag for thread to check

	};
}
