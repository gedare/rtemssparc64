
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

def function_tracer_command(int_arg, str_arg, flag_arg):
	print "My integer: %d" % int_arg
	print "My string: %s" % str_arg
	print "Flag is",
	if flag_arg:
		print "given"
	else:
		print "not given"
	return int_arg

new_command("function_tracer_command", function_tracer_command,
	args = [arg(int_t, "arg", "?", 10), arg(str_t, "name"),
	arg(flag_t, "-f")],
	type = "function_tracer_commands",
	short = "my command does it",
	doc_items = [("NOTE", "This command is best")],
	see_also = ["help"],
	doc = """
<b>my-command</b> is best.
This is its documentation. <i>arg</i>
is the first argument...""")

