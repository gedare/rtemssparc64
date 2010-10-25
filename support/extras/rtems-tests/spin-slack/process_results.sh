#!/bin/bash
#
# Script to help with analyzing results of simulator output.
# 


### HELPERS  ###
USAGE=\
"usage: $progname [ -opts ] 
  -d    -- file listing directories of results to analyze (required)
  -f    -- file listing fields of interest (required)
  -O    -- directory of processed results (required)
"

error_out() {
  echo "$*" >&2
}

fatal() {
  [ "$1" ] && error_out $*
  error_out "$USAGE"
  exit 1
}

### GLOBAL VARIABLES ###
dir_arr=
let dir_arr_len=0
dataset_arr=
let dataset_arr_len=0
field_arr=
let field_arr_len=0
PARAM_FILE=

## Arguments
directories=
fields=
OUTPUT=
PROCESS_DATA="yes"
while getopts "d:f:O:" OPT
do
  case "$OPT" in
    d) directories=$OPTARG;;
    f) fields=$OPTARG;;
    O) OUTPUT=$OPTARG;;
    *) fatal;;
  esac
done

### FUNCTIONS ###
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

canonicalize_args() {
  ## Somewhat hackish. Make sure paths are fully-qualified.
  if [[ -d `pwd`/${OUTPUT} ]]
  then
    OUTPUT=`pwd`/${OUTPUT}
  fi

  if [[ -f `pwd`/${fields} ]]
  then
    fields=`pwd`/${fields}
  fi

  if [[ -f `pwd`/${function_file} ]]
  then
    function_file=`pwd`/${function_file}
  fi
}

process_results() {
  local FIELDS=
  local i=
  local j=
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
    
  ## Deprecated
  #OLDIFS=$IFS
  #IFS=$'\n' 
  #field_arr=( `awk 'BEGIN {ORS="\n"} \
  #$1 !~ /^(#| |$).*/ {print $0} \
  #END {printf("\n");}'  ${fields}` )
  #field_arr_len=${#field_arr[@]}
  #IFS=$OLDIFS
  #
  #for (( i=0; i<field_arr_len; i++ ))
  #do
  #  tmp="-e ${field_arr[$i]} "
  #  grep_fields=${grep_fields}${tmp}
  #done

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

    dataset_arr=( `ls -1 | sed -e 's/$/ / ' | tr -d '\n'` )
    dataset_arr_len=${#dataset_arr[@]}

    for (( j=0;j<dataset_arr_len; j++ ))
    do
      dir2=${dataset_arr[$j]}
      TESTRUN=$dir2
      FHEAD=${TESTRUN}
      echo "Processing ${TAG}.${TESTRUN}"

      ## Setup output files
      touch ${D}/${FHEAD}.dat

      if [[ -d ${TESTRUN} ]]
      then
        cd ${TESTRUN}
        let count=0
        for results_file in `find . -name "*.opal" | sed -e 's/.\///' | sort`
        do
          let count=count+1
          # Write out file headers before processing first file
          if [[ $count -eq 1 ]]
          then
            BUFFER=`echo -n "Filename  "`
            FIELDS=`eval grep -f ${fields} ${results_file} | \
            sed -e 's/\[0\]\s*//' -e 's/\[.*\]/:/' -e 's/\[//g' -e 's/\]//g' \
            -e 's/  \s*/:/' \
            -e "s/^/\"/" -e "s/:.*$/\"  /" | \
            tr -d '\n' | sed -e 's/\s*$//' | \
            sed -e "s/^/${BUFFER}  /"`
            eval field_arr=( `echo -n "${FIELDS}" | sed -e "s/\"/\'/g"` )
            field_arr_len=${#field_arr[@]}
            echo "${FIELDS}">>${D}/${FHEAD}.dat
          fi

          #write out the test
          BUFFER=`echo -n ${results_file}`
          eval grep -f ${fields} ${results_file} | \
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

  ## Write out the directory and field information to a file.
  ## Allows to decouple processing and analyzing data.
  cd ${OUTPUT}
  rm info.txt
  touch info.txt
  for (( i=1; i<dir_arr_len; i++ ))
  do
    echo -n "${dir_arr[$i]}  " >> info.txt
  done
  echo "" >> info.txt
  for (( i=0; i<dataset_arr_len; i++ ))
  do
    echo -n "${dataset_arr[$i]}  " >> info.txt
  done
  echo "" >> info.txt
  for (( i=0; i<field_arr_len; i++ ))
  do
    echo "${field_arr[$i]}" >> info.txt
  done
}

load_info() {
  cd ${OUTPUT}
  if [[ ! -f info.txt ]]
  then
    error_out "Unable to find ${OUTPUT}/info.txt -- Try processing data?"
    fatal
  fi

  # Get the directories
  dir_arr=( `awk 'BEGIN {getline; ORS=" "; \
                         for ( i=1; i<=NF; i++ ) { print $i }; }' info.txt` )
  dir_arr_len=${#dir_arr[@]}

  # Get the datasets
  dataset_arr=( `awk 'BEGIN {getline;getline; ORS=" "; \
                         for ( i=1; i<=NF; i++ ) { print $i }; }' info.txt` )
  dataset_arr_len=${#dataset_arr[@]}

  # Get fields
  OLDIFS=$IFS
  IFS=$'\n' 
  eval field_arr=( `awk 'BEGIN {getline;getline; ORS=" ";} \
                    /.*/ {printf("\"%s\"  ",$0);}' info.txt \
                    | sed -e "s/\"/\'/g" -e 's/\s*$//'` )
  field_arr_len=${#field_arr[@]}
  IFS=${OLDIFS}
  cd -
}

print_info() {
  echo "Directories"
  for (( i=0; i<${dir_arr_len}; i++ ))
  do
    echo "$i  ${dir_arr[$i]}"
  done
  echo ""

  echo "Datasets"
  for (( i=0; i<${dataset_arr_len}; i++ ))
  do
    echo "$i  ${dataset_arr[$i]}"
  done
  echo ""


  echo "Fields"
  for (( i=0; i<${field_arr_len}; i++ ))
  do
    echo "\$$((i+1))  ${field_arr[$i]}"
  done
}

## script entry point
main() {
  validate_args

  canonicalize_args

  ## Process data or load info from pre-processed results
  if [ ${PROCESS_DATA} = "yes" ]
  then
    process_results
  fi
  
  load_info

  print_info
}



## Start script
main
