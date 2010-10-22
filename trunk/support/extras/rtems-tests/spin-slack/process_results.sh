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

if [[ ! ${STRINGS} ]]
then
  STRINGS=( \
    'L1\.data' 'L1\.inst' 'Total number of instructions' \
    'Total number of cycles' 'Instruction per cycle' 'total power per cycle' \
    'total number accesses of L1Dcache' 'ds1_' 'ds2_' 'sched_' \
  )
fi

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


  for dir2 in `ls`
  do
    TESTRUN=$dir2
    FHEAD=${TESTRUN}
    echo "Processing ${TAG}.${TESTRUN}"
  
    ## Setup output files
    touch ${D}/${FHEAD}.dat
    touch ${D}/${FHEAD}.test_params.dat

    if [[ -d ${TESTRUN} ]]
    then
      cd ${TESTRUN}
      echo ${TESTRUN} >> ${D}/${FHEAD}.test_params.dat
        echo "==========" >> ${D}/${FHEAD}.test_params.dat

      for results_file in `ls | grep test_params`
      do
        cat ${results_file} | sed -e 's/,/  /' >> ${D}/${FHEAD}.test_params.dat
        echo "==========" >> ${D}/${FHEAD}.test_params.dat
      done
#      PARSERUN=`echo ${TESTRUN} | tr -d '\n' | \
#        sed -e 's/\(.*\)_\(.*\)_\(.*\)_\(.*\)$/\3,\4,\1,\2/'`
#      UTIL=`echo ${TESTRUN} | tr -d '\n' | sed -e 's/.*_\(.*\)_.*$/\1/'`
#      DIST=`echo ${TESTRUN} | tr -d '\n' | sed -e 's/.*_.*_\(.*\)_.*$/\1/'`
#      SCHED=`echo ${TESTRUN} | tr -d '\n' | sed -e 's/.*_.*_.*_\(.*\)$/\1/'`
      
#      TMP_FILE=
      count=0
      for results_file in `find . -name "*.opal" | sed -e 's/.\///' | sort`
      do
        let count=count+1
        # Write out file headers before processing first file
        if [[ $count -eq 1 ]]
        then
          BUFFER=`echo "Filename  " | tr -d '\n'`
          grep -e 'L1\.data' -e 'L1\.inst' -e 'Total number of instructions' \
               -e 'Total number of cycles' -e 'Instruction per cycle' \
               -e 'total power per cycle' -e 'ds1_' -e 'ds2_' -e 'sched_' \
               ${results_file} | \
          sed -e 's/\[0\]\s*//' -e 's/\[.*\]/:/' -e 's/\[//g' -e 's/\]//g' \
              -e 's/  \s*/:/' \
              -e "s/^/\"/" -e "s/:.*$/\"  /" | \
          tr -d '\n' | sed -e 's/\s*$//' | \
          sed -e "s/^/${BUFFER}  /" \
          >>${D}/${FHEAD}.dat
          echo "" >> ${D}/${FHEAD}.dat
        fi

        #write out the test
        BUFFER=`echo ${results_file} | tr -d '\n'`
        grep -e 'L1\.data' -e 'L1\.inst' -e 'Total number of instructions' \
             -e 'Total number of cycles' -e 'Instruction per cycle' \
             -e 'total power per cycle' -e 'ds1_' -e 'ds2_' -e 'sched_' \
             ${results_file} | \
        sed -e "s/\[0\]\s*/:/" -e "s/\[/:/" -e "s/\[//g" -e "s/\]//g" \
            -e 's/  \s*/:/' -e 's/.*:/  /' \
            -e 's/:.*:/  /' -e 's/\s*:/  /' | \
        tr -d '\n' | \
        sed -e "s/^/${BUFFER}  /" >>${D}/${FHEAD}.dat
        echo "" >> ${D}/${FHEAD}.dat
        #        TMP_FILE=${results_file}
      done
      cd ..
    fi
  done
  cd ..
done

