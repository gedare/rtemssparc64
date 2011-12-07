#ifndef ALLOW__H
#define ALLOW__H

#include <stdio.h>
#include <rtems.h>
#define WITHALLOW 1


struct perm_record
{
	long long int addr;
	unsigned int perm: 3;
	unsigned int multi: 1;
	unsigned long long int sz: 60;
};


#if ( defined(__sparc) || defined(__sparc__) ) && WITHALLOW

#define ALLOW( _start, _size, _perm ) \
   __asm__ __volatile__ ( \
       "impdep2  %[rs1], %[rs2] ,%%g0\n\t" \
	   : \
       : [rs1] "r" (_start),[rs2] "r" (_perm << 62 | _size) \
   ); \

#define ALLOWM( _size, _addr ) \
   __asm__ __volatile__ ( \
       "impdep2  %[rs1], %[rs2] ,%%g0\n\t" \
	   : \
       : [rs1] "r" (_addr),[rs2] "r" ( 1LL << 61 | _size) \
   );

#define ALLOWCTX( _addr ) \
   __asm__ __volatile__ ( \
       "impdep2  %[rs1], %[rs2] ,%%g0\n\t" \
	   : \
       : [rs1] "r" (_addr),[rs2] "r" ( 1LL << 60 | 0) \
   );


#else

	#define ALLOW( _start, _size, _perm )
	#define ALLOWM( _size, _addr )
	#define ALLOWCTX( _addr )


#endif


/*
#define ALLOW(_start, _size, _perm ) \
	__asm__ ("\n");
	//nothing

#define ALLOWM(_size, _addr  ) \
	__asm__ ("\n");
	//nothing

*/

/* data structures */

typedef unsigned long long uint64;
typedef uint64 md_addr_t;

typedef struct addressCell *addressList; //address access list

struct addressCell
{
  md_addr_t startAddress;
  md_addr_t endAddress;
  addressList next;
};

typedef struct staticpermlist{
	md_addr_t entryAddress; // the entry address in the container
	md_addr_t endAddress;
	md_addr_t opalOffsetLocateContainerInPermissions;
	md_addr_t opalSizeOfPermissionLists;//use in the opal module.

} staticpermlist;


typedef struct packed_permission_record{
	uint64 startaddr;
	uint64 permandsize;
} packed_permission_rec;

typedef struct unpacked_permission_record{
	uint64 startaddr;
	uint64 endaddr;
	unsigned char perm;
} unpacked_permission_rec;

typedef struct staticpermssions
{
	uint64 containersSize;
	staticpermlist *containers;
	uint64 staticPermissionsSize;
	packed_permission_rec *staticPermissions;
	uint64 permissionStackMaxSize;
	packed_permission_rec *permissionStack;
	uint64 dynamicPermissionBufferSize;
	packed_permission_rec *dynamicPermissionBuffer;
	uint64 dynamicContainerRuntimeRecordSize;
	packed_permission_rec *dynamicContainerRuntimeRecord;
} staticpermssions;



inline void * mymalloc(size_t);

packed_permission_rec create_permrec(uint64 startaddr,uint64 endaddr,char perm);
bool task_container_create(Thread_Control * executing,  Thread_Control * creating);
void task_container_switch (  Thread_Control * a,  Thread_Control * b);
bool task_tlb_create(Thread_Control * executing,  Thread_Control * creating);
void task_tlb_switch (  Thread_Control * a,  Thread_Control * b);




rtems_task KillTaskFunction(rtems_task_argument argument);
void LoadContainersFromDecodedAccessListFile(const char * FileWithPrefix, Thread_Control *creating);






#endif

