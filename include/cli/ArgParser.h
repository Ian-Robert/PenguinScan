#pragma once

#include <string>
#include <vector>

struct ScanConfig {
	std::string target;
	std::string output = "txt";
	std::string outfile;
	int startPort = 1;
	int endPort = 1024;
	int speed = 1;
	int timeout = 1000;
	bool showBanner = false;
	bool help = false;
	bool version = false;
};

class ArgParser {
public:
	ArgParser();
	bool parse(int argc, char* argv[], ScanConfig& config);
	void printHelp() const;
	void printVersion() const;
private:
	bool isInteger(const std::string &val) const;
	bool parsePortRange(const std::string& value, ScanConfig& config);
};