// Some helper functions

#ifndef _GAB_HELPER_H_
#define _GAB_HELPER_H_

#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>

// reads @a length up to 8 bytes from memory
uint64 myMemoryRead(generic_address_t vaddr, int length)
{
  SIM_clear_exception();
  assert(length <= 8);

  physical_address_t physaddr =
    SIM_logical_to_physical(SIM_current_processor(),Sim_DI_Data, vaddr);

  return SIM_read_phys_memory(SIM_current_processor(), physaddr, length);
}

// writes @a length bytes to memory from @a value
void myMemoryWrite(generic_address_t vaddr, uint64 value, int length)
{
  SIM_clear_exception();
  assert(length <= 8);

  physical_address_t physaddr =
    SIM_logical_to_physical(SIM_current_processor(),Sim_DI_Data, vaddr);

  SIM_write_phys_memory(SIM_current_processor(), physaddr, value, length);
}


#endif
