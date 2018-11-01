#!/bin/bash

DIRECTORY=$1
HOSTNAMES="hostnames.txt"
PORT="35808"
KILL_TCP_PROCESS_COMMAND="netstat -plunt | grep ./prj2 | awk {'print \$7'} | cut -d'/' -f 1 | xargs kill"

COMMAND_RUN="./prj2 -p $PORT -h $HOSTNAMES"

echo "Outputs will be in the log directory"
rm -rf log
mkdir -p log

echo "Command to be run: $COMMAND_RUN"
for host in $(cat $HOSTNAMES);
do
	echo "Running on $host"
	ssh $host "cd $DIRECTORY && $COMMAND_RUN" >> log/output.$host &
	sleep 3
done

echo "Going to start killing the processes"
for host in $(cat $HOSTNAMES);
do
	echo "Deleting process at $host"
	echo $KILL_TCP_PROCESS_COMMAND
	ssh $host $KILL_TCP_PROCESS_COMMAND
done

wait

