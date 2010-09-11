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
import subprocess

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
      max_u = int(arg)
    else:
      assert False, "unhandled option"

  utilization = 0
  Tasks = ''

## Loop through all of the tasks, creating each task with a 
## uniform period between 1 and 100, and utilization based on 
## the distribution parameter.
  for i in xrange(num_tasks):
    # Uniform random period between 1 and 100.
    period = math.floor(1 + random.random()*MAXIMUM_PERIOD) ## 1 + [0.0,1.0)*100

    if   (distribution == 1): # uniform(num_tasks)
      u = random.uniform(0.001, max_u / float(num_tasks))
    elif (distribution == 2): #bimodal(num_tasks)
      print "bimodal"
    elif (distribution == 3): #expon(num_tasks, 0.25)
      print "exponential with mean 0.25"
    elif (distribution == 4): #expon(num_tasks, 0.5)
      print "exponential with mean 0.5"
    else:
      assert False, "invalid distribution"

    if (u < 0.001): 
      u = 0.001
    if (u > 0.999):
      u = 0.999

    utilization = utilization + u

    # Keep only 3 decimal places
    precise_u = '%.3f' % u

    Tasks += '-T ' + str(int(period)) + ',' + precise_u + ',0 '

  print Tasks
  subprocess.call(["lua","gen-headers.lua " + Tasks])

if __name__ == "__main__":
  main()


