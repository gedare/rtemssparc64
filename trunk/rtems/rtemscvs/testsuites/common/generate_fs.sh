#!/bin/bash

#TOTAL=456710
TOTAL=496710
PERM=`ls -lt files/cont* | awk '{tot += $5} END {tot=tot ; printf("%4.2d\n",tot)}'`

#PAD=`expr $TOTAL-$PERM`
let PAD=$TOTAL-$PERM
#echo $PAD
dd if=/dev/zero of=files/padding.dat  bs=1  count=$PAD

# generate filesystem
cd files
tar -cvf ../FileSystemImage --exclude CVS --exclude .cvsignore --exclude .svn .
cd ..
rtems-bin2c FileSystemImage FileSystemImage
