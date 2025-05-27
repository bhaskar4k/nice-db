#!/bin/bash

# Enable strict mode
set -e

# Output binary name
OUTPUT="main.exe"

# Compiler flags
CXX_FLAGS="-std=c++17"

# Include directories
INCLUDE_DIRS="-Icommand-control/header-file -Ienvironment/header-file -Isrc"

# Library flags
LIBS="-lws2_32"

# Find all .cpp files recursively
CPP_FILES=$(find . -name "*.cpp")

# Compile
echo "Compiling..."
g++ $CXX_FLAGS $CPP_FILES $INCLUDE_DIRS -o $OUTPUT $LIBS

# Run
echo "Running $OUTPUT"
./$OUTPUT
