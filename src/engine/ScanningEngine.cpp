#include "engine/ScanningEngine.h"
#include "net/TcpSocket.h"
#include <iostream>
#include <string>

void ScanningEngine::run(const ScanConfig &config) {
	std::cout << "Starting scan on " << config.target << "..." << std::endl;
	std::cout << "Starting scan on port " << config.startPort << "..." << std::endl;

	int threadCount = 1;
	switch (config.speed) {
	case 0: threadCount = 1; break;
	case 1: threadCount = 5; break;
	case 2: threadCount = 20; break;
	case 3: threadCount = 50; break;
	case 4: threadCount = 100; break;
	case 5: threadCount = 200; break;
	}
	
	// Atomic lets threads safely grab the next port # without fighting
	std::atomic<int> currentPort(config.startPort);
	
	// Mutex prevents threads from garbling console output
	std::mutex consoleMutex;

	std::vector<std::thread> threadPool;

	// Pass shared resources by reference count
	for (int i = 0; i < threadCount; ++i)
	{
		threadPool.emplace_back(&ScanningEngine::worker, this,
			config.target,
			config.timeout,
			std::ref(currentPort),
			config.endPort,
			std::ref(consoleMutex));
	}

	// Wait for every thread to finish
	for (std::thread& t : threadPool)
	{
		if (t.joinable()) {
			t.join();
		}
	}
	std::cout << "Scan complete" << std::endl;
}

void ScanningEngine::worker(const std::string& target, int timeout, std::atomic<int>& currentPort, int endPort, std::mutex& consoleMutex) {
	while (true) {
		// Grab work
		// fetch_add return the current value, then increments it atomically
		int port = currentPort.fetch_add(1);

		if (port > endPort) {
			return;
		}

		// Do work (scan)
		TcpSocket socket;
		bool isOpen = socket.connect(target, port, timeout);
			
		if (isOpen) {
			// Lock console so text doesn't mix
			std::lock_guard<std::mutex> lock(consoleMutex);
			std::cout << "[+] Port " << port << " is OPEN" << std::endl;
		}
	}
}