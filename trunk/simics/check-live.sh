#!/bin/bash

PID=$1
TESTNAME=$2

# warm up time
sleep 10

FIND=`ps -eo pid | grep "$PID"`

while [ -n "$FIND" ]
do
	LIVE=`tail $TESTNAME.txt > _test1.txt && sleep 60 && tail $TESTNAME.txt > _test2.txt && diff _test1.txt _test2.txt | wc -m`

	if [[ $LIVE -eq 0 ]]
	then
		rm _test1.txt _test2.txt
		echo "Test timed out"
		kill -9 $PID
		exit -1
	fi

	FIND=`ps -eo pid | grep "$PID"`
done
exit 0
