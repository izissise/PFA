#!/bin/bash

cd PFA
rm -fr build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SERVER=OFF -DBUILD_CLIENT=OFF -DBUILD_MASTER_SERVER=ON
make -j 4
