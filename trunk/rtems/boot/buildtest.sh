#!/bin/sh

excludes=( \
_fini \
sparc_disable_interrupts \
_CPU_Context_restore \
_fclose_r \
_fflush_r \
_fwalk \
_puts_r \
__sclose \
__swrite \
__sfvwrite_r \
__swsetup_r \
__smakebuf_r \
syscall \
_isatty \
_exit \
)

removes=( \
'T start' \
'T silo_ramdisk_image' \
'T silo_ramdisk_size' \
't zerobss' \
't setstack' \
't halted' \
't param_space' \
't initial_stack' \
't initial_stack_top' \
'T _fini' \
'T _init' \
'T __sfp_lock_acquire' \
'T __sfp_lock_release' \
'T __sinit_lock_acquire' \
'T __sinit_lock_release' \
)

INCLUDE1=../../HeleonOS/HelenOS/boot/arch/sparc64/loader
INCLUDE2=../../HeleonOS/HelenOS/boot/generic
INCLUDE3=../../HeleonOS/HelenOS/boot/genarch
HELENONSBOOT=../../HeleonOS/HelenOS/boot/arch/sparc64/loader


TOOLCHAIN=../compilers/sparc64-newlib-rtems4.10-4.4.1.gcc.full/sparc64/bin/sparc64-rtems4.10


RTEMSTESTSBASE=../b-test/sparc64-rtems4.10/c/sun4v/testsuites
RTEMSTESTSAMPLESBASE=$RTEMSTESTSBASE/samples

TEST=$2

cp $RTEMSTESTSBASE/$1/$TEST/$TEST.exe image.1.boot
$TOOLCHAIN-objdump -D image.1.boot > debug/rtems/$1/$TEST.disasm
$TOOLCHAIN-nm --numeric-sort --line-numbers image.1.boot | grep -i " T " > debug/rtems/$1/$TEST.fun.nm
#$TOOLCHAIN-nm --numeric-sort image.1.boot | grep " T " > debug/rtems/$1/$TEST.fun.nm

#remove unwanted symbols
for (( i = 0 ; i < ${#removes[@]} ; i++ ))
do
cat debug/rtems/$1/$TEST.fun.nm | grep -v " ${removes[$i]}" > debug/rtems/$1/$TEST.fun.nm.temp
mv debug/rtems/$1/$TEST.fun.nm.temp debug/rtems/$1/$TEST.fun.nm
done


#mark un-function symbols
for (( i = 0 ; i < ${#excludes[@]} ; i++ ))
do
#echo ${excludes[$i]}
#cat debug/rtems/$1/$TEST.fun.nm | sed 's/${excludes[$i]}/*&/' > debug/rtems/$1/$TEST.fun.nm.temp
cat debug/rtems/$1/$TEST.fun.nm | sed 's/ '${excludes[$i]}'/ *'${excludes[$i]}'/' > debug/rtems/$1/$TEST.fun.nm.temp
mv debug/rtems/$1/$TEST.fun.nm.temp debug/rtems/$1/$TEST.fun.nm
done



$TOOLCHAIN-readelf -a image.1.boot > debug/rtems/$1/$TEST.elf
rm image.1.boot.gz
cp image.1.boot debug/rtems/$1/$TEST.exe
gzip image.1.boot 
cp image.1.boot.gz isofiles/rtems/$1/$TEST.gz -f

