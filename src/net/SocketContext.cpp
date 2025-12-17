#include "net/SocketContext.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // DO NOT TOUCH, NEED TO PREVENT LINKING ERRORS ON MSVC

SocketContext::SocketContext()
{
	WSADATA wsaData;
	// Request version 2.2
	// MAKEWORD(2, 2) creates the version identifier for Winsock 2.2
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) 
	{
		throw std::runtime_error("WSAStartup failed with error: " + std::to_string(result));
	}
	std::cout << "[DEBUG] Winsock Intialized Succesfully" << std::endl;
}

SocketContext::~SocketContext() 
{
	// Should run automatically when SocketContext Object goes out of scope
	WSACleanup();
	std::cout << "[DEBUG] Winsock cleaned up" << std::endl;
}
