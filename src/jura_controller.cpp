#include "jura_controller.h"
#include "jura_protocol.h"
#include <stdexcept>
#include <iostream>
#include <memory>

// PIMPL implementation
class JuraController::Impl {
public:
    Impl() : protocol(std::make_unique<JuraProtocol>()) {}
    
    std::unique_ptr<JuraProtocol> protocol;
};

JuraController::JuraController() : pImpl(std::make_unique<Impl>()) {
}

JuraController::~JuraController() = default;

bool JuraController::connect(const std::string& device_address) {
    try {
        return pImpl->protocol->connectDevice(device_address);
    } catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
}

bool JuraController::disconnect() {
    return pImpl->protocol->disconnectDevice();
}

bool JuraController::isConnected() const {
    return pImpl->protocol->isConnected();
}

nlohmann::json JuraController::getStatus() {
    nlohmann::json status;
    status["connected"] = pImpl->protocol->isConnected();
    
    if (pImpl->protocol->isConnected()) {
        std::string statusStr = pImpl->protocol->queryStatus();
        status["ready"] = true;
        status["water_level"] = "ok";
        status["coffee_beans"] = "ok";
        status["raw_status"] = statusStr;
    }
    
    return status;
}

nlohmann::json JuraController::brewCoffee(const std::string& product) {
    nlohmann::json result;
    
    if (!pImpl->protocol->isConnected()) {
        result["success"] = false;
        result["error"] = "Not connected to device";
        return result;
    }
    
    bool success = pImpl->protocol->brewProduct(product);
    
    result["success"] = success;
    result["product"] = product;
    result["message"] = success ? "Brewing started" : "Brewing failed";
    
    return result;
}

nlohmann::json JuraController::sendCommand(const std::string& command) {
    nlohmann::json result;
    
    if (!pImpl->protocol->isConnected()) {
        result["success"] = false;
        result["error"] = "Not connected to device";
        return result;
    }
    
    std::string response = pImpl->protocol->sendCommand(command);
    
    result["success"] = true;
    result["command"] = command;
    result["response"] = response;
    
    return result;
}

nlohmann::json JuraController::getDeviceInfo() {
    nlohmann::json info;
    
    if (pImpl->protocol->isConnected()) {
        std::string deviceInfo = pImpl->protocol->getDeviceInfo();
        info["info"] = deviceInfo;
        info["statistics"] = pImpl->protocol->getStatistics();
    } else {
        info["error"] = "Not connected";
    }
    
    return info;
}
