#!/bin/sh

cd b-sparc64
rm * -rf

../rtemscvs/configure --target=sparc64-rtems4.10 --disable-networking --enable-rtemsbsp=sun4v --enable-tests

time make -j 2

