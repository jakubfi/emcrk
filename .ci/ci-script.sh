#!/bin/bash
cmake -E make_directory "$BUILD_DIR"
cmake -B "$SRC_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -G "Unix Makefiles"
cmake --build "$BUILD_DIR"
sudo cmake --install "$BUILD_DIR"
