#!/bin/bash

DIRECTORY=$1
HOSTNAMES="hostnames.txt"
PORT="35808"
KILL_TCP_PROCESS_COMMAND="netstat -plunt | grep ./prj2 | awk {'print \$7'} | cut -d'/' -f 1 | xargs kill"

COMMAND_RUN="./prj2 -p $PORT -h $HOSTNAMES"
COMMAND_RUN_T4="./prj2 -p $PORT -h $HOSTNAMES -t 4"
HOSTS=0
echo "Outputs will be in the log directory"
rm -rf log
mkdir -p log

echo "Command to be run: $COMMAND_RUN"
for host in $(cat $HOSTNAMES);
do
	echo "Running on $host"
	((HOSTS++))
	if [ "$HOSTS" == "1" ]
	then
		echo "Running Test case 4 on $host"
		ssh $host "cd $DIRECTORY && $COMMAND_RUN_T4" >> log/output.$host &
	else
		ssh $host "cd $DIRECTORY && $COMMAND_RUN" >> log/output.$host &
	fi
	sleep 3
done
clear

echo "Starting test case 4: Crashing Peer 5"
PROCESS="vdi-linux-034.ccs.neu.edu"
echo "Killing process at $PROCESS"
ssh $PROCESS $KILL_TCP_PROCESS_COMMAND
echo "Expectation, Peer 1 will crash after sending DEL to P3, P4. Peer 2 will assume leadership and execute the rest"
sleep 15
clear

for host in $(cat $HOSTNAMES)
do
	echo "Going to start killing the remaining processes"
	echo "Killing process at $host"
	ssh $host $KILL_TCP_PROCESS_COMMAND
	clear
done

wait

