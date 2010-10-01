#!/bin/bash

if [[ $# -ne 2 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 in_results_dir out_results_dir"
  exit 1
fi

if [[ ! -d $1 ]]
then
  echo "$1 is not a directory"
  exit 1
fi

if [[ ! -d $2 ]]
then
  echo "$2 is not a directory"
  exit 1
fi

PWD=`pwd`

# Make sure it is a fully-specified directory
if [[ ! -d ${PWD}/$1 ]]
then
  RESULTS=$1
else
  RESULTS=${PWD}/$1
fi

if [[ ! -d ${PWD}/$2 ]]
then
  OUTPUT=$2
else
  OUTPUT=${PWD}/$2
fi

STRINGS=('L1\.data' 'L1\.inst' 'Total number of instructions' 'Total number of cycles' 'Instruction per cycle' 'total power per cycle' 'average total power per cycle' 'average total power per cycle w/o FP and L2 power' 'average total power per insn' 'average total power per insn w/o FP and L2 power' 'total power usage of L1Dcache' 'total power usage of clock' 'avg power usage of L1Dcache' 'avg power usage of clock' 'total number accesses of L1Dcache' 'ds1_accesses' 'ds2_accesses' 'ds1_cycles_saved' 'ds2_cycles_saved' 'ds1_cycles_max' 'ds2_cycles_max' 'ds2_critical_max' 'sched_ds1_max' 'sched_ds1_cycles' 'sched_accesses' 'sched_cycles_saved' 'sched_cycles_max' 'sched_cycles_max_ds')
FILES=(L1data.csv L1inst.csv total_insn.csv total_cycles.csv insn_per_cycle.csv total_power_per_cycle.csv avg_total_power_per_cycle.csv avg_total_power_per_cycle_no_FP_L2.csv avg_total_power_per_insn.csv avg_total_power_per_insn_no_FP_L2.csv total_power_L1data.csv total_power_clock.csv avg_power_L1data.csv avg_power_clock.csv total_L1data_accesses.csv ds1_accesses.csv ds2_accesses.csv ds1_cycles_saved.csv ds2_cycles_saved.csv ds1_cycles_max.csv ds2_cycles_max.csv ds2_critical_max.csv sched_ds1_max.csv sched_ds1_cycles.csv sched_accesses.csv sched_cycles_saved.csv sched_cycles_max.csv sched_cycles_max_ds.csv)

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

  if [[ -d ${OUTPUT}/${TESTTAG} ]]
  then
    echo "${OUTPUT}/${TESTTAG} already exists, move it or delete it"
    exit 1
  fi
  D=${OUTPUT}/${TESTTAG}
  mkdir $D

  ## Setup output files
  for ((i=0;i<${#FILES[@]};i++))
  do
    touch ${D}/${FILES[$i]}
  done
  touch ${D}/test_params.csv

  for dir2 in `ls`
  do
    TESTRUN=$dir2
    echo "Processing ${TESTRUN}"
    if [[ -d ${TESTRUN} ]]
    then
      cd ${TESTRUN}
      echo ${TESTRUN} >> ${D}/test_params.csv
      cat test_params.txt >> ${D}/test_params.csv
      echo "==========" >> ${D}/test_params.csv

      for results_file in `ls *.opal`
      do
        for ((i=0;i<${#FILES[@]};i++))
        do
          #write out the test
          BUFFER=`echo ${results_file} | tr -d '\n' | sed -e 's/$//'`
          grep "${STRINGS[$i]}" ${results_file} | \
          sed -e 's/\[0\]\s*//' -e 's/\[/:/' -e 's/\]//' \
          -e 's/                         /:/' -e 's/\s*$//' \
          -e 's/  //g' -e 's/ /_/g' -e 's/:_/:/g' -e 's/_:/:/g' \
          -e 's/:.*:/:/g' -e 's/:/,/' -e "s/^/${TESTRUN},${BUFFER},/" \
          >>${D}/${FILES[$i]}
        done
      done
      cd ..
    fi
  done
  cd ..
done

