#!/bin/bash

DIRECTORY="/home/jaison/CS7610/projects/project2/part1"
KILL_TCP_PROCESS_COMMAND="netstat -plunt | grep ./prj2 | awk {'print $7'} | cut -d'/' -f 1 | xargs kill"
COMMAND_RUN="make --directory=$DIRECTORY run"

echo "vdi-linux-030.ccs.neu.edu" >> hostnames.txt
echo "vdi-linux-031.ccs.neu.edu" >> hostnames.txt

scp ~/Documents/masters/CS7610/projects/project2/part1/scripts/hostnames.txt jaison@login.ccs.neu.edu:/home/jaison/CS7610/projects/project2/part1/

for host in $(cat hostnames.txt); do
	ssh $host "$COMMAND_RUN" &
	sleep 2
done
wait

