/**
 * @file rtems/libio.h
 */

/*
 *  System call and file system interface definition
 *
 *  General purpose communication channel for RTEMS to allow UNIX/POSIX
 *  system call behavior under RTEMS.  Initially this supported only
 *  IO to devices but has since been enhanced to support networking
 *  and support for mounted file systems.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: libio.h,v 1.59 2009/12/10 14:10:57 ralf Exp $
 */

#ifndef _RTEMS_RTEMS_LIBIO_H
#define _RTEMS_RTEMS_LIBIO_H

#include <rtems.h>
#include <rtems/chain.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>

/*
 *  Define data types which must be constructed using forward references.
 */

#include <rtems/fs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A 64bit file offset for internal use by RTEMS. Based on the newlib type.
 */
typedef _off64_t rtems_off64_t;

/*
 * Valid RTEMS file types.
 */

#define RTEMS_FILESYSTEM_DIRECTORY   1
#define RTEMS_FILESYSTEM_DEVICE      2
#define RTEMS_FILESYSTEM_HARD_LINK   3
#define RTEMS_FILESYSTEM_SYM_LINK    4
#define RTEMS_FILESYSTEM_MEMORY_FILE 5
typedef int rtems_filesystem_node_types_t;

/*
 *  File Handler Operations Table
 */

typedef int (*rtems_filesystem_open_t)(
  rtems_libio_t *iop,
  const char    *pathname,
  uint32_t       flag,
  uint32_t       mode
);

typedef int (*rtems_filesystem_close_t)(
  rtems_libio_t *iop
);

typedef ssize_t (*rtems_filesystem_read_t)(
  rtems_libio_t *iop,
  void          *buffer,
  size_t         count
);

typedef ssize_t (*rtems_filesystem_write_t)(
  rtems_libio_t *iop,
  const void    *buffer,
  size_t         count
);

typedef int (*rtems_filesystem_ioctl_t)(
  rtems_libio_t *iop,
  uint32_t       command,
  void          *buffer
);

typedef rtems_off64_t (*rtems_filesystem_lseek_t)(
  rtems_libio_t *iop,
  rtems_off64_t  length,
  int            whence
);

typedef int (*rtems_filesystem_fstat_t)(
  rtems_filesystem_location_info_t *loc,
  struct stat                      *buf
);

typedef int (*rtems_filesystem_fchmod_t)(
  rtems_filesystem_location_info_t *loc,
  mode_t                            mode
);

typedef int (*rtems_filesystem_ftruncate_t)(
  rtems_libio_t *iop,
  rtems_off64_t  length
);

typedef int (*rtems_filesystem_fpathconf_t)(
  rtems_libio_t *iop,
  int name
);

typedef int (*rtems_filesystem_fsync_t)(
  rtems_libio_t *iop
);

typedef int (*rtems_filesystem_fdatasync_t)(
  rtems_libio_t *iop
);

typedef int (*rtems_filesystem_fcntl_t)(
  int            cmd,
  rtems_libio_t *iop
);

typedef int (*rtems_filesystem_rmnod_t)(
 rtems_filesystem_location_info_t      *parent_loc,   /* IN */
 rtems_filesystem_location_info_t      *pathloc       /* IN */
);

struct _rtems_filesystem_file_handlers_r {
    rtems_filesystem_open_t         open_h;
    rtems_filesystem_close_t        close_h;
    rtems_filesystem_read_t         read_h;
    rtems_filesystem_write_t        write_h;
    rtems_filesystem_ioctl_t        ioctl_h;
    rtems_filesystem_lseek_t        lseek_h;
    rtems_filesystem_fstat_t        fstat_h;
    rtems_filesystem_fchmod_t       fchmod_h;
    rtems_filesystem_ftruncate_t    ftruncate_h;
    rtems_filesystem_fpathconf_t    fpathconf_h;
    rtems_filesystem_fsync_t        fsync_h;
    rtems_filesystem_fdatasync_t    fdatasync_h;
    rtems_filesystem_fcntl_t        fcntl_h;
    rtems_filesystem_rmnod_t        rmnod_h;
};

