
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

#
# new-tarfile
# instantiate a new tarfile object. only allows one object (tarfile0) to be
# instantiated.
#
def new_tarfile_cmd():
  tarfile_name = "tarfile0"
  try:
    obj = SIM_get_object(tarfile_name)
    print "'%s' already exists." % tarfile_name
    SIM_command_has_problem()
    return (obj,)
  except:
    pass

  try:
    obj = SIM_create_object("tarfile", tarfile_name, [])
    print ("'%s' created." % tarfile_name)
    return (obj,)
  except:
    print "Failed creating tarfile object '%s'." % tarfile_name
    SIM_command_has_problem()

new_command("new-tarfile",
            new_tarfile_cmd,
            [],
            alias = "",
            type = "tarfile commands",
            short = "create a new tarfile",
            doc = """ Create a new tarfile instance """
           )
#
# tarfile.load
#
def tarfile_load_cmd(obj):
  try:
    SIM_load_file(conf.phys_mem, obj.filename, obj.base, 1)
    print "Loaded file '%(f)s' at 0x%(b)X" % {"f": obj.filename, "b": obj.base}
    
  except:
    print "Failed copying tarfile object."
    SIM_command_has_problem()

new_command("load",
            tarfile_load_cmd,
            [],
            alias = "",
            type = "tarfile commands",
            short = "load tarfile",
      	    namespace = "tarfile",
            doc = """ Load the tarfile into memory """
           )

#
# ------------------------ info -----------------------
#

def get_tarfile_info(obj):
    return []

sim_commands.new_info_command('tarfile', get_tarfile_info)

#
# ------------------------ status -----------------------
#

def get_tarfile_status(obj):
    return [(None, 
            [("Attribute 'filename'", obj.filename)],
            [("Attribute 'base'", obj.base)]
            )]

sim_commands.new_status_command('tarfile', get_tarfile_status)

#
# ------------------------ add-log -----------------------
#

def add_log_cmd(obj, str):
    try:
        obj.add_log = str
    except Exception, msg:
        print "Error adding log string: %s" % msg


new_command("add-log", add_log_cmd,
            [arg(str_t, "log-string", "?", "default text")],
            alias = "",
            type  = "tarfile commands",
            short = "add a text line to the device log",
      	    namespace = "tarfile",
            doc = """
Add a line of text to the device log. Use the 'io' command to view the log.<br/>
""")

#
# set-filename
#
def tarfile_set_filename(obj, filename):
  try:
    obj.filename = filename
    print "Filename set to '%s'" % obj.filename
  except:
    print "Error setting tarfile filename"

new_command("set-filename",
            tarfile_set_filename,
            [arg(filename_t(0,1,0), "filename")],
            type = "tarfile commands",
            short = "set the tarfile filename",
            namespace = "tarfile",
            doc =""" Set a tarfile filename """
           )

#
# set-base
#
def tarfile_set_base(obj, base):
  try:
    obj.base = base
    print "Base address set to 0x%X" % obj.base
  except:
    print "Error setting tarfile base"

new_command("set-base",
            tarfile_set_base,
            [arg(uint64_t, "base")],
            type = "tarfile commands",
            short = "set the tarfile base",
            namespace = "tarfile",
            doc =""" Set a tarfile base """
           )

