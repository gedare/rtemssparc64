/*by Eugen Leontie*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "traceFCalls.h"



typedef struct traceData_def traceData;
//External Variables
container containerTable[6000]; 	//this array contains all containers( functions and is loaded in Loader.c)
int containerSize = 0;				//keps the size of the containerList
int componentTableSize = 1;

//end External Variables

//local variables
mystack returnAddressStack; 	//this is the main Container stack
int containerInitialized = 0; 	//local flag to track wheater the containers were initialized
md_addr_t return_addr;			//monitors the return addresses ( saved before a function call and stored to stack)



struct regs_t *regs;			//this pointer gives access to the simulated register file

char printBuffer[10000];			// use sprintf to print to this buffer and then use myprint for the final print ( full trace )
char printBuffer2[10000];			// use sprintf to print to this buffer and then use myprint for the final print ( second run trace )
char compilerPrintBuffer[10000];	// use sprintf to print to this buffer and then use myprint for the final print ( compiler trace )


struct traceData_def{
	char name[100];
	int traceLoadedAddressCount;
	int traceLoadeduniqueChildContainersCalled;
};

traceData traceLoader[6000];
int traceLoaderSize;


FILE *tracefdIn = NULL; //TODO !!
FILE *tracefdOut = NULL;
FILE *compilerInfofd = NULL;
FILE *fullTracefd = NULL;
int ld_text_base;
int ld_text_bound;
int ld_data_base;
int ld_data_bound;
int ld_stack_size;
int ld_stack_base;
int ld_environ_base;


conf_object_t *proc; //Simics Sparc current processor
int o7id; //Simics Sparc registerid for the "i7" register ( %i7 + 8 is the return address)



void container_initialize( base_trace_t *bt)
{
	int i = 0;
	//tracefdOut = stdout;

	if(containerInitialized == 0)
	{
		ignore_due_to_Exception = 0;
		containerInitialized = 1;
		returnAddressStack = stack_create();
		//containerTable = (container *)malloc(size * sizeof(container));
		if(tracefdIn){
				traceLoaderSize=0;
				while(!feof(tracefdIn)){
					fscanf(tracefdIn,"%s %d %d",traceLoader[traceLoaderSize].name,&traceLoader[traceLoaderSize].traceLoadedAddressCount,&traceLoader[traceLoaderSize].traceLoadeduniqueChildContainersCalled);
					traceLoaderSize++;
				}
				fclose(tracefdIn);
				while(i<traceLoaderSize){
						//printf("%s %d %d\n",&traceLoader[i].name,traceLoader[i].traceLoadedAddressCount,traceLoader[i].traceLoadeduniqueChildContainersCalled);
						i++;
				}
		}

		if(strlen(bt->fullTraceFileName) == 0)
		{
			fullTracefd = stdout;
		}
		else
		{
			fullTracefd = fopen(bt->fullTraceFileName,"w");
			if(!fullTracefd){
				printf("\n\n Unable to open %s, using stdout \n\n",bt->fullTraceFileName);
				fullTracefd = stdout;
			}
		}

		
		proc =	SIM_current_processor();
		o7id = SIM_get_register_number(proc,"o7");
		
	}
	else
	{
		exit(printf("\n\n Runtime ERROR ('initializeContainers' called twice"));
	}
}

void container_close()
{
	if(fullTracefd) {
		fclose(fullTracefd);
	}
	containerInitialized = 0;
}


void setFullTraceFile(base_trace_t *bt)
{
	if(strlen(bt->fullTraceFileName) > 0)
	{
		fullTracefd = fopen(bt->fullTraceFileName,"w");
		if(!fullTracefd){
			printf("\n\n Unable to open %s, using stdout \n\n",bt->fullTraceFileName);
			fullTracefd = stdout;
		}
	}
}

//Obtain a list of symbols 
void loadContainersFromSymtable(char* symFileName)
{
	FILE* symfile ;
	unsigned long long addr;
	char type[4];
	char name[200];
	symfile = fopen(symFileName,"r");
	if(!symfile)
	{
		exit(printf("\n\n Runtime ERROR : unable to open symbol file %s",symFileName));
	}
	printf("\nSymbol file loaded\n");
	while(!feof(symfile)){
		fscanf(symfile,"%llx %s %s",&addr,type,name);
		container_add(addr,name);
	}
	
}

void container_add(md_addr_t addr, char * name)
{
	container* newContainer;
	newContainer = (container*) malloc(sizeof(container));
	newContainer->entryAddress = addr;
	newContainer->totalChildContainersCalled = 0;
	newContainer->totalNumberOfReads = 0;
	newContainer->totalNumberOfBytesRead = 0;
	newContainer->totalNumberOfBytesWritten = 0;
	newContainer->totalNumberOfWrites = 0;
	newContainer->totalStackPushes = 0;
	newContainer->totalStackPops = 0;
	newContainer->uniqueChildContainersCalled = 0;
	newContainer->childFunctions = NULL;
	newContainer->addressAccessList = NULL;
	newContainer->addressAccessListInstance = NULL;
	newContainer->addressAccessListPenalty = 0;

	newContainer->staticAddressCount = 0;
	newContainer->traceLoadedAddressCount= 0;
	newContainer->traceLoadeduniqueChildContainersCalled = 0;

	newContainer->staticAddressList = NULL;
	newContainer->isCalledWithHeapData = 0;

	strncpy(newContainer->name,name, 100);

	if(tracefdIn)
	{
		int i = 0;
		while(i<traceLoaderSize){
			if(strcmp(traceLoader[i].name,name) == 0){
				newContainer->traceLoadedAddressCount = traceLoader[i].traceLoadedAddressCount;
				newContainer->traceLoadeduniqueChildContainersCalled = traceLoader[i].traceLoadeduniqueChildContainersCalled;
				break;
			}
			i++;
		}
	}
	containerTable[containerSize++] = *newContainer;
	//printf("%lld = %llx %s %d %d\n", addr,containerTable[containerSize-1].entryAddress, containerTable[containerSize-1].name, containerTable[containerSize-1].traceLoadedAddressCount, containerTable[containerSize-1].traceLoadeduniqueChildContainersCalled);
	free(newContainer);
}


struct loadingPenalties container_traceFunctioncall(md_addr_t addr, mem_tp * mem, base_trace_t *obj)
{
	int i,j=0;
	container * foundSearch;
	struct loadingPenalties loadPenalty;
	loadPenalty.containerStaticListSize = -1;
	loadPenalty.containerDynamicListSize = 0;

	//printf("\n GICA: 0x%llx\n",addr);
	//first verify if the containers were initialized . report exception and quit if not
	if(containerInitialized == 0 || containerSize == 0)
	{
		printf("\n\n Containers not initialized or No symbol table loaded. ex. Call @conf.trace0.set_sym_file = \"file.nm\"\n");
		return loadPenalty;
	}

	//check for function return
	if(!stack_empty(returnAddressStack))
	{
		//printf("\n GICA check for function return: 0x%llx\n",addr);
		//fflush(stdin);
		stackObject t = stack_top(returnAddressStack);
		UpdateAddressList(&( t.container->addressAccessList), addr, 4);
		UpdateAddressList(&( t.container->addressAccessListInstance), addr, 4);
		if( t.returnAddress == addr)
		{
			//printf("return from function , popping container\n");
			//fflush(stdin);
			stack_pop(returnAddressStack);
			t.container->totalStackPops ++;
			//for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
			//printDecodedAddressList(printBuffer,t.container->addressAccessListInstance);
			//sprintf(printBuffer,"*\n");
			//myprint(printBuffer);
			updateGlobalAddressList(t.container);
			updateHeapCalls(t.container,t.container->addressAccessListInstance);
			t.container->addressAccessListPenalty += penaltyAddressList( t.container->addressAccessListInstance);
			for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
			sprintf(printBuffer,"}\n");
			myprint(printBuffer);

			//for heap acceses we need to update the parent container with that call as well ( heap memory accesses are passed from parent to child)
			if(!stack_empty(returnAddressStack)){
				updateHeapCalls(stack_top(returnAddressStack).container,t.container->addressAccessListInstance);
				loadPenalty.containerStaticListSize = stack_top(returnAddressStack).container->traceLoadedAddressCount + stack_top(returnAddressStack).container->traceLoadeduniqueChildContainersCalled + 3; //all static memory + code + stacksize + timeout
			    loadPenalty.containerDynamicListSize = stack_top(returnAddressStack).container->isCalledWithHeapData;
			}
			t.container->addressAccessListInstance = NULL;

			if(stack_empty(returnAddressStack)) TraceSuspend(obj);
			
			return loadPenalty;
		}
	}
	
    //printf("\n GICA: searching 0x%llx\n",addr);
	//if it was not a function return , it is either a function call ( push to container stack in this case), a exit function (pop all from container stack), or just a regular memory call ( in this case, add it to the current active container)
	foundSearch = search(addr);
	if(foundSearch){
		if(ignore_due_to_Exception) {ignore_due_to_Exception = 0;}
		else
			{
				//printf("\n GICA: found 0x%llx 0x%llx %s\n",addr, foundSearch->entryAddress, foundSearch->name);
				//fflush(stdin);
				if(!stack_empty(returnAddressStack)){

					int found = 0;
					stackObject currentContainer = stack_top(returnAddressStack);
					currentContainer.container->totalChildContainersCalled++;
			        list l = currentContainer.container->childFunctions;
					while(l!= NULL)
					{
						if(l->element.container->entryAddress == addr)
						{
							found = 1;
							break;
						}
						l = l->next;
					}
					if(!found)
					{
						currentContainer.container->uniqueChildContainersCalled ++;
						stackObject t ;
						t.container = foundSearch;
						currentContainer.container->childFunctions = cons(t,currentContainer.container->childFunctions);
					}
				}
				for(j = 0; j< returnAddressStack->size; j++) myprint("|\t");
		//			sprintf(printBuffer,"%x %s \n",addr,foundSearch->name);
				//sprintf(printBuffer,"0x%llx %s %d %d\n",foundSearch->entryAddress,foundSearch->name,foundSearch->traceLoadedAddressCount,foundSearch->traceLoadeduniqueChildContainersCalled);
				sprintf(printBuffer,"0x%llx 0x%llx %s{\n",foundSearch->entryAddress,SIM_read_register(proc,o7id) + 8, foundSearch->name);
				myprint(printBuffer);
				//fflush(stdin);
				//simulate loading the access list
				int sizeOfAccessList = foundSearch->traceLoadedAddressCount + foundSearch->traceLoadeduniqueChildContainersCalled + 3; //all static memory + code + stacksize + timeout
				loadPenalty.containerStaticListSize = sizeOfAccessList;
				if(!stack_empty(returnAddressStack))
					loadPenalty.containerDynamicListSize = (-1)*stack_top(returnAddressStack).container->isCalledWithHeapData;


				stackObject t;
				foundSearch->totalStackPushes ++;
				//printf("MemAccess : FP(%d)=%x SP(%d)=%x \n",MD_REG_FP,regs->regs_R[MD_REG_FP],MD_REG_SP,regs->regs_R[MD_REG_SP]);
				//container_dumpRegisters(*regs);

				t.container = foundSearch;
				t.returnAddress = SIM_read_register(proc,o7id) + 8;//return_addr;
				//printf("\n GICA: pushed to stack 0x%llx return :0x%llx %s\n",foundSearch->entryAddress,return_addr,foundSearch->name);
				//fflush(stdout);
				stack_push(returnAddressStack, t);
				UpdateAddressList(&( t.container->addressAccessList), addr, 4);
				UpdateAddressList(&( t.container->addressAccessListInstance), addr, 4);

				//EXIT function needs to force containers to pop all remaining from stack. Otherwise the Complete access list for main
				/*
				if(strcmp(foundSearch->name,"_Exit") == 0)
				{
					while(!stack_empty(returnAddressStack))
					{
						stackObject t = stack_top(returnAddressStack);
						stack_pop(returnAddressStack);
						t.container->totalStackPops ++;
						for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
						printDecodedAddressList(printBuffer,t.container->addressAccessListInstance);
						myprint("*\n");
						updateGlobalAddressList(t.container);
						updateHeapCalls(t.container,t.container->addressAccessListInstance);
						t.container->addressAccessListPenalty += penaltyAddressList( t.container->addressAccessListInstance);
						for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
						myprint("*\n");
					}
				}
				*/
			}
		return loadPenalty;
	}


	//end of search list reached without finding the function, save the address as possible return address
	return_addr = addr+4;
	return loadPenalty;
}

