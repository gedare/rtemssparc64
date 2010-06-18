#!/bin/bash

TMP=`pwd`/niagara-isofiles
BASE=`pwd`

./buildtest-niagara.sh mibench sha_small
./buildtest-niagara.sh mibench sha_large

#./buildtest-niagara.sh samples base_mp
./buildtest-niagara.sh samples base_sp
./buildtest-niagara.sh samples capture
#./buildtest-niagara.sh samples cdtest
./buildtest-niagara.sh samples fileio
./buildtest-niagara.sh samples hello
#./buildtest-niagara.sh samples iostream
#./buildtest-niagara.sh samples loopback
./buildtest-niagara.sh samples minimum
./buildtest-niagara.sh samples nsecs
./buildtest-niagara.sh samples paranoia
#./buildtest-niagara.sh samples pppd
./buildtest-niagara.sh samples ticker
./buildtest-niagara.sh samples unlimited


#for ((  i = 1  ;  i <= 9;  i++  ))
#do 
#   ./buildtest-niagara.sh sptests sp0$i
#done
#for ((  i = 10 ;  i <= 35;  i++  ))
#do
#  ./buildtest-niagara.sh sptests sp$i
#done
#for ((  i = 37 ;  i <= 67;  i++  ))
#do
#  ./buildtest-niagara.sh sptests sp$i
#done
#./buildtest-niagara.sh sptests sp27a
#./buildtest-niagara.sh sptests spchain
#./buildtest-niagara.sh sptests spclockget
#./buildtest-niagara.sh sptests spcoverage
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest-niagara.sh sptests spfatal0$i
#done
#./buildtest-niagara.sh sptests spfatal10
#./buildtest-niagara.sh sptests spfatal11
#./buildtest-niagara.sh sptests spfatal12
##./buildtest-niagara.sh sptests spfatal_support
#for ((  i = 1 ;  i <= 9;  i++  ))
#do 
#   ./buildtest-niagara.sh sptests spintrcritical0$i
#done
#for ((  i = 10 ;  i <= 16;  i++  ))
#do 
#   ./buildtest-niagara.sh sptests spintrcritical$i
#done
##./buildtest-niagara.sh sptests spintrcritical_support
#./buildtest-niagara.sh sptests spnotepad01
#./buildtest-niagara.sh sptests spobjgetnext
#./buildtest-niagara.sh sptests spprintk
#./buildtest-niagara.sh sptests spsize
#./buildtest-niagara.sh sptests spstkalloc
#./buildtest-niagara.sh sptests spthreadq01
#./buildtest-niagara.sh sptests spwatchdog
#./buildtest-niagara.sh sptests spwkspace
#
#
## generate these by entering the appropriate directory under testsuites and 
## issuing the following command line:
##
##    ls -1 | sed 's/^/.\/buildtest-niagara.sh psxtests /'
##
#./buildtest-niagara.sh psxtests psx01
#./buildtest-niagara.sh psxtests psx02
#./buildtest-niagara.sh psxtests psx03
#./buildtest-niagara.sh psxtests psx04
#./buildtest-niagara.sh psxtests psx05
#./buildtest-niagara.sh psxtests psx06
#./buildtest-niagara.sh psxtests psx07
#./buildtest-niagara.sh psxtests psx08
#./buildtest-niagara.sh psxtests psx09
#./buildtest-niagara.sh psxtests psx10
#./buildtest-niagara.sh psxtests psx11
#./buildtest-niagara.sh psxtests psx12
#./buildtest-niagara.sh psxtests psx13
#./buildtest-niagara.sh psxtests psx14
#./buildtest-niagara.sh psxtests psxalarm01
#./buildtest-niagara.sh psxtests psxautoinit01
#./buildtest-niagara.sh psxtests psxautoinit02
#./buildtest-niagara.sh psxtests psxbarrier01
#./buildtest-niagara.sh psxtests psxcancel
#./buildtest-niagara.sh psxtests psxcancel01
#./buildtest-niagara.sh psxtests psxchroot01
#./buildtest-niagara.sh psxtests psxcleanup
#./buildtest-niagara.sh psxtests psxcleanup01
#./buildtest-niagara.sh psxtests psxclock
#./buildtest-niagara.sh psxtests psxcond01
#./buildtest-niagara.sh psxtests psxenosys
#./buildtest-niagara.sh psxtests psxfatal01
#./buildtest-niagara.sh psxtests psxfatal02
#./buildtest-niagara.sh psxtests psxfatal_support
#./buildtest-niagara.sh psxtests psxfile01
#./buildtest-niagara.sh psxtests psxhdrs
#./buildtest-niagara.sh psxtests psxintrcritical01
#./buildtest-niagara.sh psxtests psxitimer
#./buildtest-niagara.sh psxtests psxkey01
#./buildtest-niagara.sh psxtests psxkey02
#./buildtest-niagara.sh psxtests psxkey03
#./buildtest-niagara.sh psxtests psxmount
#./buildtest-niagara.sh psxtests psxmsgq01
#./buildtest-niagara.sh psxtests psxmsgq02
#./buildtest-niagara.sh psxtests psxmsgq03
#./buildtest-niagara.sh psxtests psxmsgq04
#./buildtest-niagara.sh psxtests psxmutexattr01
#./buildtest-niagara.sh psxtests psxobj01
#./buildtest-niagara.sh psxtests psxpasswd01
#./buildtest-niagara.sh psxtests psxrdwrv
#./buildtest-niagara.sh psxtests psxreaddir
#./buildtest-niagara.sh psxtests psxrwlock01
#./buildtest-niagara.sh psxtests psxsem01
#./buildtest-niagara.sh psxtests psxsignal01
#./buildtest-niagara.sh psxtests psxsignal02
#./buildtest-niagara.sh psxtests psxsignal03
#./buildtest-niagara.sh psxtests psxsignal04
#./buildtest-niagara.sh psxtests psxsignal05
#./buildtest-niagara.sh psxtests psxspin01
#./buildtest-niagara.sh psxtests psxspin02
#./buildtest-niagara.sh psxtests psxstack01
#./buildtest-niagara.sh psxtests psxstat
#./buildtest-niagara.sh psxtests psxsysconf
#./buildtest-niagara.sh psxtests psxtime
#./buildtest-niagara.sh psxtests psxtimer01
#./buildtest-niagara.sh psxtests psxtimer02
#./buildtest-niagara.sh psxtests psxualarm
#./buildtest-niagara.sh psxtests psxusleep
#

mkisofs -f -G $TMP/boot/isofs.b -B ... -iso-level 3 -r -o $BASE/image.iso $TMP
ls -l image.iso
