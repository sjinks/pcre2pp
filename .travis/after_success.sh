#!/bin/bash

PREFIX=$(readlink -enq "$(dirname $0)/../")

if [ "$CXX" = "g++-6" ]; then
	GCOV=gcov-6
	ARGS="-ablpr"
else
	exit 0
fi

/bin/bash <(curl -s https://codecov.io/bash) -x $GCOV -a $ARGS
