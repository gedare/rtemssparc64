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

STRINGS=('L1\.data' 'L1\.inst' 'Total number of instructions' 'Total number of cycles' 'Instruction per cycle' 'total power per cycle' 'average total power per cycle' 'average total power per cycle w/o FP and L2 power' 'average total power per insn' 'average total power per insn w/o FP and L2 power' 'total power usage of L1Dcache' 'total power usage of clock' 'avg power usage of L1Dcache' 'avg power usage of clock' 'total number accesses of L1Dcache' 'ds1_' 'ds2_' 'sched_')
FILES=(L1data.csv L1inst.csv total_insn.csv total_cycles.csv insn_per_cycle.csv total_power_per_cycle.csv avg_total_power_per_cycle.csv avg_total_power_per_cycle_no_FP_L2.csv avg_total_power_per_insn.csv avg_total_power_per_insn_no_FP_L2.csv total_power_L1data.csv total_power_clock.csv avg_power_L1data.csv avg_power_clock.csv total_L1data_accesses.csv hw_ds1.csv hw_ds2.csv hw_sched.csv)

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
  TAG=`echo ${TESTTAG} | tr -d '\n' | sed -e 's/_.*$//'`

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
    echo "testtag,dist,sched,tasks,util,run,field,value" >> ${D}/${FILES[$i]}
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

      PARSERUN=`echo ${TESTRUN} | tr -d '\n' | \
        sed -e 's/\(.*\)_\(.*\)_\(.*\)_\(.*\)$/\3,\4,\1,\2/'`
#      UTIL=`echo ${TESTRUN} | tr -d '\n' | sed -e 's/.*_\(.*\)_.*$/\1/'`
#      DIST=`echo ${TESTRUN} | tr -d '\n' | sed -e 's/.*_.*_\(.*\)_.*$/\1/'`
#      SCHED=`echo ${TESTRUN} | tr -d '\n' | sed -e 's/.*_.*_.*_\(.*\)$/\1/'`
      
      TMP_FILE=
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
          -e 's/:.*:/:/g' -e 's/:/,/' \
          -e "s/^/${TAG},${PARSERUN},${BUFFER},/" \
          >>${D}/${FILES[$i]}
        done
        TMP_FILE=${results_file}
      done

      # Write out a placeholder for aggregates (average / max / min)
      for ((i=0;i<${#FILES[@]};i++))
      do
        BUFFER='aggregate'
        grep "${STRINGS[$i]}" ${TMP_FILE} | \
          sed -e 's/\[0\]\s*//' -e 's/\[/:/' -e 's/\]//' \
            -e 's/                         /:/' -e 's/\s*$//' \
            -e 's/  //g' -e 's/ /_/g' -e 's/:_/:/g' -e 's/_:/:/g' \
            -e 's/:.*:/:/g' -e 's/:.*/,/' \
            -e "s/^/${TAG},${PARSERUN},${BUFFER},/" \
          >>${D}/${FILES[$i]}
        echo "" >> ${D}/${FILES[$i]}
      done

      cd ..
    fi
  done
  cd ..
done

