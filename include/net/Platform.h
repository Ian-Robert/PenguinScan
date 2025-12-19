#pragma once

// WINDOWS SPECIFIC
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#else // LINUX/MAC SPECIFIC
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  // for getaddrinfo
#include <unistd.h> // for close()
#include <fcntl.h> // for non-blocking mode
#include <errno.h>
#include <cstring>

typedef int SOCKET;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close

#define SD_BOTH SHUT_RDWR
#define WSAECONNREFUSED ECONNREFUSED
#endif