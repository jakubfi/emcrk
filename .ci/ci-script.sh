#!/bin/bash
cmake -B $SRC_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build $BUILD_DIR --config $BUILD_TYPE
sudo cmake --install $BUILD_DIR
