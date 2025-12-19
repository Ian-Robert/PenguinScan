#include "util/IpUtils.h"
#include "net/Platform.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

std::vector<std::string> IpUtils::expandTarget(const std::string& target) {

	std::cout << "[DEBUG] expandTarget received: '" << target << "'" << std::endl;
	std::vector<std::string> ips;

	// Handle Comma Separated IP Addresses
	if (target.find(',') != std::string::npos) {
		std::stringstream ss(target); 
		std::string segment;

		while (std::getline(ss, segment, ',')) {
			// trim whitespace (stay sus of this while debugging)
			size_t first = segment.find_first_not_of(' ');
			if (std::string::npos == first) continue;
			size_t last = segment.find_last_not_of(' ');
			segment = segment.substr(first, (last - first + 1));

			// recursively call function for list of ip addresses
			std::vector<std::string> subList = expandTarget(segment);

			// add results to our main list
			ips.insert(ips.end(), subList.begin(), subList.end());

		}
		return ips;
	}

	size_t slashPos = target.find('/');
	if (slashPos == std::string::npos) {
		ips.push_back(target); // Add new element to end of container
		return ips;
	}

	std::string ipStr = target.substr(0, slashPos);
	int cidr = std::stoi(target.substr(slashPos + 1));

	std::cout << "[DEBUG] Parsing CIDR from string: '" << cidr << "'" << std::endl;

	if (cidr < 0 || cidr > 32) { return ips; }

	uint32_t ip = ipToLong(ipStr);

	// calculate subnet mask
	// if cidr is 0, mask is 0. If cidr is 32, mask is all 1s. Bitwise operation shifts all 1s to the left
	uint32_t mask = (cidr == 0) ? 0 : (~0U << (32 - cidr));

	uint32_t startIp = ip & mask; // bitwise AND, calculates the network address
	uint32_t endIp = startIp | ~mask; // bitwise NOT and OR, calculates the broadcast address

	// Loop to scan everything that's usable
	for (uint32_t current = startIp; current <= endIp; ++current) {
		ips.push_back(longToIp(current));
	}

	return ips;
}

uint32_t IpUtils::ipToLong(const std::string& ip) {
	sockaddr_in sa;
	inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr));
	return ntohl(sa.sin_addr.s_addr);
}

std::string IpUtils::longToIp(uint32_t ip) {
	char buffer[INET_ADDRSTRLEN];
	in_addr addr;
	addr.s_addr = htonl(ip);

	// inet_ntop converts binary back to IP 
	inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
	return std::string(buffer);
}