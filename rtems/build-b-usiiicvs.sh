#!/bin/sh

cd b-usiiicvs
rm * -rf
#../rtemscvs/configure --target=sparc64-rtems4.11 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=usiii
#../rtemscvs/configure --target=sparc64-rtems4.11 --disable-networking --enable-rtemsbsp=usiii --disable-posix
../rtemscvs/configure --target=sparc64-rtems4.11 --disable-networking --enable-rtemsbsp=usiii --enable-tests=samples
#../rtemscvs/configure --target=sparc64-rtems4.11 --disable-networking --enable-rtemsbsp=usiii --enable-tests

time make -j 4 2> make.error

sparc64-rtems4.11-objdump -d sparc64-rtems4.11/c/usiii/testsuites/samples/hello/hello.exe > hello.disasm

sparc64-rtems4.11-objdump -d sparc64-rtems4.11/c/usiii/testsuites/samples/ticker/ticker.exe > ticker.disasm

