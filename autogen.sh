#!/bin/sh

if [ "$1" = "-r" ]
then
  build_dir="__build__/release"
  build_type="Release"
else
  build_dir="__build__/debug"
  build_type="Debug"
fi

mkdir -p $build_dir
cd $build_dir
cmake ../.. \
  -G "MinGW Makefiles" \
  -DCMAKE_BUILD_TYPE=$build_type \
  -DCMAKE_MAKE_PROGRAM=make \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCMAKE_C_COMPILER=gcc
