#!/bin/sh

TMP=../../rtems/boot/isofiles
BASE=../../rtems/boot

mkisofs -f -G $TMP/boot/isofs.b -B ... -iso-level 3 -r -o $BASE/image.iso $TMP
ls -l image.iso