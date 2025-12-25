#!/bin/bash

# Build script for jura-api-server

set -e

echo "Building Jura API Server..."
echo "=========================="
echo ""

# Create build directory
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
echo "Building..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

echo ""
echo "Build complete!"
echo "Run the server with: ./build/jura-api-server"
echo "Or with custom port: ./build/jura-api-server 3000"
