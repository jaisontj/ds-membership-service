#!/bin/bash

PART=4
DIRECTORY="/home/jaison/CS7610/projects/project2/part$PART"
BUILD="make buildccs"

rsync -av --exclude='.git/' --exclude='*.swp' ~/Documents/masters/CS7610/projects/project2/ jaison@login.ccs.neu.edu:$DIRECTORY

ssh "jaison@login.ccs.neu.edu" "cd $DIRECTORY && $BUILD";


