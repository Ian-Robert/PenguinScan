#pragma once
#include "cli/ArgParser.h"
#include "ScanTypes.h"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

class ScanningEngine {
public:
	void run(const ScanConfig& config);
private:
	void worker(const std::string& target,
		int timeout,
		std::atomic<int>& currentPort,
		int endPort,
		std::mutex& consoleMutex, bool showBanner,
		std::vector<ServiceResult> &results);
};