#
# simulate-sched.py
#
# Generate and simulate a task set for a scheduling algorithm.
#
#

import random
import math
import sys
import getopt
import array
import os
import copy

def usage():
  print "\
      Usage: gen-params.py -[ht:d:u:s:]\n\
  -h --help           print this help\n\
  -t --tasks=         specify number of tasks\n\
  -d --distribution=  specify utilization distribution, one of:\n\
                        1. Uniform distribution between 1/p and 1\n\
                        2. Bimodal distribution:\n\
                             Uniform in [0.5,1] with probability 1/9\n\
                             Uniform in [1/p,0.5] with probability 8/9\n\
                        3. Exponential distribution with mean 0.25\n\
                        4. Exponential distribution with mean 0.50\n\
  -u --utilization=   specify maximum utilization\n\
  -s --scheduler=     specify scheduling algorithm, one of:\n\
                        1. Fixed priority rate monotonic\n\
                        2. Earliest deadline first"

MAXIMUM_PERIOD = 50

def gcd(a,b):
  while b > 0: a,b = b, a%b
  return a

def lcm(num1, num2):
  result = num1*num2/gcd(num1,num2)
  return result

def simulate_scheduler(p_list, u_list, s):
  ### Setup ###
  hyperperiod = reduce(lcm, p_list)
  # TCBs is a list of lists, with ordering (could be an array) enforced
  # manually. The fields of tcb are:
  #  [ priority/deadline, period, utilization (%), budget, index ]
  tcbs = [[p_list[i],p_list[i],u_list[i],p_list[i]*u_list[i],i] for i in xrange(len(p_list))]

  readyq = []
  waitq = []
  maxp = max(p_list)
  deadlines_missed = 0

#  print tcbs

  readyq=copy.deepcopy(tcbs)
  readyq.sort()

#  t = readyq[0]
#  print t
#  t[1]-=1
#  print t
#  print readyq[0]
#  print tcbs[0]

  for ticks in xrange(maxp*2):

    # Move timed-out tasks from waitq to readyq.
    # Re-load priority/deadline, depending on scheduler algorithm
    # Reset periods.
    for i in range(len(waitq)-1,-1,-1):
      t = waitq[i]
      if (t[1] <= 0):
        waitq.remove(t)
        t[1] = tcbs[t[4]][1]+t[1]  # reload period
        t[3] = tcbs[t[4]][3]       # reload execution budget
        if (s == 1):    # fixed priority, RM
          t[0] = tcbs[t[4]][0]
        elif (s == 2):  # earliest deadline first
          t[0] = ticks + tcbs[t[4]][0]  
        else:
          printf("Error, unsupported scheduler: %d\n",s)
          exit()
        for j in xrange(len(readyq)): # insert sort
           if (readyq[j][0] > t[0]):
            readyq.insert(j,t)
            break
        else:
          readyq.append(t)
    
    # print the length of the ready queue at this tick
    print '{0:d}\t{1:d}'.format(ticks,len(readyq))

    # Execution phase. 'Run' tasks from the head of the readyq until either
    # exhausting the execution budget or reaching the end of the tick.
    tick_time_left = 1.0
    index = 0
    while (tick_time_left > 0):
      if (index >= len(readyq)):  # no work to be done
        break;
      t = readyq[index]
      if (t[3] < tick_time_left):
        tick_time_left -= t[3]
        t[3] = 0
      else:
        t[3] -= tick_time_left
        tick_time_left = 0
      index += 1;

    # Move exhausted tasks from readyq to waitq
    for i in range(len(readyq)-1,-1,-1):
      t = readyq[i]
      if (t[3] == 0):
        readyq.remove(t)
        if (t[1] <= 0): # deadline exceeded
          deadlines_missed += 1
        for j in xrange(len(waitq)): # insert sort
          if (waitq[j][0] > t[0]):
            waitq.insert(j,t)
            break
        else:
          waitq.append(t)
  
    # decrement period timeouts by 1 tick
    waitq[:] = [[t[0],t[1]-1,t[2],t[3],t[4]] for t in waitq]
    readyq[:] = [[t[0],t[1]-1,t[2],t[3],t[4]] for t in readyq]

  return deadlines_missed

def generate_bimodal(p_list, t):
  util_list = []
  for i in xrange(t):
    if (int(1+random.random()*9) == 1):  # uniform random 1 to 9
      util_list.extend( [random.uniform(0.5,1)] ) # heavy
    else:
      p_inv = 1/p_list[i]
      if (p_inv > 0.25):
        p_inv = 0.25
      util_list.extend( [random.uniform(p_inv, 0.5)] ) # light
  return util_list

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "ht:d:u:s:", 
        ["help", "tasks=", "distribution=", "utilization="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
      sys.exit()
    elif opt in ("-t", "--tasks"):
      num_tasks = int(arg)
    elif opt in ("-d", "--distribution"):
      distribution = int(arg)
    elif opt in ("-u", "--utilization"):
      max_u = float(arg)
    elif opt in ("-s", "--scheduler"):
      scheduler = int(arg)
    else:
      assert False, "unhandled option"

  # Generate a list of num_tasks uniform random periods between 1 and 50.
#  p_list = [int(1+random.random()*MAXIMUM_PERIOD) for i in xrange(num_tasks)]
  ## Uncomment the following line to use periods with max hyperperiod of 30.
  p_list = [random.choice([30,15,10,6,5,3,2,1]) for i in xrange(num_tasks)]

  if   (distribution == 1): # uniform(num_tasks)
    u_list = [random.uniform(0.001,1) for i in xrange(num_tasks)]
  elif (distribution == 2): #bimodal(num_tasks)
    u_list = generate_bimodal(p_list, num_tasks);
  elif (distribution == 3): #expon(num_tasks, 0.25)
    u_list = [random.expovariate(4) for i in xrange(num_tasks)]
  elif (distribution == 4): #expon(num_tasks, 0.5)
    u_list = [random.expovariate(2) for i in xrange(num_tasks)]
  else:
    assert False, "invalid distribution"

  # Normalize task utilizations to sum(u_list) == max_u
  u_sum = sum(u_list)
  normalization_factor = max_u / u_sum
  u_norm = [u*normalization_factor for u in u_list]
 
  u_norm_sum = 0
  for i in xrange(num_tasks):
    u_norm_sum += int(float('%.3f' % (u_norm[i]*1000)))

  misses = simulate_scheduler(p_list, u_norm, scheduler)
  if (misses > 0):
    sys.stderr.write('{0:d}\n'.format(misses))

if __name__ == "__main__":
  main()


