#!/bin/bash
#
# Script to help with analyzing results of simulator output.
# 

progname=${0##*/}        # fast basename hack for ksh, bash

USAGE=\
"usage: $progname [ -opts ] 
  -d    -- file listing directories of results to analyze
  -f    -- file listing fields of interest
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
while getopts "d:f:" OPT
do
  case "$OPT" in
    d) directories=$OPTARG;;
    f) fields=$OPTARG;;
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
}

validate_args

dir_arr=( `awk 'BEGIN {ORS=" "} \
    $1 !~ /^(#| |$).*/ {print $1} \
    END {printf("\n");}' ${directories}` )
dir_arr_len=${#dir_arr[@]}
echo ${dir_arr_len}
for (( i=0; i<dir_arr_len; i++ ))
do
  if [[ ! -d ${dir_arr[$i]} ]]
  then
    error_out "Not a valid directory: ${dir_arr[$i]}"
    fatal
  fi
  echo ${dir_arr[$i]}
done
