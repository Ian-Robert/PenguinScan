#pragma once
#include <string>
#include <vector>

struct ServiceResult {
	int port;
	std::string banner;
};

struct HostResult {
	std::string ip;
	std::vector<ServiceResult> openPorts;
};