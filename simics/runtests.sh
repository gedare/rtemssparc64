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

