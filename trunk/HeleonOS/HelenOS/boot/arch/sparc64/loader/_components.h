#ifndef ___COMPONENTS_H__
#define ___COMPONENTS_H__

extern int _binary_kernel_bin_start;
extern int _binary_kernel_bin_end;

#define KERNEL_START ((void *) &_binary_kernel_bin_start)
#define KERNEL_END ((void *) &_binary_kernel_bin_end)
#define KERNEL_SIZE ((unsigned long) KERNEL_END - (unsigned long) KERNEL_START)

extern int _binary_ns_start;
extern int _binary_ns_end;

#define NS_START ((void *) &_binary_ns_start)
#define NS_END ((void *) &_binary_ns_end)
#define NS_SIZE ((unsigned long) NS_END - (unsigned long) NS_START)

extern int _binary_init_start;
extern int _binary_init_end;

#define INIT_START ((void *) &_binary_init_start)
#define INIT_END ((void *) &_binary_init_end)
#define INIT_SIZE ((unsigned long) INIT_END - (unsigned long) INIT_START)

extern int _binary_loader_start;
extern int _binary_loader_end;

#define LOADER_START ((void *) &_binary_loader_start)
#define LOADER_END ((void *) &_binary_loader_end)
#define LOADER_SIZE ((unsigned long) LOADER_END - (unsigned long) LOADER_START)

extern int _binary_devmap_start;
extern int _binary_devmap_end;

#define DEVMAP_START ((void *) &_binary_devmap_start)
#define DEVMAP_END ((void *) &_binary_devmap_end)
#define DEVMAP_SIZE ((unsigned long) DEVMAP_END - (unsigned long) DEVMAP_START)

extern int _binary_rd_start;
extern int _binary_rd_end;

#define RD_START ((void *) &_binary_rd_start)
#define RD_END ((void *) &_binary_rd_end)
#define RD_SIZE ((unsigned long) RD_END - (unsigned long) RD_START)

extern int _binary_vfs_start;
extern int _binary_vfs_end;

#define VFS_START ((void *) &_binary_vfs_start)
#define VFS_END ((void *) &_binary_vfs_end)
#define VFS_SIZE ((unsigned long) VFS_END - (unsigned long) VFS_START)

extern int _binary_fat_start;
extern int _binary_fat_end;

#define FAT_START ((void *) &_binary_fat_start)
#define FAT_END ((void *) &_binary_fat_end)
#define FAT_SIZE ((unsigned long) FAT_END - (unsigned long) FAT_START)

extern int _binary_initrd_img_start;
extern int _binary_initrd_img_end;

#define INITRD_START ((void *) &_binary_initrd_img_start)
#define INITRD_END ((void *) &_binary_initrd_img_end)
#define INITRD_SIZE ((unsigned long) INITRD_END - (unsigned long) INITRD_START)

#define COMPONENTS 9

typedef struct {
	char *name;

	void *start;
	void *end;
	unsigned long size;
} component_t;

extern void init_components(component_t *components);

#endif