/*
 *  File System Operations Table
 */

/*
 *  XXX
 *  This routine does not allocate any space and rtems_filesystem_freenode_t
 *  is not called by the generic after calling this routine.
 *  ie. node_access does not have to contain valid data when the
 *      routine returns.
 */

typedef int (*rtems_filesystem_mknod_t)(
   const char                        *path,       /* IN */
   mode_t                             mode,       /* IN */
   dev_t                              dev,        /* IN */
   rtems_filesystem_location_info_t  *pathloc     /* IN/OUT */
);

/*
 *  rtems_filesystem_freenode_t must be called by the generic after
 *  calling this routine
 */

typedef int (*rtems_filesystem_evalpath_t)(
  const char                        *pathname,      /* IN     */
  int                                pathnamelen,   /* IN     */
  int                                flags,         /* IN     */
  rtems_filesystem_location_info_t  *pathloc        /* IN/OUT */
);

typedef int (*rtems_filesystem_evalmake_t)(
   const char                       *path,       /* IN */
   rtems_filesystem_location_info_t *pathloc,    /* IN/OUT */
   const char                      **name        /* OUT    */
);

typedef int (*rtems_filesystem_link_t)(
 rtems_filesystem_location_info_t  *to_loc,      /* IN */
 rtems_filesystem_location_info_t  *parent_loc,  /* IN */
 const char                        *name         /* IN */
);

typedef int (*rtems_filesystem_unlink_t)(
 rtems_filesystem_location_info_t  *parent_pathloc, /* IN */
 rtems_filesystem_location_info_t  *pathloc         /* IN */
);

typedef int (*rtems_filesystem_chown_t)(
 rtems_filesystem_location_info_t  *pathloc,       /* IN */
 uid_t                              owner,         /* IN */
 gid_t                              group          /* IN */
);

typedef int (*rtems_filesystem_freenode_t)(
 rtems_filesystem_location_info_t      *pathloc       /* IN */
);

typedef int (* rtems_filesystem_mount_t ) (
   rtems_filesystem_mount_table_entry_t *mt_entry     /* in */
);

typedef int (* rtems_filesystem_fsmount_me_t )(
   rtems_filesystem_mount_table_entry_t *mt_entry
);

typedef int (* rtems_filesystem_unmount_t ) (
   rtems_filesystem_mount_table_entry_t *mt_entry     /* in */
);

typedef int (* rtems_filesystem_fsunmount_me_t ) (
   rtems_filesystem_mount_table_entry_t *mt_entry    /* in */
);

typedef rtems_filesystem_node_types_t (* rtems_filesystem_node_type_t) (
  rtems_filesystem_location_info_t    *pathloc      /* in */
);

typedef int (* rtems_filesystem_utime_t)(
  rtems_filesystem_location_info_t  *pathloc,       /* IN */
  time_t                             actime,        /* IN */
  time_t                             modtime        /* IN */
);

typedef int (*rtems_filesystem_evaluate_link_t)(
  rtems_filesystem_location_info_t *pathloc,     /* IN/OUT */
  int                               flags        /* IN     */
);

typedef int (*rtems_filesystem_symlink_t)(
 rtems_filesystem_location_info_t  *loc,         /* IN */
 const char                        *link_name,   /* IN */
 const char                        *node_name
);

typedef int (*rtems_filesystem_readlink_t)(
 rtems_filesystem_location_info_t  *loc,     /* IN  */
 char                              *buf,     /* OUT */
 size_t                            bufsize
);

typedef int (*rtems_filesystem_statvfs_t)(
 rtems_filesystem_location_info_t  *loc,     /* IN  */
 struct statvfs                    *buf      /* OUT */
);

