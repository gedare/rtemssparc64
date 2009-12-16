#!/bin/sh

if [ $# -eq 0 ]
then
echo "Error - parameters missing"
echo "Syntax : $0 path_to_iso test_name "
echo "example : $0 /home/eugen/work/rtemssparc64/rtemsboot/image.iso hello "
exit 1
fi

ISOPATH=$1
ISOSIZE=`ls -la $ISOPATH  | awk '{print $5}'`
TESTNAME=$2

ISOPATH=`echo $ISOPATH | sed 's/\//\\//g'`


#cat targets/niagara-simple/niagara-simple-RTEMStemplate.simics | sed s/REPLACEDBYSED534ISOSIZE/$ISOSIZE/ | sed s/REPLACEDBYSED534TESTNAME/$TESTNAME/ | sed "s;REPLACEDBYSED534ISOPATH;$ISOPATH;"
cat targets/niagara-simple/niagara-simple-RTEMStemplate.simics | sed s/REPLACEDBYSED534ISOSIZE/$ISOSIZE/ | sed s/REPLACEDBYSED534TESTNAME/$TESTNAME/ | sed "s;REPLACEDBYSED534ISOPATH;$ISOPATH;" > temp.simics

simics temp.simics

rm temp.simics