#include "WinSock.h"

namespace Cetinkaya{

	WinSock::WinSock(size_t port) : port (port), connected (false), new_command(false),
		deprecate_after_use( false ), bytes_received( 0 ) {
		// Initialise Winsock
		if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
		{
			std::cout << "Winsock error - Winsock initialization failed\r\n";
			WSACleanup();
			system("PAUSE");
		}

		// Create our socket
		Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (Socket == INVALID_SOCKET)
		{
			std::cout << "Winsock error - Socket creation failed!\r\n";
			WSACleanup();
			system("PAUSE");
		}

		if ((host = gethostbyname( "localhost")) == NULL)
		{
			std::cout << "Failed to resolve hostname.\r\n";
			WSACleanup();
			system("PAUSE");
		}

		// Setup our socket address structure
		SockAddr.sin_port = htons(port);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		// Attempt to connect to server
		if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
		{
			std::cout << "Failed to establish connection with server\r\n";
			WSACleanup();
			system("PAUSE");
		}
		else
		{
			connected = TRUE;
		}
	}

	WinSock::~WinSock()
	{
		delete[] buffer_rx;
		delete[] buffer_tx;

		// Shutdown our socket
		shutdown(Socket, SD_SEND);

		// Close our socket entirely
		closesocket(Socket);

		// Cleanup Winsock
		WSACleanup();
		system("PAUSE");
	}

/*	int Cetinkaya::WinSock::send_all(int socket, const void *buffer, size_t length, int flags)
	{
			int n;
			const char *p = (char*)buffer;
			while (length > 0)
			{
				n = send(socket, p, length, flags);
				if (n <= 0) break;
				p += n;
				length -= n;
			}
			return (n <= 0) ? -1 : 0;
	} */

	void Cetinkaya::WinSock::append_header( packet &data_packet, enum_header header ) {
		switch ( header ) {

		case psu_header:
			data_packet.header[0] = ( char ) 'p';
			data_packet.header[1] = ( char ) 's';
			data_packet.header[2] = ( char ) 'u';
			break;

		case snp_header:
			data_packet.header[0] = ( char ) 's';
			data_packet.header[1] = ( char ) 'n';
			data_packet.header[2] = ( char ) 'p';
			break;

		case mcu_header:
			std::cout << "Append MCU\r\n";
			data_packet.header[0] = 'm';
			data_packet.header[1] = 'c';
			data_packet.header[2] = 'u';
			break;

		case spu_header:
			data_packet.header[0] = ( char ) 's';
			data_packet.header[1] = ( char ) 'p';
			data_packet.header[2] = ( char ) 'u';
			break;

		default:
			std::cout << "ERR Appending header!" << std::endl;
			break;
		}
	}

	void Cetinkaya::WinSock::append_checksum( packet &data_packet ) {
		UINT16 sum = 0;

		sum = data_packet.data_size + data_packet.header[0] + data_packet.header[1]
			+ data_packet.header[2] + data_packet.packet_type;

		for ( int i = 0; i < data_packet.data_size; i++ )
			sum += data_packet.data[i];

		data_packet.checksum[0] = ( sum & 0xff00 ) >> 8;
		data_packet.checksum[1] = ( sum & 0xff );

	}

	void Cetinkaya::WinSock::send_msg(int len){	// Transmits buffer_tx
		if ( send( WinSock::Socket, WinSock::buffer_tx, ( len ), 0 ) == SOCKET_ERROR ) {
			std::cout << "Socket error!" << std::endl;
			/* !! HANDLE */
		}
		else
			std::cout << "Sent packet of " << len << " bytes\r\n";
	}

	void Cetinkaya::WinSock::receive_msg() {
		new_command = FALSE;
		if ( deprecate_after_use == TRUE ) {	// Do not listen for answer
			deprecate_after_use = FALSE;
			return;
		}

		memset( WinSock::buffer_rx, 0, sizeof( WinSock::buffer_rx ) );	// Clear array
		bytes_received = recv( WinSock::Socket, WinSock::buffer_rx, sizeof( WinSock::buffer_rx ), 0 );

		if ( bytes_received == 0 ){
			std::cout << "Connection closed!";
			connected = FALSE;
		}

		else if (bytes_received == SOCKET_ERROR){
			// !! HANDLE
			std::cout << "Socket error!";
		}

		else{
			std::cout << WinSock::buffer_rx << std::endl;
			if ( handle_msg() == -1)
				cout << "Packet could not be read!\r\n";
		}

	}

	bool Cetinkaya::WinSock::is_connected(){
		return connected;
	}
	
	int Cetinkaya::WinSock::handle_msg() {
		enum_packet_for packet_for = undefined;

		size_t packet_index = 0;		// actual index
		bool packet_found = FALSE;
		while ( packet_found != TRUE ) {

			switch ( buffer_rx[packet_index] ) {

			case 'p':		// Is PSU message?
				if ( buffer_rx[packet_index + 1] == 's' &&
					buffer_rx[packet_index + 2] == 'u' ) {
					packet_found = TRUE;
					std::cout << "PSU Msg Received";
					packet_for = psu;
					WinSock::PSU_rx_handler( packet_index );
				}

			case 's':		// Is SNP message?
				if ( buffer_rx[packet_index + 1] == 'n' &&
					buffer_rx[packet_index + 2] == 'p' ) {
					packet_found = TRUE;
					std::cout << "SNP Msg Received";
					WinSock::SNP_rx_handler( packet_index );
				}

				else		// Is SPU message?
				if ( buffer_rx[packet_index + 1] == 'p' &&
					buffer_rx[packet_index + 2] == 'u' ) {
					packet_found = TRUE;
					WinSock::SPU_rx_handler( packet_index );
					std::cout << "SPU Msg Received";
					packet_for = spu;
				}

			case 'm':		// Is MCU message?
				if ( buffer_rx[packet_index + 1] == 'c' &&
					buffer_rx[packet_index + 2] == 'u' ) {
					packet_found = TRUE;
					std::cout << "MCU Msg Received";
					packet_for = mcu;
					WinSock::MCU_rx_handler( packet_index );
				}
			default:
				if ( packet_index < ( BUF_LEN - 2 ) ) {
					packet_index++;
					break;
				}
				else
					return -1; // Packet not found!
			}
		}
		return 1;
	}


