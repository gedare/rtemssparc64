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
import sys
import getopt

def usage():
  print "\
      Usage: gen-params.py -[ht:d:]\n\
  -h --help           print this help\n\
  -t --tasks=         specify number of tasks\n\
  -d --distribution=  specify utilization distribution, one of:\n\
                        1. Uniform distribution between 1/p and 1\n\
                        2. Bimodal distribution:\n\
                             Uniform in [0.5,1] with probability 1/9\n\
                             Uniform in [1/p,0.5] with probability 8/9\n\
                        3. Exponential distribution with mean 0.25\n\
                        4. Exponential distribution with mean 0.50"



def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], "ht:d:", 
        ["help", "tasks=", "distribution="])
  except getopt.GetoptError, err:
    print str(err)
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ("-h", "--help"):
      usage()
      sys.exit()
    elif opt in ("-t", "--tasks"):
      num_tasks = arg
    elif opt in ("-d", "--distribution"):
      distribution = arg
    else:
      assert False, "unhandled option"

#  x = random.random()

#  print x


if __name__ == "__main__":
  main()


