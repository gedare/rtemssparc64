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


for ((  i = 1 ;  i <= 9;  i++  ))
do 
   ./buildtest.sh sptests sp0$i
done
for ((  i = 10 ;  i <= 35;  i++  ))
do
  ./buildtest.sh sptests sp$i
done
for ((  i = 37 ;  i <= 67;  i++  ))
do
  ./buildtest.sh sptests sp$i
done
./buildtest.sh sptests sp27a
./buildtest.sh sptests spchain
./buildtest.sh sptests spclockget
./buildtest.sh sptests spcoverage
for ((  i = 1 ;  i <= 9;  i++  ))
do 
   ./buildtest.sh sptests spfatal0$i
done
./buildtest.sh sptests spfatal10
./buildtest.sh sptests spfatal11
./buildtest.sh sptests spfatal12
#./buildtest.sh sptests spfatal_support
for ((  i = 1 ;  i <= 9;  i++  ))
do 
   ./buildtest.sh sptests spintrcritical0$i
done
for ((  i = 10 ;  i <= 16;  i++  ))
do 
   ./buildtest.sh sptests spintrcritical$i
done
#./buildtest.sh sptests spintrcritical_support
./buildtest.sh sptests spnotepad01
./buildtest.sh sptests spobjgetnext
./buildtest.sh sptests spprintk
./buildtest.sh sptests spsize
./buildtest.sh sptests spstkalloc
./buildtest.sh sptests spthreadq01
./buildtest.sh sptests spwatchdog
./buildtest.sh sptests spwkspace

mkisofs -f -G $TMP/boot/isofs.b -B ... -iso-level 3 -r -o $BASE/image.iso $TMP
ls -l image.iso