/*
 * operations table that must be defined for every file system.
 */

/*
 * File system types
 */
struct _rtems_filesystem_operations_table {
    rtems_filesystem_evalpath_t      evalpath_h;
    rtems_filesystem_evalmake_t      evalformake_h;
    rtems_filesystem_link_t          link_h;
    rtems_filesystem_unlink_t        unlink_h;
    rtems_filesystem_node_type_t     node_type_h;
    rtems_filesystem_mknod_t         mknod_h;
    rtems_filesystem_chown_t         chown_h;
    rtems_filesystem_freenode_t      freenod_h;
    rtems_filesystem_mount_t         mount_h;
    rtems_filesystem_fsmount_me_t    fsmount_me_h;
    rtems_filesystem_unmount_t       unmount_h;
    rtems_filesystem_fsunmount_me_t  fsunmount_me_h;
    rtems_filesystem_utime_t         utime_h;
    rtems_filesystem_evaluate_link_t eval_link_h;
    rtems_filesystem_symlink_t       symlink_h;
    rtems_filesystem_readlink_t      readlink_h;
    rtems_filesystem_statvfs_t       statvfs_h;
};

#if 0
/* Now in exec/include/rtems/fs.h */

/*
 * Structure used to determine a location/filesystem in the tree.
 */

struct rtems_filesystem_location_info_tt
{
  void                                   *node_access;
  rtems_filesystem_file_handlers_r       *handlers;
  rtems_filesystem_operations_table      *ops;
  rtems_filesystem_mount_table_entry_t   *mt_entry;
};
#endif

/*
 *  Structure used to contain file system specific information which
 *  is required to support fpathconf().
 */

typedef struct {
  int    link_max;
  int    max_canon;
  int    max_input;
  int    name_max;
  int    path_max;
  int    pipe_buf;
  int    posix_async_io;
  int    posix_chown_restrictions;
  int    posix_no_trunc;
  int    posix_prio_io;
  int    posix_sync_io;
  int    posix_vdisable;
} rtems_filesystem_limits_and_options_t;

/*
 * Structure for a mount table entry.
 */

struct rtems_filesystem_mount_table_entry_tt {
  rtems_chain_node                       Node;
  rtems_filesystem_location_info_t       mt_point_node;
  rtems_filesystem_location_info_t       mt_fs_root;
  int                                    options;
  void                                  *fs_info;

  rtems_filesystem_limits_and_options_t  pathconf_limits_and_options;

  /*
   *  When someone adds a mounted filesystem on a real device,
   *  this will need to be used.
   *
   *  The lower layers can manage how this is managed. Leave as a
   *  string.
   */
  char                                  *dev;
};

/*
 *  Valid RTEMS file systems options
 */

typedef enum
{
  RTEMS_FILESYSTEM_READ_ONLY,
  RTEMS_FILESYSTEM_READ_WRITE,
  RTEMS_FILESYSTEM_BAD_OPTIONS
} rtems_filesystem_options_t;


/*
 *  An open file data structure, indexed by 'fd'
 *  TODO:
 *     should really have a separate per/file data structure that this
 *     points to (eg: size, offset, driver, pathname should be in that)
 */

struct rtems_libio_tt {
    rtems_driver_name_t                    *driver;
    rtems_off64_t                           size;      /* size of file */
    rtems_off64_t                           offset;    /* current offset into file */
    uint32_t                                flags;
    rtems_filesystem_location_info_t        pathinfo;
    rtems_id                                sem;
    uint32_t                                data0;     /* private to "driver" */
    void                                   *data1;     /* ... */
    void                                   *file_info; /* used by file handlers */
    const rtems_filesystem_file_handlers_r *handlers;  /* type specific handlers */
};

/*
 *  param block for read/write
 *  Note: it must include 'offset' instead of using iop's offset since
 *        we can have multiple outstanding i/o's on a device.
 */

