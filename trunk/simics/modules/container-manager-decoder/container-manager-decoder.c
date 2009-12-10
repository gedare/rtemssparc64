/*
  container-manager-decoder.c - decoder extension module for adding a user decoder
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>

#define DEVICE_NAME "container-manager-decoder"

/* Define OOO_MODE if Simics is running in out of order mode.
   It makes my nop dependent of %o1 */

/*#define OOO_MODE*/

/*
 * This module works with several CPU architectures, and uses
 * different encodings for different architectures.
 */
static uint8 v9_pcd[]         = { 0x81, 0xB8, 0x00, 0x00 };



static decoder_t my_decoder;
static conf_object_t *last_cpu = NULL;
static uint8 *pcd_code = NULL;
static int instr_size = 0;

static exception_type_t
my_nop(conf_object_t *cpu, unsigned int arg, void *user_data)
{
        printf("- Executing pcd (arg = %d) -\n", arg);
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
                pcd_code = v9_pcd;
                instr_size = 4;
        } else {
                printf("gica-user-decoder:" " Unknown CPU architecture: %s\n", arch);
        }
        last_cpu = cpu;
        printf("gica-decoder \"update_cpu\": CPU architecture: %s\n",arch);
}

static int
my_decode(uint8 *code, int valid_bytes, conf_object_t *cpu,
          instruction_info_t *ii, void *data)
{
		//printf("gica-decoder \"my_decode\"\n");
        if (cpu != last_cpu)
                update_cpu(cpu);

        if (instr_size > valid_bytes)
                return -instr_size;

        if (memcmp(code, pcd_code, instr_size) == 0) {
                printf("- Decoding pcd -\n");
                ii->ii_Type           = UD_IT_SEQUENTIAL;
                ii->ii_ServiceRoutine = my_nop;
                ii->ii_Arg            = 13;
                ii->ii_UserData       = 0;
#ifdef OOO_MODE
                ii->ii_RegInfo        = MM_ZALLOC(2, reg_info_t);
                ii->ii_RegInfo[0].id  = SIM_REG_ID_RN(9);
                ii->ii_RegInfo[0].in  = 1;
                ii->ii_RegInfo[0].out = 1;
#endif
                return instr_size;
        } else
                return 0;
}

static int
my_disassemble(uint8 *code, int valid_bytes, conf_object_t *cpu,
               char *string_buffer, void *data)
{
        //printf("gica-decoder \"my_disassemble\"\n");
        if (cpu != last_cpu)
                update_cpu(cpu);

        if (instr_size > valid_bytes)
                return -instr_size;

        if (memcmp(code, pcd_code, instr_size) == 0) {
                strcpy(string_buffer, "my pcd");
                return instr_size;
        } else
                return 0;
}

static int
my_flush(conf_object_t *cpu, instruction_info_t *ii, void *data)
{
        printf("gica-decoder \"my_flush\"\n");
        if (ii->ii_ServiceRoutine == my_nop) {
                /* This is the time to dealloc ii->ii_UserData */
                printf("# Flushing my nop #\n");
#ifdef OOO_MODE
                MM_FREE(ii->ii_RegInfo);
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
         printf("gica-decoder \"new_instance\"\n");

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


        printf("gica-decoder \"init_local\"\n");
}
