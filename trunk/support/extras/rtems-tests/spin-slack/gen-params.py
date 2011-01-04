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
      Usage: gen-params.py -[ha:p:d:u:]\n\
  -h --help           print this help\n\
  -a --atasks=        specify number of aperiodic tasks\n\
  -p --ptasks=        specify number of periodic tasks\n\
  -d --distribution=  specify utilization distribution, one of:\n\
                        1. Uniform distribution between 1/p and 1\n\
                        2. Bimodal distribution:\n\
                             Uniform in [0.5,1] with probability 1/9\n\
                             Uniform in [1/p,0.5] with probability 8/9\n\
                        3. Exponential distribution with mean 0.25\n\
                        4. Exponential distribution with mean 0.50\n\
  -u --utilization=   specify maximum utilization"

MAXIMUM_PERIOD = 50

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
    opts, args = getopt.getopt(sys.argv[1:], "ha:p:d:u:", 
        ["help", "atasks=", "ptasks=", "distribution=", "utilization="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
      sys.exit()
    elif opt in ("-a", "--atasks"):
      num_atasks = int(arg)
    elif opt in ("-p", "--ptasks"):
      num_ptasks = int(arg)
    elif opt in ("-d", "--distribution"):
      distribution = int(arg)
    elif opt in ("-u", "--utilization"):
      max_u = float(arg)
    else:
      assert False, "unhandled option"

  # Generate a list of num_tasks uniform random periods between 1 and 100.
  p_list = [int(1+random.random()*MAXIMUM_PERIOD) for i in xrange(num_ptasks)]

  Tasks = ''

  # Generate periodic tasks
  if   (distribution == 1): # uniform(num_tasks)
    u_list = [random.uniform(0.001,1) for i in xrange(num_ptasks)]
  elif (distribution == 2): #bimodal(num_tasks)
    u_list = generate_bimodal(p_list, num_ptasks);
  elif (distribution == 3): #expon(num_tasks, 0.25)
    u_list = [random.expovariate(4) for i in xrange(num_ptasks)]
  elif (distribution == 4): #expon(num_tasks, 0.5)
    u_list = [random.expovariate(2) for i in xrange(num_ptasks)]
  else:
    assert False, "invalid distribution"

  # Normalize task utilizations to sum(u_list) == max_u
  u_sum = sum(u_list)
  normalization_factor = max_u / u_sum
  u_norm = [u*normalization_factor for u in u_list]
 
  u_norm_sum = 0
  for i in xrange(num_ptasks):
    u_norm_sum += int(float('%.3f' % (u_norm[i]*1000)))

#  print(float(u_norm_sum)/1000)

  f = open('test_params.txt', 'w')
  f.write(str(float(u_norm_sum)/1000))
  f.write('\n')
  for i in xrange(num_ptasks):
    f.write(str(p_list[i]) + ',' + ('%.3f' % u_norm[i]) + '\n')
  f.write('\n')

  for i in xrange(num_ptasks):
    Tasks += '-T ' + str(p_list[i]) + ',' + ('%.3f' % u_norm[i]) + ',0 '

  # Generate aperiodic tasks. For now these are simply created as 
  # priority 200, execution time of 1 cycle, and immediate release time.
  for i in xrange(num_atasks):
    Tasks += '-A 200,1,0'

#  print Tasks
  os.system("lua gen-headers.lua " + Tasks)

if __name__ == "__main__":
  main()


