#!/bin/bash

cd gdal/gdal
./configure --with-proj=/usr/local --with-jpeg=internal --prefix=/usr/local --enable-shared --disable-static 
make -j 8 install
make clean
