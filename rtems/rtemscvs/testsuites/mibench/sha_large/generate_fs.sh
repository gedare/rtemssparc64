#!/bin/bash

# generate filesystem
cd files
tar -cvf ../FileSystemImage --exclude CVS --exclude .cvsignore input_large.asc
cd ..
rtems-bin2c FileSystemImage FileSystemImage

