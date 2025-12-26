# Jura Coffee Maker JSON API Server

A C++ JSON API service for controlling a Jura coffee maker over Bluetooth using the [protocol-bt-cpp](https://github.com/8de2fdb0/protocol-bt-cpp) library.

## Features

- RESTful JSON API for Jura coffee maker control
- Bluetooth connectivity using the Jura protocol
- Simple HTTP server with CORS support
- Easy-to-use endpoints for coffee brewing and device management

## Building

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Bluetooth support on your system

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

The executable `jura-api-server` will be created in the build directory.

## Running

Start the server with default port (8080):

```bash
./jura-api-server
```

Or specify a custom port:

```bash
./jura-api-server 3000
```

## API Endpoints

### GET /health
Health check endpoint.

**Response:**
```json
{
  "status": "ok",
  "service": "jura-api-server"
}
```

### GET /api
Get API documentation with all available endpoints.

### GET /api/status
Get current connection status.

**Response:**
```json
{
  "connected": true,
  "device_address": "XX:XX:XX:XX:XX:XX",
  "ready": true,
  "water_level": "ok",
  "coffee_beans": "ok"
}
```

### POST /api/connect
Connect to a Jura device.

**Request:**
```json
{
  "device_address": "XX:XX:XX:XX:XX:XX"
}
```

**Response:**
```json
{
  "success": true,
  "device_address": "XX:XX:XX:XX:XX:XX"
}
```

### POST /api/disconnect
Disconnect from the current device.

**Response:**
```json
{
  "success": true
}
```

### POST /api/brew
Brew a coffee product.

**Request:**
```json
{
  "product": "coffee"
}
```

**Response:**
```json
{
  "success": true,
  "product": "coffee",
  "message": "Brewing started"
}
```

### POST /api/command
Send a custom command to the device.

**Request:**
```json
{
  "command": "CUSTOM_COMMAND"
}
```

**Response:**
```json
{
  "success": true,
  "command": "CUSTOM_COMMAND",
  "response": "OK"
}
```

### GET /api/device-info
Get information about the connected device.

**Response:**
```json
{
  "model": "Jura Coffee Maker",
  "firmware": "1.0.0",
  "device_address": "XX:XX:XX:XX:XX:XX"
}
```

## Example Usage

### Using curl

Connect to device:
```bash
curl -X POST http://localhost:8080/api/connect \
  -H "Content-Type: application/json" \
  -d '{"device_address": "AA:BB:CC:DD:EE:FF"}'
```

Brew coffee:
```bash
curl -X POST http://localhost:8080/api/brew \
  -H "Content-Type: application/json" \
  -d '{"product": "espresso"}'
```

Get status:
```bash
curl http://localhost:8080/api/status
```

### Using JavaScript/Fetch

```javascript
// Connect to device
fetch('http://localhost:8080/api/connect', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({ device_address: 'AA:BB:CC:DD:EE:FF' })
})
  .then(res => res.json())
  .then(data => console.log(data));

// Brew coffee
fetch('http://localhost:8080/api/brew', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({ product: 'coffee' })
})
  .then(res => res.json())
  .then(data => console.log(data));
```

## Dependencies

This project uses the following libraries (automatically fetched by CMake):

- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) - C++ HTTP server library
- [protocol-bt-cpp](https://github.com/8de2fdb0/protocol-bt-cpp) - Jura protocol implementation

## Architecture

- **main.cpp**: Application entry point and signal handling
- **api_server.cpp/h**: HTTP server implementation with REST endpoints
- **jura_controller.cpp/h**: Jura device control and protocol integration

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. 
