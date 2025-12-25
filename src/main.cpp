#include "api_server.h"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<ApiServer> server;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received." << std::endl;
    if (server) {
        server->stop();
    }
    exit(signum);
}

int main(int argc, char* argv[]) {
    // Default port
    int port = 8080;
    
    // Parse command line arguments
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid port number. Using default port 8080." << std::endl;
        }
    }
    
    // Register signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    std::cout << "Jura Coffee Maker JSON API Server" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Using protocol-bt-cpp for Jura protocol communication" << std::endl;
    std::cout << std::endl;
    
    try {
        server = std::make_unique<ApiServer>(port);
        server->start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
