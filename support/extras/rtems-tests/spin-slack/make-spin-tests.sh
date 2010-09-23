#!/bin/bash

if [[ $# -ne 3 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 rtems_dir simics_workspace results_tag"
  echo "Example: $0 ${HOME}/work/rtems/rtems-diff ${HOME}/work/simics/gems/gems-2.1.1-gedare/simics baseline"
  exit 1
fi

# Check that $1 is a valid rtems source directory
if [[ ! -d $1/cpukit || ! -d $1/c ]]
then
  echo "$1 is not a valid rtems source-code directory"
  exit 1
fi

# Check that $2 is a prepared simics workspace
if [[ ! -f $2/opal-quicktest.sh ]]
then
  echo "$2 is noa  proper simics workspace"
  exit 1
fi

count=0
RTEMS_DIR=$1
SIMICS_WKSP=$2
RESULTS_TAG=${3}_`date +%Y%m%d%H%M`

## prepare the pre-build (configure) environment.
## this makes for faster re-building later.
${SIMICS_WKSP}/build-sparc64/build-b-usiiicvs.sh
mkdir ${SIMICS_WKSP}/results
mkdir results/${RESULTS_TAG}

## make this a loop
## get some parameters ...
distribution=1
for tasks in 80 60 40 20
do
  for utilization in 0.8 0.6 0.4 0.2
  do
    ## Generate tests
    for count in {1..50}
      do
      if [ $count -lt 10 ]; then
        spintest=spspin0${count}
      else
        spintest=spspin${count}
      fi
      spindir=${RTEMS_DIR}/testsuites/gabtests/${spintest}
  
      if [[ ! -d ${spindir} ]]
      then
        echo "Unable to find directory: ${spindir}"
        exit 1
      fi
  
      python gen-params.py -t ${tasks} -d ${distribution} -u ${utilization}
  
      mv params.h ${spindir}/params.h
      mv macros.h ${spindir}/macros.h
    done

    ## build tests and image.iso
    cd ${SIMICS_WKSP}/build-sparc64/b-usiiicvs/sparc64-rtems4.11/c/usiii/testsuites/gabtests/
    make
    cd -
    cd ${SIMICS_WKSP}/build-sparc64/boot
    ./build-usiii.sh
    cd -
  
    cd ${SIMICS_WKSP}
    ## clean up from previous run
    rm -rf output/*
    for count in {1..50}
    do
      if [ $count -lt 10 ]; then
        spintest=spspin0${count}
      else
        spintest=spspin${count}
      fi
    
      ./opal-quicktest.sh gabtests ${spintest} > output/${spintest}.stdout 2> output/${spintest}.stderr
    done
    mkdir results/${RESULTS_TAG}/${tasks}_${utilization}_${distribution}
    mv output/* results/${RESULTS_TAG}/${tasks}_${utilization}_${distribution}
  done
done

