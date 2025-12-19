#pragma once

#include <string>
#include <vector>
#include <cstdint>

class IpUtils {
public:
	static std::vector<std::string> expandTarget(const std::string& target);
private:
	static uint32_t ipToLong(const std::string& ip);
	static std::string longToIp(uint32_t ip);
};