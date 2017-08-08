#!/bin/sh

make -j$(getconf _NPROCESSORS_ONLN) COVERAGE=1 test
