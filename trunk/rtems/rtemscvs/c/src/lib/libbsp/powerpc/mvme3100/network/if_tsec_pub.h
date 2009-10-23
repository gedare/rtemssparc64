#ifndef IF_TSEC_PUBLIC_INTERFACE_H
#define IF_TSEC_PUBLIC_INTERFACE_H

/* 
 * Authorship
 * ----------
 * This software ('mvme3100' RTEMS BSP) was created by
 *
 *     Till Straumann <strauman@slac.stanford.edu>, 2005-2007,
 * 	   Stanford Linear Accelerator Center, Stanford University.
 * 
 * Acknowledgement of sponsorship
 * ------------------------------
 * The 'mvme3100' BSP was produced by
 *     the Stanford Linear Accelerator Center, Stanford University,
 * 	   under Contract DE-AC03-76SFO0515 with the Department of Energy.
 * 
 * Government disclaimer of liability
 * ----------------------------------
 * Neither the United States nor the United States Department of Energy,
 * nor any of their employees, makes any warranty, express or implied, or
 * assumes any legal liability or responsibility for the accuracy,
 * completeness, or usefulness of any data, apparatus, product, or process
 * disclosed, or represents that its use would not infringe privately owned
 * rights.
 * 
 * Stanford disclaimer of liability
 * --------------------------------
 * Stanford University makes no representations or warranties, express or
 * implied, nor assumes any liability for the use of this software.
 * 
 * Stanford disclaimer of copyright
 * --------------------------------
 * Stanford University, owner of the copyright, hereby disclaims its
 * copyright and all other rights in this software.  Hence, anyone may
 * freely use it for any purpose without restriction.  
 * 
 * Maintenance of notices
 * ----------------------
 * In the interest of clarity regarding the origin and status of this
 * SLAC software, this and all the preceding Stanford University notices
 * are to remain affixed to any copy or derivative of this software made
 * or distributed by the recipient and are to be affixed to any copy of
 * software made or distributed by the recipient that contains a copy or
 * derivative of this software.
 * 
 * ------------------ SLAC Software Notices, Set 4 OTT.002a, 2004 FEB 03
 */ 

#include <rtems.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque driver handle */
struct tsec_private;

/********** Low-level Driver API ****************/

/*
 * This API provides driver access to applications that
 * want to use e.g., the second ethernet interface
 * independently from the BSD TCP/IP stack. E.g., for
 * raw ethernet packet communication...
 */

