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

reduce_results() {
  ## The directory names are in dir_arr, with length dir_arr_len
  ## The data set names are in dataset_arr, with length dataset_arr_len
  ## The field names are in field_arr, with length field_arr_len
  ## This function should be customized to produce the desired data files, 
  ## which can be read in by further tools such as gnuplot and xmgrace.
  ##
  ## The delimiters for the data files are two or more spaces, which allows 
  ## for some fields to contain multiple elements that are comma-separated.

  ## Set up some groupings for the datasets. Whatever makes sense.
  ## These are indices for the dataset_arr.
  local \
  dataset_indices=( 0   8   16  24 \
                    1   9   17  25 \
                    2   10  18  26 \
                    3   11  19  27 \
                    4   12  20  28 \
                    5   13  21  29 \
                    6   14  22  30 \
                    7   15  23  31 )
  local indices_per_set=4
  local num_sets=8
  local dataset_indices_files=
  
  local X_vals=( 20 40 60 80 )
  local tmp=
  local index=
  cp graph_params.txt ${OUTPUT}/
  cp graph_perf_EDF.txt ${OUTPUT}/
  cp graph_perf_RM.txt ${OUTPUT}/
  cd ${OUTPUT}

  ## Reduced results will be placed in the reduced_results subdirectory of the 
  ## processed outputs. 
  if [[ ! -d reduced_results ]]
  then
    mkdir reduced_results
  fi

  ## This is where the 'work' gets done.  Loop over each grouping of dataset
  ## indices and reduce results.  Typically, each individual dataset index
  ## constitutes a single point of interest, probably an average with stdev
  ## or range, or maybe a maximum.

  ## Calculate a performance metric over dir_arr[1], using the 
  ## Total number of cycles, ds1_cycles_saved, and ds2_cycles_saved.
  ## Save the results to the following files (.dat is extended)
  dataset_indices_files=( 'perf_0.4_EDF' \
                          'perf_0.4_RM' \
                          'perf_0.6_EDF' \
                          'perf_0.6_RM' \
                          'perf_0.8_EDF' \
                          'perf_0.8_RM' \
                          'perf_1.0_EDF' \
                          'perf_1.0_RM' )
  for (( i=0; i < num_sets; i++ ))
  do
    rm reduced_results/${dataset_indices_files[$i]}.dat
    touch reduced_results/${dataset_indices_files[$i]}.dat
    for (( j=0; j<indices_per_set; j++ ))
    do
      let tmp=(i*indices_per_set+j)
      index=${dataset_indices[$tmp]}
      echo -n "${X_vals[$j]}  " >>reduced_results/${dataset_indices_files[$i]}.dat
      awk 'BEGIN {count=0; FS = " [ ]+";} \
          /Filename/ {for (i=1;i<=NF;i++) \
                      {if ($i == "\"Total number of cycles\"") idx1=i; \
                       if ($i == "\"ds1_cycles_saved\"") idx2=i; \
                       if ($i == "\"ds2_cycles_saved\"") idx3=i; \
                       if ($i == "\"ds1_first\"") idx4=i; \
                       if ($i == "\"ds1_enqueue\"") idx5=i; \
                       if ($i == "\"ds1_extract\"") idx6=i; \
                       if ($i == "\"ds1_requeue\"") idx7=i; \
                       if ($i == "\"ds2_enqueue\"") idx8=i; \
                       if ($i == "\"ds2_extract\"") idx9=i; \
                     }} \
          /spspin/ {A[count]=$idx1; \
                    B[count]=$idx2+$idx3; \
                    C[count]=$idx4*1+($idx5+$idx6+$idx8+$idx9)*2+$idx7*3 ; \
                    count++;} \
          END {for (i=0;i<count;i++) {sum+=(B[i]-C[i])/A[i]}; \
               mean=sum/count; \
               for (i=0;i<count;i++) {stdev_sum+=((B[i]-C[i])/A[i] - mean)*((B[i]-C[i])/A[i] - mean)}; \
               stdev=sqrt(stdev_sum/count); \
          printf("%f\t%f\n",mean,stdev)}' \
          ${dir_arr[1]}/${dataset_arr[$index]}.dat >> reduced_results/${dataset_indices_files[$i]}.dat
    done
  done

  ## Now plot perf graphs
  xmgrace -param graph_params.txt -param graph_perf_EDF.txt \
    reduced_results/${dataset_indices_files[0]}.dat \
    reduced_results/${dataset_indices_files[2]}.dat \
    reduced_results/${dataset_indices_files[4]}.dat \
    reduced_results/${dataset_indices_files[6]}.dat \
    -hardcopy

  xmgrace -param graph_params.txt -param graph_perf_RM.txt \
    reduced_results/${dataset_indices_files[1]}.dat \
    reduced_results/${dataset_indices_files[3]}.dat \
    reduced_results/${dataset_indices_files[5]}.dat \
    reduced_results/${dataset_indices_files[7]}.dat \
    -hardcopy

  cd -
  return
}

## script entry point
main() {
  validate_args

  canonicalize_args

  load_info

  print_info

  reduce_results
}



## Start script
main
