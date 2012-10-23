/*
 * hwpq-decoder.c - decoder extension module for adding a user decoder
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>
#include <simics/arch/sparc.h>

#include "opal-bitlib.h"
#include "pq.h"

#define QUEUE_SIZE (101)
priority_queue queues[1024];

#define DEVICE_NAME "hwpq-decoder"

static uint8 v9_impdep2[]         = { 0x81, 0xB8, 0x00, 0x00 };

//#define GAB_DEBUG
//#undef GAB_DEBUG


static decoder_t my_decoder;
static conf_object_t *last_cpu = NULL;
static uint8 *impdep2_code = NULL;
static int instr_size = 0;

  static exception_type_t
impdep2_execute(conf_object_t *cpu, unsigned int arg, void *user_data)
{
#ifdef GAB_DEBUG
  fprintf(stderr,"- Executing impdep2 (arg = %u = %X) -\n", arg, arg);
#endif
  // Re-package code
  unsigned int inst = arg;
  exception_type_t re = Sim_PE_No_Exception;

  // Pick the ones you need 
//  int op = maskBits32( inst, 30, 31 );
//  int op2 = maskBits32( inst, 22, 24 );
//  int op3 = maskBits32( inst, 19, 24 );
  int rd = maskBits32( inst, 25, 29 );
  int rs1 = maskBits32( inst, 14, 18 );
  int rs2 = maskBits32( inst, 0, 4 );
//  int opf_hi = maskBits32( inst, 9, 13 );
//  int opf_lo = maskBits32( inst, 5, 8 );
//  int cond = maskBits32( inst, 25, 28 );
//  int mcond = maskBits32( inst, 14, 17 );
//  int rcond = maskBits32( inst, 10, 12 );
  int i = maskBits32( inst, 13, 13 );
//  int asi = maskBits32( inst, 12, 5 );


  /* the impdep2 insn is tailored to particular uses.
   * there is always rs1 and rd field, and either 13-bit immediate
   * or rs2.
   */

  /* for the hw pq, need to implement:
   *  insert
   *  extract
   *  first (peek)
   *  setrange
   *
   *  insert requires a pointer and a priority value.
   *
   *  extract writes to a register, and does not need source regs.
   *
   *  first writes to a register, and does not need source regs.
   *
   *  setrange requires a pointer (base) and a bounds.
   *
   *  To multiplex these instructions, it is necessary to define another
   *  "opcode".  We are somewhat constrained, but we load the rs2 register
   *  with the additional information, reserving rs1 for the payload and 
   *  putting in rs2 the priority value (if any), an encoding of the 
   *  queue index to use, and an encoding of the operation to use.
   */

  // check the immediate bit
  if ( i ) {
    fprintf(stderr,"Unimplemented instruction\n");
    return Sim_PE_No_Exception;
  }
  int queue_idx;
  int operation;
  int priority;
//  int pointer_priority;
//  int last_operation;
  uint64 payload;
  uint64 encoded;
  uint64 result = 0;
  pq_node *node;
  pq_node *new_node;

  // maintain context for HWDS
  static uint64 context = 0;
  static uint64 pointer = 0;
  static int current_queue = 0;

  // FIXME: add per queue?
  static uint64 trap_payload = (uint64)-2;
//  static uint64 trap_context;

  payload = SIM_read_register(cpu, rs1);
  encoded = SIM_read_register(cpu, rs2);

  /* 31:20 specify the queue to use. */
  queue_idx = maskBits32( encoded, 31, 20 );
  //    priority = maskBits32( encoded, 19, 4 );
  priority = maskBits64( payload, 63, 32 );
  operation = maskBits32( encoded, 16, 0 );
//  last_operation = maskBits32( context, 16, 0 );

#ifdef GAB_DEBUG
  fprintf(stderr,"Queue %d\n", queue_idx);
  fprintf(stderr,"operation %d\n", operation);
  fprintf(stderr,"priority %d\n", priority);
