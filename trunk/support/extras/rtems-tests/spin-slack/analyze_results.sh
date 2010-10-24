#!/bin/bash
#
# Script to help with analyzing results of simulator output.
# 


### HELPERS  ###
USAGE=\
"usage: $progname [ -opts ] 
  -d    -- file listing directories of results to analyze (required)
  -f    -- file listing fields of interest (required)
  -P    -- process data (default=no)
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
PROCESS_DATA="no"
while getopts "d:f:PO:" OPT
do
  case "$OPT" in
    d) directories=$OPTARG;;
    f) fields=$OPTARG;;
    P) PROCESS_DATA="yes";;
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
  ## Somewhat hackish. Make sure OUTPUT and fields are fully-qualified paths.
  if [[ -d `pwd`/${OUTPUT} ]]
  then
    OUTPUT=`pwd`/${OUTPUT}
  fi

  if [[ -f `pwd`/${fields} ]]
  then
    fields=`pwd`/${fields}
  fi
}

process_results() {
  local FIELDS=
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
            if [[ ! ${FIELDS} ]]
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
            fi
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
    echo "$i  ${field_arr[$i]}"
  done
}

dataentry=
func=
title=
direntry=

function_avg() {
  field_num=$1
  average=
  for (( i=0; i<${#dataentry[@]}; i++ ))
  do
    average[$i]=`eval "awk 'BEGIN {FS=\" [ ]+\"; getline header;} \
                   /.*/ {sum+=${func[${field_num}]};count++} \
                   END {printf(\"%f\n\",sum/count);}' \
                   ${OUTPUT}/${dir_arr[${direntry[0]}]}/${dataset_arr[${dataentry[${i}]}]}.dat"`
  echo "${average[$i]}"
  done
}
  

## function_1 computes the 
function_1() {
  func_len=${#func[@]}
  field1=${func[1]}
  field2=${func[2]}
  eval "awk 'BEGIN {FS=\" [ ]+\"; getline header;} \
    /.*/ {sum1+=${field1}; count1++} \
    /.*/ {sum2+=${field2}; count2++;} \
        END {printf(\"%f\n\",sum1/count1); printf(\"%f\n\",sum2/count2);}' \
        ${OUTPUT}/${dir_arr[${direntry[0]}]}/${dataset_arr[${dataentry[0]}]}.dat"
}

## script entry point
main() {
  validate_args

  canonicalize_args

  ## Process data or load info from pre-processed results
  if [ ${PROCESS_DATA} = "yes" ]
  then
    process_results
  else
    load_info
  fi

  print_info

  title=`awk 'BEGIN {getline; print $0}' ${PWD}/functions.txt`
  direntry=`awk 'BEGIN {getline; getline; print $1}' functions.txt`
  dataentry=( `awk 'BEGIN {getline; getline; getline; ORS=" "; print $0}' functions.txt` )
  func=( `awk 'BEGIN {getline; getline; getline; getline; ORS=" ";print $0}' functions.txt` )

  if [[ ${func[0]} -eq 1 ]]
  then
    function_1
    arr1=( `function_avg 1 | sed -e 's/$/  /' | tr -d '\n'` )
    arr2=( `function_avg 2 | sed -e 's/$/  /' | tr -d '\n'` )
    for (( i=0; i<${#arr1[@]}; i++ ))
    do
      a=${arr1[$i]}
      b=${arr2[$i]}
      result=`echo "scale=6; ($a-$b)/$a" | bc`
      echo $result
    done
  else
    error_out "unknown function: ${func[0]}"
    fatal
  fi

}



## Start script
main