void container_MemoryCall(mem_tp cmd,md_addr_t addr, int nbytes)
{

	if(containerInitialized == 1 && !stack_empty(returnAddressStack))
	{

		stackObject t = stack_top(returnAddressStack);
		//container_dumpRegisters(*regs);

		//collect the continuous address accesses
		//this implementation : take care only of the memaccesses that are in sequence.

		UpdateAddressList(&( t.container->addressAccessList), addr, nbytes);
		UpdateAddressList(&( t.container->addressAccessListInstance), addr, nbytes);


		if(cmd == Read)
		{
			t.container->totalNumberOfReads++;
			t.container->totalNumberOfBytesRead +=nbytes;
			//for(j = 0; j< returnAddressStack->size; j++) myprint("|\t");
			//sprintf(printBuffer,"R %x %x(%d)\n",regs->regs_PC,addr,nbytes);
			//myprint(printBuffer);
		}
		else
		{
			t.container->totalNumberOfBytesWritten +=nbytes;
			t.container->totalNumberOfWrites++;
			//for(j = 0; j< returnAddressStack->size; j++) myprint("|\t");
			//sprintf(printBuffer,"W %x %x(%d)\n",regs->regs_PC,addr,nbytes);
			//myprint(printBuffer);
		}
	}

}


void container_printStatistics ()
{
	int i;
	int totalFunctionCalls = 0;
	int totalFunctionReturns = 0;

	sprintf(printBuffer,"max-stack-size: %d\n",returnAddressStack->maxsize);
	myprint(printBuffer);
	sprintf(printBuffer,"container-table-size: %d\n",containerSize);
	myprint(printBuffer);

	sprintf(printBuffer,"Container statistics :\n");
	myprint(printBuffer);
	sprintf(printBuffer,"Address \t Name \t Reads \t BytesRead \t Writes \t BytesWritten \t totalChildContainersCalled \t totalStackPushes \t totalStackPops \t uniqueChildContainersCalled \t SizeOfAccessList\n");
	myprint(printBuffer);
	for ( i=0 ; i < containerSize; i++)
	{

		sprintf(printBuffer,"%llx \t %s \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \n",
				containerTable[i].entryAddress,
				(containerTable[i].name),
				containerTable[i].totalNumberOfReads,
				containerTable[i].totalNumberOfBytesRead,
				containerTable[i].totalNumberOfWrites,
				containerTable[i].totalNumberOfBytesWritten,
				containerTable[i].totalChildContainersCalled,
				containerTable[i].totalStackPushes,
				containerTable[i].totalStackPops,
				containerTable[i].uniqueChildContainersCalled,
				containerTable[i].addressAccessListPenalty
				);
		myprint(printBuffer);
		totalFunctionCalls += containerTable[i].totalStackPushes;
		totalFunctionReturns += containerTable[i].totalStackPops;
	}
	sprintf(printBuffer,"totalFunctionCalls: %d\n",totalFunctionCalls);
	myprint(printBuffer);
	sprintf(printBuffer,"totalFunctionReturns: %d\n",totalFunctionReturns);
	myprint(printBuffer);

	sprintf(printBuffer,"Static Data Access List address name heap staticlist:\n");
	myprint(printBuffer);

	for ( i=0 ; i < containerSize; i++)
	{

		if(containerTable[i].totalStackPushes > 0){
			sprintf(printBuffer,"%llx\t%s\t\t\t\t\t%s\t",
					containerTable[i].entryAddress,
					(containerTable[i].name),
					containerTable[i].isCalledWithHeapData ? "true":"false"
					);
			myprint(printBuffer);
			printDecodedAddressList(printBuffer,containerTable[i].staticAddressList);
			sprintf(printBuffer,"\n");
			myprint(printBuffer);

			if(containerTable[i].isCalledWithHeapData){
				sprintf(compilerPrintBuffer,"%s %d\n",
						(containerTable[i].name),
						containerTable[i].isCalledWithHeapData
						);
				myprint(compilerPrintBuffer);
			}

			sprintf(printBuffer2,"%s ",
					containerTable[i].name);
			myprint(printBuffer2);
			//printAddressList(printBuffer2,containerTable[i].staticAddressList);
			addressList l = containerTable[i].staticAddressList;
			while(l!=NULL)
			{
				containerTable[i].staticAddressCount++;
				l = l->next;
			}
			sprintf(printBuffer2,"%d %d\n",containerTable[i].staticAddressCount,containerTable[i].uniqueChildContainersCalled);
			myprint(printBuffer2);
		}
	}

}

