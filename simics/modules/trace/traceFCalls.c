/*by Eugen Leontie*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "traceFCalls.h"

#define CONTAINERMAX 6000
#define PRINTBUFFMAX 10000

typedef struct traceData_def traceData;
//External Variables
container containerTable[CONTAINERMAX]; 	//this array contains all containers( functions and is loaded in Loader.c)
int containerSize = 0;				//keps the size of the containerList
int componentTableSize = 1;

//end External Variables

//local variables
//mystack returnAddressStack; 	//this is the main Container stack
int containerInitialized = 0; 	//local flag to track wheater the containers were initialized
md_addr_t return_addr;			//monitors the return addresses ( saved before a function call and stored to stack)

thread_monitor_t *thread_active;
int ignore_due_to_Exception;



struct regs_t *regs;			//this pointer gives access to the simulated register file

char printBuffer[PRINTBUFFMAX];			// use sprintf to print to this buffer and then use myprint for the final print ( full trace )
char printBuffer2[PRINTBUFFMAX];			// use sprintf to print to this buffer and then use myprint for the final print ( second run trace )
char compilerPrintBuffer[PRINTBUFFMAX];	// use sprintf to print to this buffer and then use myprint for the final print ( compiler trace )
char stdoutPrintBuffer[PRINTBUFFMAX];

struct traceData_def{
	char name[100];
	int traceLoadedAddressCount;
	int traceLoadeduniqueChildContainersCalled;
};

traceData traceLoader[CONTAINERMAX];
int traceLoaderSize;

char *fullTracefdFileName = NULL;
FILE *tracefdIn = NULL; //TODO !!
FILE *tracefdOut = NULL;
FILE *compilerInfofd = NULL;
FILE *fullTracefd = NULL;
FILE *overrideOutputfd = NULL;

md_addr_t ld_text_base = 0x4000;
md_addr_t ld_text_bound = 0x4DBF0;
md_addr_t ld_data_base = 0x4dbf0;
md_addr_t ld_data_bound = 0x4EFA8;
md_addr_t ld_stack_size = 0x302090;
md_addr_t ld_stack_base = 0x351040;
md_addr_t ld_environ_base = 0;


conf_object_t *proc = NULL; //Simics Sparc current processor
int o7id; //Simics Sparc registerid for the "i7" register ( %i7 + 8 is the return address)
int o6id;  //sp
int i6id;  //fp


thread_monitor_t* ThreadAdd(int64 id, int64 name)
{
	thread_monitor_t* newThread;
	newThread = (thread_monitor_t*) calloc (sizeof(thread_monitor_t),1);
	newThread->thread_id = id;
	newThread->thread_name = name;
	newThread->maxStack = 0;
	newThread->minStack = ULLONG_MAX ;
	newThread->maxFP = 0;
	newThread->minFP = ULLONG_MAX ;

	newThread->container_runtime_stack = stack_create();
	//create a name for the thread trace file
	if(fullTracefdFileName != NULL){
		char fname[500];
		memset(fname,0,500);
		if(id == 0)
			strcpy((char *)fname,fullTracefdFileName);
		else{
			strcpy(fname,fullTracefdFileName);
			strcat(fname,"_");
			char idstr[64];
			itoa(id,idstr);
			strcat(fname,idstr);
			strcat(fname,"_");
			char taskName[5];
			toStringRTEMSTaksName(taskName,name);
			strcat(fname,taskName);
		}
		newThread->traceFD = fopen(fname,"w");
	}
	if(thread_active != NULL){
		newThread->next = thread_active->next;
		thread_active->next = newThread;
	}
	else{
		newThread->next = newThread;
	}
	return newThread;
}

thread_monitor_t* ThreadFind(int64 id)
{
	thread_monitor_t* iterate;
	//ASSERT(thread_active != NULL);
	if(thread_active->thread_id== id) return thread_active;
	iterate = thread_active->next;

	while (iterate != thread_active)
	{
		if(iterate->thread_id == id) return iterate;
		iterate = iterate->next;
	}
	return NULL;
}

void Thread_switch( int64 id, int64 name)
{
	thread_monitor_t* newThread;
	newThread = ThreadFind(id);
	if(newThread == thread_active) return;
	if(!newThread) newThread = ThreadAdd(id,name);
	fflush(thread_active->traceFD);
	thread_active = newThread;
    
	printf("\nThread_switch 0x%llx ",thread_active->thread_id);
	printRTEMSTaksName(thread_active->thread_name);
	printf("\n");

	int64 StackArea = mySimicsIntSymbolRead("_Per_CPU_Information.executing.Start.Initial_stack.area");
	uint64 StackSize = mySimicsIntSymbolRead("_Per_CPU_Information.executing.Start.Initial_stack.size");

	ld_stack_base = StackArea;
	ld_stack_size = StackSize;
}



void container_initialize( base_trace_t *bt)
{
	int i = 0;
	//tracefdOut = stdout;

	if(containerInitialized == 0)
	{
		ignore_due_to_Exception = 0;
		containerInitialized = 1;
		//returnAddressStack = stack_create();
		thread_active = ThreadAdd(0,0);
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

		o6id = SIM_get_register_number(proc,"o6");
		i6id = SIM_get_register_number(proc,"i6");

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
		fullTracefdFileName = bt->fullTraceFileName;
		fullTracefd = fopen(bt->fullTraceFileName,"w");
		if(!fullTracefd){
			printf("\n\n Unable to open %s, using stdout \n\n",bt->fullTraceFileName);
			fullTracefd = stdout;
		}



		thread_monitor_t* th;
		th = ThreadFind(0);
		th->traceFD = fullTracefd;
	}
}

//Obtain a list of symbols
void loadContainersFromSymtable(const char* symFileName)
{
	FILE* symfile ;
	unsigned long long addr;
	char type[4];
	char name[200];
	char linenumber[2000];
	symfile = fopen(symFileName,"r");
	if(!symfile)
	{
		exit(printf("\n\n Runtime ERROR : unable to open symbol file %s",symFileName));
	}
	//printf("\nSymbol file loaded\n");
	while(!feof(symfile)){
		fscanf(symfile,"%llx %s %s\t%s",&addr,type,name,linenumber);
		//printf("%llx %s %s %s\n",addr, type, name,linenumber);
		container * newcont = container_add(addr,name);
		strncpy(newcont->linenumber,linenumber,1000);
	}

	//container_quickprint();

	//get function list from gicasymtable
	//The end address is not in the NM file, but can be found in the simics symtable.functions list
	attr_value_t functions = SIM_get_attribute(SIM_get_object("gicasymtable"),"functions");
	//printf("functions attr type %d\n", functions.kind);
	//printf("functions list lenght %lld\n", functions.u.list.size);
	//FILE * functionsoutFD;
	//functionsoutFD = fopen("listafunctiidinSymtable.txt","w");
	//functionsoutFD = stdout;
	for (int i = 0; i< functions.u.list.size; i++ )
	{
		//printf("functions %d attr type %d\n",i, functions.u.list.vector[i].kind);
		//printf("functions %d list lenght %lld\n",i, functions.u.list.vector[i].u.list.size);
		uint64 startaddr = 0;
		uint64 endaddr = 0 ;
		for(int j=0;j< functions.u.list.vector[i].u.list.size; j++)
		{
			attr_value_t detail = functions.u.list.vector[i].u.list.vector[j];
			//printf("functions %d %d attr type %d\n",i ,j , detail.kind);
			if(detail.kind == 1){
			;
				//fprintf(functionsoutFD,"%s ",detail.u.string);
			}
			else if(detail.kind == 2){
				//fprintf(functionsoutFD,"0x%llx ",detail.u.integer);
				if( j == 2)
					startaddr = detail.u.integer;
				else if (j==3)
					endaddr = detail.u.integer;
			}
		}
		//fprintf(functionsoutFD,"\n");
		container * foundSearch = search(startaddr);
		if(foundSearch)
			foundSearch->endAddress = endaddr;
	}
	//fclose(functionsoutFD);
	//printf("\nloadContainersFromSymtable ended");

	//container_quickprint();
	//exit(0);
}

container* container_add(md_addr_t addr, char * name)
{
	container* newContainer;
	newContainer = (container*) malloc(sizeof(container));
	newContainer->entryAddress = addr;
	newContainer->endAddress = 0;
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

	newContainer->nonFunction = 0;

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
	if(newContainer->name[0] == '*') {
		newContainer->nonFunction = 1;
		//printf("%s %d\n\n",newContainer->name, newContainer->nonFunction);
	}
	containerTable[containerSize++] = *newContainer;
	//printf("%lld = %llx %s %d %d\n", addr,containerTable[containerSize-1].entryAddress, containerTable[containerSize-1].name, containerTable[containerSize-1].traceLoadedAddressCount, containerTable[containerSize-1].traceLoadeduniqueChildContainersCalled);
	free(newContainer);
	return &containerTable[containerSize-1];
}


struct loadingPenalties container_traceFunctioncall(md_addr_t addr, mem_tp * mem, base_trace_t *bt)
{
	int i,j=0;
	container * foundSearch;
	mystack returnAddressStack = thread_active->container_runtime_stack;
	struct loadingPenalties loadPenalty;
	loadPenalty.containerStaticListSize = -1;
	loadPenalty.containerDynamicListSize = 0;

	cycles_t cycles =  SIM_cycle_count(SIM_current_processor());
	getSP();
	getFP();

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
		fflush(stdin);
		stackObject t = stack_top(returnAddressStack);
		UpdateAddressList(&( t.containerObj->addressAccessList), addr, 4);
		UpdateAddressList(&( t.containerObj->addressAccessListInstance), addr, 4);

		int returned = 0;
		while(t.returnAddress == addr || t.containerObj->endAddress == addr)
		{
			//printf("return from function , popping container %s\n", t.containerObj->name);
			//fflush(stdin);
			stack_pop(returnAddressStack);
			t.containerObj->totalStackPops ++;
			//for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
			//printDecodedAddressList(printBuffer,t.containerObj->addressAccessListInstance);
			//sprintf(printBuffer,"*\n");
			//myprint(printBuffer);
			updateGlobalAddressList(t.containerObj);
			updateHeapCalls(t.containerObj,t.containerObj->addressAccessListInstance);
			t.containerObj->addressAccessListPenalty += penaltyAddressList( t.containerObj->addressAccessListInstance);
			sprintf(printBuffer,"%lld\t",cycles);
			myprint(printBuffer);
			for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
			sprintf(printBuffer,"}\n");
			myprint(printBuffer);

			//for heap acceses we need to update the parent container with that call as well ( heap memory accesses are passed from parent to child)
			if(!stack_empty(returnAddressStack)){
				updateHeapCalls(stack_top(returnAddressStack).containerObj,t.containerObj->addressAccessListInstance);
				loadPenalty.containerStaticListSize = stack_top(returnAddressStack).containerObj->traceLoadedAddressCount + stack_top(returnAddressStack).containerObj->traceLoadeduniqueChildContainersCalled + 3; //all static memory + code + stacksize + timeout
			    loadPenalty.containerDynamicListSize = stack_top(returnAddressStack).containerObj->isCalledWithHeapData;
			}
			t.containerObj->addressAccessListInstance = NULL;

			//if(stack_empty(returnAddressStack)) TraceSuspend(bt);
			returned = 1;
			if(!stack_empty(returnAddressStack)) t = stack_top(returnAddressStack);
			else break;
		}
		if(returned) return loadPenalty;
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
					currentContainer.containerObj->totalChildContainersCalled++;
			        llist l = currentContainer.containerObj->childFunctions;
					while(l!= NULL)
					{
						if(l->element.containerObj->entryAddress == addr)
						{
							found = 1;
							break;
						}
						l = l->next;
					}
					if(!found)
					{
						currentContainer.containerObj->uniqueChildContainersCalled ++;
						stackObject t ;
						t.containerObj = foundSearch;
						currentContainer.containerObj->childFunctions = cons(t,currentContainer.containerObj->childFunctions);
					}
				}
				sprintf(printBuffer,"%lld\t",cycles);
				myprint(printBuffer);
				for(j = 0; j< returnAddressStack->size; j++) myprint("|\t");
				if(!foundSearch->nonFunction){
					//sprintf(printBuffer,"%x %s \n",addr,foundSearch->name);
					//sprintf(printBuffer,"0x%llx %s %d %d\n",foundSearch->entryAddress,foundSearch->name,foundSearch->traceLoadedAddressCount,foundSearch->traceLoadeduniqueChildContainersCalled);
					if(bt->displayLineNumbers)
						sprintf(printBuffer,"%llx %s {%s \n",addr, foundSearch->name, foundSearch->linenumber);
					else
						sprintf(printBuffer,"%s {\n", foundSearch->name);
					myprint(printBuffer);
					//fflush(stdin);
					//simulate loading the access list
					int sizeOfAccessList = foundSearch->traceLoadedAddressCount + foundSearch->traceLoadeduniqueChildContainersCalled + 3; //all static memory + code + stacksize + timeout
					loadPenalty.containerStaticListSize = sizeOfAccessList;
					if(!stack_empty(returnAddressStack))
						loadPenalty.containerDynamicListSize = (-1)*stack_top(returnAddressStack).containerObj->isCalledWithHeapData;


					stackObject t;
					foundSearch->totalStackPushes ++;
					//printf("MemAccess : FP(%d)=%x SP(%d)=%x \n",MD_REG_FP,regs->regs_R[MD_REG_FP],MD_REG_SP,regs->regs_R[MD_REG_SP]);
					//container_dumpRegisters(*regs);

					t.containerObj = foundSearch;
					t.returnAddress = getRet();//return_addr;
					//printf("\n GICA: pushed to stack 0x%llx return :0x%llx %s\n",foundSearch->entryAddress,return_addr,foundSearch->name);
					//fflush(stdout);
					stack_push(returnAddressStack, t);
					UpdateAddressList(&( t.containerObj->addressAccessList), addr, 4);
					UpdateAddressList(&( t.containerObj->addressAccessListInstance), addr, 4);
				}
				else
				{
					if(bt->displayLineNumbers)
						sprintf(printBuffer,"%s %s\n", foundSearch->name,foundSearch->linenumber);
					else
						sprintf(printBuffer,"%s\n", foundSearch->name);
					myprint(printBuffer);
				}
				//EXIT function needs to force containers to pop all remaining from stack. Otherwise the Complete access list for main
				/*
				if(strcmp(foundSearch->name,"*halt") == 0)
				{
					while(!stack_empty(returnAddressStack))
					{
						stackObject t = stack_top(returnAddressStack);
						stack_pop(returnAddressStack);
						t.containerObj->totalStackPops ++;
						for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
						printDecodedAddressList(printBuffer,t.containerObj->addressAccessListInstance);
						myprint("*\n");
						updateGlobalAddressList(t.containerObj);
						updateHeapCalls(t.containerObj,t.containerObj->addressAccessListInstance);
						t.containerObj->addressAccessListPenalty += penaltyAddressList( t.containerObj->addressAccessListInstance);
						for(i = 0; i< returnAddressStack->size; i++) myprint("|\t");
						myprint("*\n");
					}
					exit(printf("FORCE EXIT - please fix"));
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
    
	mystack returnAddressStack = thread_active->container_runtime_stack;
	
	if(containerInitialized == 1 && !stack_empty(returnAddressStack))
	{

		stackObject t = stack_top(returnAddressStack);
		//container_dumpRegisters(*regs);

		//collect the continuous address accesses
		//this implementation : take care only of the memaccesses that are in sequence.
		//if(strcmp(t.containerObj->name,"crc32file") == 0)
    	//	printf("MemoryCall %d %llx\n",cmd,addr);

		UpdateAddressList(&( t.containerObj->addressAccessList), addr, nbytes);
		UpdateAddressList(&( t.containerObj->addressAccessListInstance), addr, nbytes);


		if(cmd == Read)
		{
			t.containerObj->totalNumberOfReads++;
			t.containerObj->totalNumberOfBytesRead +=nbytes;
			//for(j = 0; j< returnAddressStack->size; j++) myprint("|\t");
			//sprintf(printBuffer,"R %x %x(%d)\n",regs->regs_PC,addr,nbytes);
			//myprint(printBuffer);
		}
		else
		{
			t.containerObj->totalNumberOfBytesWritten +=nbytes;
			t.containerObj->totalNumberOfWrites++;
			//for(j = 0; j< returnAddressStack->size; j++) myprint("|\t");
			//sprintf(printBuffer,"W %x %x(%d)\n",regs->regs_PC,addr,nbytes);
			//myprint(printBuffer);
		}
	}

}

void container_quickprint()
{
	for (int i=0 ; i < containerSize; i++)
	{
		fprintf(stdout,"%llx %llx \t %s \n",
						containerTable[i].entryAddress,
						containerTable[i].endAddress,
				(containerTable[i].name));
	}
}

void container_printMemoryRanges(int bAll )
{
	for (int i=0 ; i < containerSize; i++)
	{
		addressList l = containerTable[i].addressAccessList;
		int bUsed = containerTable[i].totalStackPushes > 0;
		if(bAll || bUsed){
			sprintf(printBuffer,"%llx %llx\t%s\t",
							containerTable[i].entryAddress,
							containerTable[i].endAddress,
					(containerTable[i].name));
			myprint(printBuffer);
			
			while(l!=NULL)
			{
				sprintf(printBuffer,"[%llx,%llx) ",l->startAddress, l->endAddress);
				myprint(printBuffer);
				l = l->next;
			}
			sprintf(printBuffer,"\n");
			myprint(printBuffer);
		}
	}
}

void container_printDecodedMemoryRanges(int bAll )
{
	sprintf(printBuffer,"code: %llx %llx initialized: %llx %llx stack: %llx %llx \n",ld_text_base,ld_text_bound,ld_data_base,ld_data_bound,ld_stack_base ,ld_stack_base+ ld_stack_size);
	myprint(printBuffer);
	for (int i=0 ; i < containerSize; i++)
	{
		addressList l = containerTable[i].addressAccessList;
		int bUsed = containerTable[i].totalStackPushes > 0;
		if(bAll || bUsed){
			sprintf(printBuffer,"%llx %llx\t%s\t",
							containerTable[i].entryAddress,
							containerTable[i].endAddress,
					(containerTable[i].name));
			myprint(printBuffer);
			
				printDecodedAddressList(printBuffer,l);
				myprint(printBuffer);
				sprintf(printBuffer,"\n");
				myprint(printBuffer);
		}
	}
}

void container_printSimpleCountAddressAcess(int bAll )
{
	sprintf(printBuffer,"entryAddress endAddress\tname\tcode initialized stack heap\n");
	myprint(printBuffer);
	for (int i=0 ; i < containerSize; i++)
	{
		addressList l = containerTable[i].addressAccessList;
		int bUsed = containerTable[i].totalStackPushes > 0;
		if(bAll || bUsed){
			sprintf(printBuffer,"%llx %llx\t%s\t",
							containerTable[i].entryAddress,
							containerTable[i].endAddress,
					(containerTable[i].name));
			myprint(printBuffer);
			
				printCountMemoryAccesses(printBuffer,l);
				myprint(printBuffer);
				sprintf(printBuffer,"\n");
				myprint(printBuffer);
		}
	}

	
}

void container_printChildFunctionsCalled(int bAll)
{
	for (int i=0 ; i < containerSize; i++)
	{
		llist l = containerTable[i].childFunctions;
		if(bAll || l){
			sprintf(printBuffer,"%llx %llx\t%s\t",
							containerTable[i].entryAddress,
							containerTable[i].endAddress,
					(containerTable[i].name));
			myprint(printBuffer);

			llist l = containerTable[i].childFunctions;
			while(l)
			{
				sprintf(printBuffer,"%llx %s\t",
							l->element.containerObj->endAddress,
							l->element.containerObj->name);
				myprint(printBuffer);
				l = l->next;
			}
			sprintf(printBuffer,"\n");
			myprint(printBuffer);	
		}
	}
	
}


void printAllStatsFiles(base_trace_t *bt)
{
	char * baseFileName = "\0";

	if(strlen(bt->fStatsFileBaseName) != 0)
	{
		baseFileName = bt->fStatsFileBaseName;
	}
	
	char * fullAddressAccessListFileName = "FullAddressAccessList.txt";
	char * fullDecodedAddressAccessListFileName = "FullDecodedAddressAccessList.txt";
	char * simpleCountAddressAcessFileName = "SimpleCountAddressAccessList.txt";
	char * containerCallListFileName = "ContainerCallList.txt";
	char * containerStatisticsFileName = "ContainerStats.txt";

	FILE * fullAddressAccessListFile;
	FILE * fullDecodedAddressAccessListFile;
	FILE * simpleCountAddressAcessFile;
	FILE * containerCallListFile;
	FILE * containerStatisticsFile;
	
	char *s = (char *)malloc(snprintf(NULL, 0, "%s %s", baseFileName, fullAddressAccessListFileName) + 1);
	sprintf(s, "%s%s", baseFileName, fullAddressAccessListFileName);
    fullAddressAccessListFile = fopen(s,"w");
		if(!fullAddressAccessListFile){
			printf("\n\n Unable to open %s, using stdout \n\n",s);
			fullAddressAccessListFile = stdout;
		}
	free(s);
	s = (char *)malloc(snprintf(NULL, 0, "%s %s", baseFileName, fullDecodedAddressAccessListFileName) + 1);
	sprintf(s, "%s%s", baseFileName, fullDecodedAddressAccessListFileName);
	fullDecodedAddressAccessListFile = fopen(s,"w");
		if(!fullDecodedAddressAccessListFile){
			printf("\n\n Unable to open %s, using stdout \n\n",s);
			fullDecodedAddressAccessListFile = stdout;
		}
	free(s);
	s = (char *)malloc(snprintf(NULL, 0, "%s %s", baseFileName, simpleCountAddressAcessFileName) + 1);
	sprintf(s, "%s%s", baseFileName, simpleCountAddressAcessFileName);
	simpleCountAddressAcessFile = fopen(s,"w");
		if(!simpleCountAddressAcessFile){
			printf("\n\n Unable to open %s, using stdout \n\n",s);
			simpleCountAddressAcessFile = stdout;
		}
	free(s);
	s = (char *)malloc(snprintf(NULL, 0, "%s %s", baseFileName, containerCallListFileName) + 1);
	sprintf(s, "%s%s", baseFileName, containerCallListFileName);
	containerCallListFile = fopen(s,"w");
		if(!containerCallListFile){
			printf("\n\n Unable to open %s, using stdout \n\n",s);
			containerCallListFile = stdout;
		}
	free(s);
	s = (char *)malloc(snprintf(NULL, 0, "%s %s", baseFileName, containerStatisticsFileName) + 1);
	sprintf(s, "%s%s", baseFileName, containerStatisticsFileName);
	containerStatisticsFile = fopen(s,"w");
		if(!containerStatisticsFile){
			printf("\n\n Unable to open %s, using stdout \n\n",s);
			containerStatisticsFile = stdout;
		}
	free(s);


	overrideOutputfd = containerStatisticsFile;
	container_printStatistics(0);
	fflush(overrideOutputfd);
	overrideOutputfd = fullAddressAccessListFile;
	container_printMemoryRanges(0);
	fflush(overrideOutputfd);
	overrideOutputfd = fullDecodedAddressAccessListFile;
	container_printDecodedMemoryRanges(0);
	fflush(overrideOutputfd);
	overrideOutputfd = simpleCountAddressAcessFile;
	container_printSimpleCountAddressAcess(0);
	fflush(overrideOutputfd);
	overrideOutputfd = containerCallListFile;
	container_printChildFunctionsCalled(0);
	fflush(overrideOutputfd);
	overrideOutputfd = NULL;
}


void container_printStatistics (int bAll)
{
	int i;
	int totalFunctionCalls = 0;
	int totalFunctionReturns = 0;
	//mystack returnAddressStack = thread_active->container_runtime_stack;

	//sprintf(printBuffer,"max-stack-size: %d\n",returnAddressStack->maxsize);
	//myprint(printBuffer);
	//sprintf(printBuffer,"container-table-size: %d\n",containerSize);
	//myprint(printBuffer);

	//sprintf(printBuffer,"Container statistics :\n");
	//myprint(printBuffer);
	sprintf(printBuffer,"Address \t Name \t Reads \t BytesRead \t Writes \t BytesWritten \t totalChildContainersCalled \t totalStackPushes \t totalStackPops \t uniqueChildContainersCalled \t SizeOfAccessList\n");
	myprint(printBuffer);
	for ( i=0 ; i < containerSize; i++)
	{
		int bUsed = containerTable[i].totalStackPushes > 0;
		if(bAll || bUsed){
			sprintf(printBuffer,"%llx %llx \t %s \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \n",
					containerTable[i].entryAddress,
					containerTable[i].endAddress,
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
	}
	//sprintf(printBuffer,"totalFunctionCalls: %d\n",totalFunctionCalls);
	//myprint(printBuffer);
	//sprintf(printBuffer,"totalFunctionReturns: %d\n",totalFunctionReturns);
	//myprint(printBuffer);

	//sprintf(printBuffer,"Static Data Access List address name heap staticlist:\n");
	//myprint(printBuffer);
	/*
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
	*/

}

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
		sprintf(printbuff,"[%llx,%llx) ",l->startAddress, l->endAddress);
		myprint(printbuff);
		l = l->next;
	}

}

