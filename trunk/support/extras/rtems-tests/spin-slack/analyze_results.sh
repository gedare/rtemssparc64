#!/bin/bash
#
# Script to help with analyzing results of simulator output.
# 

PWD=`pwd`

progname=${0##*/}        # fast basename hack for ksh, bash

USAGE=\
"usage: $progname [ -opts ] 
  -d    -- file listing directories of results to analyze
  -f    -- file listing fields of interest
  -O    -- directory for output of processed results
"

error_out() {
  echo "$*" >&2
}

fatal() {
  [ "$1" ] && error_out $*
  error_out "$USAGE"
  exit 1
}

## Parse arguments
directories=
fields=
OUTPUT=
while getopts "d:f:O:" OPT
do
  case "$OPT" in
    d) directories=$OPTARG;;
    f) fields=$OPTARG;;
    O) OUTPUT=$OPTARG;;
    *) fatal;;
  esac
done

validate_args() {

  if [[ ! -f $directories ]]
  then
    error_out "Invalid directory file $directories"
    fatal
  fi

  if [[ ! -f ${fields} ]]
  then
    error_out "Invalid fields file: ${fields}"
    fatal
  fi

  if [[ ! -d ${OUTPUT} ]]
  then
    error_out "Invalid output directory: ${OUTPUT}"
    fatal
  fi
}

validate_args

## A bit of a hack to make sure OUTPUT is fully qualified.
if [[ -d ${PWD}/${OUTPUT} ]]
then
  OUTPUT=${PWD}/${OUTPUT}
fi

if [[ -f ${PWD}/${fields} ]]
then
  fields=${PWD}/${fields}
fi

dir_arr=( `awk 'BEGIN {ORS=" "} \
$1 !~ /^(#| |$).*/ {print $1} \
END {printf("\n");}' ${directories}` )
dir_arr_len=${#dir_arr[@]}
if [[ ! -d ${dir_arr[0]} ]]
then
  error_out "Not a valid base directory: ${dir_arr[0]}"
  fatal
fi
for (( i=1; i<dir_arr_len; i++ ))
do
  if [[ ! -d ${dir_arr[0]}/${dir_arr[$i]} ]]
  then
    error_out "Not a valid directory: ${dir_arr[0]}/${dir_arr[$i]}"
    fatal
  fi
done

OLDIFS=$IFS
IFS=$'\n' 
field_arr=( `awk 'BEGIN {ORS="\n"} \
$1 !~ /^(#| |$).*/ {print $0} \
END {printf("\n");}'  ${fields}` )
field_arr_len=${#field_arr[@]}
IFS=$OLDIFS

for (( i=0; i<field_arr_len; i++ ))
do
  tmp="-e ${field_arr[$i]} "
  grep_fields=${grep_fields}${tmp}
done

cd ${dir_arr[0]}
for (( i=1; i<dir_arr_len; i++ ))
do
  dir=${dir_arr[$i]}
  TESTTAG=$dir
  cd ${TESTTAG}
  TAG=`echo -n ${TESTTAG} |  sed -e 's/_.*$//'`

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

    if [[ -d ${TESTRUN} ]]
    then
      cd ${TESTRUN}
      count=0
      for results_file in `find . -name "*.opal" | sed -e 's/.\///' | sort`
      do
        let count=count+1
        # Write out file headers before processing first file
        if [[ $count -eq 1 ]]
        then
          BUFFER=`echo -n "Filename  "`
          eval grep -f ${fields} ${results_file} | \
          sed -e 's/\[0\]\s*//' -e 's/\[.*\]/:/' -e 's/\[//g' -e 's/\]//g' \
          -e 's/  \s*/:/' \
          -e "s/^/\"/" -e "s/:.*$/\"  /" | \
          tr -d '\n' | sed -e 's/\s*$//' | \
          sed -e "s/^/${BUFFER}  /" \
          >>${D}/${FHEAD}.dat
          echo "" >> ${D}/${FHEAD}.dat
        fi

        #write out the test
        BUFFER=`echo -n ${results_file}`
        eval grep "${grep_fields}" ${results_file} | \
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

cd ${PWD}
