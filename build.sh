#!/usr/bin/env bash

BUILD_DIR=".build"
action=$1
target=$2

if [ -z "$action" ]; then
  action="all"
fi

do_cmake() {
  echo "CMake..."

  mkdir -p "$BUILD_DIR" &&
  cd "$BUILD_DIR" &&
  cmake .. &&
  cd ..
}

do_make() {
  echo "Make..."
  cd "$BUILD_DIR" &&
  make &&
  cd ..
}

do_run() {
	if [ -z "$target" ]; then
		target="server"
	fi

  	executable="./$BUILD_DIR/$target/$target"

	if [ ! -f "$executable" ]; then
		echo "Invalid target"
		exit 1
	fi

	$executable
}

case "$action" in
  "all")
    if do_cmake && do_make ; then
      do_run
    fi
    ;;
  "cmake")
    do_cmake
    ;;
  "make")
    do_make
    ;;
  "run")
    do_run
    ;;
   *)
	echo "Usage: $0 [all <target> | cmake | make | run <target>]"
	exit 0
	;;
esac