void printCurrentContainerStack( )
{
	mystack returnAddressStack = thread_active->container_runtime_stack;
	char name[4];
	toStringRTEMSTaksName(name,thread_active->thread_name);
	printf("Thread: %s id:0x%llx\n",
		name,
		thread_active->thread_id
		);
	if(!stack_empty(thread_active->container_runtime_stack)){
		//list start = returnAddressStack->elements;
		llist next = returnAddressStack->elements;
		for(int i=0;i<returnAddressStack->size && next != NULL ;i++)
		{
			printf("%d 0x %llx %s ret = 0x%lld \n",
				i,
				next->element.containerObj->entryAddress,
				next->element.containerObj->name,
				next->element.returnAddress);
			printAddressList(stdoutPrintBuffer,next->element.containerObj->addressAccessListInstance);
			next = next->next;
		}
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
		else if(l->startAddress >= ld_stack_base  && l->startAddress <= ld_stack_base + ld_stack_size) {
			type = 's'; //stack
		}
		else {
			type = 'h'; //heap
		}
		printbuff += sprintf(printbuff,"%c[%llx,%llx) ",type,l->startAddress, l->endAddress);
		l = l->next;
	}

}

void printCountMemoryAccesses(char * printbuff,addressList l)
{
    int c = 0, i = 0, s = 0, h = 0;
	char type = 'c';
	while(l!=NULL)
	{
		if(l->startAddress >= ld_text_base && l->startAddress <= ld_text_bound) {
			type = 'c'; //code
			c++;
		}
		else if(l->startAddress >= ld_data_base && l->startAddress <= ld_data_bound) {
			type = 'i'; //initialized
			i++;
		}
		else if(l->startAddress >= ld_stack_base  && l->startAddress <= ld_stack_base + ld_stack_size) {
			type = 's'; //stack
			s++;
		}
		else {
			type = 'h'; //heap
			h++;
		}
		l = l->next;
	}

	sprintf(printbuff,"%d %d %d %d",c, i, s, h);

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
		else if(l->startAddress >= ld_stack_base  && l->startAddress <= ld_stack_base + ld_stack_size) {
			type = 's'; //stack
		}
		//else if(l->startAddress >= 0xb0000000 && l->startAddress <= ld_environ_base){
		//	type = 'e'; //enviroment(static)
		//}
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
		else if(l->startAddress >= ld_stack_base  && l->startAddress <= ld_stack_base + ld_stack_size) {
			type = 's'; //stack
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
	if(overrideOutputfd)
	{
		fprintf(overrideOutputfd,"%s",toPrint);
		return;
	}

	if(toPrint == printBuffer2)
		fprintf(tracefdOut,"%s",toPrint);
	else if(toPrint == compilerPrintBuffer && compilerInfofd)
		fprintf(compilerInfofd,"%s",toPrint);
	else if(fullTracefd){
		fprintf(thread_active->traceFD,"%s",toPrint);
		//fflush(thread_active->traceFD);
		//fprintf(stdout,"%s",toPrint);
		//fflush(stdout);
	}
	else
	{
		fprintf(stdout,"%s",toPrint);
		//fflush(stdout);
	}

	//printf("%s",toPrint);
}

void containers_flush()
{
	fflush(stdin);
	fflush(stderr);
	if(fullTracefd)
		fflush(thread_active->traceFD);
	if(compilerInfofd)
		fflush(compilerInfofd);
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
	addressList temp = (addressList) malloc(sizeof(struct addressCell));
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


llist cons(stackObject element, llist l){
    llist temp = (llist) malloc(sizeof(struct cell));
    temp -> element = element;
    temp -> next = l;
    return temp;
}

llist cdr_and_free(llist l){
    llist temp = l -> next;
    free(l);
    return temp;
  }

mystack stack_create(void)
{
  mystack temp = (mystack) malloc(sizeof(struct mystackStr));
  temp -> elements = NULL;
  temp->size = 0;
  temp->maxsize = 0;
  return temp;
}

void stack_push(mystack s,stackObject element)
{
  fflush(stdin);
  s -> elements = cons(element, s -> elements);
  s->size++;
  if(s->size > s->maxsize) s->maxsize = s->size;
  //printf("stack push %s\n",s->elements->element.container->name);
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

int checkAlphaNumeric(char x)
{
	if( (x >= 'A' && x <='Z' ) || (x >= 'a' && x <= 'z' ) || (x >= '0' && x <= '9' ))
		return 1;
	return 0;
}

void toStringRTEMSTaksName(char * dest, int _name)
{
	dest[0] = ((_name) >> 24) & 0xff;
	dest[1] = ((_name) >> 16) & 0xff;
	dest[2] = ((_name) >> 8) & 0xff;
	dest[3] = (_name) & 0xff;
	dest[4] = 0;

	if(!checkAlphaNumeric(dest[0])) dest[0] = 0;
	if(!checkAlphaNumeric(dest[1])) dest[1] = 0;
	if(!checkAlphaNumeric(dest[2])) dest[2] = 0;
	if(!checkAlphaNumeric(dest[3])) dest[3] = 0;
}


/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

uint64 getSP()
{

	uint64 sp = SIM_read_register(proc,o6id);
	if(sp < thread_active->minStack){
		thread_active->minStack = sp;
		//char name[] = "\0\0\0\0\0";
		//toStringRTEMSTaksName(name,thread_active->thread_name);
		//printf("%s : spmin=0x%llx\n",name,sp);
	}
	if(sp > thread_active->maxStack){
		thread_active->maxStack = sp;
		//char name[] = "\0\0\0\0\0";
		//toStringRTEMSTaksName(name,thread_active->thread_name);
		//printf("%s : spmax=0x%llx\n",name,sp);
	}


	return	sp;
}

uint64 getFP()
{
	uint64 fp = SIM_read_register(proc,i6id);
	if(fp < thread_active->minFP){
		thread_active->minFP = fp;
		//char name[] = "\0\0\0\0\0";
		//toStringRTEMSTaksName(name,thread_active->thread_name);
		//printf("%s : fpmin=0x%llx\n",name,fp);
	}
	if(fp > thread_active->maxFP){
		thread_active->maxFP = fp;
		//char name[] = "\0\0\0\0\0";
		//toStringRTEMSTaksName(name,thread_active->thread_name);
		//printf("%s : fpmax=0x%llx\n",name,fp);
	}
	return	fp;
}

uint64 getRet()
{
	return	SIM_read_register(proc,o7id) + 8;
}


void printThreads()
{
	if(thread_active == NULL ) printf("NO THREAD ADDED\n");
	thread_monitor_t* iterate;
	iterate = thread_active;

	do
	{
		char name[4];
		toStringRTEMSTaksName(name,iterate->thread_name);
		printf("Thread: %s id:%lld spmin:0x%llx spmax:0x%llx fpmin:0x%llx fpmax:0x%llx\n",
			name,
			iterate->thread_id,
			iterate->minStack,
			iterate->maxStack,
			iterate->minFP,
			iterate->maxFP
			);
		iterate = iterate->next;
	}
	while (iterate != thread_active);
}



