#include "allow.h"
#include <stdlib.h>



inline void * mymalloc(size_t size)
{
	void * ret;

	ret = malloc(size);
	#if defined(WITHALLOW)
		ALLOW( ret, size, 3LL );
	#endif
	
	return ret;
}



bool test_create(Thread_Control * executing,  Thread_Control * creating)
{
	char namea[6];

	if( !strncmp(rtems_object_get_name(creating->Object.id, 5, namea),"TA",2)){
		
		char nameb[6];
		char filename[22];
		rtems_object_get_name(executing->Object.id, 5, namea); 
		rtems_object_get_name(creating->Object.id, 5, nameb);
		nameb[3] = 0;

		sprintf(filename,"contUSE_%s.txt",nameb);



//		int * x = (int *) malloc( 2 * sizeof(int));
//		x[0] = 3;
//		x[1] = 4;
//		creating->extensions = (void **)x;
		LoadContainersFromDecodedAccessListFile(filename,  creating);
		//creating->extensions[0] = 0xABCDEF;
		//creating->extensions[1] = 0xABCDEF;

		printf( "Thread create extensions %s.\n" ,filename);
		
	}

	return true;
}



void test_switch (  Thread_Control * a,  Thread_Control * b)
{

	char namea[6];
	char nameb[6];
	rtems_object_get_name(a->Object.id, 6, namea); 
	if(!strncmp(rtems_object_get_name(b->Object.id, 6, nameb),"TA",2)){
	
		//staticpermssions * sstatic = (staticpermssions *)b->extensions;
		ALLOWCTX(b->extensions);
		//printk("%d \n", sstatic->containersSize);
#if 0			 
		int i=0;
		for(;i< sstatic->containersSize ; i++){
			unsigned long long  s = sstatic->containers[i].entryAddress;
			unsigned long long  e = sstatic->containers[i].endAddress;


			//addressList nn = sstatic->pStatic[i]->opalCodeAccessList;
			//while(nn){
			//	unsigned long long  ss = nn->startAddress;
			//	unsigned long long  se = nn->endAddress;	
			//	nn = nn->next;
			//}
			//printf("%llx %llx ", s , e);

			int j=sstatic->containers[i].opalOffsetLocateContainerInPermissions;
			int jlim = j + sstatic->containers[i].opalSizeOfPermissionLists;
			for( ; j< jlim ;j++)
			{
				packed_permission_rec* rec = &sstatic->staticPermissions[j];
				//printf("[%llx %llx) ", rec->startaddr, rec->permandsize);
			}

			printf("\n");

		}
#endif
		//printk( "User extension thread switch %s %s\n",namea, nameb);	
	}


}



rtems_task KillTaskFunction(rtems_task_argument argument)
{
  printf( "DONE !\n");
  MAGIC_BREAKPOINT;
  exit(0);
}

packed_permission_rec create_permrec(uint64 startaddr,uint64 endaddr,char perm)
{
	packed_permission_rec p;
	p.startaddr = startaddr;
	p.permandsize = ((uint64)perm) << 62 | (endaddr - startaddr);
	return p;
}

addressList consAddressList(md_addr_t startAddress, md_addr_t endAddress, addressList l)
{
	addressList temp = (addressList) malloc(sizeof(struct addressCell));
    temp -> startAddress = startAddress;
	temp -> endAddress = endAddress;
    temp -> next = l;
    return temp;
}

addressList freeAddressList(addressList l)
{
	while(l != NULL){
		addressList temp = l;
		l = l->next;
		free(temp);
	}
    return NULL;
}

