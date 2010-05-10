#!/bin/sh

cd b-sun4ucvs
rm * -rf
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=sun4u
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4u --disable-posix
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4u --enable-tests=samples
../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4u --enable-tests

time make -j 4 2> make.error

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/sun4u/testsuites/samples/hello/hello.exe > hello.disasm

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/sun4u/testsuites/samples/ticker/ticker.exe > ticker.disasm

