#!/bin/bash

outdir=simout
cd ${outdir}

echo "#plotter">plotter.p
echo "set size 1.0, 0.6" >> plotter.p
echo "set terminal postscript eps color dashed lw 1 \"Helvetica\" 14" >> plotter.p

for sched in 1 2
do
  scheduler=
  if [[ $sched -eq 1 ]]
  then
    scheduler=`echo "Rate Monotonic (fixed priority)"`
  else
    scheduler=`echo "Earliest Deadline First (dynamic priority)"`
  fi
  for dist in 1 2 3
  do
    if [[ $dist -eq 1 ]]
    then
      distribution=`echo "Uniformly"`
    elif [[ $dist -eq 2 ]]
    then
      distribution=`echo "Bimodally"`
    elif [[ $dist -eq 3 ]]
    then
      distribution=`echo "Exponentially"`
    fi

    for tasks in 20 40 60 80
    do
        echo "set output \"sim_${sched}_${dist}_${tasks}.eps\"" >> plotter.p
        echo "set multiplot" >> plotter.p
        echo "set title \"${scheduler} Scheduling with\nExecution times ${distribution} distributed, ${tasks} Tasks\" font \"Times-Roman,20\" " >> plotter.p
        echo "set xlabel 'OS Tick Count' font \"Times-Roman,12\"" >> plotter.p
        echo "set ylabel 'Tasks in Ready Queue' font \"Times-Roman,12\"" >> plotter.p
        echo "plot \"sim_${sched}_${dist}_${tasks}_0.4.dat\" title '0.4' , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_0.6.dat\" title '0.6' , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_0.8.dat\" title '0.8' , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_1.0.dat\" title '1.0' " \
          >> plotter.p
        echo "unset multiplot" >> plotter.p

        echo "set output \"sim_${sched}_${dist}_${tasks}_avg.eps\"" >> plotter.p
        echo "set multiplot" >> plotter.p
        echo "set title \"${scheduler} Scheduling with\nExecution times ${distribution} distributed, ${tasks} Tasks\" font \"Times-Roman,20\" " >> plotter.p
        echo "set xlabel 'OS Tick Count' font \"Times-Roman,12\"" >> plotter.p
        echo "set ylabel 'Tasks in Ready Queue' font \"Times-Roman,12\"" >> plotter.p
        echo "plot \"sim_${sched}_${dist}_${tasks}_0.4_avg.dat\" using 1:2 title '0.4' with lp, \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_0.6_avg.dat\" using 1:2 title '0.6' with lp  , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_0.8_avg.dat\" using 1:2 title '0.8' with lp , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_1.0_avg.dat\" using 1:2 title '1.0' with lp " \
          >> plotter.p
        echo "unset multiplot" >> plotter.p

        echo "set output \"sim_${sched}_${dist}_${tasks}_avg_min_max.eps\"" >> plotter.p
        echo "set multiplot" >> plotter.p
        echo "set title \"${scheduler} Scheduling with\nExecution times ${distribution} distributed, ${tasks} Tasks\" font \"Times-Roman,20\" " >> plotter.p
        echo "set xlabel 'OS Tick Count' font \"Times-Roman,12\"" >> plotter.p
        echo "set ylabel 'Tasks in Ready Queue' font \"Times-Roman,12\"" >> plotter.p
        echo "plot \"sim_${sched}_${dist}_${tasks}_0.4_avg.dat\" using 1:2:3:4 title '0.4' with yerrorbars , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_0.6_avg.dat\" using 1:2:3:4 title '0.6' with yerrorbars  , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_0.8_avg.dat\" using 1:2:3:4 title '0.8' with yerrorbars , \\" \
          >> plotter.p
        echo "\"sim_${sched}_${dist}_${tasks}_1.0_avg.dat\" using 1:2:3:4 title '1.0' with yerrorbars " \
          >> plotter.p
        echo "unset multiplot" >> plotter.p

    done
  done
done