#endif

  if ( operation != 5 ) {
    // first 32 bits are the previous context, except when requesting context
    context = (context<<32|encoded);
  }

  // check for exceptions
  if (operation != 5 /* get_context */ &&
      operation != 10 /* get payload */ &&
      operation != 11 /* adjust_spill_count */ &&
      operation != 12 /* get_size */ &&
      operation != 13 /* set current id */ && 
      operation != 14 /* get current id */ &&
      operation != 16 /* invalidate */ &&
      operation != 19 /* get trap payload */ &&
      operation != 20 /* set trap payload */ &&
      1 /* true */
     ) {
    // check if the chosen queue is available
    if ( queue_idx != current_queue) {
      #ifdef GAB_DEBUG
      fprintf(stderr, "context switch: %d->%d\n",current_queue, queue_idx); 
      #endif
      if ( trap_payload != (uint64)-2 ) {
        result = trap_payload;
        trap_payload = (uint64)-2;
        SIM_write_register(cpu, rd, result);
        return re;
      }
      pointer = payload;
      SIM_write_register(
          cpu, 
          SIM_get_register_number(cpu,"softint"), 
          (1<<4) /* 0x44 -- softint<4> */
          );
      re = Interrupt_Level_4;
    } else if (operation != 7 && pq_need_spill(&queues[queue_idx])) {
      SIM_write_register(
          cpu, 
          SIM_get_register_number(cpu,"softint"), 
          (1<<1) /* 0x41 -- softint<1> */
          );
      re = Interrupt_Level_1;
    } else if ( operation != 7 &&
                operation != 8 && pq_need_fill(&queues[queue_idx])) {
      #ifdef GAB_DEBUG
      /* need to refill, the head of the queue is not valid */
      fprintf(stderr,"%d\tFilling during operation %d. spill count %d\n",
          queue_idx,
          operation,
          queues[queue_idx].spill_count
          );
      #endif
      SIM_write_register(
          cpu,
          SIM_get_register_number(cpu,"softint"),
          (1<<2) /* 0x42 -- softint<2> */
          );
      re = Interrupt_Level_2;
    }
    if (re != Sim_PE_No_Exception) {
//      trap_context = encoded;
      //        SIM_write_register(cpu, rd, result);
      return re;
    }
  }

  switch ( operation ) {
    case 1: // first
      // FIXME: allow to failover (and check for past failover)
      if ( trap_payload != (uint64)-2 ) {
        result = trap_payload;
        trap_payload = (uint64)-2;
        SIM_write_register(cpu, rd, result);
        return re;
      }
      node = pq_first(&queues[queue_idx]);
      if (node) {
        result = (uint64)node->payload;
      } else {
        result = (uint64)-1;
      }
      break;

    case 2: // enqueue
      // FIXME: allow to failover (and check for past failover)
      if ( trap_payload != (uint64)-2 ) {
        result = trap_payload;
        trap_payload = (uint64)-2;
        //queues[queue_idx].spill_count++;

        SIM_write_register(cpu, rd, result);
        return re;
      }
      pointer = payload;
      new_node = (pq_node*)malloc(sizeof(pq_node));
      if (!new_node) {
        fprintf(stderr,"Unable to allocate space for new pq node\n");
        break; // should throw exception
      }
      if ( priority & (3<<30) ) {
        fprintf(stderr,"HWPQ: Max Priority reached\n");
      }
      new_node->priority = priority;
      new_node->payload = payload;
      pq_insert(&queues[queue_idx], new_node);
      new_node = NULL;
      break;

    case 3: // extract
      /* check the pointer field first */
      if ( trap_payload != (uint64)-2 ) {
        result = trap_payload;
        trap_payload = (uint64)-2;
        if ( result != (uint64)-1 )
          //queues[queue_idx].spill_count--;
        SIM_write_register(cpu, rd, result);
        return re;
      }

//      pointer_priority =  maskBits64( pointer, 63, 32 );
//      if ( pointer_priority == priority && last_operation == 18 ) {
//        /* hit! already extracted ... */
//        result = pointer;
//      } else {
        pointer = payload;
        node = pq_extract(&queues[queue_idx], priority);
        if (node) {
          result = (uint64)node->payload;
          free(node);
        } else { /* couldn't find the node in hw pq, complain to sw */
          SIM_write_register(
              cpu,
              SIM_get_register_number(cpu,"softint"),
              (1<<3) /* 0x43 -- softint<3> */
              );
          re = Interrupt_Level_3;
          result = (uint64)-1; /* FIXME: actually use this knowledge... */
        }
//      }
      break;

    case 4: // get current size
      result = queues[queue_idx].current_size;
      break;

    case 5: // getcontext
      result = context;
      break;

    case 6: // extract last
      node = pq_extract_last(&queues[queue_idx]);
      if (node) {
        result = (uint64)node->payload;
        free(node);
      }
      break;

    case 7: // spill at
      result = pq_spill(&queues[queue_idx], (uint32_t)payload);
      break;

    case 8: // fill at
      new_node = (pq_node*)malloc(sizeof(pq_node));
      if (!new_node) {
        fprintf(stderr,"Unable to allocate space for new pq node\n");
        break; // should throw exception
      }
      new_node->priority = priority;
      new_node->payload = payload;
      pq_fill(&queues[queue_idx], new_node);

      /* Avoid taking a spill exception while filling */
      if (pq_need_spill(&queues[queue_idx]))
        result = 1;
      break;

    case 9: // last prio
      node = pq_last(&queues[queue_idx]);
      if (node) {
        result = (uint64)node->priority;
      }
      break;

    case 10: // get pointer (get payload)
      result = pointer;
      break;

    case 11: // adjust spill count
      queues[queue_idx].spill_count += (int32_t)payload;
      break;

    case 12: // get size limit
      result = (uint64)queues[queue_idx].max_size;
      break;

    case 13: // set current id
      current_queue = queue_idx;
      break;

    case 14: // get current id
      result = current_queue;
      break;

      // FIXME 
    case 15: // set size limit
      pq_set_max_size(&queues[queue_idx], (uint32_t)payload);
      break;

    case 16: // invalidate
      pq_invalidate(&queues[queue_idx]);
      break;

    case 17: // search
      if ( trap_payload != (uint64)-2) {
        result = trap_payload;
        trap_payload = (uint64)-2;
        SIM_write_register(cpu, rd, result);
        return re;
      }

//      pointer_priority =  maskBits64( pointer, 63, 32 );
//      if ( pointer_priority == priority && last_operation == 18 ) { /* hit! */
//        result = pointer;
//      } else {
        pointer = payload;
        node = pq_search(&queues[queue_idx], priority);
        if (!node) {
          SIM_write_register(
              cpu,
              SIM_get_register_number(cpu,"softint"),
              (1<<3) /* 0x43 -- softint<3> */
              );
          re = Interrupt_Level_3;
          result = (uint64)-1; /* FIXME: actually use this knowledge... */
        } else {
          result = (uint64)node->payload;
        }
//      }
      break;

    case 18: // set payload
      pointer = payload; // use by emulation to communicate result
      break;

    case 19: // get trap payload
      result = trap_payload;
      break;

    case 20: // set trap payload
      trap_payload = payload;
      break;

    default:
      fprintf(stderr,"Unknown operation: %d\n", operation);
      return Sim_PE_No_Exception; // should throw exception
  }

  SIM_write_register(cpu, rd, result);
  return re;
}

