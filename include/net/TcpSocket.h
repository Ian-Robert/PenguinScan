#pragma once
#include "net/Platform.h"
// #include <WinSock2.h
#include "net/ScanResult.h"
#include <string>

const int BANNER_BUFFER_SIZE = 4096;

class TcpSocket {
public:
	TcpSocket();
	~TcpSocket();
	ScanResult connect(const std::string& ip, int port, int timeoutMs = 1000);
	std::string receiveBanner(int timeoutMs = 2000);
private:
	SOCKET m_socket;
	bool m_isValid;
	void setNonBlocking(bool mode);
};