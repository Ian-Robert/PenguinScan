#include "engine/ScanningEngine.h"
#include "net/TcpSocket.h"
#include "util/IpUtils.h"
#include <iostream>
#include <string>

void ScanningEngine::run(const ScanConfig &config) {

	std::vector<std::string> targets = IpUtils::expandTarget(config.target);
	std::cout << targets.size() << " IP(s) found" << std::endl;

	for (const std::string& targetIp : targets) {
		std::cout << "\nScanning Target: " << targetIp << "..." << std::endl;
		std::cout << "Starting scan on " << targetIp << "..." << std::endl;

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
				targetIp,
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
	}

	std::cout << "Scan complete" << std::endl;
}

// engine/ScanningEngine.cpp

void ScanningEngine::worker(const std::string& target, int timeout, std::atomic<int>& currentPort, int endPort, std::mutex& consoleMutex, bool showBanner) {
	while (true) {
		int port = currentPort.fetch_add(1);
		if (port > endPort) return;

		TcpSocket socket;
		ScanResult result = socket.connect(target, port, timeout);

		if (result == ScanResult::OPEN) {
			std::string banner = "";
			if (showBanner) {
				banner = socket.receiveBanner(2000);
			}
			{
				std::lock_guard<std::mutex> lock(consoleMutex);
				std::cout << "[+] Port " << port << " is OPEN";
				if (!banner.empty()) {
					std::cout << "  [Version: " << banner << "]";
				}
				std::cout << std::endl;
			} // Lock releases here immediately
		}
	}
}