//void container_SetRegisterFile( struct regs_t * globalregs)
//{
//
//	regs = globalregs;
//}


//void container_dumpRegisters(struct regs_t regfile)
//{
//	int i;
//	for(i = 0 ; i< 32 ; i++)
//		fprintf(stderr,"R%d = %x \n", i, regfile.regs_R[i]);
//
//
//}

void UpdateAddressList(addressList *list,md_addr_t addr,int nbytes)
{
	//parse the list
	//try every range , if the new address is inside , ignore
	//if the addr_nbytes overlaps one range, addit and continue searching . If the address hits another range, merge the 2 ranges

	// if the end is reached, add new range
	int found = 0;
	//int ignore = 0;
	addressList lprevious = NULL;
	addressList l = *list;

	while(l!= NULL)
	{
		md_addr_t start = l->startAddress;
		md_addr_t end = l->endAddress;
		if( start <= addr && end >= addr+nbytes)
		{
			if(found == 0)
			{
				found = 1;
				break;
			}
			else
			{
				joinAddress(l, lprevious);
			}
		}
		else if( addr <= start  && addr + nbytes >= start )
		{
			if(found == 1) joinAddress(l, lprevious);
			else
			{
				l->startAddress = addr;
				l->endAddress = ( (addr+nbytes) > l->endAddress) ? addr+nbytes : l->endAddress;
				found = 1;
				lprevious = l;
			}
		}
		else if( addr <= end && addr + nbytes > end )
		{
			if(found == 1) joinAddress(l, lprevious);
			else
			{
				l->endAddress = addr + nbytes;
				found = 1;
				lprevious = l;
			}
		}
		l = l->next;
	}
	if(!found)
	{
		*list = consAddressList(addr,addr + nbytes,*list);
	}


}

