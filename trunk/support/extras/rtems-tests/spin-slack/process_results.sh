#!/bin/bash

if [[ $# -ne 1 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 results_dir"
  exit 1
fi

if [[ ! -d $1 ]]
then
  echo "$1 is not a directory"
  exit 1
fi

PWD=`pwd`
RESULTS=$1

cd ${RESULTS}
for file in `ls | grep ".tgz"`
do
  tar -zxf $file
done

cd results
for dir in `ls`
do
  TESTTAG=$dir
  cd ${TESTTAG}
  echo "TESTTAG"
  echo ${TESTTAG}
  for dir2 in `ls`
  do
    TESTRUN=$dir2
    echo "TESTRUN"
    echo ${TESTRUN}
    if [[ -d ${TESTRUN} ]]
    then
      cd ${TESTRUN}
      cat test_params.txt
      for results_file in `ls *.opal`
      do
        echo ${results_file}
        grep -F 'L1.data
L1.inst
Total number of instructions
Total number of cycles
Instruction per cycle
total power per cycle
average total power per cycle
average total power per cycle w/o FP and L2 power
average total power per insn
average total power per insn w/o FP and L2 power
total power usage of L1Dcache
total power usage of clock
avg power usage of L1Dcache
avg power usage of clock
total number accesses of L1Dcache
ds1_accesses
ds2_accesses
ds1_cycles_saved
ds2_cycles_saved
ds1_cycles_max
ds2_cycles_max
ds2_critical_max
sched_ds1_max
sched_ds1_cycles
sched_accesses
sched_cycles_saved
sched_cycles_max
sched_cycles_max_ds' ${results_file}

      done
      cd ..
    fi
  done
  cd ..
done

