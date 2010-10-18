#!/bin/bash

if [[ $# -ne 4 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 rtems_dir1 simics_workspace1 last_test_tag new_tag"
  echo "Example: $0 ${HOME}/work/rtems/gsoc/rtems-sched/rtems-hwsched-cache ${HOME}/work/simics/gems/gems-2.1.1-hwsched-cache/simics hwsched-cache-uni_10_201010032158 hwsched-cache"
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
distribution=2
archive_dir=results-cache2
utilization=0.6
for tasks in 80 60 40 20
do
    ## Generate tests
    for count in {0..9}
      do
      if [ $count -lt 10 ]; then
        cachetest=cache0${count}
      else
        cachetest=cache${count}
      fi
      cachedir1=${RTEMS_DIR1}/testsuites/gabtests/${cachetest}
  
      if [[ ! -d ${cachedir1} ]]
      then
        echo "Unable to find directory: ${cachedir1} or ${cachedir2}"
        exit 1
      fi
  
  #    python gen-params.py -t ${tasks} -d ${distribution} -u ${utilization}
      cp ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV}/${tasks}_${utilization}_${distribution}_RM/${cachetest}.params.h ${cachedir1}/params.h
      cp ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV}/${tasks}_${utilization}_${distribution}_RM/${cachetest}.macros.h ${cachedir1}/macros.h
      cp ${SIMICS_WKSP1}/results/${RESULTS_TAG_PREV}/${tasks}_${utilization}_${distribution}_RM/${cachetest}.test_params.txt ${PWD}/${archive_dir}/${cachetest}.test_params.txt

      ## enforce RM (priority) scheduling first
      sed -i -e 's/#define CONFIGURE_SCHEDULER_EDF/\/\/#define CONFIGURE_SCHEDULER_EDF/' ${cachedir1}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests-cache.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "RM" ${PWD}/${archive_dir} 

    ## Now do the EDF scheduler
    for count in {0..9}
      do
      if [ $count -lt 10 ]; then
        cachetest=cache0${count}
      else
        cachetest=cache${count}
      fi
      cachedir1=${RTEMS_DIR1}/testsuites/gabtests/${cachetest}

      sed -i -e 's/\/*#define CONFIGURE_SCHEDULER_EDF/#define CONFIGURE_SCHEDULER_EDF/' ${cachedir1}/system.h
    done

    cd ${PWD}
    ## build and run tests
    ./runtests-cache.sh ${SIMICS_WKSP1} ${RESULTS_TAG1} ${tasks} ${utilization} ${distribution} "EDF" ${PWD}/${archive_dir} 
done
cd ${PWD}
svn commit -m "updating results"

