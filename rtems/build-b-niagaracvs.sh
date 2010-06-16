#!/bin/sh

cd b-niagaracvs
rm * -rf
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=niagara
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=niagara --disable-posix
../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=niagara --enable-tests=samples 

time make -j 4 2> make.error

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/niagara/testsuites/samples/hello/hello.exe > hello.disasm

sparc64-rtems4.10-objdump -d sparc64-rtems4.10/c/niagara/testsuites/samples/ticker/ticker.exe > ticker.disasm

