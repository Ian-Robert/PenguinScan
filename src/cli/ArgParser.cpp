#include "cli/ArgParser.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

ArgParser::ArgParser() {}

bool ArgParser::parse(int argc, char* argv[], ScanConfig& config) {
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];

		if (arg == "--help")
		{
			printHelp();
			return false;
		}
		else if (arg == "--version" || arg == "-v")
		{
			printVersion();
			return false;
		}
		else if (arg == "--target" || arg == "-t")
		{
			if (i + 1 < argc)
			{
				config.target = argv[++i];
			}
			else 
			{
				std::cerr << "Error: --target requires a value" << std::endl;
				return false;
			}
		}
		else if (arg == "--ports" || arg == "-p")
		{
			if (i + 1 < argc)
			{
				if (!parsePortRange(argv[++i], config)) return false;
			}
			else {
				std::cerr << "Error: --ports requires a value (e.g. '80' or '20-100')" << std::endl;
				return false;
			}
		}
		else if (arg == "--all-ports" || arg == "-ap")
		{
			config.startPort = 1;
			config.endPort = 65535;
		}
		else if (arg == "--speed" || arg == "-s")
		{
			if (i + 1 < argc)
			{
				try {
					config.speed = std::stoi(argv[++i]);
				}
				catch (...) {
					std::cerr << "Error: Speed must be an integer" << std::endl;
					return false;
					}
			}
			else {
				std::cerr << "Error: --speed requires a value" << std::endl;
				return false;
			}
		}
		else if (arg == "--timeout" || arg == "-ti")
		{
			if (i + 1 < argc)
			{
				try { config.timeout = std::stoi(argv[++i]); 
				}
				catch (...) {
					std::cerr << "Error: Timeout must be an integer" << std::endl;
					return false;
				}
			}
			else {
				std::cerr << "Error: --timeout requires a value" << std::endl;
				return false;
			}
		}
		else if (arg == "--banner" || arg == "-b")
		{
			config.showBanner = true;
		}
		else {
			std::cerr << "Unknown Argument: " << arg << std::endl;
			return false;
		}
	}
	return true;
}

bool ArgParser::parsePortRange(const std::string& value, ScanConfig& config) {
	try {
		size_t dashPos = value.find("-");
		// IF A DASH IS FOUND (It is a range, e.g., "20-100") (npos means "No position)
		if (dashPos != std::string::npos) {
			std::string startStr = value.substr(0, dashPos);
			std::string endStr = value.substr(dashPos + 1);

			int start = std::stoi(startStr);
			int end = std::stoi(endStr);

			if (start > end) {
				std::cerr << "Error: Start port cannot be greater than end port." << std::endl;
				return false;
			}
			config.startPort = start;
			config.endPort = end;
		}
		// IF NO DASH IS FOUND (It is a single port, e.g., "80")
		else {
			int port = std::stoi(value);
			config.startPort = port;
			config.endPort = port;
		}

		// Global check for both cases
		if (config.startPort < 1 || config.endPort > 65535) {
			std::cerr << "Error: Ports must be between 1 and 65535." << std::endl;
			return false;
		}

		return true;

	}
	catch (const std::exception& e) {
		std::cerr << "Error: Invalid port format '" << value << "'. Use format '80' or '20-100'." << std::endl;
		return false;
	}
}

void ArgParser::printHelp() const {
	std::cout << "Usage: scanner [options]\n"
		<< "Options: \n"
		<< "     -t --target <IP>     Target IP Address\n"
		<< "     -p --ports <range>   Port Range (80 or 20-100)\n"
		<< "     -ap --all-ports      Scan all ports (1-65535)\n"
		<< "     -s --speed           Scan Speed (0-5)\n"
		<< "     -ti --timeout        Timeout in miliseconds\n"
		<< "     -v  --version        Show Version\n"
		<< "     -h --help            Show this help message\n"
		<< std::endl;
}

void ArgParser::printVersion() const {
	std::cout << "Version 0.1.0" << std::endl;
}