typedef struct {
    rtems_libio_t          *iop;
    rtems_off64_t           offset;
    char                   *buffer;
    uint32_t                count;
    uint32_t                flags;
    uint32_t                bytes_moved;
} rtems_libio_rw_args_t;

/*
 *  param block for open/close
 */

typedef struct {
    rtems_libio_t          *iop;
    uint32_t                flags;
    uint32_t                mode;
} rtems_libio_open_close_args_t;

/*
 *  param block for ioctl
 */

typedef struct {
    rtems_libio_t          *iop;
    uint32_t                command;
    void                   *buffer;
    uint32_t                ioctl_return;
} rtems_libio_ioctl_args_t;

/*
 *  Values for 'flag'
 */

#define LIBIO_FLAGS_NO_DELAY      0x0001  /* return immediately if no data */
#define LIBIO_FLAGS_READ          0x0002  /* reading */
#define LIBIO_FLAGS_WRITE         0x0004  /* writing */
#define LIBIO_FLAGS_OPEN          0x0100  /* device is open */
#define LIBIO_FLAGS_APPEND        0x0200  /* all writes append */
#define LIBIO_FLAGS_CREATE        0x0400  /* create file */
#define LIBIO_FLAGS_CLOSE_ON_EXEC 0x0800  /* close on process exec() */
#define LIBIO_FLAGS_READ_WRITE    (LIBIO_FLAGS_READ | LIBIO_FLAGS_WRITE)

void rtems_libio_init(void);

/*
 *  External I/O handlers
 */

typedef int (*rtems_libio_open_t)(
  const char  *pathname,
  uint32_t    flag,
  uint32_t    mode
);

typedef int (*rtems_libio_close_t)(
  int  fd
);

typedef int (*rtems_libio_read_t)(
  int         fd,
  void       *buffer,
  uint32_t    count
);

typedef int (*rtems_libio_write_t)(
  int         fd,
  const void *buffer,
  uint32_t    count
);

typedef int (*rtems_libio_ioctl_t)(
  int         fd,
  uint32_t    command,
  void       *buffer
);

typedef rtems_off64_t (*rtems_libio_lseek_t)(
  int           fd,
  rtems_off64_t offset,
  int           whence
);

/*
 *  The following macros are used to build up the permissions sets
 *  used to check permissions.  These are similar in style to the
 *  mode_t bits and should stay compatible with them.
 */

#define RTEMS_LIBIO_PERMS_READ   S_IROTH
#define RTEMS_LIBIO_PERMS_WRITE  S_IWOTH
#define RTEMS_LIBIO_PERMS_RDWR   (S_IROTH|S_IWOTH)
#define RTEMS_LIBIO_PERMS_EXEC   S_IXOTH
#define RTEMS_LIBIO_PERMS_SEARCH RTEMS_LIBIO_PERMS_EXEC
#define RTEMS_LIBIO_PERMS_RWX    S_IRWXO

/*
 *  Macros
 */

#if 0
#define rtems_filesystem_make_dev_t( _major, _minor ) \
  ((((dev_t)(_major)) << 32) | (dev_t)(_minor))

#define rtems_filesystem_dev_major_t( _dev ) \
  (rtems_device_major_number) ((_dev) >> 32)

#define rtems_filesystem_dev_minor_t( _dev ) \
  (rtems_device_minor_number) ((_dev) & 0xFFFFFFFF)
#else

#include <unistd.h>

union __rtems_dev_t {
  dev_t device;
  struct {
     rtems_device_major_number major;
     rtems_device_minor_number minor;
  } __overlay;
};

static inline dev_t rtems_filesystem_make_dev_t(
  rtems_device_major_number _major,
  rtems_device_minor_number _minor
)
{
  union __rtems_dev_t temp;

  temp.__overlay.major = _major;
  temp.__overlay.minor = _minor;
  return temp.device;
}

