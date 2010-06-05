#!/bin/bash

TMP=../../rtems/boot/usiii-isofiles
BASE=../../rtems/boot

./buildtest-usiii.sh mibench sha_small
./buildtest-usiii.sh mibench sha_large

#./buildtest-usiii.sh samples base_mp
./buildtest-usiii.sh samples base_sp
./buildtest-usiii.sh samples capture
#./buildtest-usiii.sh samples cdtest
./buildtest-usiii.sh samples fileio
./buildtest-usiii.sh samples hello
#./buildtest-usiii.sh samples iostream
#./buildtest-usiii.sh samples loopback
./buildtest-usiii.sh samples minimum
./buildtest-usiii.sh samples nsecs
./buildtest-usiii.sh samples paranoia
#./buildtest-usiii.sh samples pppd
./buildtest-usiii.sh samples ticker
./buildtest-usiii.sh samples unlimited


#for ((  i = 1  ;  i <= 9;  i++  ))
#do 
#   ./buildtest-usiii.sh sptests sp0$i
#done
#for ((  i = 10 ;  i <= 35;  i++  ))
#do
#  ./buildtest-usiii.sh sptests sp$i
#done
#for ((  i = 37 ;  i <= 67;  i++  ))
#do
#  ./buildtest-usiii.sh sptests sp$i
#done
#./buildtest-usiii.sh sptests sp27a
#./buildtest-usiii.sh sptests spchain
#./buildtest-usiii.sh sptests spclockget
#./buildtest-usiii.sh sptests spcoverage
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest-usiii.sh sptests spfatal0$i
#done
#./buildtest-usiii.sh sptests spfatal10
#./buildtest-usiii.sh sptests spfatal11
#./buildtest-usiii.sh sptests spfatal12
##./buildtest-usiii.sh sptests spfatal_support
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest-usiii.sh sptests spintrcritical0$i
#done
#for ((  i = 10 ;  i <= 16;  i++  ))
#do 
#   ./buildtest-usiii.sh sptests spintrcritical$i
#done
##./buildtest-usiii.sh sptests spintrcritical_support
#./buildtest-usiii.sh sptests spnotepad01
#./buildtest-usiii.sh sptests spobjgetnext
#./buildtest-usiii.sh sptests spprintk
#./buildtest-usiii.sh sptests spsize
#./buildtest-usiii.sh sptests spstkalloc
#./buildtest-usiii.sh sptests spthreadq01
#./buildtest-usiii.sh sptests spwatchdog
#./buildtest-usiii.sh sptests spwkspace
#
#
## generate these by entering the appropriate directory under testsuites and 
## issuing the following command line:
##
##    ls -1 | sed 's/^/.\/buildtest-usiii.sh psxtests /'
##
#./buildtest-usiii.sh psxtests psx01
#./buildtest-usiii.sh psxtests psx02
#./buildtest-usiii.sh psxtests psx03
#./buildtest-usiii.sh psxtests psx04
#./buildtest-usiii.sh psxtests psx05
#./buildtest-usiii.sh psxtests psx06
#./buildtest-usiii.sh psxtests psx07
#./buildtest-usiii.sh psxtests psx08
#./buildtest-usiii.sh psxtests psx09
#./buildtest-usiii.sh psxtests psx10
#./buildtest-usiii.sh psxtests psx11
#./buildtest-usiii.sh psxtests psx12
#./buildtest-usiii.sh psxtests psx13
#./buildtest-usiii.sh psxtests psx14
#./buildtest-usiii.sh psxtests psxalarm01
#./buildtest-usiii.sh psxtests psxautoinit01
#./buildtest-usiii.sh psxtests psxautoinit02
#./buildtest-usiii.sh psxtests psxbarrier01
#./buildtest-usiii.sh psxtests psxcancel
#./buildtest-usiii.sh psxtests psxcancel01
#./buildtest-usiii.sh psxtests psxchroot01
#./buildtest-usiii.sh psxtests psxcleanup
#./buildtest-usiii.sh psxtests psxcleanup01
#./buildtest-usiii.sh psxtests psxclock
#./buildtest-usiii.sh psxtests psxcond01
#./buildtest-usiii.sh psxtests psxenosys
#./buildtest-usiii.sh psxtests psxfatal01
#./buildtest-usiii.sh psxtests psxfatal02
#./buildtest-usiii.sh psxtests psxfatal_support
#./buildtest-usiii.sh psxtests psxfile01
#./buildtest-usiii.sh psxtests psxhdrs
#./buildtest-usiii.sh psxtests psxintrcritical01
#./buildtest-usiii.sh psxtests psxitimer
#./buildtest-usiii.sh psxtests psxkey01
#./buildtest-usiii.sh psxtests psxkey02
#./buildtest-usiii.sh psxtests psxkey03
#./buildtest-usiii.sh psxtests psxmount
#./buildtest-usiii.sh psxtests psxmsgq01
#./buildtest-usiii.sh psxtests psxmsgq02
#./buildtest-usiii.sh psxtests psxmsgq03
#./buildtest-usiii.sh psxtests psxmsgq04
#./buildtest-usiii.sh psxtests psxmutexattr01
#./buildtest-usiii.sh psxtests psxobj01
#./buildtest-usiii.sh psxtests psxpasswd01
#./buildtest-usiii.sh psxtests psxrdwrv
#./buildtest-usiii.sh psxtests psxreaddir
#./buildtest-usiii.sh psxtests psxrwlock01
#./buildtest-usiii.sh psxtests psxsem01
#./buildtest-usiii.sh psxtests psxsignal01
#./buildtest-usiii.sh psxtests psxsignal02
#./buildtest-usiii.sh psxtests psxsignal03
#./buildtest-usiii.sh psxtests psxsignal04
#./buildtest-usiii.sh psxtests psxsignal05
#./buildtest-usiii.sh psxtests psxspin01
#./buildtest-usiii.sh psxtests psxspin02
#./buildtest-usiii.sh psxtests psxstack01
#./buildtest-usiii.sh psxtests psxstat
#./buildtest-usiii.sh psxtests psxsysconf
#./buildtest-usiii.sh psxtests psxtime
#./buildtest-usiii.sh psxtests psxtimer01
#./buildtest-usiii.sh psxtests psxtimer02
#./buildtest-usiii.sh psxtests psxualarm
#./buildtest-usiii.sh psxtests psxusleep
#

mkisofs -f -G $TMP/boot/isofs.b -B ... -r -o $BASE/image.iso $TMP
ls -l image.iso
