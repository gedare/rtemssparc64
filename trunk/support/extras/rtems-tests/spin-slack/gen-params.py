#
# gen-params.py
#
# Generate the parameters for a scheduler test case.
#
# Requires as arguments the number of tasks and the distribution of 
# processor utilization. The deadlines are assumed implicit with the
# task period, and the task periods are generated with a uniform 
# distribution between 1 and 100 clock ticks (1ms to 100ms, for 
# 1000 us/tick.
#

import random
import math
import sys
import getopt
import array
import os

def usage():
  print "\
      Usage: gen-params.py -[ht:d:u:]\n\
  -h --help           print this help\n\
  -t --tasks=         specify number of tasks\n\
  -d --distribution=  specify utilization distribution, one of:\n\
                        1. Uniform distribution between 1/p and 1\n\
                        2. Bimodal distribution:\n\
                             Uniform in [0.5,1] with probability 1/9\n\
                             Uniform in [1/p,0.5] with probability 8/9\n\
                        3. Exponential distribution with mean 0.25\n\
                        4. Exponential distribution with mean 0.50\n\
  -u --utilization=   specify maximum utilization"

MAXIMUM_PERIOD = 100

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "ht:d:u:", 
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
    else:
      assert False, "unhandled option"

  # Generate a list of num_tasks uniform random periods between 1 and 100.
  p_list = [int(1+random.random()*100) for i in xrange(num_tasks)]

  Tasks = ''

  if   (distribution == 1): # uniform(num_tasks)
    u_list = [random.uniform(0.001,1) for i in xrange(num_tasks)]
  elif (distribution == 2): #bimodal(num_tasks)
    print "bimodal"
  elif (distribution == 3): #expon(num_tasks, 0.25)
    print "exponential with mean 0.25"
  elif (distribution == 4): #expon(num_tasks, 0.5)
    print "exponential with mean 0.5"
  else:
    assert False, "invalid distribution"

  # Normalize task utilizations to sum(u_list) == max_u
  u_sum = sum(u_list)
  normalization_factor = max_u / u_sum
  u_norm = [u*normalization_factor for u in u_list]
 
  u_norm_sum = 0
  for i in xrange(num_tasks):
    u_norm_sum += int(float('%.3f' % (u_norm[i]*1000)))

  print(float(u_norm_sum)/1000)

  for i in xrange(num_tasks):
    Tasks += '-T ' + str(p_list[i]) + ',' + ('%.3f' % u_norm[i]) + ',0 '

  print Tasks
  os.system("lua gen-headers.lua " + Tasks)

if __name__ == "__main__":
  main()


