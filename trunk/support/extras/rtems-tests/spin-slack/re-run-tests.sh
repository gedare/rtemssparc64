#!/bin/bash

if [[ $# -ne 4 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 rtems_dir1 simics_workspace1 last_test_tag new_tag"
  echo "Example: $0 ${HOME}/work/rtems/rtems-sched/rtems-hwsched ${HOME}/work/simics/gems/gems-2.1.1-hwsched/simics hwsched-uni_10_201010032158 hwsched"
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

PWD=`pwd`
count=0
RTEMS_DIR1=$1
SIMICS_WKSP1=$2


RESULTS_TAG_PREV=${3}

RESULTS_TAG1=${4}'_rerun_'${3}_`date +%Y%m%d%H%M`

## prepare the pre-build (configure) environment.
## this makes for faster re-building later.
#cd ${SIMICS_WKSP1}/build-sparc64
#./build-b-usiiicvs.sh
#cd -
#cd ${SIMICS_WKSP2}/build-sparc64
#./build-b-usiiicvs.sh
#cd -
mkdir ${SIMICS_WKSP1}/results
mkdir ${SIMICS_WKSP1}/results/${RESULTS_TAG1}

if [[ ! -d ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV} ]]
then
  echo "${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV} is not a valid directory"
  exit 1
fi

## make this a loop
## get some parameters ...
distribution=3
archive_dir=results-rerun2
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
  
      if [[ ! -d ${spindir1} ]]
      then
        echo "Unable to find directory: ${spindir1} or ${spindir2}"
        exit 1
      fi
  
  #    python gen-params.py -t ${tasks} -d ${distribution} -u ${utilization}
      cp ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV}/${tasks}_${utilization}_${distribution}_RM/${spintest}.params.h ${spindir1}/params.h
      cp ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV}/${tasks}_${utilization}_${distribution}_RM/${spintest}.macros.h ${spindir1}/macros.h
      cp ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV}/${tasks}_${utilization}_${distribution}_RM/${spintest}.test_params.txt ${PWD}/${archive_dir}/${spintest}.test_params.txt

      ## enforce RM (priority) scheduling first
      sed -i -e 's/#define CONFIGURE_SCHEDULER_EDF/\/\/#define CONFIGURE_SCHEDULER_EDF/' ${spindir1}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "RM" ${PWD}/${archive_dir} 

    ## Now do the EDF scheduler
    for count in {1..10}
      do
      if [ $count -lt 10 ]; then
        spintest=spspin0${count}
      else
        spintest=spspin${count}
      fi
      spindir1=${RTEMS_DIR1}/testsuites/gabtests/${spintest}

      sed -i -e 's/\/*#define CONFIGURE_SCHEDULER_EDF/#define CONFIGURE_SCHEDULER_EDF/' ${spindir1}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "EDF" ${PWD}/${archive_dir} 
  done
done
cd ${PWD}
svn commit -m "updating results"

