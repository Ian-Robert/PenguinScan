#include "net/TcpSocket.h"
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
		#ifdef _WIN32
		std::cerr << "Error Creating Socket: " << WSAGetLastError() << std::endl;
		#else
		std::cerr << "Error Creating Socket " << errno << std::endl;
		#endif
		m_isValid = false;
	}
	else {
		m_isValid = true;
	}
}

TcpSocket::~TcpSocket() {
	if (m_isValid)
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
	}
}

void TcpSocket::setNonBlocking(bool mode) {
#ifdef _WIN32
	u_long iMode = mode ? 1 : 0;
	int result = ioctlsocket(m_socket, FIONBIO, &iMode);
	if (result != 0) {
		std::cerr << "[ERROR] WIndows Nonblocking failed" << std::endl;
	}
#else
	// Linux
	int flags = fcntl(m_socket, F_GETFL, 0);
	if (flags == -1) return;

	// Add or remove O_NONBLOCK flag
	if (mode) {
		fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
	}
	else {
		fcntl(m_socket, F_SETFL, flags & ~O_NONBLOCK);
	}
#endif
}

ScanResult TcpSocket::connect(const std::string& ip, int port, int timeoutMs) {
	
	if (!m_isValid) return ScanResult::FAILURE;

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(static_cast<unsigned short>(port));
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

	// Switch to non-blocking mode
	setNonBlocking(true);

	// Start Connection
	int result = ::connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR) {
		#ifdef _WIN32
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			return ScanResult::FAILURE;
		}
		// Linux check
		#else
		if (errno != EINPROGRESS) {
			return ScanResult::FAILURE;
		}
		#endif
	}

	// Wait timeout using select()
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(m_socket, &writeSet);

	timeval tv;
	tv.tv_sec = timeoutMs / 1000;
	tv.tv_usec = (timeoutMs % 1000) * 1000;

	// Select() returns:
	//  0 -> Timeout (port is filtered)
	// >0 -> Socket is Open or Closed
	// -1 -> Error
	int selectResult = select(m_socket + 1, NULL, &writeSet, NULL, &tv);

	if (selectResult == 0) {
		return ScanResult::FILTERED;
	}
	else if (selectResult == SOCKET_ERROR) {
		return ScanResult::FAILURE;
	}

	// Check if it was open or refused by checking the error code on the socket itself
	int soError;

#ifdef _WIN32
	int len = sizeof(soError);
#else
	socklen_t len = sizeof(soError);
#endif
	getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&soError, &len);

	if (soError == 0) {
		return ScanResult::OPEN;
	}
	else if (soError == WSAECONNREFUSED) {
		return ScanResult::CLOSED;
	}
	else {
		return ScanResult::FILTERED;
	}
	


}

// net/TcpSocket.cpp

std::string TcpSocket::receiveBanner(int timeoutMs) {
	if (!m_isValid) return "";

	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(m_socket, &readSet);

	timeval tv;
	tv.tv_sec = timeoutMs / 1000;
	tv.tv_usec = (timeoutMs % 1000) * 1000;

	// Wait for data
	if (select(m_socket + 1, &readSet, NULL, NULL, &tv) <= 0) {
		return "";
	}

	char buffer[BANNER_BUFFER_SIZE];
	int bytesReceived = recv(m_socket, buffer, BANNER_BUFFER_SIZE - 1, 0);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		std::string banner = std::string(buffer);

		// Sanitize string (Remove \r and \n)
		while (!banner.empty() && (banner.back() == '\r' || banner.back() == '\n')) {
			banner.pop_back();
		}
		return banner;
	}

	return "";
}

