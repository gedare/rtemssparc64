#!/bin/sh

TMP=../../rtems/boot/isofiles
BASE=../../rtems/boot

#./buildtest.sh samples base_mp
./buildtest.sh samples base_sp
./buildtest.sh samples capture
#./buildtest.sh samples cdtest
./buildtest.sh samples fileio
./buildtest.sh samples hello
#./buildtest.sh samples iostream
#./buildtest.sh samples loopback
./buildtest.sh samples minimum
./buildtest.sh samples nsecs
./buildtest.sh samples paranoia
#./buildtest.sh samples pppd
./buildtest.sh samples ticker
./buildtest.sh samples unlimited


mkisofs -f -G $TMP/boot/isofs.b -B ... -r -o $BASE/image.iso $TMP
ls -l image.iso