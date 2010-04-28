#!/bin/sh

cd b-sparc64
rm * -rf

../rtems-sparc64/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4v --enable-tests

time make -j 2

