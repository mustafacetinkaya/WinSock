#include "WinSock.h"

namespace Cetinkaya{

	WinSock::WinSock() {
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
			std::cout << "Winsock error - Socket creation Failed!\r\n";
			WSACleanup();
			system("PAUSE");
		}

		// Resolve IP address for hostname
		if ((host = gethostbyname("localhost")) == NULL)
		{
			std::cout << "Failed to resolve hostname.\r\n";
			WSACleanup();
			system("PAUSE");
		}

		// Setup our socket address structure
		SockAddr.sin_port = htons(5555);
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
			connected = true;
		char buffer_tx[20] = { 'a' };
	}

	WinSock::~WinSock()
	{
		// Shutdown our socket
		shutdown(Socket, SD_SEND);

		// Close our socket entirely
		closesocket(Socket);

		// Cleanup Winsock
		WSACleanup();
		system("PAUSE");
	}

/*	int Cetinkaya::send_all(int socket, const void *buffer, size_t length, int flags)
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

	void Cetinkaya::WinSock::send_msg(){
		if (send(WinSock::Socket, WinSock::buffer_tx, sizeof( WinSock::buffer_tx ), 0) == SOCKET_ERROR){
			std::cout << "Socket error!" << std::endl;
			/* !! HANDLE */
		}
	}

	void Cetinkaya::WinSock::receive_msg(){
		memset(WinSock::buffer_rx, 0, sizeof(WinSock::buffer_rx));
		int bytesReceived = recv(WinSock::Socket, WinSock::buffer_rx, sizeof(WinSock::buffer_rx), 0);

		if (bytesReceived == 0)
			std::cout << "Connection closed!";

		else if (bytesReceived == SOCKET_ERROR){
			// !! HANDLE
			std::cout << "Socket error!";
		}

		else
		std::cout << WinSock::buffer_rx << std::endl;

	}

	bool Cetinkaya::WinSock::isConnected(){
		return connected;
	}
	
	void Cetinkaya::WinSock::handle_msg(){

	}
}