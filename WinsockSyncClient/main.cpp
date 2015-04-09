#include <iostream>
#include <winsock2.h>
#include <thread>
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
/*	// Initialise Winsock
	WSADATA WsaDat;
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		std::cout << "Winsock error - Winsock initialization failed\r\n";
		WSACleanup();
		system("PAUSE");
		return 0;
	}

	// Create our socket
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Winsock error - Socket creation Failed!\r\n";
		WSACleanup();
		system("PAUSE");
		return 0;
	}

	// Resolve IP address for hostname
	struct hostent *host;
	if ((host = gethostbyname("localhost")) == NULL)
	{
		std::cout << "Failed to resolve hostname.\r\n";
		WSACleanup();
		system("PAUSE");
		return 0;
	}

	// Setup our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(5555);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
std::thread connectThread (connectSv, Socket, SockAddr );
	// Attempt to connect to server
	connectThread.join();

	/*if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		std::cout << "Failed to establish connection with server\r\n";
		WSACleanup();
		system("PAUSE");
		return 0;
	} */

	// Display message from server
/*	char buffer[1000];
	memset(buffer, 0, 999);
	int inDataLength = recv(Socket, buffer, 1000, 0);
	std::cout << buffer;

	// Shutdown our socket
	shutdown(Socket, SD_SEND);

	// Close our socket entirely
	closesocket(Socket);

	// Cleanup Winsock
	WSACleanup();
	system("PAUSE");
	return 0;
	*/
	Cetinkaya::WinSock winSock;

		//winSock.send_msg();
		std::cout << "Turn" << std::endl;
		//winSock.send_msg();
		//winSock.receive_msg();
		winSock.append_command(command_type::BROADCAST_MODE_REPORT);
	
	return 0;

}

