#include <iostream>
#include <chrono>
#include <thread>
#include "cli/ArgParser.h"
#include "net/SocketContext.h"
#include "net/TcpSocket.h"
#include "engine/ScanningEngine.h"
using namespace std;

int main(int argc, char *argv[]) {

    try {
        // 1. Init Winsock
        SocketContext ctx;

        // 2. Parse Args
        ArgParser parser;
        ScanConfig config;
        if (!parser.parse(argc, argv, config)) {
            return 1;
        }

        // 3. Run
        ScanningEngine engine;
        engine.run(config);
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}