void joinAddress(addressList future, addressList present)
{
	present->startAddress = present->startAddress > future->startAddress ? future->startAddress : present->startAddress;
	present->endAddress = present->endAddress < future->startAddress ? future->startAddress : present->endAddress;
	//exit(0);
	//find and delete future
	addressList l = present->next;
	addressList lprev = present;
	while( l != future)
	{
		lprev = l;
		l = l->next;
	}

	lprev->next = l->next;
	free(l);

}

void printAddressList(char * printbuff,addressList l){
	while(l!=NULL)
	{
		sprintf(printbuff,"[%x,%x) ",l->startAddress, l->endAddress);
		myprint(printbuff);
		l = l->next;
	}

}

void printDecodedAddressList(char * printbuff,addressList l)
{
	char type = 'c';
	while(l!=NULL)
	{
		if(l->startAddress >= ld_text_base && l->startAddress <= ld_text_bound) {
			type = 'c'; //code
		}
		else if(l->startAddress >= ld_data_base && l->startAddress <= ld_data_bound) {
			type = 'i'; //initialized
		}
		else if(l->startAddress >= ld_stack_base - ld_stack_size && l->startAddress <= ld_stack_base) {
			type = 's'; //stack
		}
		else if(l->startAddress >= 0xb0000000 && l->startAddress <= ld_environ_base){
			type = 'e'; //enviroment(static)
		}
		else {
			type = 'h'; //heap
		}
		sprintf(printbuff,"%c[%x,%x) ",type,l->startAddress, l->endAddress);
		myprint(printbuff);
		l = l->next;
	}

}

