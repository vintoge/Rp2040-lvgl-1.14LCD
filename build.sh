#!/bin/sh
#cmake -DCMAKE_BUILD_TYPE=Release . -B ./build 
cmake -DCMAKE_BUILD_TYPE=Debug . -B ./build
cd build
cmake --build . -j $(nproc)
cd ..
