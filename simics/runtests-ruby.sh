#!/bin/bash


if [[ $# -eq 0 ]]
then
echo "Error - parameter missing"
echo "Syntax : $0 path_to_iso path_to_source path_to_symbols "
echo "example : $0 /svn/rtemssparc64/rtems/boot/image.iso /svn/rtemssparc64/ /svn/rtemssparc64/rtems/boot/debug/rtems/"
exit 1
fi

OUTDIR=`date +%m%d%y-%H%M`
mkdir $OUTDIR

ISOFILE=$1
SOURCE=$2
SYMBOLS=$3

./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples base_sp ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples base_mp ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples base_sp ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples capture ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples cdtest ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples fileio ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples hello ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples iostream ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples loopback ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples minimum ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples nsecs ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples paranoia ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples pppd ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples ticker ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE samples unlimited ${SOURCE} ${SYMBOLS}

for ((  i = 1  ;  i <= 9;  i++  ))
do
	./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests sp0$i ${SOURCE} ${SYMBOLS}

done
for ((  i = 10 ;  i <= 35;  i++  ))
do
	./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests sp$i ${SOURCE} ${SYMBOLS}

done
for ((  i = 37 ;  i <= 67;  i++  ))
do
	./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests sp$i ${SOURCE} ${SYMBOLS}

done
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests sp27a ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spchain ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spclockget ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spcoverage ${SOURCE} ${SYMBOLS}
 
for ((  i = 1 ;  i <= 9;  i++  )) ${SOURCE} ${SYMBOLS}
do 
	./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spfatal0$i ${SOURCE} ${SYMBOLS}
done 
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spfatal10 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spfatal11 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spfatal12 ${SOURCE} ${SYMBOLS}
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spfatal_support ${SOURCE} ${SYMBOLS}
for ((  i = 1 ;  i <= 9;  i++  )) ${SOURCE} ${SYMBOLS}
do 
	./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spintrcritical0$i ${SOURCE} ${SYMBOLS}
done 
for ((  i = 10 ;  i <= 16;  i++  )) ${SOURCE} ${SYMBOLS}
do 
	./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spintrcritical$i ${SOURCE} ${SYMBOLS}
done 
#./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spintrcritical_support ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spnotepad01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spobjgetnext ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spprintk ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spsize ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spstkalloc ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spthreadq01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spwatchdog ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE sptests spwkspace ${SOURCE} ${SYMBOLS}

./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx03 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx04 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx05 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx06 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx07 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx08 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx09 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx10 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx11 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx12 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx13 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psx14 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxalarm01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxautoinit01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxautoinit02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxbarrier01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxcancel ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxcancel01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxchroot01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxcleanup ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxcleanup01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxclock ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxcond01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxenosys ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxfatal01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxfatal02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxfatal_support ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxfile01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxhdrs ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxintrcritical01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxitimer ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxkey01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxkey02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxkey03 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxmount ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxmsgq01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxmsgq02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxmsgq03 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxmsgq04 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxmutexattr01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxobj01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxpasswd01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxrdwrv ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxreaddir ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxrwlock01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsem01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsignal01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsignal02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsignal03 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsignal04 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsignal05 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxspin01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxspin02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxstack01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxstat ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxsysconf ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxtime ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxtimer01 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxtimer02 ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxualarm ${SOURCE} ${SYMBOLS}
./runRTEMStest-ruby.sh ${OUTDIR} $ISOFILE psxtests psxusleep ${SOURCE} ${SYMBOLS}
