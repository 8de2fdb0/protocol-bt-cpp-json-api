#ifndef JURA_PROTOCOL_H
#define JURA_PROTOCOL_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

/**
 * Jura protocol interface wrapper
 * This provides an abstraction layer for the Jura Bluetooth protocol
 * using the protocol-bt-cpp library
 */
class JuraProtocol {
public:
    JuraProtocol();
    ~JuraProtocol();

    // Connection management
    bool connectDevice(const std::string& device_address);
    bool disconnectDevice();
    bool isConnected() const;
    
    // Device discovery
    std::vector<std::string> listDevices();

    // Protocol commands
    std::string sendCommand(const std::string& command);
    std::string getDeviceInfo();
    std::string getStatistics();
    
    // Coffee brewing commands
    bool brewProduct(const std::string& product_code);
    
    // Status queries
    std::string queryStatus();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    bool connected_;
    std::string device_address_;
};

#endif // JURA_PROTOCOL_H
