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
	echo ""
	sleep 3
done

PEER3="vdi-linux-032.ccs.neu.edu"
echo "Killing peer 3"
ssh $PEER3 $KILL_TCP_PROCESS_COMMAND
sleep 6

clear
for host in $(cat $HOSTNAMES);
do
	echo "Peers should have detected peer 3 failure.Going to start killing the processes"
	echo "Killing process at $host"
	echo $KILL_TCP_PROCESS_COMMAND
	ssh $host $KILL_TCP_PROCESS_COMMAND
	clear
done

wait

