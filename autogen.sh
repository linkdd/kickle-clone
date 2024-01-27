#!/usr/bin/env sh

case $SHIPP_TARGET_FAMILY in
  unix)
    export GEN="Unix Makefiles"
    ;;
  windows)
    export GEN="MinGW Makefiles"
    ;;
esac

mkdir -p __build__
cd __build__

cmake .. \
  -G "$GEN" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_MAKE_PROGRAM=make \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_INSTALL_PREFIX=$SHIPP_DIST_DIR
