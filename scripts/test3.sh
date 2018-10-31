#!/bin/bash

DIRECTORY=$1
HOSTNAMES="hostnames.txt"
PORT="35808"
KILL_TCP_PROCESS_COMMAND="netstat -plunt | grep ./prj2 | awk {'print \$7'} | cut -d'/' -f 1 | xargs kill"

COMMAND_RUN="./prj2 -p $PORT -h $HOSTNAMES -v info"
HOSTS=0
echo "Outputs will be in the log directory"
rm -rf log
mkdir -p log

echo "Command to be run: $COMMAND_RUN"
for host in $(cat $HOSTNAMES);
do
	echo "Running on $host"
	((HOSTS++))
	ssh $host "cd $DIRECTORY && $COMMAND_RUN" >> log/output.$host &
	sleep 3
done
clear

echo "Starting test case 3: "
for (( i = $HOSTS - 1; i > -1; i-- ))
do
	echo "Going to start killing the processes"
	PROCESS="vdi-linux-03$i.ccs.neu.edu"
	echo "Killing process at $PROCESS"
	ssh $PROCESS $KILL_TCP_PROCESS_COMMAND
	sleep 10
	clear
done

wait

