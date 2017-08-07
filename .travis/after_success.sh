#!/bin/bash

PREFIX=$(readlink -enq "$(dirname $0)/../")

if [ "$CXX" = "g++" ]; then
	GCOV=gcov
	ARGS="-s \"$PREFIX\" -ablpr"
else
	GCOV=llvm-cov
	ARGS="gcov -a -b -l -p"
fi

rm -rf   "$PREFIX/.gcov"
mkdir -p "$PREFIX/.gcov"

$GCOV $ARGS -o .build/test   test/*.cpp
mv *.gcov "$PREFIX/.gcov"

/bin/bash <(curl -s https://codecov.io/bash) -f "$PREFIX/.gcov/*.gcov" -X gcov
