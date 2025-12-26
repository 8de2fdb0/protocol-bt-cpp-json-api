#ifndef JURA_CONTROLLER_H
#define JURA_CONTROLLER_H

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

// Forward declaration for protocol-bt-cpp classes
// These will be included in the implementation file
class JuraConnection;

class JuraController {
public:
    JuraController();
    ~JuraController();

    // Connection management
    bool connect(const std::string& device_address);
    bool disconnect();
    bool isConnected() const;
    
    // Device discovery
    nlohmann::json listDevices();

    // Coffee maker commands
    nlohmann::json getStatus();
    nlohmann::json brewCoffee(const std::string& product);
    nlohmann::json sendCommand(const std::string& command);
    nlohmann::json getDeviceInfo();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // JURA_CONTROLLER_H
