#!/bin/sh

set -e

make -j$(getconf _NPROCESSORS_ONLN) COVERAGE=1
