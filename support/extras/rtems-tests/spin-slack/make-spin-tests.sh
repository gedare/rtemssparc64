#!/bin/bash

if [[ $# -ne 6 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 rtems_dir1 simics_workspace1 rtems_dir2 simics_workspace2 results_tag1 results_tag2"
  echo "Example: $0 ${HOME}/work/rtems/rtems-sched/rtems ${HOME}/work/simics/gems/gems-2.1.1-gedare/simics ${HOME}/work/rtems/rtems-sched/rtems-hwds ${HOME}/work/simics/gems/gems-2.1.1-hwds/simics baseline hwds"
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
archive_dir=results-qd
distribution=3
for tasks in 80 60 40 20
do
  for utilization in 0.8 0.6 0.4 0.2
  do
    ## Generate tests
    for count in {1..10}
      do
      if [ $count -lt 10 ]; then
        spintest=spspin0${count}
      else
        spintest=spspin${count}
      fi
      spindir1=${RTEMS_DIR1}/testsuites/gabtests/${spintest}
      spindir2=${RTEMS_DIR2}/testsuites/gabtests/${spintest}
  
      if [[ ! -d ${spindir1} || ! -d ${spindir2} ]]
      then
        echo "Unable to find directory: ${spindir1} or ${spindir2}"
        exit 1
      fi
  
      python gen-params.py -t ${tasks} -d ${distribution} -u ${utilization}
  
      cp test_params.txt ${archive_dir}/${spintest}.test_params.txt
      cp params.h ${spindir1}/params.h
      cp macros.h ${spindir1}/macros.h
      cp params.h ${spindir2}/params.h
      cp macros.h ${spindir2}/macros.h

      ## enforce RM (priority) scheduling first
      sed -i -e 's/#define CONFIGURE_SCHEDULER_EDF/\/\/#define CONFIGURE_SCHEDULER_EDF/' ${spindir1}/system.h
      sed -i -e 's/#define CONFIGURE_SCHEDULER_EDF/\/\/#define CONFIGURE_SCHEDULER_EDF/' ${spindir2}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "RM" ${PWD}/${archive_dir} &
    PID1=$!
    ./runtests.sh ${SIMICS_WKSP2} ${RESULTS_TAG2} ${tasks} ${utilization} ${distribution} "RM" ${PWD}/${archive_dir} &
    PID2=$!
    wait ${PID1} ${PID2}

    ## Now do the EDF scheduler
    for count in {1..10}
      do
      if [ $count -lt 10 ]; then
        spintest=spspin0${count}
      else
        spintest=spspin${count}
      fi
      spindir1=${RTEMS_DIR1}/testsuites/gabtests/${spintest}
      spindir2=${RTEMS_DIR2}/testsuites/gabtests/${spintest}

      sed -i -e 's/\/*#define CONFIGURE_SCHEDULER_EDF/#define CONFIGURE_SCHEDULER_EDF/' ${spindir1}/system.h
      sed -i -e 's/\/*#define CONFIGURE_SCHEDULER_EDF/#define CONFIGURE_SCHEDULER_EDF/' ${spindir2}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "EDF" ${PWD}/${archive_dir} &
    PID1=$!
    ./runtests.sh ${SIMICS_WKSP2} ${RESULTS_TAG2} ${tasks} ${utilization} ${distribution} "EDF" ${PWD}/${archive_dir} &
    PID2=$!
    wait ${PID1} ${PID2}
  done
done
cd ${PWD}
svn commit -m "updating results"