//Computes how many distinct heap memory access a function makes.
//It takes the maximum number of different accesses that a particular instance makes ( this is a conservative approach)
// I can take the average or mean in the future.
void updateHeapCalls(container* c,addressList l)
{
	//addressList l = c->addressAccessListInstance;
	int heapAccessesInInstance = 0;
	char type = 'c';
	while(l!=NULL)
	{
		if(l->startAddress >= ld_text_base && l->startAddress <= ld_text_bound) {
			type = 'c'; //code
		}
		else if(l->startAddress >= ld_data_base && l->startAddress <= ld_data_bound) {
			type = 'i'; //initialized
		}
		else if(l->startAddress >= ld_stack_base - ld_stack_size && l->startAddress <= ld_stack_base) {
			type = 's'; //stack
		}
		else if(l->startAddress >= 0xb0000000 && l->startAddress <= ld_environ_base){
			type = 'e'; //enviroment(static)
		}
		else {
			UpdateAddressList( &(c->addressAccessListInstance), l->startAddress,l->endAddress-l->startAddress);
			heapAccessesInInstance ++;
			type = 'h'; //heap
		}
		l = l->next;
	}
	if(c->isCalledWithHeapData < heapAccessesInInstance) c->isCalledWithHeapData = heapAccessesInInstance;
}

