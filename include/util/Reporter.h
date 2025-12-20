#pragma once

#include "ScanTypes.h"
#include <vector>
#include <string>

class Reporter {
public:
    // Helper to combine dir + filename safely
    static std::string buildPath(const std::string& dir, const std::string& filename);

    // Save functions
    static void saveToJson(const std::string& fullPath, const std::vector<HostResult>& results);
    static void saveToText(const std::string& fullPath, const std::vector<HostResult>& results);
    static std::string sanitize(const std::string& input);
};