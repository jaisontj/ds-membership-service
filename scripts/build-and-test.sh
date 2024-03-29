#!/bin/bash

DIRECTORY="ds_prj2_part1"
NUM_PEERS=10

if [ -z $1 ]
then
	echo "No directory provided for remote. Defaulting to $DIRECTORY"
else
	DIRECTORY=$1
fi

echo "Step 1: Copying and building project at $DIRECTORY"
sh build.sh $DIRECTORY
clear

echo "Step 2: Creating a hostnames.txt file in the current directory. Number of peers: $NUM_PEERS"
rm -f hostnames.txt
for (( i=0; i<$NUM_PEERS; i++ ))
do
	echo "vdi-linux-03$i.ccs.neu.edu" >> hostnames.txt
done
clear

echo "Step 2.1: Ensuring authenticity is verified at all hosts"
for host in $(cat hostnames.txt);
do
	echo "Trying to SSH into $host"
	ssh $host 'sleep 1 >/dev/null &'
done

echo "Step 3: Copying hostnames.txt into remote machine at directory: $DIRECTORY"
scp hostnames.txt login.ccs.neu.edu:$DIRECTORY
clear

echo "Step 4: Running test case"
sh test1.sh $DIRECTORY
echo $STEP_LINES
clear

echo "Step 5: Printing output\n\n"
sleep 2
for file_name in $(ls log)
do
	echo "Content from $file_name\n"
	cat log/$file_name
	echo "\n\n\n"
done

echo "Step 5: Cleaning up"
current_date_time="`date +%Y%m%d%H%M%S`";
mv log/ log.$current_date_time
rm hostnames.txt

