#!/bin/bash

set -e
dir="gdal"

if [ ! -d "${dir}" ] ; then
   git clone https://github.com/OSGeo/gdal.git "${dir}"
   cd "${dir}"
   git checkout "tags/v3.0.2"
fi
