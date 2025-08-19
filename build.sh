#!/bin/sh

debug=$1

if [ -d "./build/" ]; then 
  rm -rf build
fi

if [ "$debug" == "--debug" ]; then 
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
  cmake --build build
else 
  cmake -S . -B build
  cmake --build build
fi

