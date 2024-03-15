@echo off

cmake . -B build

cd build
start devenv /Run hxla.sln 
cd ..