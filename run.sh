#!/bin/bash

set -e

BUILD_DIR="cmake-build-debug"

rm -rf $BUILD_DIR
cmake -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug
cmake --build $BUILD_DIR --target pcap_main -j
$BUILD_DIR/pcap_main