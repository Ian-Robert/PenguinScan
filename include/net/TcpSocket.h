#pragma once
#include <WinSock2.h>
#include "net/ScanResult.h"
#include <string>

class TcpSocket {
public:
	TcpSocket();
	~TcpSocket();
	ScanResult connect(const std::string& ip, int port, int timeoutMs = 1000);
private:
	SOCKET m_socket;
	bool m_isValid;
	void setNonBlocking(bool mode);
};