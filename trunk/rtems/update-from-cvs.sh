#!/bin/bash

cd rtemscvs
./bootstrap -c
svn up
svn st
cvs up -Pd 2>&1 | grep -v ^cvs | grep ^C
cd ..

echo "Fix any conflicts, then run commit-to-svn.sh"

