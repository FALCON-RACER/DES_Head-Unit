#!/bin/bash

mkdir -p build
cd build
cmake ..
make
cd ..
sudo ./vehicle-controller