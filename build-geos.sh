#!/bin/bash

mkdir build
cd geos/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/build ..
make -j 8 VERBOSE=1 install
make clean
