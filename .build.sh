#!/usr/bin/env bash

action=$1

if [ -z "$action" ]; then
  action="all"
fi

do_cmake() {
  echo "CMake..."

  mkdir -p build &&
  cd build &&
  cmake .. &&
  cd ..
}

do_make() {
  echo "Make..."
  cd build &&
  make &&
  cd ..
}

do_run() {
  ./build/server
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
esac

