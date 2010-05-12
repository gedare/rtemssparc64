#!/bin/bash

TMP=../../rtems/boot/isofiles
BASE=../../rtems/boot

./buildtest.sh mibench sha_small
./buildtest.sh mibench sha_large

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


#for ((  i = 1  ;  i <= 9;  i++  ))
#do 
#   ./buildtest.sh sptests sp0$i
#done
#for ((  i = 10 ;  i <= 35;  i++  ))
#do
#  ./buildtest.sh sptests sp$i
#done
#for ((  i = 37 ;  i <= 67;  i++  ))
#do
#  ./buildtest.sh sptests sp$i
#done
#./buildtest.sh sptests sp27a
#./buildtest.sh sptests spchain
#./buildtest.sh sptests spclockget
#./buildtest.sh sptests spcoverage
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest.sh sptests spfatal0$i
#done
#./buildtest.sh sptests spfatal10
#./buildtest.sh sptests spfatal11
#./buildtest.sh sptests spfatal12
##./buildtest.sh sptests spfatal_support
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest.sh sptests spintrcritical0$i
#done
#for ((  i = 10 ;  i <= 16;  i++  ))
#do 
#   ./buildtest.sh sptests spintrcritical$i
#done
##./buildtest.sh sptests spintrcritical_support
#./buildtest.sh sptests spnotepad01
#./buildtest.sh sptests spobjgetnext
#./buildtest.sh sptests spprintk
#./buildtest.sh sptests spsize
#./buildtest.sh sptests spstkalloc
#./buildtest.sh sptests spthreadq01
#./buildtest.sh sptests spwatchdog
#./buildtest.sh sptests spwkspace
#
#
## generate these by entering the appropriate directory under testsuites and 
## issuing the following command line:
##
##    ls -1 | sed 's/^/.\/buildtest.sh psxtests /'
##
#./buildtest.sh psxtests psx01
#./buildtest.sh psxtests psx02
#./buildtest.sh psxtests psx03
#./buildtest.sh psxtests psx04
#./buildtest.sh psxtests psx05
#./buildtest.sh psxtests psx06
#./buildtest.sh psxtests psx07
#./buildtest.sh psxtests psx08
#./buildtest.sh psxtests psx09
#./buildtest.sh psxtests psx10
#./buildtest.sh psxtests psx11
#./buildtest.sh psxtests psx12
#./buildtest.sh psxtests psx13
#./buildtest.sh psxtests psx14
#./buildtest.sh psxtests psxalarm01
#./buildtest.sh psxtests psxautoinit01
#./buildtest.sh psxtests psxautoinit02
#./buildtest.sh psxtests psxbarrier01
#./buildtest.sh psxtests psxcancel
#./buildtest.sh psxtests psxcancel01
#./buildtest.sh psxtests psxchroot01
#./buildtest.sh psxtests psxcleanup
#./buildtest.sh psxtests psxcleanup01
#./buildtest.sh psxtests psxclock
#./buildtest.sh psxtests psxcond01
#./buildtest.sh psxtests psxenosys
#./buildtest.sh psxtests psxfatal01
#./buildtest.sh psxtests psxfatal02
#./buildtest.sh psxtests psxfatal_support
#./buildtest.sh psxtests psxfile01
#./buildtest.sh psxtests psxhdrs
#./buildtest.sh psxtests psxintrcritical01
#./buildtest.sh psxtests psxitimer
#./buildtest.sh psxtests psxkey01
#./buildtest.sh psxtests psxkey02
#./buildtest.sh psxtests psxkey03
#./buildtest.sh psxtests psxmount
#./buildtest.sh psxtests psxmsgq01
#./buildtest.sh psxtests psxmsgq02
#./buildtest.sh psxtests psxmsgq03
#./buildtest.sh psxtests psxmsgq04
#./buildtest.sh psxtests psxmutexattr01
#./buildtest.sh psxtests psxobj01
#./buildtest.sh psxtests psxpasswd01
#./buildtest.sh psxtests psxrdwrv
#./buildtest.sh psxtests psxreaddir
#./buildtest.sh psxtests psxrwlock01
#./buildtest.sh psxtests psxsem01
#./buildtest.sh psxtests psxsignal01
#./buildtest.sh psxtests psxsignal02
#./buildtest.sh psxtests psxsignal03
#./buildtest.sh psxtests psxsignal04
#./buildtest.sh psxtests psxsignal05
#./buildtest.sh psxtests psxspin01
#./buildtest.sh psxtests psxspin02
#./buildtest.sh psxtests psxstack01
#./buildtest.sh psxtests psxstat
#./buildtest.sh psxtests psxsysconf
#./buildtest.sh psxtests psxtime
#./buildtest.sh psxtests psxtimer01
#./buildtest.sh psxtests psxtimer02
#./buildtest.sh psxtests psxualarm
#./buildtest.sh psxtests psxusleep
#

mkisofs -f -G $TMP/boot/isofs.b -B ... -iso-level 3 -r -o $BASE/image.iso $TMP
ls -l image.iso
