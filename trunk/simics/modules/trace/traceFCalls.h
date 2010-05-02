/*by Eugen Leontie*/

#ifndef COMPONENTS_H
#define COMPONENTS_H

#define STRINGTABLEID 24

typedef enum mem_cmd {
  Read,			/* read memory from target (simulated prog) to host */
  Write			/* write memory from host (simulator) to target */
}mem_tp;

typedef struct container_def container;
typedef struct stack_Object_def stackObject;
typedef struct cell *list;//Linked List
typedef unsigned long long md_addr_t;

typedef struct addressCell *addressList; //address access list

struct container_def
{
	md_addr_t entryAddress; // the entry address in the container
	md_addr_t endAddress; // the end address (using this as a default return for non-function symbols)
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

	addressList staticAddressList; //the static values will add-up here as the tracing is performed

	int nonFunction; //1 or 0. For non-function do not trace this symbol, but it might be sometimes usefull to print it
	char linenumber[1000]; //if this info is available for symbols, load it and display it in the trace
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
  md_addr_t startAddress;
  md_addr_t endAddress;
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


void container_initialize( base_trace_t *bt);
void container_close();


container * container_add(md_addr_t addr, char * name);

struct loadingPenalties container_traceFunctioncall(md_addr_t addr, mem_tp * mem, base_trace_t *obj);
void container_printStatistics();
void container_MemoryCall(mem_tp cmd,md_addr_t addr, int nbytes);

//void container_SetRegisterFile( struct regs_t *regs);
//void container_dumpRegisters(struct regs_t regfile);

void myprint(char * toPrint);

char * funcNameFromAddress(int addr);

void printDecodedAddressList(char * printbuff,addressList l);
void updateHeapCalls(container* c,addressList l);
void updateGlobalAddressList(container* c);
int penaltyAddressList( addressList l);





void loadContainersFromSymtable(const char* symFileName);
void setFullTraceFile(base_trace_t *bt);

//the container list is sorted by address
container * search(md_addr_t addr);

//this value keep a state that tells the container manager to ignore a duplicate push due to a trap
int ignore_due_to_Exception; 

void toStringRTEMSTaksName(char * dest, int _name);


#define printRTEMSTaksName( _name) \
{\
	int32   c0, c1, c2, c3; \
	\
	c0 = ((_name) >> 24) & 0xff; \
	c1 = ((_name) >> 16) & 0xff; \
	c2 = ((_name) >> 8) & 0xff; \
	c3 = (_name) & 0xff; \
	putchar( (char)c0 ); \
	if ( c1 ) putchar( (char)c1 ); \
	if ( c2 ) putchar( (char)c2 ); \
	if ( c3 ) putchar( (char)c3 ); \
}


void itoa(int n, char s[]);
void reverse(char s[]);



//Adding thread awareness to the containers. 
// We need a list of containers, not only one, one for each threads. 
// 1. a list of Threads ( circular linked list ), as data : thread id, thread name, container 
// 2.global Thread_active pointer to the current thread. 
// 3.Thread_switch(thread id, thread name), an external event that detects the thread switch. 

typedef struct thread_monitor
{
	struct thread_monitor* next;
	int64 thread_id;
	int64 thread_name;
	mystack container_runtime_stack;
	FILE *traceFD;
	uint64 minStack;
	uint64 maxStack;
	uint64 minFP;
	uint64 maxFP;
	
} thread_monitor_t;

thread_monitor_t *thread_active;
thread_monitor_t* ThreadAdd(int64 id, int64 name);
thread_monitor_t* ThreadFind(int64 id);
void Thread_switch( int64 id, int64 name);

void printCurrentContainerStack( );

uint64 getSP();
uint64 getFP();
uint64 getRet();
void printThreads();




#endif /* COMPONENTS_H */
