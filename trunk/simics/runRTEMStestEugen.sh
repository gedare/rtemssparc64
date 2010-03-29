#!/bin/bash

if [[ $# -eq 0 || $# -eq 1 ]]
then
echo "Error - parameters missing"
echo "Syntax : $0 path_to_iso test_group test_name "
echo "example : $0 /svn/rtemssparc64/rtems/boot/image.iso samples hello "
exit 1
fi

ISOPATH=$1
ISOSIZE=`ls -la $ISOPATH  | awk '{print $5}'`
TESTGROUP=$2
TESTNAME=$3

ISOPATH=`echo $ISOPATH | sed 's/\//\\//g'`

#rm $TESTNAME.txt

#cat targets/niagara-simple/niagara-simple-RTEMSoneTESTtemplate.simics | sed s/REPLACEDBYSED534ISOSIZE/$ISOSIZE/ | sed s/REPLACEDBYSED534TESTNAME/$TESTNAME/ | sed "s;REPLACEDBYSED534ISOPATH;$ISOPATH;"
cat targets/niagara-simple/niagara-simple-RTEMSoneTESTtemplate.simics | sed s/REPLACEDBYSED534ISOSIZE/$ISOSIZE/ | sed s/REPLACEDBYSED534TESTGROUP/$TESTGROUP/ | sed s/REPLACEDBYSED534TESTNAME/$TESTNAME/ | sed "s;REPLACEDBYSED534ISOPATH;$ISOPATH;" > temp.simics

./simics temp.simics
#if [[ $? -ne 0 ]]
#then
#	echo "***** Exiting with -1 *****"
#	exit -1
#fi
#
#cat $TESTNAME.txt | sed '/^\s*$/d' | sed '/END_TOKEN/d' | sed 's/\x0D$//' > temp.txt
#mv temp.txt $TESTNAME.txt
#diff $TESTNAME.txt results-expected/$TESTNAME.scn -bU 0 --strip-trailing-cr > $TESTNAME.diff

#exit 0
