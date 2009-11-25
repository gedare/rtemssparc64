#!/bin/sh



INCLUDE1=../../HeleonOS/HelenOS/boot/arch/sparc64/loader
INCLUDE2=../../HeleonOS/HelenOS/boot/generic
INCLUDE3=../../HeleonOS/HelenOS/boot/genarch
HELENONSBOOT=../../HeleonOS/HelenOS/boot/arch/sparc64/loader

TOOLCHAINRTEMS=../../../rtems/rtems4.10/bin/sparc-rtems4.10
TOOLCHAINHELENOS=../compilers/sparc64-newlib-rtems4.10-4.4.1.gcc.full/sparc64/bin/sparc64-rtems4.10
#TOOLCHAINLINUX=/home/eugen/work/simics/sparc64crosscompiler/sparc64/bin/sparc64-linux-gnu

LINKSCRIPT32=_link32.ld
LINKSCRIPT64=../../HeleonOS/HelenOS/boot/arch/sparc64/loader/_link.ld


RTEMSTESTSBASE=../b-sun4vcvs/sparc64-rtems4.10/c/sun4v/testsuites
RTEMSTESTSAMPLESBASE=$RTEMSTESTSBASE/samples

TEST=$2

cp $RTEMSTESTSBASE/$1/$TEST/$TEST.exe image.1.boot
$TOOLCHAINHELENOS-objdump -D image.1.boot > debug/rtems/$1/$TEST.disasm
$TOOLCHAINHELENOS-readelf -a image.1.boot > debug/rtems/$1/$TEST.elf
rm image.1.boot.gz
cp image.1.boot debug/rtems/$1/$TEST.exe
gzip image.1.boot 
cp image.1.boot.gz isofiles/rtems/$1/$TEST.gz -f

