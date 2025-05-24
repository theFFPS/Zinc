#!/bin/bash

mkdir -p build
conan install . --output-folder=build --build=missing
cmake --preset conan-release
cmake --build build -j$(nproc)