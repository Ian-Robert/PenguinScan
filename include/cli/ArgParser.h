#pragma once

#include <string>
#include <vector>

struct ScanConfig {
	std::string target = "127.0.0.1";
	std::string output = "txt";
	std::string outfile = "scan_results.txt";
	std::string outFilePath = "./";
	int startPort = 1;
	int endPort = 1024;
	int speed = 2;
	int timeout = 1000;
	bool showBanner = false;
	bool help = false;
	bool version = false;
	bool saveToFile = false;
};

class ArgParser {
public:
	ArgParser();
	bool parse(int argc, char* argv[], ScanConfig& config);
	void printHelp() const;
	void printVersion() const;
private:
	bool parsePortRange(const std::string& value, ScanConfig& config);
};