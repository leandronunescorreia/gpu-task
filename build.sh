#!/bin/bash

# Build script for gpu-task

set -e

echo "=== Building GPU-Task ==="

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build
echo "Building..."
make -j$(nproc)

echo ""
echo "=== Build Complete ==="
echo "Library: build/libgputask.a"
echo "Examples:"
echo "  - build/device_info (check GPU availability)"
echo "  - build/vector_add (vector addition)"
echo "  - build/scalar_multiply (scalar multiplication)"
echo ""
echo "Run the device info utility:"
echo "  cd build && ./device_info"
