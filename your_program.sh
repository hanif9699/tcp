#!/bin/sh

set -e

cmake -B build -S .
cmake --build build
sudo setcap cap_net_admin=eip build/my_program
exec ./build/my_program "$@"


