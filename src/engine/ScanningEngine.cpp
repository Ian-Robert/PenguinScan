#include "engine/ScanningEngine.h"
#include "net/TcpSocket.h"
#include "util/IpUtils.h"
#include "util/Reporter.h"
#include <iostream>
#include <string>

void ScanningEngine::run(const ScanConfig &config) {

	std::vector<std::string> targets = IpUtils::expandTarget(config.target);
	std::cout << targets.size() << " IP(s) found" << std::endl;

	std::vector<HostResult> allResults;

	for (const std::string& targetIp : targets) {
		std::cout << "\nScanning Target: " << targetIp << "..." << std::endl;
		std::cout << "Starting scan on " << targetIp << "..." << std::endl;

		HostResult currentHost;
		currentHost.ip = targetIp;

		int threadCount = 3;
		switch (config.speed) {
		case 0: threadCount = 1; break;
		case 1: threadCount = 5; break;
		case 2: threadCount = 20; break;
		case 3: threadCount = 50; break;
		case 4: threadCount = 75; break;
		case 5: threadCount = 120; break;
		}

		// Atomic lets threads safely grab the next port # without fighting
		std::atomic<int> currentPort(config.startPort);

		// Mutex prevents threads from garbling console output 
		std::mutex sharedMutex;

		std::vector<std::thread> threadPool;

		// Pass shared resources by reference count
		for (int i = 0; i < threadCount; ++i)
		{
			threadPool.emplace_back(&ScanningEngine::worker, this,
				targetIp,
				config.timeout,
				std::ref(currentPort),
				config.endPort,
				std::ref(sharedMutex), 
				config.showBanner,
				std::ref(currentHost.openPorts));
		}

		// Wait for every thread to finish
		for (std::thread& t : threadPool)
		{
			if (t.joinable()) {
				t.join();
			}

		}
		allResults.push_back(currentHost);
	}

	std::cout << "Scan complete" << std::endl;

	if (config.saveToFile) {
		std::string fullPath = Reporter::buildPath(config.outFilePath, config.outfile);

		// Simple check: does filename end in .json?
		if (fullPath.find(".json") != std::string::npos) {
			Reporter::saveToJson(fullPath, allResults);
		}
		else {
			Reporter::saveToText(fullPath, allResults);
		}
	}
}


void ScanningEngine::worker(const std::string& target, int timeout, std::atomic<int>& currentPort, int endPort, std::mutex& sharedMutex, bool showBanner, std::vector<ServiceResult> &results) {
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
				std::lock_guard<std::mutex> lock(sharedMutex);
				std::cout << "[+] Port " << port << " is OPEN";
				if (!banner.empty()) {
					std::cout << "  [Version: " << banner << "]";
				}
				std::cout << std::endl;

				ServiceResult sr;
				sr.port = port;
				sr.banner = banner;
				results.push_back(sr);// adding to results vector for printing data 
			} // Lock releases here immediately
		}
	}
}