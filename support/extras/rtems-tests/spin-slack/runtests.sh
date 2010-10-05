#!/bin/bash

SIMICS_WKSP=$1
RESULTS_TAG=$2
tasks=$3
utilization=$4
distribution=$5
RUN_TAG=$6
ARCHIVE_DIR=$7

## build tests and image
cd ${SIMICS_WKSP}/build-sparc64/b-usiiicvs/sparc64-rtems4.11/c/usiii/testsuites/gabtests/
make -j2
cd ${SIMICS_WKSP}/build-sparc64/boot
./build-usiii.sh

cd ${SIMICS_WKSP}
mkdir results/${RESULTS_TAG}/${tasks}_${utilization}_${distribution}_${RUN_TAG}
## clean up from previous run
rm -rf output/*
for count in {1..10}
do
  if [ $count -lt 10 ]; then
    spintest=spspin0${count}
  else
    spintest=spspin${count}
  fi
      spindir=build-sparc64/rtems/testsuites/gabtests/${spintest}

  cp ${ARCHIVE_DIR}/${spintest}.test_params.txt results/${RESULTS_TAG}/${tasks}_${utilization}_${distribution}_${RUN_TAG}/
  ./opal-quicktest.sh gabtests ${spintest} > output/${spintest}.stdout 2> output/${spintest}.stderr
  cp ${spindir}/params.h output/${spintest}.params.h
  cp ${spindir}/macros.h output/${spintest}.macros.h
done

mv output/* results/${RESULTS_TAG}/${tasks}_${utilization}_${distribution}_${RUN_TAG}
tar -zcf results/${RESULTS_TAG}/${RESULTS_TAG}_${tasks}_${utilization}_${distribution}_${RUN_TAG}.tgz results/${RESULTS_TAG}/${tasks}_${utilization}_${distribution}_${RUN_TAG}
cp results/${RESULTS_TAG}/${RESULTS_TAG}_${tasks}_${utilization}_${distribution}_${RUN_TAG}.tgz ${ARCHIVE_DIR}
svn add  ${ARCHIVE_DIR}/${RESULTS_TAG}_${tasks}_${utilization}_${distribution}_${RUN_TAG}.tgz
cd -
