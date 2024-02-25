#!/bin/sh

rm -rf build udp_receiver
mkdir build && cd build
g++ -std=c++11 -Wall -Wextra -O3 ../app/test/UdpTransmitter.cpp -o udp_transmitter

cmake -DCMAKE_BUILD_TYPE=Release ../
make -j4
cp udp_receiver ../