void LoadContainersFromDecodedAccessListFile(const char * FileWithPrefix, Thread_Control *creating)
{
	unsigned long long codeA;
    unsigned long long codeB;
	unsigned long long stackA;
	unsigned long long stackB;
	char junk[7][200];
	FILE * file;
	int totalCount;
	
	file = fopen(FileWithPrefix,"r");

	if(!file){ printf("\n\n Runtime ERROR : unable to open symbol file %s",FileWithPrefix);exit(0);}
	
	
 	fscanf(file,"count: %x\n",&totalCount);
 	fscanf(file,"data: %llx %llx stack: %llx %llx\n",&codeA,&codeB,&stackA,&stackB);
	fscanf(file,"%s %s\t%s\t%s\t%s\t%s\t%s\n",junk[0],junk[1],junk[2],junk[3],junk[4],junk[5],junk[6]);

	staticpermssions * sstatic = (staticpermssions *) calloc(1,  sizeof(staticpermssions));
	if( !sstatic ){ printk( "ERROR ON MALLOC \n"); exit(0);}
	sstatic->containersSize =  totalCount;
	sstatic->containers =  (staticpermlist *)calloc( 1000,  sizeof( staticpermlist));
	if( !sstatic->containers ){ printk( "ERROR ON MALLOC \n"); exit(0);}

	creating->extensions = (void **) sstatic;
	//printk( "creating->extensions =  %x \n", creating->extensions);
	
	#if 0
		printf("count:%d\n",totalCount);
 		printf("data: %llx %llx stack: %llx %llx\n",codeA,codeB,stackA,stackB);
 		printf("%s %s\t%s\t%s\t%s\t%s\t%s\n",junk[0],junk[1],junk[2],junk[3],junk[4],junk[5],junk[6]);fflush(stdin);
	#endif
	//exit(0);
	md_addr_t offset = 0;
	int idx = 0;
	while(idx < totalCount){
		unsigned long long addrStart;
		unsigned long long addrEnd;
		char name[50];
		int fullListLength;
		int usableListLength;
		int totalHeapCalls;
		int totalStackPushes;
		
		fscanf(file,"%llx %llx\t%s\t%d\t%d\t%d\t%d\n",&addrStart,&addrEnd,name,&totalHeapCalls,&totalStackPushes,&fullListLength,&usableListLength);
		#if 0
			printf("\n %llx %llx\t%s\t%d\t%d\t%d ",addrStart,addrEnd,name,totalHeapCalls,totalStackPushes,listLength);
			fflush(stdout);
		#endif
	
		staticpermlist * newcont = &(sstatic->containers[idx++]);
		newcont->opalOffsetLocateContainerInPermissions = offset;
		newcont->opalSizeOfPermissionLists = usableListLength;
		newcont->entryAddress = addrStart;
		newcont->endAddress = addrEnd;

		offset += usableListLength; //* sizeof(packed_permission_rec)
	}
	uint64 totalPermissionsCount;
	fscanf(file,"%lld", &totalPermissionsCount);
	sstatic->staticPermissionsSize = totalPermissionsCount;
	//sstatic->staticPermissions = (packed_permission_rec *) malloc (sizeof(packed_permission_rec) * totalPermissionsCount);
	sstatic->staticPermissions = (packed_permission_rec *) calloc (1, sizeof(packed_permission_rec) * 10000);
	sstatic->permissionStackMaxSize = 10*RTEMS_MINIMUM_STACK_SIZE;
	sstatic->permissionStack = (packed_permission_rec *) calloc (1, 10*RTEMS_MINIMUM_STACK_SIZE );
	sstatic->dynamicContainerRuntimeRecordSize = 0;
	sstatic->dynamicContainerRuntimeRecord = (packed_permission_rec *) calloc (1, 5*RTEMS_MINIMUM_STACK_SIZE );
	sstatic->dynamicPermissionBufferSize = 0;
	sstatic->dynamicPermissionBuffer = (packed_permission_rec *) calloc (1, 5*RTEMS_MINIMUM_STACK_SIZE );
	
	 
	//printf("sizeof(packed_permission_rec *) = %ld ", sizeof(packed_permission_rec *));
	//printf("%llx %llx %lld %lld \n", (uint64)sstatic->staticPermissions,(uint64)sstatic->permissionStack,sstatic->staticPermissionsSize, sstatic->permissionStackMaxSize );
	idx = 0;
	while(idx < totalPermissionsCount){
		int usableListLength;
		fscanf(file,"%d ", &usableListLength);
		int i=0 ;
		for (; i < usableListLength; i++){
			
			//packed_permission_rec * itt = &( sstatic->staticPermissions[idx]);
			char c;
			uint64 astart;
			uint64 aend;
			fscanf(file,"%c[%llx,%llx) ", &c, &astart, &aend);
			sstatic->staticPermissions[idx] = create_permrec(astart, aend, 3);
			//printf("%c[%llx,%llx) ", c, sstatic->staticPermissions[idx].startaddr, sstatic->staticPermissions[idx].permandsize);
			idx++;
		}
		fscanf(file,"\n"); 
		//printf("\n");
	}

	
	fclose(file);

}


