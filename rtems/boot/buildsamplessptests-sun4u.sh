#!/bin/bash

TMP=../../rtems/boot/sun4u-isofiles
BASE=../../rtems/boot

./buildtest-sun4u.sh mibench sha_small
./buildtest-sun4u.sh mibench sha_large

#./buildtest-sun4u.sh samples base_mp
./buildtest-sun4u.sh samples base_sp
./buildtest-sun4u.sh samples capture
#./buildtest-sun4u.sh samples cdtest
./buildtest-sun4u.sh samples fileio
./buildtest-sun4u.sh samples hello
#./buildtest-sun4u.sh samples iostream
#./buildtest-sun4u.sh samples loopback
./buildtest-sun4u.sh samples minimum
./buildtest-sun4u.sh samples nsecs
./buildtest-sun4u.sh samples paranoia
#./buildtest-sun4u.sh samples pppd
./buildtest-sun4u.sh samples ticker
./buildtest-sun4u.sh samples unlimited


#for ((  i = 1  ;  i <= 9;  i++  ))
#do 
#   ./buildtest-sun4u.sh sptests sp0$i
#done
#for ((  i = 10 ;  i <= 35;  i++  ))
#do
#  ./buildtest-sun4u.sh sptests sp$i
#done
#for ((  i = 37 ;  i <= 67;  i++  ))
#do
#  ./buildtest-sun4u.sh sptests sp$i
#done
#./buildtest-sun4u.sh sptests sp27a
#./buildtest-sun4u.sh sptests spchain
#./buildtest-sun4u.sh sptests spclockget
#./buildtest-sun4u.sh sptests spcoverage
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest-sun4u.sh sptests spfatal0$i
#done
#./buildtest-sun4u.sh sptests spfatal10
#./buildtest-sun4u.sh sptests spfatal11
#./buildtest-sun4u.sh sptests spfatal12
##./buildtest-sun4u.sh sptests spfatal_support
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest-sun4u.sh sptests spintrcritical0$i
#done
#for ((  i = 10 ;  i <= 16;  i++  ))
#do 
#   ./buildtest-sun4u.sh sptests spintrcritical$i
#done
##./buildtest-sun4u.sh sptests spintrcritical_support
#./buildtest-sun4u.sh sptests spnotepad01
#./buildtest-sun4u.sh sptests spobjgetnext
#./buildtest-sun4u.sh sptests spprintk
#./buildtest-sun4u.sh sptests spsize
#./buildtest-sun4u.sh sptests spstkalloc
#./buildtest-sun4u.sh sptests spthreadq01
#./buildtest-sun4u.sh sptests spwatchdog
#./buildtest-sun4u.sh sptests spwkspace
#
#
## generate these by entering the appropriate directory under testsuites and 
## issuing the following command line:
##
##    ls -1 | sed 's/^/.\/buildtest-sun4u.sh psxtests /'
##
#./buildtest-sun4u.sh psxtests psx01
#./buildtest-sun4u.sh psxtests psx02
#./buildtest-sun4u.sh psxtests psx03
#./buildtest-sun4u.sh psxtests psx04
#./buildtest-sun4u.sh psxtests psx05
#./buildtest-sun4u.sh psxtests psx06
#./buildtest-sun4u.sh psxtests psx07
#./buildtest-sun4u.sh psxtests psx08
#./buildtest-sun4u.sh psxtests psx09
#./buildtest-sun4u.sh psxtests psx10
#./buildtest-sun4u.sh psxtests psx11
#./buildtest-sun4u.sh psxtests psx12
#./buildtest-sun4u.sh psxtests psx13
#./buildtest-sun4u.sh psxtests psx14
#./buildtest-sun4u.sh psxtests psxalarm01
#./buildtest-sun4u.sh psxtests psxautoinit01
#./buildtest-sun4u.sh psxtests psxautoinit02
#./buildtest-sun4u.sh psxtests psxbarrier01
#./buildtest-sun4u.sh psxtests psxcancel
#./buildtest-sun4u.sh psxtests psxcancel01
#./buildtest-sun4u.sh psxtests psxchroot01
#./buildtest-sun4u.sh psxtests psxcleanup
#./buildtest-sun4u.sh psxtests psxcleanup01
#./buildtest-sun4u.sh psxtests psxclock
#./buildtest-sun4u.sh psxtests psxcond01
#./buildtest-sun4u.sh psxtests psxenosys
#./buildtest-sun4u.sh psxtests psxfatal01
#./buildtest-sun4u.sh psxtests psxfatal02
#./buildtest-sun4u.sh psxtests psxfatal_support
#./buildtest-sun4u.sh psxtests psxfile01
#./buildtest-sun4u.sh psxtests psxhdrs
#./buildtest-sun4u.sh psxtests psxintrcritical01
#./buildtest-sun4u.sh psxtests psxitimer
#./buildtest-sun4u.sh psxtests psxkey01
#./buildtest-sun4u.sh psxtests psxkey02
#./buildtest-sun4u.sh psxtests psxkey03
#./buildtest-sun4u.sh psxtests psxmount
#./buildtest-sun4u.sh psxtests psxmsgq01
#./buildtest-sun4u.sh psxtests psxmsgq02
#./buildtest-sun4u.sh psxtests psxmsgq03
#./buildtest-sun4u.sh psxtests psxmsgq04
#./buildtest-sun4u.sh psxtests psxmutexattr01
#./buildtest-sun4u.sh psxtests psxobj01
#./buildtest-sun4u.sh psxtests psxpasswd01
#./buildtest-sun4u.sh psxtests psxrdwrv
#./buildtest-sun4u.sh psxtests psxreaddir
#./buildtest-sun4u.sh psxtests psxrwlock01
#./buildtest-sun4u.sh psxtests psxsem01
#./buildtest-sun4u.sh psxtests psxsignal01
#./buildtest-sun4u.sh psxtests psxsignal02
#./buildtest-sun4u.sh psxtests psxsignal03
#./buildtest-sun4u.sh psxtests psxsignal04
#./buildtest-sun4u.sh psxtests psxsignal05
#./buildtest-sun4u.sh psxtests psxspin01
#./buildtest-sun4u.sh psxtests psxspin02
#./buildtest-sun4u.sh psxtests psxstack01
#./buildtest-sun4u.sh psxtests psxstat
#./buildtest-sun4u.sh psxtests psxsysconf
#./buildtest-sun4u.sh psxtests psxtime
#./buildtest-sun4u.sh psxtests psxtimer01
#./buildtest-sun4u.sh psxtests psxtimer02
#./buildtest-sun4u.sh psxtests psxualarm
#./buildtest-sun4u.sh psxtests psxusleep
#

mkisofs -f -G $TMP/boot/isofs.b -B ... -r -o $BASE/image.iso $TMP
ls -l image.iso
