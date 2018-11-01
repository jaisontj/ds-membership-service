NAME: JAISON JOHN TITUS

NOTE: The scripts are contingent on the fact that the public key(id_rsa.pub) is present in the remote machines. Otherwise you will be prompted for a password and the scripts will not work.

Running Each Part:

- Navigate into part*/scripts
- Run "sh build-and-test.sh"
- Each part will run its own respective test case
- The output from each peer will be printed on the console and also will be present in the log.[TIMESTAMP] directory


More information about the script:

- The above script will ssh into "login@ccs.neu.edu", copy all the contents of the respective part and paste it into a directory "ds_prj_part*" in the home directory.
- It then will create a hostname.txt file with 5 peers (030...034)
- It will ssh into all of the provided machines and try to kill any remenant processes from previous runs. This also serves as a way to get rid of the authenticity verification prompt later on.
- It then copies the hostname.txt file it created into the remote machine at "login@ccs.neu.edu"
- It finally executes the "test*.sh" script which will run the respective testcase.
- The logs from the testcase are saved inside of the log/ directory within files of format "output.[HOSTNAME]".
- The script copies these logs for each peer and prints it on the screen.
- It then removes the hostnames.txt file and renames the log/ directory to log.[CURRENT_TIMESTAMP]

All tests run on port 35808 and are on 5 peers. Changing this number will require change to the SLEEPS for each test. 
As this program DOES NOT (intentionally) handle more than one saved operation for a peer. In these scenarios the peer WILL crash(deterministic) to avoid unexpected behaviour.

LOGS:

Informative logs are printed as required by the project. For more logs, add the flag "-v" which can take "verbose" "debug" or "info" as arguments and prints logs.

TESTCASE 1:
- Brings up the processes from the hostnames.txt file one by one with a sleep interval of 3
- After all the processes have come up, it kills the processes one by one.

TESTCASE 2:
- Brings up the processes from the hostnames.txt file one by one with a sleep interval of 3
- Kills peer 3 and waits for 6 seconds.
- Starts killing off the other processes one by one.

TESTCASE 3:
- Brings up the processes from the hostnames.txt file one by one with a sleep interval of 3
- Starts killing off the other processes one by one with a 10 second interval in between them. The interval is given for the leader to perform 2pc with the alive peers.

TESTCASE 4:
For part 4, the run command accepts another flag "-t 4". Any process run with this command (and if they're the leader) will crash 
after a peer crashes. More specifically, the peer will crash after sending OP DEL to every peer except the next leader.
- Brings up the processes from the hostnames with a sleep interval of 3. The first process is considered the leader and provided 
the "-t 4" argument when running.
- Kills process 5 and sleeps for 15 seconds. This is done to give enough time for the leader to detect crash, initiate 2pc, crash and the new leader to reconsile.
- Starts killing off all the processes one by one.

