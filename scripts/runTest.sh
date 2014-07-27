#!/bin/bash

PROJECT="pfa"

# Run and execute tests with coverage info
mkdir -p Debug
mkdir -p coverage
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=True ..
make
cd ..
lcov -d Debug/CMakeFiles/${PROJECT}.dir/src/main/cpp -d Debug/CMakeFiles/UnitTester.dir/src/test/cpp --zerocounters
Debug/UnitTester
lcov -d Debug/CMakeFiles/${PROJECT}.dir/src/main/cpp -d Debug/CMakeFiles/UnitTester.dir/src/test/cpp --capture --output-file coverage/${PROJECT}.info
lcov -r coverage/${PROJECT}.info "/usr/include/*" -o coverage/${PROJECT}.info
genhtml -o coverage coverage/${PROJECT}.info
