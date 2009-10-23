#/bin/sh
export PREFIX=/home/eugen/work/simics/rtems/compilers/sparc64-newlib-rtems4.10-4.4.1.gcc/sparc64/bin/sparc-rtems4.10
export AR=$PREFIX-ar
export AS=$PREFIX-as
export LD=$PREFIX-ld
export NM=$PREFIX-nm
export SIZE=$PREFIX-size


/home/eugen/work/simics/rtems/rtems/configure --target=sparc-rtems4.10 --enable-rtemsbsp=leon2 --disable-posix --disable-itron --disable-networking --disable-multiprocessing CC_FOR_BUILD=gcc CC_FOR_HOST=$PREFIX-gcc  CC_FOR_TARGET=$PREFIX-gcc
