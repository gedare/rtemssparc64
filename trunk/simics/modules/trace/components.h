/*by Eugen Leontie*/

#ifndef COMPONENTS_H
#define COMPONENTS_H


#define STRINGTABLEID 24

#include <stdio.h>

#include "host.h"
#include "misc.h"
#include "machine.h"
#include "regs.h"
#include "memory.h"
#include "target-arm/elf.h"

typedef struct container_def container; 
typedef struct stack_Object_def stackObject;
typedef struct cell *list;//Linked List


typedef struct addressCell *addressList; //address access list  

struct container_def
{
	md_addr_t entryAddress; // the entry address in the container
	char name[100];			 //name of the function
	int totalStackPushes;
	int totalStackPops;
	int totalNumberOfReads;
	int totalNumberOfBytesRead;
	int totalNumberOfBytesWritten;
	int totalNumberOfWrites;
	int totalChildContainersCalled;
	int uniqueChildContainersCalled; // count the number of child functions
	int	staticAddressCount;
	int traceLoadedAddressCount; //this value will be added from the trace file on the second run
	int traceLoadeduniqueChildContainersCalled;//this value will be added from the trace file on the second run
	int addressAccessListPenalty;
	char isCalledWithHeapData; //counts the number of heap regions used. I will use it to recognize where in the code should I put ASSIGN directives
	
	list childFunctions; 			 //keep the id of the child function ( or the entire container )
	addressList addressAccessList;  //keeps the global access list for mem Regions that this function accesses
	addressList addressAccessListInstance;  //keeps the access list for mem Regions that this function accesses, only for the last instance called

	list *staticAddressList; //the static values will add-up here as the tracing is performed


};

struct stack_Object_def
{
	container * container;
	md_addr_t returnAddress; //dynamic location . keeps the return address of the last f. call. each return should equal this value ( other wise it is an attack)	
};

struct loadingPenalties
{
	int containerStaticListSize;
	int containerDynamicListSize;
};

extern container containerTable[];
extern int componentTableSize ;
extern int containerSize ;

struct addressCell
{
  int startAddress;
  int endAddress;
  addressList next;
};


struct cell
{
  stackObject element;
  list next;
};
extern list cons(stackObject element, list l);
extern list cdr_and_free(list l);

extern addressList consAddressList(int startAddress, int endAddress, addressList l);
extern addressList freeAddressList(addressList l);
void UpdateAddressList(addressList *l,md_addr_t addr,int nbytes);
void joinAddress(addressList future, addressList present);
void printAddressList(char* printbuff, addressList l);



//end Linked List

//Stack

struct mystack
{
  list elements;
  int size;
  int maxsize;
};

typedef struct mystack *mystack;

/* create a new, empty stack */
extern mystack stack_create(void);

/* push a new element on top of the stack */
extern void stack_push(mystack s, stackObject element);

/* pop the top element from the stack.	The stack must not be
   empty. */
extern void stack_pop(mystack s);

/* return the top element of the stack */
extern stackObject stack_top(mystack s);

/* return a true value if and only if the stack is empty */
extern int stack_empty(mystack s);


//end Stack


void container_initialize(void);
void container_add(md_addr_t addr, char * name);

struct loadingPenalties container_traceFunctioncall(md_addr_t addr, struct mem_t * mem);
void container_printStatistics();
void container_MemoryCall(enum mem_cmd cmd,md_addr_t addr, int nbytes);

void container_SetRegisterFile( struct regs_t *regs);
void container_dumpRegisters(struct regs_t regfile);

void myprint(char * toPrint);

char * funcNameFromAddress(int addr);


#endif /* COMPONENTS_H */

