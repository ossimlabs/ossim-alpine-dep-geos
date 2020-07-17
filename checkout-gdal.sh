#!/bin/bash

set -e
dir="${DEPS_DIR}/gdal"

if [ ! -d "${dir}" ] ; then
   git clone https://github.com/OSGeo/gdal.git "${dir}"
   cd "${dir}"
   git checkout "tags/v${GDAL_VERSION}"
fi