/* This function is called when we are called from a new CPU. */
  static void
update_cpu(conf_object_t *cpu)
{
  attr_value_t attr = SIM_get_attribute(cpu, "architecture");
  const char *arch = attr.u.string;
  assert(attr.kind == Sim_Val_String);

  if (strcmp(arch, "sparc-v9") == 0) {
    impdep2_code = v9_impdep2;
    instr_size = 4;
  } else {
    fprintf(stderr,"gica-user-decoder:" " Unknown CPU architecture: %s\n", arch);
  }
  last_cpu = cpu;
#ifdef GAB_DEBUG
  fprintf(stderr,"gica-decoder \"update_cpu\": CPU architecture: %s\n",arch);
#endif
}

  static int
my_decode(uint8 *code, int valid_bytes, conf_object_t *cpu,
    instruction_info_t *ii, void *data)
{
  // Re-package code
  unsigned int inst = 0;
  inst = (code[0] << 24)|(code[1]<<16)|(code[2]<<8)|(code[3]);

  // Pick the ones you need 
  int op = maskBits32( inst, 30, 31 );
//  int op2 = maskBits32( inst, 22, 24 );
  int op3 = maskBits32( inst, 19, 24 );
//  int rd = maskBits32( inst, 25, 29 );
//  int rs1 = maskBits32( inst, 14, 18 );
//  int rs2 = maskBits32( inst, 0, 4 );
//  int opf_hi = maskBits32( inst, 9, 13 );
//  int opf_lo = maskBits32( inst, 5, 8 );
//  int cond = maskBits32( inst, 25, 28 );
//  int mcond = maskBits32( inst, 14, 17 );
//  int rcond = maskBits32( inst, 10, 12 );
//  int i = maskBits32( inst, 13, 13 );
//  int asi = maskBits32( inst, 12, 5 );
  
  if (cpu != last_cpu)
    update_cpu(cpu);

  if (instr_size > valid_bytes)
    return -instr_size;

  // compare op1
  switch (op) {

    case 2:
      switch (op3) {

        case 55:  // impdep2
#ifdef GAB_DEBUG
            fprintf(stderr,"gab: inst %X\n", inst);
            fprintf(stderr,"- Decoding impdep2 -\n");
#endif
            ii->ii_Type           = UD_IT_SEQUENTIAL;
            ii->ii_ServiceRoutine = impdep2_execute;
            ii->ii_Arg            = inst;
            ii->ii_UserData       = 0;

            return instr_size;
          break;

        default:
          break;
      }
    break;

    default:
      break;
  }
  
  // No match for user decoder
  return 0;
}

  static int
