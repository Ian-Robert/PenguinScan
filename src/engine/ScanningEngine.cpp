#include "engine/ScanningEngine.h"
#include "net/TcpSocket.h"
#include <iostream>
#include <string>

void ScanningEngine::run(const ScanConfig &config) {
	std::cout << "Starting scan on " << config.target << "..." << std::endl;

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
			std::ref(consoleMutex), config.showBanner);
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

// engine/ScanningEngine.cpp

void ScanningEngine::worker(const std::string& target, int timeout, std::atomic<int>& currentPort, int endPort, std::mutex& consoleMutex, bool showBanner) {
	while (true) {
		// 1. Grab next port
		int port = currentPort.fetch_add(1);
		if (port > endPort) return;

		// 2. Scan
		TcpSocket socket;
		ScanResult result = socket.connect(target, port, timeout);

		if (result == ScanResult::OPEN) {
			std::string banner = "";

			// 3. Grab Banner (Slow operation - NO LOCK yet)
			if (showBanner) {
				banner = socket.receiveBanner(2000);
			}

			// 4. Print Result (Fast operation - WITH LOCK)
			{
				std::lock_guard<std::mutex> lock(consoleMutex);
				std::cout << "[+] Port " << port << " is OPEN";
				if (!banner.empty()) {
					std::cout << "  [Version: " << banner << "]";
				}
				std::cout << std::endl;
			} // Lock releases here immediately
		}
		// FILTERED block removed to prevent locking floods
	}
}