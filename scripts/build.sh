#!/bin/bash

DIRECTORY="/home/jaison/CS7610/projects/project2/part1"
BUILD="make buildccs"

rm -f hostnames.txt

rsync -av --exclude='.git/' --exclude='*.swp' ~/Documents/masters/CS7610/projects/project2 jaison@login.ccs.neu.edu:/home/jaison/CS7610/projects/

ssh "jaison@login.ccs.neu.edu" "cd $DIRECTORY && $BUILD";


