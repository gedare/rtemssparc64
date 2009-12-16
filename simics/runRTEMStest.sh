#!/bin/sh

if [ $# -eq 0 ]
then
echo "Error - parameter missing"
echo "Syntax : $0 test_name"
exit 1
fi

ISOSIZE=43233280
TESTNAME=$1


cat targets/niagara-simple/niagara-simple-RTEMStemplate.simics | sed s/REPLACEDBYSED534ISOSIZE/$ISOSIZE/ | sed s/REPLACEDBYSED534TESTNAME/$TESTNAME/ > temp.simics

simics temp.simics

rm temp.simics