#define TSEC_TXIRQ ( (1<<(31-9)) | (1<<(31-11))                )
#define TSEC_RXIRQ ( (1<<(31-0)) | (1<<(31- 3)) | (1<<(31-24)) )
#define TSEC_LKIRQ (  1<<(31- 4)                               )
/*
 * Setup an interface.
 * Allocates resources for descriptor rings and sets up the driver software structure.
 *
 * Arguments:
 *	unit:
 *		interface # (1..2). The interface must not be attached to BSD already.
 *
 *  driver_tid:
 *		ISR posts RTEMS event # ('unit' - 1) to task with ID 'driver_tid' and disables interrupts
 *		from this interface.
 *
 *	void (*cleanup_txbuf)(void *user_buf, void *cleanup_txbuf_arg, int error_on_tx_occurred):
 *		Pointer to user-supplied callback to release a buffer that had been sent
 *		by BSP_tsec_send_buf() earlier. The callback is passed 'cleanup_txbuf_arg'
 *		and a flag indicating whether the send had been successful.
 *		The driver no longer accesses 'user_buf' after invoking this callback.
 *		CONTEXT: This callback is executed either by BSP_tsec_swipe_tx() or 
 *		BSP_tsec_send_buf(), BSP_tsec_init_hw(), BSP_tsec_stop_hw() (the latter 
 *		ones calling BSP_tsec_swipe_tx()).
 *	void *cleanup_txbuf_arg:
 *		Closure argument that is passed on to 'cleanup_txbuf()' callback;
 *
 *	void *(*alloc_rxbuf)(int *p_size, uintptr_t *p_data_addr),
 *		Pointer to user-supplied callback to allocate a buffer for subsequent
 *		insertion into the RX ring by the driver.
 *		RETURNS: opaque handle to the buffer (which may be a more complex object
 *				 such as an 'mbuf'). The handle is not used by the driver directly
 *				 but passed back to the 'consume_rxbuf()' callback.
 *				 Size of the available data area and pointer to buffer's data area
 *				 in '*psize' and '*p_data_area', respectively.
 *				 If no buffer is available, this routine should return NULL in which
 *				 case the driver drops the last packet and re-uses the last buffer
 *				 instead of handing it out to 'consume_rxbuf()'.
 *		CONTEXT: Called when initializing the RX ring (BSP_tsec_init_hw()) or when
 *				 swiping it (BSP_tsec_swipe_rx()).
 *		
 *
 *	void (*consume_rxbuf)(void *user_buf, void *consume_rxbuf_arg, int len);
 *		Pointer to user-supplied callback to pass a received buffer back to
 *		the user. The driver no longer accesses the buffer after invoking this
 *		callback (with 'len'>0, see below). 'user_buf' is the buffer handle
 *		previously generated by 'alloc_rxbuf()'.
 *		The callback is passed 'cleanup_rxbuf_arg' and a 'len'
 *		argument giving the number of bytes that were received.
 *		'len' may be <=0 in which case the 'user_buf' argument is NULL.
 *		'len' == 0 means that the last 'alloc_rxbuf()' had failed,
 *		'len' < 0 indicates a receiver error. In both cases, the last packet
 *		was dropped/missed and the last buffer will be re-used by the driver.
 *		NOTE: the data are 'prefixed' with two bytes, i.e., the ethernet packet header
 *		      is stored at offset 2 in the buffer's data area. Also, the FCS (4 bytes)
 *		      is appended. 'len' accounts for both.
 *		CONTEXT: Called from BSP_tsec_swipe_rx().
 *	void *cleanup_rxbuf_arg:
 *		Closure argument that is passed on to 'consume_rxbuf()' callback;
 *
 *  rx_ring_size, tx_ring_size:
 *		How many big to make the RX and TX descriptor rings. Note that the sizes
 *		may be 0 in which case a reasonable default will be used.
 *		If either ring size is < 0 then the RX or TX will be disabled.
 *		Note that it is illegal in this case to use BSP_tsec_swipe_rx() or
 *		BSP_tsec_swipe_tx(), respectively.
 *
 *  irq_mask:
 *		Interrupts to enable. OR of flags from above.
 *
 */
struct tsec_private *
BSP_tsec_setup(
	int		 unit,
	rtems_id driver_tid,
	void     (*cleanup_txbuf)(void *user_buf, void *cleanup_txbuf_arg, int error_on_tx_occurred), 
	void *   cleanup_txbuf_arg,
	void *   (*alloc_rxbuf)(int *p_size, uintptr_t *p_data_addr),
	void     (*consume_rxbuf)(void *user_buf, void *consume_rxbuf_arg, int len),
	void *   consume_rxbuf_arg,
	int		 rx_ring_size,
	int		 tx_ring_size,
	int		 irq_mask
);

/*
 * Alternate 'setup' routine allowing the user to install an ISR rather
 * than a task ID.
 * All parameters (other than 'isr' / 'isr_arg') and the return value
 * are identical to the BSP_tsec_setup() entry point.
 */
struct tsec_private *
BSP_tsec_setup_1(
	int		 unit,
	void     (*isr)(void *isr_arg),
	void *   isr_arg,
	void     (*cleanup_txbuf)(void *user_buf, void *cleanup_txbuf_arg, int error_on_tx_occurred), 
	void *   cleanup_txbuf_arg,
	void *   (*alloc_rxbuf)(int *p_size, uintptr_t *p_data_addr),
	void     (*consume_rxbuf)(void *user_buf, void *consume_rxbuf_arg, int len),
	void *   consume_rxbuf_arg,
	int		 rx_ring_size,
	int		 tx_ring_size,
	int		 irq_mask
);


/*
 * Descriptor scavenger; cleanup the TX ring, passing all buffers
 * that have been sent to the cleanup_tx() callback.
 * This routine is called from BSP_tsec_send_buf(), BSP_tsec_init_hw(),
 * BSP_tsec_stop_hw().
 *
 * RETURNS: number of buffers processed.
 */

int
BSP_tsec_swipe_tx(struct tsec_private *mp);


/*
 * Reset statistics counters.
 */
void
BSP_tsec_reset_stats(struct tsec_private *mp);

/*
 * Initialize interface hardware
 *
 * 'mp'			handle obtained by from BSP_tsec_setup(). 
 * 'promisc'	whether to set promiscuous flag.
 * 'enaddr'		pointer to six bytes with MAC address. Read
 *				from the device if NULL.
 * NOTE:        multicast filter is cleared by this routine.
 */
