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

#include "opal-bitlib.h"
#include "pq.h"

priority_queue ready_queue;
priority_queue timer_queue;

#define DEVICE_NAME "hwpq-decoder"

static uint8 v9_impdep2[]         = { 0x81, 0xB8, 0x00, 0x00 };

#define GAB_DEBUG

static decoder_t my_decoder;
static conf_object_t *last_cpu = NULL;
static uint8 *impdep2_code = NULL;
static int instr_size = 0;

  static exception_type_t
impdep2_execute(conf_object_t *cpu, unsigned int arg, void *user_data)
{
#ifdef GAB_DEBUG
  printf("- Executing impdep2 (arg = %u = %X) -\n", arg, arg);
#endif
  // Re-package code
  unsigned int inst = arg;

  // Pick the ones you need 
//  int op = maskBits32( inst, 30, 31 );
//  int op2 = maskBits32( inst, 22, 24 );
//  int op3 = maskBits32( inst, 19, 24 );
  int rd = maskBits32( inst, 25, 29 );
  int rs1 = maskBits32( inst, 14, 18 );
//  int rs2 = maskBits32( inst, 0, 4 );
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
   *
   *  insert requires a pointer and a priority value, so two registers, 
   *  but does not require to write to a register.
   *
   *  extract writes to a register, and does not need source regs.
   *
   *  first writes to a register, and does not need source regs.
   *
   *  To multiplex these instructions, it is necessary to define another
   *  "opcode".  We are somewhat constrained, but we can use the rd register
   *  in insert, without actually writing anything to it.  This is hackish, 
   *  but should work. So rd will hold the input pointer for insert, and the
   *  output pointer for extract or first. rs1 will hold the priority 
   *  value for insert, and ignored by extract and first.  It might be useful
   *  later to use the rs1 field for setting up some conditions perhaps.
   *  
   *  This leaves the immediate field available for encoding the instruction 
   *  type and any other options that might be useful.  In particular, 
   *  the priority queue to manipulate should also be specified somehow.
   */

  // check the immediate bit
  if ( i ) {
    /* don't sign extend on purpose */
    int imm = maskBits32( inst, 12, 0 );

    /* high order bit(s) specify the queue to use. 
     * For now there are only two queues, but we will use 2 bits
     * so that it can support up to 4 queues.
     */
    int queue = maskBits32( imm, 12, 11 );

    /* low order bits specify the queue operation. */
    int operation = maskBits32( imm, 2, 0 );
    pq_node *node = NULL;
    static pq_node *new_node = NULL;
    static int flag = 0;
    uint64 payload = 0;

    switch ( queue ) {
      case 0:   // ready queue
        switch ( operation ) {
          case 0:   // first
            node = pq_first(&ready_queue);
            if (node) {
              SIM_write_register(cpu, rd, (uint64)node->payload);
            } else {
              SIM_write_register(cpu, rd, 0);
            }
            break;
          case 1:   // extract
            payload = SIM_read_register(cpu, rs1);
            node = pq_extract(&ready_queue, payload);
            if (node)
              free(node);
            break;

          /* There are two cases to cover insert. This is because of the 
           * lack of enough fields to properly encode instructions and 
           * registers. In a true ISA extension, we could add extra opcode,
           * but we are constrained to the impdep2 insn.
           * As long as both instructions are executed, the node should be 
           * properly generated and inserted; the order of instructions 
           * should not affect the correctness of these operations...
           * This is not thread-safe (in the target) since it is non-reentrant
           * in the simulator :(
           */
          case 2:   // insert
            if (!flag) {
              new_node = (pq_node*)malloc(sizeof(pq_node));
              flag = 1;
            } else
              flag = 0;
            if (!new_node) {
              printf("Unable to allocate space for new pq node\n");
              return Sim_PE_No_Exception; // should throw exception
            }
            new_node->priority = SIM_read_register(cpu, rs1);
            if (!flag) {
              pq_insert(&ready_queue, new_node);
              new_node = NULL;
            }
            break;
          case 3:   // insert
             if (!flag) {
              new_node = (pq_node*)malloc(sizeof(pq_node));
              flag = 1;
            } else
              flag = 0;
            if (!new_node) {
              printf("Unable to allocate space for new pq node\n");
              return Sim_PE_No_Exception; // should throw exception
            }
            new_node->payload = SIM_read_register(cpu, rs1);
            if (!flag) {
              pq_insert(&ready_queue, new_node);
              new_node = NULL;
            }
            break;

          default:
            printf("Unknown operation: %d\n", operation);
            return Sim_PE_No_Exception; // should throw exception
        }
        
        break;
      case 1:   // timer queue
        switch ( operation ) {
          case 0:   // first
            node = pq_first(&timer_queue);
            if (node) {
              SIM_write_register(cpu, rd, (uint64)node->payload);
            } else {
              SIM_write_register(cpu, rd, 0);
            }
            break;
          case 1:   // extract
            payload = SIM_read_register(cpu, rs1);
            node = pq_extract(&timer_queue, payload);
            if (node) {
              free(node);
            }
            break;

          /* There are two cases to cover insert. This is because of the 
           * lack of enough fields to properly encode instructions and 
           * registers. In a true ISA extension, we could add extra opcode,
           * but we are constrained to the impdep2 insn.
           * As long as both instructions are executed, the node should be 
           * properly generated and inserted; the order of instructions 
           * should not affect the correctness of these operations...
           * Note that this is not thread-safe.
           */
          case 2:   // insert
            if (!flag) {
              new_node = (pq_node*)malloc(sizeof(pq_node));
              flag = 1;
            } else
              flag = 0;
            if (!new_node) {
              printf("Unable to allocate space for new pq node\n");
              return Sim_PE_No_Exception; // should throw exception
            }
            new_node->priority = SIM_read_register(cpu, rs1);
            if (!flag) {
              pq_insert(&timer_queue, new_node);
              SIM_write_register(cpu, rd, new_node->priority);
              new_node = NULL;
            }
            break;
          case 3:   // insert
             if (!flag) {
              new_node = (pq_node*)malloc(sizeof(pq_node));
              flag = 1;
            } else
              flag = 0;
            if (!new_node) {
              printf("Unable to allocate space for new pq node\n");
              return Sim_PE_No_Exception; // should throw exception
            }
            new_node->payload = SIM_read_register(cpu, rs1);
            if (!flag) {
              pq_insert(&timer_queue, new_node);
              SIM_write_register(cpu, rd, new_node->priority);
              new_node = NULL;
            }
            break;

          default:
            printf("Unknown operation: %d\n", operation);
            return Sim_PE_No_Exception; // should throw exception
        }
        break;

      default:
        printf("Unknown queue operation\n");
        return Sim_PE_No_Exception; // fail silently, maybe throw exception?
        break;
    }

    

  } else {
    printf("Unimplemented instruction\n");
  }

  

  return Sim_PE_No_Exception;
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
    printf("gica-user-decoder:" " Unknown CPU architecture: %s\n", arch);
  }
  last_cpu = cpu;
#ifdef GAB_DEBUG
  printf("gica-decoder \"update_cpu\": CPU architecture: %s\n",arch);
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
            printf("gab: inst %X\n", inst);
            printf("- Decoding impdep2 -\n");
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

  //printf("gica-decoder \"my_disassemble\"\n");
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
  //        printf("gica-decoder \"my_flush\"\n");
  if (ii->ii_ServiceRoutine == impdep2_execute) {
    /* This is the time to dealloc ii->ii_UserData */
#ifdef GAB_DEBUG
    printf("# Flushing impdep2 #\n");
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
  printf("gica-decoder \"new_instance\"\n");
#endif

  return &empty->log.obj;
}

  static exception_type_t
operation(conf_object_t *obj, generic_transaction_t *mop, map_info_t info)
{
  empty_template_t *empty = (empty_template_t *)obj;
  int offset = (int)(mop->physical_address + info.start - info.base);

  /* USER-TODO: Handle accesses to the device here */

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

  ready_queue.head = NULL;
  ready_queue.options = 0;
  timer_queue.head = NULL;
  timer_queue.options = 2;

#ifdef GAB_DEBUG
  printf("gica-decoder \"init_local\"\n");
#endif
}