my_disassemble(uint8 *code, int valid_bytes, conf_object_t *cpu,
    char *string_buffer, void *data)
{
  unsigned int inst = 0;
  // Re-package code:
  inst = (code[0] << 24)|(code[1]<<16)|(code[2]<<8)|code[3];

  // Pick the ones you need 
  int op = maskBits32( inst, 30, 31 );
//  int op2 = maskBits32( inst, 22, 24 );
  int op3 = maskBits32( inst, 19, 24 );
//  int rd = maskBits32( inst, 25, 29 );
//  int rs1 = maskBits32( inst, 14, 18 );
//  int rs2 = maskBits32( inst, 0, 4 );
//  int opf_hi = maskBits32( inst, 9, 13 );
//  int opf_lo = maskBits32( inst, 5, 8 );
//  int cond = maskBits32( inst, 25, 28 );
//  int mcond = maskBits32( inst, 14, 17 );
//  int rcond = maskBits32( inst, 10, 12 );
//  int i = maskBits32( inst, 13, 13 );
//  int asi = maskBits32( inst, 12, 5 );

  //fprintf(stderr,"gica-decoder \"my_disassemble\"\n");
  if (cpu != last_cpu)
    update_cpu(cpu);

  if (instr_size > valid_bytes)
    return -instr_size;


  // compare op1
  switch (op) {

    case 2:
      switch (op3) {

        case 55:  // impdep2
          strcpy(string_buffer, "impdep2");
          return instr_size;
          break;

        default:
          break;
      }
    break;

    default:
      break;
  }
  
  return 0;
}

  static int
my_flush(conf_object_t *cpu, instruction_info_t *ii, void *data)
{
  //        fprintf(stderr,"gica-decoder \"my_flush\"\n");
  if (ii->ii_ServiceRoutine == impdep2_execute) {
    /* This is the time to dealloc ii->ii_UserData */
#ifdef GAB_DEBUG
    fprintf(stderr,"# Flushing impdep2 #\n");
#endif
    return 1;
  } else {
    /* Not mine */
    return 0;
  }
}


typedef struct {
  /* log_object_t must be the first thing in the device struct */
  log_object_t log;

  /* USER-TODO: Add user specific members here. The 'value' member
     is only an example to show how to implement an attribute */
  int value;
  int hwpq_size;
} empty_template_t;
/*
 * The new_instance() function is registered with the SIM_register_class
 * call (see init_local() below), and is used as a constructor for every
 * instance of the empty-template class.
 */
  static conf_object_t *
