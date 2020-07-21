#!/bin/bash

mkdir geosBuild
cd geos/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/geosBuild ..
make -j 8 VERBOSE=1 install
make clean