void
BSP_tsec_init_hw(struct tsec_private *mp, int promisc, unsigned char *enaddr);

/*
 * Clear multicast hash filter. No multicast frames are accepted
 * after executing this routine (unless the hardware was initialized
 * in 'promiscuous' mode).
 *
 * Reset reference count for all hash-table entries
 * to zero (see BSP_tsec_mcast_filter_accept_del()).
 */
void
BSP_tsec_mcast_filter_clear(struct tsec_private *mp);

/*
 * Program multicast filter to accept all multicast frames.
 *
 * Increment reference count for all hash-table entries
 * by one (see BSP_tsec_mcast_filter_accept_del()).
 */
void
BSP_tsec_mcast_filter_accept_all(struct tsec_private *mp);

/*
 * Add a MAC address to the multicast filter and increment
 * the reference count for the matching hash-table entry
 * (see BSP_tsec_mcast_filter_accept_del()).
 * 
 * Existing entries are not changed but note that
 * the filter is imperfect, i.e., multiple MAC addresses
 * may alias to a single filter entry. Hence software
 * filtering must still be performed.
 *
 */
void
BSP_tsec_mcast_filter_accept_add(struct tsec_private *mp, unsigned char *enaddr);

/*
 * Remove a MAC address from the (imperfec) multicast
 * filter.
 * Note that the driver maintains an internal reference
 * counter for each multicast hash. The hash-table
 * entry is only cleared when the reference count
 * reaches zero ('del' has been called the same
 * amount of times as 'add' for an address (or
 * any alias) that matches a given table entry.
 * BSP_tsec_mcast_filter_clear() resets all reference
 * counters to zero.
 */
void
BSP_tsec_mcast_filter_accept_del(struct tsec_private *mp, unsigned char *enaddr);

/*
 * Dump statistics to FILE 'f'. If NULL, stdout is used.
 */
void
BSP_tsec_dump_stats(struct tsec_private *mp, FILE *f);

/*
 * Shutdown hardware and clean out the rings
 */
void
BSP_tsec_stop_hw(struct tsec_private *mp);

/*
 * calls BSP_tsec_stop_hw(), releases all resources and marks the interface
 * as unused.
 * RETURNS 0 on success, nonzero on failure.
 * NOTE:   the handle MUST NOT be used after successful execution of this
 *         routine.
 */
int
BSP_tsec_detach(struct tsec_private *mp);

/*
 * Enqueue a mbuf chain or a raw data buffer for transmission;
 * RETURN: #bytes sent or -1 if there are not enough free descriptors
 *
 * If 'len' is <=0 then 'm_head' is assumed to point to a mbuf chain.
 * OTOH, a raw data packet (or a different type of buffer)
 * may be sent (non-BSD driver) by pointing data_p to the start of
 * the data and passing 'len' > 0.
 * 'm_head' is passed back to the 'cleanup_txbuf()' callback.
 *
 * Comments: software cache-flushing incurs a penalty if the
 *           packet cannot be queued since it is flushed anyways.
 *           The algorithm is slightly more efficient in the normal
 *			 case, though.
 *
 * RETURNS: # bytes enqueued to device for transmission or -1 if no
 *          space in the TX ring was available.
 */

int
BSP_tsec_send_buf(struct tsec_private *mp, void *m_head, void *data_p, int len);

/*
 * Retrieve all received buffers from the RX ring, replacing them
 * by fresh ones (obtained from the alloc_rxbuf() callback). The
 * received buffers are passed to consume_rxbuf().
 *
 * RETURNS: number of buffers processed.
 */
int
BSP_tsec_swipe_rx(struct tsec_private *mp);

/* read ethernet address from hw to buffer */
void
BSP_tsec_read_eaddr(struct tsec_private *mp, unsigned char *eaddr);

/* Read MII register */
uint32_t
BSP_tsec_mdio_rd(struct tsec_private *mp, unsigned reg);

/* Write MII register */
int
BSP_tsec_mdio_wr(struct tsec_private *mp, unsigned reg, uint32_t val);

/*
 * read/write media word.
 *   'cmd': can be SIOCGIFMEDIA, SIOCSIFMEDIA, 0 or 1. The latter
 *          are aliased to the former for convenience.
 *  'parg': pointer to media word.
 *
 * RETURNS: 0 on success, nonzero on error
 */
int
BSP_tsec_media_ioctl(struct tsec_private *mp, int cmd, int *parg);

/* Interrupt related routines */

