#!/bin/bash

if [[ $# -ne 9 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 rtems_dir1 simics_workspace1 rtems_dir2 simics_workspace2 results_tag1 results_tag2"
  echo "Example: $0 ${HOME}/work/rtems/rtems-sched/rtems-hwds-cache ${HOME}/work/simics/gems/gems-2.1.1-hwds-cache/simics ${HOME}/work/rtems/rtems-sched/rtems-hwds-nocache ${HOME}/work/simics/gems/gems-2.1.1-hwds-nocache/simics hwds-cache hwds-nocache"
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
  echo "$2 is not a proper simics workspace"
  exit 1
fi

# Check that $3 is a valid rtems source directory
if [[ ! -d $3/cpukit || ! -d $3/c ]]
then
  echo "$3 is not a valid rtems source-code directory"
  exit 1
fi

# Check that $4 is a prepared simics workspace
if [[ ! -f $4/opal-quicktest.sh ]]
then
  echo "$4 is not a proper simics workspace"
  exit 1
fi

PWD=`pwd`
count=0
RTEMS_DIR1=$1
SIMICS_WKSP1=$2
RTEMS_DIR2=$3
SIMICS_WKSP2=$4

RESULTS_TAG1=${5}_`date +%Y%m%d%H%M`
RESULTS_TAG2=${6}_`date +%Y%m%d%H%M`

## prepare the pre-build (configure) environment.
## this makes for faster re-building later.
#cd ${SIMICS_WKSP1}/build-sparc64
#./build-b-usiiicvs.sh
#cd -
#cd ${SIMICS_WKSP2}/build-sparc64
#./build-b-usiiicvs.sh
#cd -
mkdir ${SIMICS_WKSP1}/results
mkdir ${SIMICS_WKSP2}/results
mkdir ${SIMICS_WKSP1}/results/${RESULTS_TAG1}
mkdir ${SIMICS_WKSP2}/results/${RESULTS_TAG2}

## make this a loop
## get some parameters ...
distribution=1
archive_dir=results-cache2
if [[ ! -d ${archive_dir} ]]
then
  mkdir ${archive_dir}
fi
utilization=0.6  ## IF THIS IS CHANGED, NEED TO UPDATE ALL tasks_periodic.c
for tasks in 80 60 40 20 
do
    for count in {0..9}
    do
      cachetest=cache0${count}
      cachedir1=${RTEMS_DIR1}/testsuites/gabtests/${cachetest}
      cachedir2=${RTEMS_DIR2}/testsuites/gabtests/${cachetest}
  
      if [[ ! -d ${cachedir1} || ! -d ${cachedir2} ]]
      then
        echo "Unable to find directory: ${cachedir1} or ${cachedir2}"
        exit 1
      fi
    
      ## Generate tests
      python gen-params.py -t ${tasks} -d ${distribution} -u ${utilization}
  
      cp test_params.txt ${archive_dir}/cache0${count}.test_params.txt
      cp params.h ${cachedir1}/params.h
      cp macros.h ${cachedir1}/macros.h
      cp params.h ${cachedir2}/params.h
      cp macros.h ${cachedir2}/macros.h

      ## enforce RM (priority) scheduling first
      sed -i -e 's/#define CONFIGURE_SCHEDULER_EDF/\/\/#define CONFIGURE_SCHEDULER_EDF/' ${cachedir1}/system.h
      sed -i -e 's/#define CONFIGURE_SCHEDULER_EDF/\/\/#define CONFIGURE_SCHEDULER_EDF/' ${cachedir2}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests-cache.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "RM" ${PWD}/${archive_dir} &
    PID1=$!
    ./runtests-cache.sh ${SIMICS_WKSP2} ${RESULTS_TAG2} ${tasks} ${utilization} ${distribution} "RM" ${PWD}/${archive_dir} &
    PID2=$!
    wait ${PID1} ${PID2}

    ## Now do the EDF scheduler
    for count in {0..9}
      do
      cachetest=cache0${count}
      cachedir1=${RTEMS_DIR1}/testsuites/gabtests/${cachetest}
      cachedir2=${RTEMS_DIR2}/testsuites/gabtests/${cachetest}

      sed -i -e 's/\/*#define CONFIGURE_SCHEDULER_EDF/#define CONFIGURE_SCHEDULER_EDF/' ${cachedir1}/system.h
      sed -i -e 's/\/*#define CONFIGURE_SCHEDULER_EDF/#define CONFIGURE_SCHEDULER_EDF/' ${cachedir2}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests-cache.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "EDF" ${PWD}/${archive_dir} &
    PID1=$!
    ./runtests-cache.sh ${SIMICS_WKSP2} ${RESULTS_TAG2} ${tasks} ${utilization} ${distribution} "EDF" ${PWD}/${archive_dir} &
    PID2=$!
    wait ${PID1} ${PID2}
done
cd ${PWD}
svn commit -m "updating results"

