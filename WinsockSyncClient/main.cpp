
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
Cetinkaya::WinSock* winSock = new Cetinkaya::WinSock( 5555 );

void wrapper( void* arg ) {
	winSock->begin_loop();
}

int main()
{

		winSock->prepare_packet(enum_header::mcu_header, enum_command_type::STOP_ALL, 45);

		tthread::thread a( wrapper, 0 );

		winSock->prepare_packet( enum_header::mcu_header, enum_command_type::SET_POINT_X, 90 );
		a.join();
		winSock->~WinSock();

	return 0;

}

