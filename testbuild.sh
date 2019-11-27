#!/bin/bash
cd Core
mkdir build
cd build
cmake ../src
make -j8
cd ../../CMD
mkdir build
cd build
cmake -DWITH_ZED=1 ../src/
make -j8
