#!/bin/bash

# generate filesystem
cd files
tar -cvf ../FileSystemImage --exclude CVS --exclude .cvsignore --exclude .svn .
cd ..
rtems-bin2c FileSystemImage FileSystemImage

