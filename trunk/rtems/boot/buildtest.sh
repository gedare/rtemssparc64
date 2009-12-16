#!/bin/sh



INCLUDE1=../../HeleonOS/HelenOS/boot/arch/sparc64/loader
INCLUDE2=../../HeleonOS/HelenOS/boot/generic
INCLUDE3=../../HeleonOS/HelenOS/boot/genarch
HELENONSBOOT=../../HeleonOS/HelenOS/boot/arch/sparc64/loader


TOOLCHAIN=../compilers/sparc64-newlib-rtems4.10-4.4.1.gcc.full/sparc64/bin/sparc64-rtems4.10


RTEMSTESTSBASE=../b-sun4vcvs/sparc64-rtems4.10/c/sun4v/testsuites
RTEMSTESTSAMPLESBASE=$RTEMSTESTSBASE/samples

TEST=$2

cp $RTEMSTESTSBASE/$1/$TEST/$TEST.exe image.1.boot
$TOOLCHAIN-objdump -D image.1.boot > debug/rtems/$1/$TEST.disasm
$TOOLCHAIN-nm image.1.boot | grep " T " > debug/rtems/$1/$TEST.fun.nm
$TOOLCHAIN-readelf -a image.1.boot > debug/rtems/$1/$TEST.elf
rm image.1.boot.gz
cp image.1.boot debug/rtems/$1/$TEST.exe
gzip image.1.boot 
cp image.1.boot.gz isofiles/rtems/$1/$TEST.gz -f

