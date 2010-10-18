#!/bin/bash

outdir=simout
cd ${outdir}

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
      for util in 0.4 0.6 0.8 1.0
      do
        if [[ -f sim_${sched}_${dist}_${tasks}_${util}.dat ]]
        then
          rm temp.txt
          touch temp.txt
          for i in {0..100}
          do
            awk -v num="$i" -v pat="^$i$" 'BEGIN {printf("%d\t",num) } 
              $1 ~ pat {print $2}' sim_${sched}_${dist}_${tasks}_${util}.dat \
              | sed -e 's/$/\t/' | tr -d '\n' | sed -e 's/$/\n/' >> temp.txt
          done
          cp temp.txt sim_${sched}_${dist}_${tasks}_${util}.dat2
        fi
      done
    done
  done
done

