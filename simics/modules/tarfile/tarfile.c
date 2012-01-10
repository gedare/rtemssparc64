/*
  tarfile.c - a Simics device that supports copying a tarfile into the
        simulated system's memory space.

  Copyright 1998-2007 Virtutech AB
  
  The contents herein are Source Code which are a subset of Licensed
  Software pursuant to the terms of the Virtutech Simics Software
  License Agreement (the "Agreement"), and are being distributed under
  the Agreement.  You should have received a copy of the Agreement with
  this Licensed Software; if not, please contact Virtutech for a copy
  of the Agreement prior to using this Licensed Software.
  
  By using this Source Code, you agree to be bound by all of the terms
  of the Agreement, and use of this Source Code is subject to the terms
  the Agreement.
  
  This Source Code and any derivatives thereof are provided on an "as
  is" basis.  Virtutech makes no warranties with respect to the Source
  Code or any derivatives thereof and disclaims all implied warranties,
  including, without limitation, warranties of merchantability and
  fitness for a particular purpose and non-infringement.

*/   

#include <string.h>

#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>

#define DEVICE_NAME "tarfile"

#include "tarfile.h"

typedef struct tarfile {
  /* log_object_t must be the first thing in the device struct */
  log_object_t log;

  char *filename;
  uintptr_t base;
} tarfile_t;

/*
 * This function is registered with the SIM_register_class
 * call (see init_local() below), and is used as a constructor
 * for every instance of the tarfile class.
 */  
static conf_object_t * tarfile_new_instance(parse_object_t *parse_obj)
{
  tarfile_t *tarfile = MM_ZALLOC(1, tarfile_t);
  SIM_log_constructor(&tarfile->log, parse_obj);
  return &tarfile->log.obj;
}

/* Dummy function that doesn't really do anything. */
static void simple_function(conf_object_t *obj)
{
	tarfile_t *dev = (tarfile_t *) obj;

  SIM_log_info(1, &dev->log, 0, "'simple_function' called.");
}

/*** Attribute accessors/mutators ***/
static set_error_t set_filename_attribute(
    void *arg, 
    conf_object_t *obj, 
    attr_value_t *val, 
    attr_value_t *idx
) {
  tarfile_t *tarfile = (tarfile_t *)obj;
  tarfile->filename = strdup(val->u.string);
  return Sim_Set_Ok;
}

static attr_value_t get_filename_attribute(
    void *arg,
    conf_object_t *obj,
    attr_value_t *idx
) {
  tarfile_t *tarfile = (tarfile_t *)obj;
  return SIM_make_attr_string(tarfile->filename);
}

static set_error_t set_base_attribute(
    void *arg, 
    conf_object_t *obj, 
    attr_value_t *val, 
    attr_value_t *idx
) {
  tarfile_t *tarfile = (tarfile_t *)obj;
  tarfile->base = val->u.integer;
  return Sim_Set_Ok;
}

static attr_value_t get_base_attribute(
    void *arg,
    conf_object_t *obj,
    attr_value_t *idx
) {
  tarfile_t *tarfile = (tarfile_t *)obj;
  return SIM_make_attr_integer(tarfile->base);
}

static set_error_t set_add_log_attribute(
    void *arg,
    conf_object_t *obj,
    attr_value_t *val,
    attr_value_t *idx
) {
  tarfile_t *tarfile = (tarfile_t *)obj;
  SIM_log_info(1, &tarfile->log, 0, val->u.string);
  return Sim_Set_Ok;
}

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * init_local() is called once when the device module is loaded into Simics.
 */
void init_local(void)
{
  class_data_t funcs;
  conf_class_t *tarfile_class;
  tarfile_interface_t *tarfile_interface;

  /*
   * Register the tarfile device class. The 'tarfile_new_instance'
   * function serve as a constructor, and is called every time
   * a new instance is created.
   */
  memset(&funcs, 0, sizeof(class_data_t));
  funcs.new_instance = tarfile_new_instance;
  funcs.description =
    "The tarfile device is a dummy device that compiles and "
    "that can be loaded into Simics. It supports writing a "
    "tarfile directly into the memory of the simulated system.";

  tarfile_class = SIM_register_class(DEVICE_NAME, &funcs);

  /*
   * Register the 'tarfile-interface', which is an example
   * of a unique, customized interface that we've implemented
   * for this device.
   */
  tarfile_interface = MM_ZALLOC(1, tarfile_interface_t);
  tarfile_interface->simple_function = simple_function;
  SIM_register_interface(tarfile_class, "tarfile_interface", tarfile_interface);

  /*
   * Register attributes (device specific data) together with
   * functions for getting and setting these attributes.
   * The 'Sim_Attr_Optional' attribute will be saved with a configuration
   */
  SIM_register_typed_attribute(
    tarfile_class, "base",
    get_base_attribute, NULL,
    set_base_attribute, NULL,
    Sim_Attr_Optional, "i", NULL,
    "The <i>base</i> address field.");

  SIM_register_typed_attribute(
    tarfile_class, "filename",
    get_filename_attribute, NULL,
    set_filename_attribute, NULL,
    Sim_Attr_Optional, "s", NULL,
    "The <i>filename</i> field.");

  /* Pseudo attribute, not saved in configuration */
  SIM_register_typed_attribute(
    tarfile_class, "add_log",
    0, NULL,
    set_add_log_attribute, NULL,
    Sim_Attr_Pseudo,
    "s", NULL,
    "<i>Write-only</i>. Strings written to this "
    "attribute will end up in the device's log file.");
}

#if defined(__cplusplus)
}
#endif
