#include "api_server.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include <regex>

using json = nlohmann::json;

class ApiServer::Impl {
public:
    std::unique_ptr<httplib::Server> server;
};

ApiServer::ApiServer(int port) 
    : pImpl(std::make_unique<Impl>())
    , controller_(std::make_shared<JuraController>())
    , port_(port) {
    
    pImpl->server = std::make_unique<httplib::Server>();
    
    // CORS headers helper
    auto setCorsHeaders = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };
    
    // Health check endpoint
    pImpl->server->Get("/health", [setCorsHeaders](const httplib::Request&, httplib::Response& res) {
        json response = {
            {"status", "ok"},
            {"service", "jura-api-server"}
        };
        setCorsHeaders(res);
        res.set_content(response.dump(), "application/json");
    });
    
    // Get status endpoint
    pImpl->server->Get("/api/status", [this, setCorsHeaders](const httplib::Request&, httplib::Response& res) {
        json response = controller_->getStatus();
        setCorsHeaders(res);
        res.set_content(response.dump(), "application/json");
    });
    
    // Connect to device endpoint
    pImpl->server->Post("/api/connect", [this, setCorsHeaders](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string device_address = body.value("device_address", "");
            
            if (device_address.empty()) {
                json response = {
                    {"success", false},
                    {"error", "device_address is required"}
                };
                setCorsHeaders(res);
                res.status = 400;
                res.set_content(response.dump(), "application/json");
                return;
            }
            
            // Validate Bluetooth MAC address format (XX:XX:XX:XX:XX:XX)
            std::regex mac_regex("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");
            if (!std::regex_match(device_address, mac_regex)) {
                json response = {
                    {"success", false},
                    {"error", "Invalid device_address format. Expected format: XX:XX:XX:XX:XX:XX"}
                };
                setCorsHeaders(res);
                res.status = 400;
                res.set_content(response.dump(), "application/json");
                return;
            }
            
            bool success = controller_->connect(device_address);
            json response = {
                {"success", success},
                {"device_address", device_address}
            };
            
            setCorsHeaders(res);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            json response = {
                {"success", false},
                {"error", e.what()}
            };
            setCorsHeaders(res);
            res.status = 500;
            res.set_content(response.dump(), "application/json");
        }
    });
    
    // Disconnect endpoint
    pImpl->server->Post("/api/disconnect", [this, setCorsHeaders](const httplib::Request&, httplib::Response& res) {
        bool success = controller_->disconnect();
        json response = {{"success", success}};
        setCorsHeaders(res);
        res.set_content(response.dump(), "application/json");
    });
    
    // Brew coffee endpoint
    pImpl->server->Post("/api/brew", [this, setCorsHeaders](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string product = body.value("product", "coffee");
            
            json response = controller_->brewCoffee(product);
            setCorsHeaders(res);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            json response = {
                {"success", false},
                {"error", e.what()}
            };
            setCorsHeaders(res);
            res.status = 500;
            res.set_content(response.dump(), "application/json");
        }
    });
    
    // Send command endpoint
    pImpl->server->Post("/api/command", [this, setCorsHeaders](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string command = body.value("command", "");
            
            if (command.empty()) {
                json response = {
                    {"success", false},
                    {"error", "command is required"}
                };
                setCorsHeaders(res);
                res.status = 400;
                res.set_content(response.dump(), "application/json");
                return;
            }
            
            json response = controller_->sendCommand(command);
            setCorsHeaders(res);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            json response = {
                {"success", false},
                {"error", e.what()}
            };
            setCorsHeaders(res);
            res.status = 500;
            res.set_content(response.dump(), "application/json");
        }
    });
    
    // Get device info endpoint
    pImpl->server->Get("/api/device-info", [this, setCorsHeaders](const httplib::Request&, httplib::Response& res) {
        json response = controller_->getDeviceInfo();
        setCorsHeaders(res);
        res.set_content(response.dump(), "application/json");
    });
    
    // API documentation endpoint
    pImpl->server->Get("/api", [setCorsHeaders](const httplib::Request&, httplib::Response& res) {
        json endpoints = {
            {"endpoints", {
                {
                    {"path", "/health"},
                    {"method", "GET"},
                    {"description", "Health check endpoint"}
                },
                {
                    {"path", "/api/status"},
                    {"method", "GET"},
                    {"description", "Get current connection status"}
                },
                {
                    {"path", "/api/connect"},
                    {"method", "POST"},
                    {"description", "Connect to Jura device"},
                    {"body", {{"device_address", "string"}}}
                },
                {
                    {"path", "/api/disconnect"},
                    {"method", "POST"},
                    {"description", "Disconnect from Jura device"}
                },
                {
                    {"path", "/api/brew"},
                    {"method", "POST"},
                    {"description", "Brew coffee"},
                    {"body", {{"product", "string"}}}
                },
                {
                    {"path", "/api/command"},
                    {"method", "POST"},
                    {"description", "Send custom command to device"},
                    {"body", {{"command", "string"}}}
                },
                {
                    {"path", "/api/device-info"},
                    {"method", "GET"},
                    {"description", "Get device information"}
                }
            }}
        };
        setCorsHeaders(res);
        res.set_content(endpoints.dump(2), "application/json");
    });
}

ApiServer::~ApiServer() {
    stop();
}

void ApiServer::start() {
    std::cout << "Starting Jura API Server on port " << port_ << "..." << std::endl;
    std::cout << "API documentation available at http://localhost:" << port_ << "/api" << std::endl;
    pImpl->server->listen("0.0.0.0", port_);
}

void ApiServer::stop() {
    if (pImpl->server) {
        pImpl->server->stop();
    }
}
