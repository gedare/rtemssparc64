#!/bin/bash

OUTDIR=simout
rm ${OUTDIR}/*
for sched in 1 2
do
  for dist in 1 2 3
  do
    for tasks in 20 40 60 80
    do
      for util in 0.4 0.6 0.8 1.0
      do
        FILE=sim_${sched}_${dist}_${tasks}_${util}.dat
        FILE2=sim_${sched}_${dist}_${tasks}_${util}_dl.dat
        echo "" > ${OUTDIR}/${FILE}
        echo "" > ${OUTDIR}/${FILE2}
        for i in {1..100}
        do
          python simulate-sched.py -t ${tasks} -d ${dist} -u ${util} \
            -s ${sched} >> ${OUTDIR}/${FILE} 2>> ${OUTDIR}/${FILE2}
        done
      done
    done
  done
done

