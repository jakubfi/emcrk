#!/bin/bash

ACTION=$1
BUID_DIR=$SRC_DIR/build

case $ACTION in

	init)
		cmake -E make_directory "$BUILD_DIR"
	;;

	configure)
		cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -G "Unix Makefiles" "$SRC_DIR"
	;;

	build)
		cmake --build "$BUILD_DIR"
	;;

	install)
		sudo cmake --install "$BUILD_DIR"
	;;

	*)
		echo "Unknown action: $ACTION"
		exit 1
	;;

esac

# vim: tabstop=4
