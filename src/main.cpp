#include <iostream>
#include "cli/ArgParser.h"
#include "net/SocketContext.h"
#include "net/TcpSocket.h"
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

        std::cout << "Scanning " << config.target << "..." << std::endl;

        // 3. THE TEST: Try to connect
        TcpSocket mySocket;
        bool isOpen = mySocket.connect(config.target, config.startPort);

        if (isOpen) {
            std::cout << "[+] Port " << config.startPort << " is OPEN" << std::endl;
        }
        else {
            std::cout << "[-] Port " << config.startPort << " is CLOSED" << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}