	void Cetinkaya::WinSock::prepare_packet(enum_header header,
												enum_command_type command_type,
													int command_data){
		cout << "Type: " << endl; cout << command_type << endl;
		packet data_packet;
		int package_size = 0;	// ( FIXED_SIZE = 5 ) + data_size + ( checksum = 2 )


		Cetinkaya::WinSock::append_header ( data_packet, header );
		data_packet.packet_type = (byte) command_type;

		switch ( header ) {
		
/* Power Supply Unit Command Set */
		
		case psu:
			break;


/* SNP Command Set */

		case snp:
			switch ( command_type ) {
			case GET_IMU_DATA:
				break;
			case IMU_DATA:
				break;
			default:
				cout << "SNP command not defined!" << endl;
				return;
			}
			break; // end of snp


/* Motor Control Unit Command Set */

		case mcu:

			data_packet.data_size = 1;
			deprecate(true);	// Do not listen after mcu commands
			//data_packet.data = new byte[1];

			switch ( command_type ) {

				/* 1 Byte */
			case SET_POINT_X:
				std::cout << "Append SET_POINT_X "; std::cout << command_data << std::endl;
				command_data += 127;
				break;
			case SET_POINT_Y:
				std::cout << "Append SET_POINT_Y "; std::cout << command_data << std::endl;
				break;
			case SET_POINT_Z:
				std::cout << "Append SET_POINT_Z "; std::cout << command_data << std::endl;
				break;
			case SET_POINT_ROLL:
				std::cout << "Append SET_POINT_ROLL "; std::cout << command_data << std::endl;
				break;
			case SET_POINT_PITCH:
				std::cout << "Append SET_POINT_PITCH "; std::cout << command_data << std::endl;
				break;
			case SET_POINT_YAW:
				std::cout << "Append SET_POINT_YAW "; std::cout << command_data << std::endl;
				break;
			
			case STOP_ALL:
				std::cout << "Append STOP_ALL "; std::cout << command_data << std::endl;
				data_packet.data_size = 0;
				break;

			default:
				std::cout << "ERR: Nothing to append as MCU command!" << std::endl;
				return;
			}

			data_packet.data = new byte[data_packet.data_size];
			if ( data_packet.data_size > 0  )
				data_packet.data[0] = command_data;
			
			break; // end of mcu

			
/* Signal Processing Unit Command Set */

		case spu:
			Cetinkaya::WinSock::append_header( data_packet, enum_header::spu_header );

			switch ( command_type ) {
			case GET_IMU_DATA:
				data_packet.data = new byte[0];
				data_packet.data_size = 0;
				break;
			}
			
			break; // end of spu


/* Error */

		default:
			std::cout << "Command could not be set!" << std::endl;
			return;
		}

		package_size = ( FIXED_SIZE + 2 + data_packet.data_size );

		append_checksum( data_packet );

		//buffer_tx = (char*) &data_packet;	// buffer ready
		 buffer_tx = new char[ package_size ];
		 size_t index = 0;

		 // Fill the buffer
		 buffer_tx[index++] = data_packet.header[0];
		 buffer_tx[index++] = data_packet.header[1];
		 buffer_tx[index++] = data_packet.header[2];
		 buffer_tx[index++] = data_packet.packet_type;
		 buffer_tx[index++] = data_packet.data_size;

		 for ( int i = 0; i < data_packet.data_size; i++ )
			 buffer_tx[index++] = data_packet.data[i];
		 
		 buffer_tx[index++] = data_packet.checksum[0];
		 buffer_tx[index] = data_packet.checksum[1];



		 WinSock::send_msg( data_packet.data_size );
		 new_command = TRUE;	// let the thread know
		 delete[] buffer_tx;

}
	
	void Cetinkaya::WinSock::PSU_rx_handler( size_t index ) {};
	void Cetinkaya::WinSock::SNP_rx_handler( size_t index ) {};
	void Cetinkaya::WinSock::MCU_rx_handler( size_t index ) {};
	void Cetinkaya::WinSock::SPU_rx_handler( size_t index ) {};

	void Cetinkaya::WinSock::begin_loop() {
		for ( ;; ) {
			
			if ( new_command == FALSE ) {
				Sleep( 50 );			// Sleep until command
}
			else {
				WinSock::receive_msg();
			}
		}
	}
	void Cetinkaya::WinSock::set_command_type( enum_command_type command ) {
		command_type = command;
	}

	void Cetinkaya::WinSock::deprecate(bool deprec) {
		deprecate_after_use = deprec;
	}
}

// YOW - PITCH - ROLL 
