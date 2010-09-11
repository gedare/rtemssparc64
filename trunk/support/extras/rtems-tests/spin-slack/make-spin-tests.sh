#!/bin/bash

if [[ $# -ne 2 ]]
then
  echo "Error - parameters missing"
  echo "Syntax: $0 rtems_dir spspin.tgz"
  echo "Example: $0 ${HOME}/work/rtems/rtems-diff ${HOME}/work/rtems/rtemssparc64/support/extras/rtems-tests/spin-slack/spspin.tgz"
  exit 1
fi

# Check that $1 is a valid rtems source directory
if [[ ! -d $1/cpukit || ! -d $1/c ]]
then
  echo "$1 is not a valid rtems source-code directory"
  exit 1
fi

count=0
RTEMS_DIR=$1

tar -zxf $2

## make this a loop
## get some parameters ...

  lua gen-headers.lua -T 1,0.5,0 -A 2,0.5,0

  spindir=${RTEMS_DIR}/testsuites/gabtests/spspin${count}
  rm -rf ${spindir}

  mkdir ${spindir}
  if [[ ! -d ${spindir} ]]
  then
    echo "Failed to create directory: ${spindir}"
    exit 1
  fi

  cp spspin/* ${spindir}/
  sed -i s/spspin/spspin${count}/ ${spindir}/Makefile.am

  cp params.h ${spindir}/params.h
  cp macros.h ${spindir}/macros.h

  sed -i "s/spspin${count}//" ${RTEMS_DIR}/testsuites/gabtests/Makefile.am
  sed -i "s/spspin/spspin spspin${count}/" ${RTEMS_DIR}/testsuites/gabtests/Makefile.am

  sed -i "/spspin${count}/ d" ${RTEMS_DIR}/testsuites/gabtests/configure.ac
  sed -i "/spspin\/Makefile/ a\
spspin${count}\/Makefile" ${RTEMS_DIR}/testsuites/gabtests/configure.ac



  let count=${count}+1
  