void updateGlobalAddressList(container* c)
{
 	addressList l = c->addressAccessList;
//	c->isCalledWithHeapData = 0;
	char type = 'c';
	while(l!=NULL)
	{
		if(l->startAddress >= ld_text_base && l->startAddress <= ld_text_bound) {
			type = 'c'; //code
		}
		else if(l->startAddress >= ld_data_base && l->startAddress <= ld_data_bound) {
			UpdateAddressList( &c->staticAddressList, l->startAddress,l->endAddress-l->startAddress);
			type = 'i'; //initialized
		}
		else if(l->startAddress >= ld_stack_base - ld_stack_size && l->startAddress <= ld_stack_base) {
			type = 's'; //stack
		}
		else if(l->startAddress >= 0xb0000000 && l->startAddress <= ld_environ_base){
			UpdateAddressList( &c->staticAddressList, l->startAddress,l->endAddress-l->startAddress);
			type = 'e'; //enviroment(static)
		}
		else {
			//c->isCalledWithHeapData ++;
			type = 'h'; //heap
		}
		l = l->next;
	}

}

int penaltyAddressList( addressList l) //counts the number of ranges in the access list.
{
	int val = 0;
	while(l!=NULL)
	{
		val ++;
		l = l->next;
	}
	return val;
}


void myprint(char * toPrint)
{
	if(toPrint == printBuffer2)
		fprintf(tracefdOut,"%s",toPrint);
	else if(toPrint == compilerPrintBuffer && compilerInfofd)
		fprintf(compilerInfofd,"%s",toPrint);
	else if(fullTracefd)
		fprintf(fullTracefd,"%s",toPrint);
	printf("%s",toPrint);
}

char * funcNameFromAddress(int addr)
{
	int i;
	for ( i=0 ; i < containerSize; i++)
	{
		//function call
		if(addr == containerTable[i].entryAddress)
			return containerTable[i].name;
	}
	return NULL;
}

addressList consAddressList(int startAddress, int endAddress, addressList l)
{
	addressList temp = malloc(sizeof(struct addressCell));
    temp -> startAddress = startAddress;
	temp -> endAddress = endAddress;
    temp -> next = l;
    return temp;
}
addressList freeAddressList(addressList l)
{
	addressList temp = l -> next;
    free(l);
    return temp;
}


list cons(stackObject element, list l){
    list temp = malloc(sizeof(struct cell));
    temp -> element = element;
    temp -> next = l;
    return temp;
}

list cdr_and_free(list l){
    list temp = l -> next;
    free(l);
    return temp;
  }

mystack stack_create(void)
{
  mystack temp = malloc(sizeof(struct mystack));
  temp -> elements = NULL;
  temp->size = 0;
  temp->maxsize = 0;
  return temp;
}

void stack_push(mystack s,stackObject element)
{
  s -> elements = cons(element, s -> elements);
  s->size++;
  if(s->size > s->maxsize) s->maxsize = s->size;
}

int stack_empty(mystack s)
{
  return s -> elements == NULL;
}

void stack_pop(mystack s)
{
  //assert(!empty(s));
  s -> elements = cdr_and_free(s -> elements);
  s->size--;
}

stackObject stack_top(mystack s)
{
  //assert(!empty(s));
  return s -> elements -> element;
}

int container_comparison(const void * m1, const void * m2){
	//printf("GICA: comparing %llx %llx ? %llx\n", ((container *)m1)->entryAddress, ((container *)m2)->entryAddress, ((container *)m1)->entryAddress - ((container *)m2)->entryAddress);
	return ((container *)m1)->entryAddress - ((container *)m2)->entryAddress;
}

//the container list is sorted by address
container * search(md_addr_t addr)
{
	container key;
	key.entryAddress = addr;
	return (container *)bsearch(&key, containerTable, containerSize,sizeof(container),container_comparison);
}
