#!/bin/bash


if [[ $# -eq 0 ]]
then
echo "Error - parameter missing"
echo "Syntax : $0 path_to_iso "
echo "example : $0 /svn/rtemssparc64/rtems/boot/image.iso"
exit 1
fi

ISOFILE=$1

./runRTEMStest.sh $ISOFILE base_sp
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE base_mp
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE base_sp
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE capture
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE cdtest
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE fileio
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE hello
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE iostream
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE loopback
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE minimum
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE nsecs
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE paranoia
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE pppd
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE ticker
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE unlimited
if [ $? -ne 0 ]; then
	exit -1
fi

for ((  i = 1  ;  i <= 9;  i++  ))
do
	./runRTEMStest.sh $ISOFILE sp0$i
if [ $? -ne 0 ]; then
	exit -1
fi
done
for ((  i = 10 ;  i <= 35;  i++  ))
do
	./runRTEMStest.sh $ISOFILE sp$i
if [ $? -ne 0 ]; then
	exit -1
fi
done
for ((  i = 37 ;  i <= 67;  i++  ))
do
	./runRTEMStest.sh $ISOFILE sp$i
if [ $? -ne 0 ]; then
	exit -1
fi
done
./runRTEMStest.sh $ISOFILE sp27a
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spchain
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spclockget
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spcoverage
if [ $? -ne 0 ]; then
	exit -1
fi

for ((  i = 1 ;  i <= 9;  i++  ))
do
	./runRTEMStest.sh $ISOFILE spfatal0$i
if [ $? -ne 0 ]; then
	exit -1
fi
done
./runRTEMStest.sh $ISOFILE spfatal10
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spfatal11
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spfatal12
if [ $? -ne 0 ]; then
	exit -1
fi
#./runRTEMStest.sh $ISOFILE spfatal_support
if [ $? -ne 0 ]; then
	exit -1
fi
for ((  i = 1 ;  i <= 9;  i++  ))
do
	./runRTEMStest.sh $ISOFILE spintrcritical0$i
if [ $? -ne 0 ]; then
	exit -1
fi
done
for ((  i = 10 ;  i <= 16;  i++  ))
do
	./runRTEMStest.sh $ISOFILE spintrcritical$i
if [ $? -ne 0 ]; then
	exit -1
fi
done
#./runRTEMStest.sh $ISOFILE spintrcritical_support
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spnotepad01
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spobjgetnext
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spprintk
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spsize
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spstkalloc
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spthreadq01
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spwatchdog
if [ $? -ne 0 ]; then
	exit -1
fi
./runRTEMStest.sh $ISOFILE spwkspace
if [ $? -ne 0 ]; then
	exit -1
fi

./runRTEMStest.sh $ISOFILE psx01
./runRTEMStest.sh $ISOFILE psx02
./runRTEMStest.sh $ISOFILE psx03
./runRTEMStest.sh $ISOFILE psx04
./runRTEMStest.sh $ISOFILE psx05
./runRTEMStest.sh $ISOFILE psx06
./runRTEMStest.sh $ISOFILE psx07
./runRTEMStest.sh $ISOFILE psx08
./runRTEMStest.sh $ISOFILE psx09
./runRTEMStest.sh $ISOFILE psx10
./runRTEMStest.sh $ISOFILE psx11
./runRTEMStest.sh $ISOFILE psx12
./runRTEMStest.sh $ISOFILE psx13
./runRTEMStest.sh $ISOFILE psx14
./runRTEMStest.sh $ISOFILE psxalarm01
./runRTEMStest.sh $ISOFILE psxautoinit01
./runRTEMStest.sh $ISOFILE psxautoinit02
./runRTEMStest.sh $ISOFILE psxbarrier01
./runRTEMStest.sh $ISOFILE psxcancel
./runRTEMStest.sh $ISOFILE psxcancel01
./runRTEMStest.sh $ISOFILE psxchroot01
./runRTEMStest.sh $ISOFILE psxcleanup
./runRTEMStest.sh $ISOFILE psxcleanup01
./runRTEMStest.sh $ISOFILE psxclock
./runRTEMStest.sh $ISOFILE psxcond01
./runRTEMStest.sh $ISOFILE psxenosys
./runRTEMStest.sh $ISOFILE psxfatal01
./runRTEMStest.sh $ISOFILE psxfatal02
./runRTEMStest.sh $ISOFILE psxfatal_support
./runRTEMStest.sh $ISOFILE psxfile01
./runRTEMStest.sh $ISOFILE psxhdrs
./runRTEMStest.sh $ISOFILE psxintrcritical01
./runRTEMStest.sh $ISOFILE psxitimer
./runRTEMStest.sh $ISOFILE psxkey01
./runRTEMStest.sh $ISOFILE psxkey02
./runRTEMStest.sh $ISOFILE psxkey03
./runRTEMStest.sh $ISOFILE psxmount
./runRTEMStest.sh $ISOFILE psxmsgq01
./runRTEMStest.sh $ISOFILE psxmsgq02
./runRTEMStest.sh $ISOFILE psxmsgq03
./runRTEMStest.sh $ISOFILE psxmsgq04
./runRTEMStest.sh $ISOFILE psxmutexattr01
./runRTEMStest.sh $ISOFILE psxobj01
./runRTEMStest.sh $ISOFILE psxpasswd01
./runRTEMStest.sh $ISOFILE psxrdwrv
./runRTEMStest.sh $ISOFILE psxreaddir
./runRTEMStest.sh $ISOFILE psxrwlock01
./runRTEMStest.sh $ISOFILE psxsem01
./runRTEMStest.sh $ISOFILE psxsignal01
./runRTEMStest.sh $ISOFILE psxsignal02
./runRTEMStest.sh $ISOFILE psxsignal03
./runRTEMStest.sh $ISOFILE psxsignal04
./runRTEMStest.sh $ISOFILE psxsignal05
./runRTEMStest.sh $ISOFILE psxspin01
./runRTEMStest.sh $ISOFILE psxspin02
./runRTEMStest.sh $ISOFILE psxstack01
./runRTEMStest.sh $ISOFILE psxstat
./runRTEMStest.sh $ISOFILE psxsysconf
./runRTEMStest.sh $ISOFILE psxtime
./runRTEMStest.sh $ISOFILE psxtimer01
./runRTEMStest.sh $ISOFILE psxtimer02
./runRTEMStest.sh $ISOFILE psxualarm
./runRTEMStest.sh $ISOFILE psxusleep
