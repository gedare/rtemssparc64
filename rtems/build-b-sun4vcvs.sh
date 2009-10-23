#/bin/sh

cd b-sun4vcvs
rm * -rf
#../rtemscvs/configure --target=sparc64-rtems4.10 --disable-posix --disable-itron --disable-networking --disable-multiprocessing --enable-rtemsbsp=sun4v
../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking

make