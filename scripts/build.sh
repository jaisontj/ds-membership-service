#!/bin/bash

DIRECTORY=$1
BUILD="make buildccs"
rsync -av --exclude='.git/' --exclude='*.swp' ../*  login.ccs.neu.edu:$DIRECTORY
ssh "login.ccs.neu.edu" << SSHCOMMANDS
	cd $DIRECTORY
	make buildccs
SSHCOMMANDS


