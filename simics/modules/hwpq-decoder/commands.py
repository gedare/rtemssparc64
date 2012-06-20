
##  Copyright 2000-2007 Virtutech AB
##  
##  The contents herein are Source Code which are a subset of Licensed
##  Software pursuant to the terms of the Virtutech Simics Software
##  License Agreement (the "Agreement"), and are being distributed under
##  the Agreement.  You should have received a copy of the Agreement with
##  this Licensed Software; if not, please contact Virtutech for a copy
##  of the Agreement prior to using this Licensed Software.
##  
##  By using this Source Code, you agree to be bound by all of the terms
##  of the Agreement, and use of this Source Code is subject to the terms
##  the Agreement.
##  
##  This Source Code and any derivatives thereof are provided on an "as
##  is" basis.  Virtutech makes no warranties with respect to the Source
##  Code or any derivatives thereof and disclaims all implied warranties,
##  including, without limitation, warranties of merchantability and
##  fitness for a particular purpose and non-infringement.

from cli import *
import sim_commands

def new_hwpq_cmd():
  hwpq_name = "hwpq0"
  try:
    obj = SIM_get_object(hwpq_name)
    print "HWPQ '%s' already exists." % hwpq_name
    SIM_command_has_problem()
    return (obj,)
  except:
    pass

  try:
    obj = SIM_create_object("hwpq-decoder", hwpq_name, [])
    print ("'%s' created." % hwpq_name)
    return (obj,)
  except:
    print "Failed creating hwpq-decoder object '%s'." % hwpq_name
    SIM_command_has_problem()

new_command("new-hwpq",
            new_hwpq_cmd,
            [],
            alias = "",
            type = "hwpq-decoder commands",
            short = "create a new hwpq",
            doc = """ Create a new hwpq instance """
           )

def hwpq_set_size(obj, size):
  try:
    obj.hwpq_size = size
  except:
    print "Error setting hwpq size"

new_command("set_size",
            hwpq_set_size,
            [arg(int_t, "hwpq_size")],
            type = "hwpq-decoder commands",
            short = "set the hwpq size",
            namespace = "hwpq-decoder",
            doc =""" Set a hwpq size """
           )

# increment command
def increment_value_cmd(obj):
    try:
        obj.value += 1
        print "The new counter value is:", obj.value
    except Exception, msg:
        print "Error incrementing counter:", msg

new_command("increment", increment_value_cmd, [],
            alias = "",
            type  = "hwpq-decoder commands",
            short = "increment value",
	    namespace = "hwpq-decoder",
            doc = """
Increments the value by adding 1 to it.
""")

#
# ------------------------ info -----------------------
#

def get_info(obj):
    # USER-TODO: Return something useful here
    return []

sim_commands.new_info_command('hwpq-decoder', get_info)

#
# ------------------------ status -----------------------
#

def get_status(obj):
    # USER-TODO: Return something useful here
    return [("Internals",
             [("Attribute 'value'", SIM_get_attribute(obj, "value"))])]

sim_commands.new_status_command('hwpq-decoder', get_status)
