#ifndef API_SERVER_H
#define API_SERVER_H

#include <memory>
#include <string>
#include "jura_controller.h"

class ApiServer {
public:
    ApiServer(int port = 8080);
    ~ApiServer();

    void start();
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    std::shared_ptr<JuraController> controller_;
    int port_;
};

#endif // API_SERVER_H
