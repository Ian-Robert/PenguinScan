#include "engine/ScanningEngine.h"
#include "net/TcpSocket.h"
#include <iostream>
#include <string>

void ScanningEngine::run(const ScanConfig &config) {
	std::cout << "Starting scan on " << config.target << "..." << std::endl;
	std::cout << "Starting scan on port " << config.startPort << "..." << std::endl;

	for (int port = config.startPort; port <= config.endPort; ++port)
	{
		TcpSocket mySocket;
		bool isOpen = mySocket.connect(config.target, config.startPort);

		if (isOpen)
		{
			std::cout << "[+] Port " << port << " is OPEN" << std::endl;
		}
		else {
			std::cout << "[-] Port " << port << " is CLOSED" << std::endl;
		}

	}

	std::cout << "Scan complete!" << std::endl;

}