/*
 * When it comes to interrupts the chip has two rather
 * annoying features:
 *   1 once an IRQ is pending, clearing the IMASK does not
 *     de-assert the interrupt line.
 *   2 the chip has three physical interrupt lines even though
 *     all events are reported in a single register. Rather
 *     useless; we must hook 3 ISRs w/o any real benefit.
 *     In fact, it makes our life a bit more difficult:
 *
 * Hence, for (1) we would have to mask interrupts at the PIC
 * but to re-enable them we would have to do that three times
 * because of (2).
 *
 * Therefore, we take the following approach:
 *
 *   ISR masks all interrupts on the TSEC, acks/clears them
 *   and stores the acked irqs in the device struct where
 *   it is picked up by BSP_tsec_ack_irqs().
 *   Since all interrupts are disabled until the daemon
 *   re-enables them after calling BSP_tsec_ack_irqs()
 *   no interrupts are lost.
 *
 * BUT:  NO isr (including PHY isrs) MUST INTERRUPT ANY
 *       OTHER ONE, i.e., they all must have the same
 *       priority. Otherwise, integrity of the cached
 *       irq_pending variable may be compromised.
 */

/* Note: the BSP_tsec_enable/disable/ack_irqs() entry points
 *       are deprecated.
 *       The newer API where the user passes a mask allows
 *       for more selective control.
 */

/* Enable interrupts at device */
void
BSP_tsec_enable_irqs(struct tsec_private *mp);

/* Disable interrupts at device */
void
BSP_tsec_disable_irqs(struct tsec_private *mp);

/*
 * Acknowledge (and clear) interrupts.
 * RETURNS: interrupts that were raised.
 */
uint32_t
BSP_tsec_ack_irqs(struct tsec_private *mp);

/* Enable interrupts included in 'mask' (leaving
 * already enabled interrupts on). If the mask includes
 * bits that were not passed to the 'setup' routine then
 * the behavior is undefined.
 */
void
BSP_tsec_enable_irq_mask(struct tsec_private *mp, uint32_t irq_mask);

/* Disable interrupts included in 'mask' (leaving
 * other ones that are currently enabled on). If the mask
 * includes bits that were not passed to the 'setup' routine
 * then the behavior is undefined.

 * RETURNS: Bitmask of interrupts that were enabled upon entry
 *          into this routine. This can be used to restore the previous
 *          state.
 */
uint32_t
BSP_tsec_disable_irq_mask(struct tsec_private *mp, uint32_t irq_mask);

/* Acknowledge and clear selected interrupts.
 *
 * RETURNS: All pending interrupts.
 * 
 * NOTE:    Only pending interrupts contained in 'mask'
 *          are cleared. Others are left pending.
 *
 *          This routine can be used to check for pending
 *          interrupts (pass mask ==  0) or to clear all
 *          interrupts (pass mask == -1).
 */
uint32_t
BSP_tsec_ack_irq_mask(struct tsec_private *mp, uint32_t mask);


/* Retrieve the driver daemon TID that was passed to
 * BSP_tsec_setup().
 */

rtems_id
BSP_tsec_get_tid(struct tsec_private *mp);

struct tsec_private *
BSP_tsec_getp(unsigned index);

/* 
 *
 * Example driver task loop (note: no synchronization of
 * buffer access shown!).
 * RTEMS_EVENTx = 0,1 or 2 depending on IF unit.
 *
 *    / * setup (obtain handle) and initialize hw here * /
 *
 *    do {
 *      / * ISR disables IRQs and posts event * /
 *		rtems_event_receive( RTEMS_EVENTx, RTEMS_WAIT | RTEMS_EVENT_ANY, RTEMS_NO_TIMEOUT, &evs );
 *		irqs = BSP_tsec_ack_irqs(handle);
 *      if ( irqs & BSP_TSEC_IRQ_TX ) {
 *			BSP_tsec_swipe_tx(handle); / * cleanup_txbuf() callback executed * /
 *		}
 *      if ( irqs & BSP_TSEC_IRQ_RX ) {
 *			BSP_tsec_swipe_rx(handle); / * alloc_rxbuf() and consume_rxbuf() executed * /
 *		}
 *		BSP_tsec_enable_irqs(handle);
 *    } while (1);
 *
 */

/* PUBLIC RTEMS BSDNET ATTACH FUNCTION */
struct rtems_bsdnet_ifconfig;

int
rtems_tsec_attach(struct rtems_bsdnet_ifconfig *ifcfg, int attaching);

#ifdef __cplusplus
}
#endif

#endif
