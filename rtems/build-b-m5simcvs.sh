#!/bin/bash

BSP=m5sim
TARGET=sparc64-rtems4.10
EXTENSION=

cd b-m5simcvs
rm * -rf
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=m5sim
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=m5sim --disable-posix
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=m5sim --enable-tests=samples
../rtemscvs/configure --target=${TARGET} --disable-networking --enable-rtemsbsp=${BSP} --enable-tests

time make -j 4 2> make.error

echo ""

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/m5sim/testsuites/samples/hello/hello.exe > hello.disasm

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/m5sim/testsuites/samples/ticker/ticker.exe > ticker.disasm

