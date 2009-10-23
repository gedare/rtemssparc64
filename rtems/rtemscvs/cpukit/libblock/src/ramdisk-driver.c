/**
 * @file
 *
 * @ingroup rtems_ramdisk
 *
 * @brief RAM disk block device implementation.
 */

/*
 * Copyright (C) 2001 OKTET Ltd., St.-Petersburg, Russia
 * Author: Victor V. Vengerov <vvv@oktet.ru>
 *
 * @(#) $Id: ramdisk-driver.c,v 1.1 2009/10/16 08:44:51 thomas Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include <rtems.h>
#include <rtems/ramdisk.h>

static void
rtems_ramdisk_printf (const ramdisk *rd, const char *format, ...)
{
  if (rd->trace)
  {
    va_list args;
    va_start (args, format);
    printf ("ramdisk:");
    vprintf (format, args);
    printf ("\n");
  }
}

static int
ramdisk_read(struct ramdisk *rd, rtems_blkdev_request *req)
{
    uint8_t *from = rd->area;
    uint32_t   i;
    rtems_blkdev_sg_buffer *sg;

    rtems_ramdisk_printf (rd, "ramdisk read: start=%d, blocks=%d",
                          req->bufs[0].block, req->bufnum);

    for (i = 0, sg = req->bufs; i < req->bufnum; i++, sg++)
    {
        rtems_ramdisk_printf (rd, "ramdisk read: buf=%d block=%d length=%d off=%d addr=%p",
                              i, sg->block, sg->length, sg->block * rd->block_size,
                              from + (sg->block * rd->block_size));
        memcpy(sg->buffer, from + (sg->block * rd->block_size), sg->length);
    }
    req->req_done(req->done_arg, RTEMS_SUCCESSFUL, 0);
    return 0;
}

static int
ramdisk_write(struct ramdisk *rd, rtems_blkdev_request *req)
{
    uint8_t *to = rd->area;
    uint32_t   i;
    rtems_blkdev_sg_buffer *sg;

    rtems_ramdisk_printf (rd, "ramdisk write: start=%d, blocks=%d",
                          req->bufs[0].block, req->bufnum);
    for (i = 0, sg = req->bufs; i < req->bufnum; i++, sg++)
    {
        rtems_ramdisk_printf (rd, "ramdisk write: buf=%d block=%d length=%d off=%d addr=%p",
                              i, sg->block, sg->length, sg->block * rd->block_size,
                              to + (sg->block * rd->block_size));
        memcpy(to + (sg->block * rd->block_size), sg->buffer, sg->length);
    }
    req->req_done(req->done_arg, RTEMS_SUCCESSFUL, 0);
    return 0;
}

int
ramdisk_ioctl(rtems_disk_device *dd, uint32_t req, void *argp)
{
    switch (req)
    {
        case RTEMS_BLKIO_REQUEST:
        {
            rtems_blkdev_request *r = argp;
            struct ramdisk *rd = rtems_disk_driver_data(dd);

            switch (r->req)
            {
                case RTEMS_BLKDEV_REQ_READ:
                    return ramdisk_read(rd, r);

                case RTEMS_BLKDEV_REQ_WRITE:
                    return ramdisk_write(rd, r);

                default:
                    errno = EINVAL;
                    return -1;
            }
            break;
        }
  
        default:
            return rtems_blkdev_ioctl (dd, req, argp);
            break;
    }

    errno = EINVAL;
    return -1;
}
