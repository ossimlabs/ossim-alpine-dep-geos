#!/bin/bash

set -e
dir="geos"

if [ ! -d "${dir}" ] ; then
    git clone https://github.com/libgeos/geos.git "${dir}"
    cd "${dir}"
    git checkout "tags/3.8.0"
    mkdir build
fi 