static inline rtems_device_major_number rtems_filesystem_dev_major_t(
  dev_t device
)
{
  union __rtems_dev_t temp;

  temp.device = device;
  return temp.__overlay.major;
}


static inline rtems_device_minor_number rtems_filesystem_dev_minor_t(
  dev_t device
)
{
  union __rtems_dev_t temp;

  temp.device = device;
  return temp.__overlay.minor;
}

#endif

#define rtems_filesystem_split_dev_t( _dev, _major, _minor ) \
  do { \
    (_major) = rtems_filesystem_dev_major_t ( _dev ); \
    (_minor) = rtems_filesystem_dev_minor_t( _dev ); \
  } while(0)

/*
 * Verifies that the permission flag is valid.
 */
#define rtems_libio_is_valid_perms( _perm )     \
 (~ ((~RTEMS_LIBIO_PERMS_RWX) & _perm ))


/*
 *  Prototypes for filesystem
 */

void rtems_filesystem_initialize( void );


/*
 * Callbacks from TERMIOS routines to device-dependent code
 */

#include <termios.h>

typedef struct rtems_termios_callbacks {
  int    (*firstOpen)(int major, int minor, void *arg);
  int    (*lastClose)(int major, int minor, void *arg);
  int    (*pollRead)(int minor);
  ssize_t (*write)(int minor, const char *buf, size_t len);
  int    (*setAttributes)(int minor, const struct termios *t);
  int    (*stopRemoteTx)(int minor);
  int    (*startRemoteTx)(int minor);
  int    outputUsesInterrupts;
} rtems_termios_callbacks;

/*
 *  Device-independent TERMIOS routines
 */

void rtems_termios_initialize (void);

/*
 * CCJ: Change before opening a tty. Newer code from Eric is coming
 * so extra work to handle an open tty is not worth it. If the tty
 * is open, close then open it again.
 */
rtems_status_code rtems_termios_bufsize (
  int cbufsize,     /* cooked buffer size */
  int raw_input,    /* raw input buffer size */
  int raw_output    /* raw output buffer size */
);

rtems_status_code rtems_termios_open (
  rtems_device_major_number      major,
  rtems_device_minor_number      minor,
  void                          *arg,
  const rtems_termios_callbacks *callbacks
);

rtems_status_code rtems_termios_close(
  void *arg
);

rtems_status_code rtems_termios_read(
  void *arg
);

rtems_status_code rtems_termios_write(
  void *arg
);

rtems_status_code rtems_termios_ioctl(
  void *arg
);

int rtems_termios_enqueue_raw_characters(
  void *ttyp,
  char *buf,
  int   len
);

int rtems_termios_dequeue_characters(
  void *ttyp,
  int   len
);

int unmount(
  const char *mount_path
);

int mount(
  rtems_filesystem_mount_table_entry_t    **mt_entry,
  const rtems_filesystem_operations_table  *fs_ops,
  rtems_filesystem_options_t                fsoptions,
  const char                               *device,
  const char                               *mount_point
);

/*
 *  Boot Time Mount Table Structure
 */

typedef struct {
  const rtems_filesystem_operations_table *fs_ops;
  rtems_filesystem_options_t               fsoptions;
  const char                              *device;
  const char                              *mount_point;
} rtems_filesystem_mount_table_t;

extern const rtems_filesystem_mount_table_t *rtems_filesystem_mount_table;
extern const int                             rtems_filesystem_mount_table_size;


typedef void (*rtems_libio_init_functions_t)(void);
extern  rtems_libio_init_functions_t rtems_libio_init_helper;

void    open_dev_console(void);

typedef void (*rtems_libio_supp_functions_t)(void);
extern  rtems_libio_supp_functions_t rtems_libio_supp_helper;

typedef void (*rtems_fs_init_functions_t)(void);
extern  rtems_fs_init_functions_t    rtems_fs_init_helper;

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_LIBIO_H */
