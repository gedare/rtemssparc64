#!/bin/sh

cd b-sun4vcvs
rm * -rf
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=sun4v
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4v --disable-posix
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4v --enable-tests=samples
../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4v --enable-tests
make

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/sun4v/testsuites/samples/hello/hello.exe > hello.disasm

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/sun4v/testsuites/samples/ticker/ticker.exe > ticker.disasm

