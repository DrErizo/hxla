#!/bin/bash

cmake . -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE

cmake --build build
cd build
./hxla
cd ..

