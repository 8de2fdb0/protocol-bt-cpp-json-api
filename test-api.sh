#!/bin/bash

# Test script for Jura API Server

set -e

echo "Starting Jura API Server Test Suite"
echo "===================================="
echo ""

# Start server
cd "$(dirname "$0")/build"
timeout 60 ./jura-api-server 8080 > /tmp/jura-server.log 2>&1 &
SERVER_PID=$!
sleep 2

echo "Server started with PID: $SERVER_PID"
echo ""

# Test function
test_endpoint() {
    local name="$1"
    local method="$2"
    local endpoint="$3"
    local data="$4"
    
    echo "Testing: $name"
    if [ "$method" = "GET" ]; then
        response=$(curl -s "http://localhost:8080$endpoint")
    else
        response=$(curl -s -X "$method" "http://localhost:8080$endpoint" \
            -H "Content-Type: application/json" \
            -d "$data")
    fi
    echo "Response: $response"
    echo ""
}

# Run tests
test_endpoint "Health Check" "GET" "/health" ""
test_endpoint "API Documentation" "GET" "/api" ""
test_endpoint "Initial Status" "GET" "/api/status" ""
test_endpoint "List Devices" "GET" "/api/devices" ""
test_endpoint "Connect to Device" "POST" "/api/connect" '{"device_address":"AA:BB:CC:DD:EE:FF"}'
test_endpoint "Status After Connection" "GET" "/api/status" ""
test_endpoint "Device Info" "GET" "/api/device-info" ""
test_endpoint "Brew Coffee" "POST" "/api/brew" '{"product":"espresso"}'
test_endpoint "Send Custom Command" "POST" "/api/command" '{"command":"TEST_COMMAND"}'
test_endpoint "Disconnect" "POST" "/api/disconnect" ""
test_endpoint "Status After Disconnect" "GET" "/api/status" ""

# Stop server
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

echo "All tests completed!"
echo ""
echo "Server logs:"
echo "============"
cat /tmp/jura-server.log
