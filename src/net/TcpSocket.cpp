#include "net/TcpSocket.h"
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

TcpSocket::TcpSocket()
{
	// 1. Create the socket
	// AF_NET = IPv4
	// SOCK_STREAM = TCP
	// IPPROTO_TCP = TCP Protocol
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "Error Creating Socket: " << WSAGetLastError() << std::endl;
		m_isValid = false;
	}
	else {
		m_isValid = true;
	}
}

TcpSocket::~TcpSocket() {
	if (m_isValid)
	{
		closesocket(m_socket);
	}
}

bool TcpSocket::connect(const std::string& ip, int port, int timeoutMs) {
	// 2. Setup the Address Structure
	// socketaddr_in is a struct that holds the TargetIP and Port
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET; // IPv4

	// 3. Convert Port to Network Byte Order
	// Computers use Little Endian (Intel/AMD) but the internet uses Big Endian
	// htons() = "Host TO Network Short" handles this flip for us
	serverAddr.sin_port = htons(port);

	// 4. Convert String IP to Binary IP
	// inet_pton = "Pointer to String to Number"
	// It returns 1 on success.
	if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
		std::cerr << "Invalid IP address format: " << ip << std::endl;
		return false;
	}

	// 5. Attempt Connection
	// This will block until success or OS timeout (about 20 sec)
	int result = ::connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}