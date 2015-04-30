
#include "WinSock.h"

#pragma comment(lib,"ws2_32.lib")

int connectSv(SOCKET Socket, SOCKADDR_IN SockAddr){
	connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr));
	// Display message from server
	char buffer[1000];
	memset(buffer, 0, 999);
	int inDataLength = recv(Socket, buffer, 1000, 0);
	std::cout << buffer;
	return 1;
}

int main()
{
	Cetinkaya::WinSock winSock( 5555 );

		winSock.prepare_packet(enum_header::mcu_header, enum_command_type::STOP_ALL, 45);

		std::thread a( &Cetinkaya::WinSock::begin_loop, &winSock );

		winSock.prepare_packet( enum_header::mcu_header, enum_command_type::SET_POINT_X, 90 );
		a.join();
		winSock.~WinSock();

	return 0;

}

