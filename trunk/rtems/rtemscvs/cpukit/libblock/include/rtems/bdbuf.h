/**
 * @file
 *
 * @ingroup rtems_bdbuf
 *
 * Block device buffer management.
 */
 
/*
 * Copyright (C) 2001 OKTET Ltd., St.-Petersburg, Russia
 * Author: Victor V. Vengerov <vvv@oktet.ru>
 *
 * Copyright (C) 2008,2009 Chris Johns <chrisj@rtems.org>
 *    Rewritten to remove score mutex access. Fixes many performance
 *    issues.
      Change to support demand driven variable buffer sizes.
 *
 * @(#) bdbuf.h,v 1.9 2005/02/02 00:06:18 joel Exp
 */

#ifndef _RTEMS_BDBUF_H
#define _RTEMS_BDBUF_H

#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/chain.h>

#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>

#ifdef __cplusplus
extern "C" {
#endif
	
/**
 * @defgroup rtems_libblock Block Device Library
 *
 * Block device modules.
 */

/**
 * @defgroup rtems_bdbuf Block Device Buffer Management
 *
 * @ingroup rtems_libblock
 *
 * The Block Device Buffer Management implements a cache between the disk
 * devices and file systems. The code provides read ahead and write queuing to
 * the drivers and fast cache look up using an AVL tree.
 *
 * The block size used by a file system can be set at runtime and must be a
 * multiple of the disk device block size. The disk device's physical block
 * size is called the media block size. The file system can set the block size
 * it uses to a larger multiple of the media block size. The driver must be
 * able to handle buffers sizes larger than one media block.
 *
 * The user configures the amount of memory to be used as buffers in the cache,
 * and the minimum and maximum buffer size. The cache will allocate additional
 * memory for the buffer descriptors and groups. There are enough buffer
 * descriptors allocated so all the buffer memory can be used as minimum sized
 * buffers.
 *
 * The cache is a single pool of buffers. The buffer memory is divided into
 * groups where the size of buffer memory allocated to a group is the maximum
 * buffer size. A group's memory can be divided down into small buffer sizes
 * that are a multiple of 2 of the minimum buffer size. A group is the minumum
 * allocation unit for buffers of a specific size. If a buffer of maximum size
 * is request the group will have a single buffer. If a buffer of minium size
 * is requested the group is divided into minimum sized buffers and the
 * remaining buffers are held ready for use. A group keeps track of which
 * buffers are with a file system or driver and groups who have buffer in use
 * cannot be realloced. Groups with no buffers in use can be taken and
 * realloced to a new size. This is how buffers of different sizes move around
 * the cache.

 * The buffers are held in various lists in the cache. All buffers follow this
 * state machine:
 *                                  
 * @dot
 * digraph g {
 *   ready [label="Ready\nRead Ahead"];
 *   transfer [label="Transfer"];
 *   accessed [label="Accessed\nAccessed Modified"];
 *   modified [label="Modified\nSynchronized"];
 *   cached [label="Cached"];
 *   ready -> transfer [label="Read\nRead Ahead"];
 *   transfer -> ready [label="Read Ahead Complete"];
 *   ready -> accessed [label="Get"];
 *   transfer -> accessed [label="Read or Write\nComplete"];
 *   transfer -> cached [label="Read or Write\nComplete"];
 *   accessed -> cached [label="Release"];
 *   cached -> accessed [label="Get"];
 *   modified -> accessed [label="Get"];
 *   accessed -> modified [label="Modified"];
 *   accessed -> transfer [label="Swap"];
 * }
 * @enddot
 *         
 * Empty buffers are added to the ready list and removed from this queue when a
 * caller requests a buffer. This is referred to as getting a buffer in the
 * code and the event get in the state diagram. The buffer is assigned to a
 * block and inserted to the AVL based on the block/device key. If the block is
 * to be read by the user and not in the cache (ready) it is transfered from
 * the disk into memory. If no ready buffers exist the buffer is taken from the
 * LRU list. If no buffers are on the LRU list the modified list is check. If
 * no buffers are on the modified list the request blocks. If buffers are on
 * the modified list the buffers hold timer is expired and the swap out task
 * woken.
 *
 * A block being accessed is given to the file system layer and not accessable
 * to another requester until released back to the cache. The same goes to a
 * buffer in the transfer state. The transfer state means being read or
 * written. If the file system has modifed the block and releases it as
 * modified it placed on the cache's modified list and a hold timer
 * initialised. The buffer is held for the hold time before being written to
 * disk. Buffers are held for a configurable period of time on the modified
 * list as a write sets the state to transfer and this locks the buffer out
 * from the file system until the write completes. Buffers are often accessed
 * and modified in a series of small updates so if sent to the disk when
 * released as modified the user would have to block waiting until it had been
 * written. This would be a performance problem.
 *
 * The code performs mulitple block reads and writes. Multiple block reads or
 * read ahead increases performance with hardware that supports it. It also
 * helps with a large cache as the disk head movement is reduced. It how-ever
 * is a speculative operation so excessive use can remove valuable and needed
 * blocks from the cache. The get call knows if a read is a for the file system
 * or if it is a read ahead get. If the get is for a read ahead block and the
 * block is already in the cache or no ready buffers are available the read
 * ahead is stopped. The transfer occurs with the blocks so far. If a buffer is
 * in the read ahead state and release it is placed on the ready list rather
 * than the LRU list. This means these buffers are used before buffers used by
 * the file system.
 *
 * The cache has the following lists of buffers:
 *  - @c ready: Empty buffers created when the pool is initialised.
 *  - @c modified: Buffers waiting to be written to disk.
 *  - @c sync: Buffers to be synced to disk.
 *  - @c lru: Accessed buffers released in least recently used order.
 *
 * The cache scans the ready list then the LRU list for a suitable buffer in
 * this order. A suitable buffer is one that matches the same allocation size
 * as the device the buffer is for. The a buffer's group has no buffers in use
 * with the file system or driver the group is reallocated. This means the
 * buffers in the group are invalidated, resized and placed on the ready queue.
 * There is a performance issue with this design. The reallocation of a group
 * may forced recently accessed buffers out of the cache when they should
 * not. The design should be change to have groups on a LRU list if they have
 * no buffers in use.
 *
 * @{
 */

/**
 * State of a buffer in the cache.
 */
typedef enum
{
  RTEMS_BDBUF_STATE_EMPTY = 0,            /**< Not in use. */
  RTEMS_BDBUF_STATE_READ_AHEAD = 1,       /**< Holds read ahead data only */
  RTEMS_BDBUF_STATE_CACHED = 2,           /**< In the cache and available */
  RTEMS_BDBUF_STATE_ACCESS = 3,           /**< The user has the buffer */
  RTEMS_BDBUF_STATE_MODIFIED = 4,         /**< In the cache but modified */
  RTEMS_BDBUF_STATE_ACCESS_MODIFIED = 5,  /**< With the user but modified */
  RTEMS_BDBUF_STATE_SYNC = 6,             /**< Requested to be sync'ed */
  RTEMS_BDBUF_STATE_TRANSFER = 7          /**< Being transferred to or from disk */
} rtems_bdbuf_buf_state;

/**
 * Forward reference to the block.
 */
struct rtems_bdbuf_group;
typedef struct rtems_bdbuf_group rtems_bdbuf_group;

/**
 * To manage buffers we using buffer descriptors (BD). A BD holds a buffer plus
 * a range of other information related to managing the buffer in the cache. To
 * speed-up buffer lookup descriptors are organized in AVL-Tree. The fields
 * 'dev' and 'block' are search keys.
 */
typedef struct rtems_bdbuf_buffer
{
  rtems_chain_node link;       /**< Link the BD onto a number of lists. */

  struct rtems_bdbuf_avl_node
  {
    signed char                cache;  /**< Cache */
    struct rtems_bdbuf_buffer* left;   /**< Left Child */
    struct rtems_bdbuf_buffer* right;  /**< Right Child */
    signed char                bal;    /**< The balance of the sub-tree */
  } avl;

  dev_t             dev;        /**< device number */

  rtems_blkdev_bnum block;      /**< block number on the device */

  unsigned char*    buffer;     /**< Pointer to the buffer memory area */
  int               error;      /**< If not 0 indicate an error value (errno)
                                 * which can be used by user later */

  volatile rtems_bdbuf_buf_state state;  /**< State of the buffer. */

  volatile uint32_t  waiters;    /**< The number of threads waiting on this
                                  * buffer. */
  rtems_bdbuf_group* group;      /**< Pointer to the group of BDs this BD is
                                  * part of. */
  volatile uint32_t  hold_timer; /**< Timer to indicate how long a buffer
                                  * has been held in the cache modified. */

  int   references;              /**< Allow reference counting by owner. */
  void* user;                    /**< User data. */
} rtems_bdbuf_buffer;

/**
 * A group is a continuous block of buffer descriptors. A group covers the
 * maximum configured buffer size and is the allocation size for the buffers to
 * a specific buffer size. If you allocate a buffer to be a specific size, all
 * buffers in the group, if there are more than 1 will also be that size. The
 * number of buffers in a group is a multiple of 2, ie 1, 2, 4, 8, etc.
 */
struct rtems_bdbuf_group
{
  rtems_chain_node    link;          /**< Link the groups on a LRU list if they
                                      * have no buffers in use. */
  size_t              bds_per_group; /**< The number of BD allocated to this
                                      * group. This value must be a multiple of
                                      * 2. */
  uint32_t            users;         /**< How many users the block has. */
  rtems_bdbuf_buffer* bdbuf;         /**< First BD this block covers. */
};

/**
 * Buffering configuration definition. See confdefs.h for support on using this
 * structure.
 */
typedef struct rtems_bdbuf_config {
  uint32_t            max_read_ahead_blocks;   /**< Number of blocks to read
                                                * ahead. */
  uint32_t            max_write_blocks;        /**< Number of blocks to write
                                                * at once. */
  rtems_task_priority swapout_priority;        /**< Priority of the swap out
                                                * task. */
  uint32_t            swapout_period;          /**< Period swapout checks buf
                                                * timers. */
  uint32_t            swap_block_hold;         /**< Period a buffer is held. */
  size_t              swapout_workers;         /**< The number of worker
                                                * threads for the swapout
                                                * task. */
  rtems_task_priority swapout_worker_priority; /**< Priority of the swap out
                                                * task. */
  size_t              size;                    /**< Size of memory in the
                                                * cache */
  uint32_t            buffer_min;              /**< Minimum buffer size. */
  uint32_t            buffer_max;              /**< Maximum buffer size
                                                * supported. It is also the
                                                * allocation size. */
} rtems_bdbuf_config;

/**
 * External reference to the configuration.
 *
 * The configuration is provided by the application.
 */
extern const rtems_bdbuf_config rtems_bdbuf_configuration;

/**
 * The max_read_ahead_blocks value is altered if there are fewer buffers
 * than this defined max. This stops thrashing in the cache.
 */
#define RTEMS_BDBUF_MAX_READ_AHEAD_BLOCKS_DEFAULT    32

/**
 * Default maximum number of blocks to write at once.
 */
#define RTEMS_BDBUF_MAX_WRITE_BLOCKS_DEFAULT         16

/**
 * Default swap-out task priority.
 */
#define RTEMS_BDBUF_SWAPOUT_TASK_PRIORITY_DEFAULT    15

/**
 * Default swap-out task swap period in milli seconds.
 */
#define RTEMS_BDBUF_SWAPOUT_TASK_SWAP_PERIOD_DEFAULT 250

/**
 * Default swap-out task block hold time in milli seconds.
 */
#define RTEMS_BDBUF_SWAPOUT_TASK_BLOCK_HOLD_DEFAULT  1000

/**
 * Default swap-out worker tasks. Currently disabled.
 */
#define RTEMS_BDBUF_SWAPOUT_WORKER_TASKS_DEFAULT     0

/**
 * Default swap-out worker task priority. The same as the swapout task.
 */
#define RTEMS_BDBUF_SWAPOUT_WORKER_TASK_PRIORITY_DEFAULT \
                             RTEMS_BDBUF_SWAPOUT_TASK_PRIORITY_DEFAULT

/**
 * Default size of memory allocated to the cache.
 */
#define RTEMS_BDBUF_CACHE_MEMORY_SIZE_DEFAULT (64 * 512)

/**
 * Default minimum size of buffers.
 */
#define RTEMS_BDBUF_BUFFER_MIN_SIZE_DEFAULT (512)

/**
 * Default maximum size of buffers.
 */
#define RTEMS_BDBUF_BUFFER_MAX_SIZE_DEFAULT (4096)

/**
 * Prepare buffering layer to work - initialize buffer descritors and (if it is
 * neccessary) buffers. After initialization all blocks is placed into the
 * ready state.
 *
 * @return RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *         successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_init (void);

/**
 * Get block buffer for data to be written into. The buffers is set to the
 * access or modifed access state. If the buffer is in the cache and modified
 * the state is access modified else the state is access. This buffer contents
 * are not initialised if the buffer is not already in the cache. If the block
 * is already resident in memory it is returned how-ever if not in memory the
 * buffer is not read from disk. This call is used when writing the whole block
 * on a disk rather than just changing a part of it. If there is no buffers
 * available this call will block. A buffer obtained with this call will not be
 * involved in a transfer request and will not be returned to another user
 * until released. If the buffer is already with a user when this call is made
 * the call is blocked until the buffer is returned. The highest priority
 * waiter will obtain the buffer first.
 *
 * The block number is the linear block number. This is relative to the start
 * of the partition on the media.
 *
 * @param device Device number (constructed of major and minor device number)
 * @param block  Linear media block number
 * @param bd     Reference to the buffer descriptor pointer.
 *
 * @return       RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *               successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_get (dev_t device, rtems_blkdev_bnum block, rtems_bdbuf_buffer** bd);

/**
 * Get the block buffer and if not already in the cache read from the disk. If
 * specified block already cached return. The buffer is set to the access or
 * modifed access state. If the buffer is in the cache and modified the state
 * is access modified else the state is access. If block is already being read
 * from disk for being written to disk this call blocks. If the buffer is
 * waiting to be written it is removed from modified queue and returned to the
 * user. If the buffer is not in the cache a new buffer is obtained and the
 * data read from disk. The call may block until these operations complete. A
 * buffer obtained with this call will not be involved in a transfer request
 * and will not be returned to another user until released. If the buffer is
 * already with a user when this call is made the call is blocked until the
 * buffer is returned. The highest priority waiter will obtain the buffer
 * first. 
 *
 * @param device Device number (constructed of major and minor device number)
 * @param block  Linear media block number
 * @param bd     Reference to the buffer descriptor pointer.
 *
 * @return       RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *               successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_read (dev_t device, rtems_blkdev_bnum block, rtems_bdbuf_buffer** bd);

/**
 * Release the buffer obtained by a read call back to the cache. If the buffer
 * was obtained by a get call and was not already in the cache the release
 * modified call should be used. A buffer released with this call obtained by a
 * get call may not be in sync with the contents on disk. If the buffer was in
 * the cache and modified before this call it will be returned to the modified
 * queue. The buffers is returned to the end of the LRU list.
 *
 * @param bd Reference to the buffer descriptor.
 *
 * @return RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *         successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_release (rtems_bdbuf_buffer* bd);

/**
 * Release the buffer allocated with a get or read call placing it on the
 * modidied list.  If the buffer was not released modified before the hold
 * timer is set to the configuration value. If the buffer had been released
 * modified before but not written to disk the hold timer is not updated. The
 * buffer will be written to disk when the hold timer has expired, there are
 * not more buffers available in the cache and a get or read buffer needs one
 * or a sync call has been made. If the buffer is obtained with a get or read
 * before the hold timer has expired the buffer will be returned to the user.
 *
 * @param bd Reference to the buffer descriptor.
 *
 * @return RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *         successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_release_modified (rtems_bdbuf_buffer* bd);

/**
 * Release the buffer as modified and wait until it has been synchronized with
 * the disk by writing it. This buffer will be the first to be transfer to disk
 * and other buffers may also be written if the maximum number of blocks in a
 * requests allows it.
 *
 * @note This code does not lock the sync mutex and stop additions to the
 *       modified queue.

 * @param bd Reference to the buffer descriptor.
 *
 * @return RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *         successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_sync (rtems_bdbuf_buffer* bd);

/**
 * Synchronize all modified buffers for this device with the disk and wait
 * until the transfers have completed. The sync mutex for the cache is locked
 * stopping the addition of any further modifed buffers. It is only the
 * currently modified buffers that are written.
 *
 * @note Nesting calls to sync multiple devices will be handled sequentially. A
 * nested call will be blocked until the first sync request has complete.
 *
 * @param dev Block device number
 *
 * @return RTEMS status code (RTEMS_SUCCESSFUL if operation completed
 *         successfully or error code if error is occured)
 */
rtems_status_code
rtems_bdbuf_syncdev (dev_t dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif