#!/bin/bash
# Clean previous builds
rm -rf build/
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)