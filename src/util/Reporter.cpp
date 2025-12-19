#include "util/Reporter.h"
#include <fstream>
#include <iostream>

std::string Reporter::buildPath(const std::string& dir, const std::string& filename) {
    std::string fullPath = dir;
    if (!fullPath.empty() && fullPath.back() != '/' && fullPath.back() != '\\') {
        fullPath += "/";
    }
    return fullPath + filename;
}

void Reporter::saveToJson(const std::string& fullPath, const std::vector<HostResult>& results) {
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << fullPath << std::endl;
        return;
    }

    file << "[\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& host = results[i];
        file << "  {\n";
        file << "    \"ip\": \"" << host.ip << "\",\n";
        file << "    \"ports\": [\n";

        for (size_t j = 0; j < host.openPorts.size(); ++j) {
            file << "      {\n";
            file << "        \"port\": " << host.openPorts[j].port << ",\n";
            // Simple quote escape could be added here
            file << "        \"banner\": \"" << host.openPorts[j].banner << "\"\n";
            file << "      }" << (j < host.openPorts.size() - 1 ? "," : "") << "\n";
        }

        file << "    ]\n";
        file << "  }" << (i < results.size() - 1 ? "," : "") << "\n";
    }
    file << "]\n";
    std::cout << "Report saved to " << fullPath << std::endl;
}

void Reporter::saveToText(const std::string& fullPath, const std::vector<HostResult>& results) {
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << fullPath << std::endl;
        return;
    }

    for (const auto& host : results) {
        file << "Target: " << host.ip << "\n";
        for (const auto& service : host.openPorts) {
            file << "  Port " << service.port << " [OPEN] " << service.banner << "\n";
        }
        file << "--------------------------------------\n";
    }
    std::cout << "Report saved to " << fullPath << std::endl;
}