new_instance(parse_object_t *parse_obj)
{
  empty_template_t *empty = MM_ZALLOC(1, empty_template_t);
  SIM_log_constructor(&empty->log, parse_obj);

  /* USER-TODO: Add initialization code for new instances here */
#ifdef GAB_DEBUG
  fprintf(stderr,"gica-decoder \"new_instance\"\n");
#endif

  empty->hwpq_size = QUEUE_SIZE;

  for (int i = 0;i < 1024; i++) {
    pq_init(&queues[i],0,empty->hwpq_size);
    queues[i].queue_id = i;
  }

  return &empty->log.obj;
}

  static exception_type_t
operation(conf_object_t *obj, generic_transaction_t *mop, map_info_t info)
{
  empty_template_t *empty = (empty_template_t *)obj;
  int offset = (int)(mop->physical_address + info.start - info.base);

  /* USER-TODO: Handle accesses to the device here */
fprintf(stderr,"operation\n");
  if (SIM_mem_op_is_read(mop)) {
    SIM_log_info(2, &empty->log, 0,
        "Read from offset %d.", offset);
    SIM_set_mem_op_value_le(mop, 0);
  } else {
    SIM_log_info(2, &empty->log, 0,
        "Write to offset %d.", offset);
  }
  return Sim_PE_No_Exception;
}

  static set_error_t
set_hwpq_size_attribute(void *arg, conf_object_t *obj,
    attr_value_t *val, attr_value_t *idx)
{
  empty_template_t *empty = (empty_template_t *)obj;
  empty->hwpq_size = val->u.integer;
#ifdef GAB_DEBUG
  fprintf(stderr,"gica-decoder \"set pq size\"\n");
#endif

  for (int i = 0;i < 1024; i++) {
    pq_init(&queues[i],0,empty->hwpq_size);
  }

  return Sim_Set_Ok;
}

  static attr_value_t
get_hwpq_size_attribute(void *arg, conf_object_t *obj, attr_value_t *idx)
{
  empty_template_t *empty = (empty_template_t *)obj;
  return SIM_make_attr_integer(empty->hwpq_size);
}

  static set_error_t
set_value_attribute(void *arg, conf_object_t *obj,
    attr_value_t *val, attr_value_t *idx)
{
  empty_template_t *empty = (empty_template_t *)obj;
  empty->value = val->u.integer;
  return Sim_Set_Ok;
}

  static attr_value_t
get_value_attribute(void *arg, conf_object_t *obj, attr_value_t *idx)
{
  empty_template_t *empty = (empty_template_t *)obj;
  return SIM_make_attr_integer(empty->value);
}

  DLL_EXPORT void
init_local(void)
{
  class_data_t funcs;
  conf_class_t *conf_class;
  io_memory_interface_t *memory_iface;

  /* Register the empty device class. */
  memset(&funcs, 0, sizeof(class_data_t));
  funcs.new_instance = new_instance;
  funcs.description =
    "Description for " DEVICE_NAME " class.";
  conf_class = SIM_register_class(DEVICE_NAME, &funcs);

  /* Register the 'io-memory' interface, that is used to implement
     memory mapped accesses */
  memory_iface = MM_ZALLOC(1, io_memory_interface_t);
  memory_iface->operation = operation;
  SIM_register_interface(conf_class, IO_MEMORY_INTERFACE, memory_iface);

  /* USER-TODO: Add any attributes for the device here */

  SIM_register_typed_attribute(
      conf_class, "value",
      get_value_attribute, NULL,
      set_value_attribute, NULL,
      Sim_Attr_Optional,
      "i", NULL,
      "Value containing a valid valuable valuation.");

  my_decoder.decode = my_decode;
  my_decoder.disassemble = my_disassemble;
  my_decoder.flush = my_flush;

  SIM_register_arch_decoder(&my_decoder, "sparc-v9", 0);

  SIM_register_typed_attribute(
      conf_class, "hwpq_size",
      get_hwpq_size_attribute, NULL,
      set_hwpq_size_attribute, NULL,
      Sim_Attr_Optional,
      "i", NULL,
      "Hardware priority queue size.");


#ifdef GAB_DEBUG
  fprintf(stderr,"gica-decoder \"init_local\"\n");
#endif
}
