#pragma once
#include <WinSock2.h>
#include <string>

class TcpSocket {
public:
	TcpSocket();
	~TcpSocket();
	bool connect(const std::string& ip, int port, int timeoutMs = 1000);
private:
	SOCKET m_socket;
	bool m_isValid;
	void close();
};