#/bin/sh
#export PREFIX=/home/eugen/work/simics/rtems/compilers/sparc64-newlib-rtems4.10-4.4.1.gcc/sparc64/bin/sparc-rtems4.10
#export AR=$PREFIX-ar
#export AS=$PREFIX-as
#export LD=$PREFIX-ld
#export NM=$PREFIX-nm
#export SIZE=$PREFIX-size

#export PATH=$PATH:/home/eugen/work/simics/rtems/compilers/sparc64-newlib-rtems4.10-4.4.1.gcc.full/sparc64/bin

/home/eugen/work/simics/rtems/rtems/configure --target=sparc64-rtems4.10 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=sun4v
