# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
CFLAGS  = -std=c++11 -Wall -pthread

TARGET = prj2
MAIN = main.cpp
PORT = 35808
HOSTFILE = hostnames.txt

UTILS_FILES = utils/Log.cpp utils/SocketImpl.cpp utils/TcpListener.cpp utils/TcpSender.cpp

COMPONENTS = components/CommandArgParser.cpp components/MembershipList.cpp components/ReqMessageOkCollecter.cpp components/ProcessInfoHelper.cpp components/MessageHandler.cpp components/MessageSender.cpp components/RequestIdProvider.cpp

buildccs: clean build

build: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) $(UTILS_FILES) $(COMPONENTS) $(MAIN) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) -p $(PORT) -h $(HOSTFILE) -v info


generate:
	echo "vdi-linux-030.ccs.neu.edu" >> hostnames.txt
	echo "vdi-linux-031.ccs.neu.edu" >> hostnames.txt
	echo "vdi-linux-032.ccs.neu.edu" >> hostnames.txt
	echo "vdi-linux-033.ccs.neu.edu" >> hostnames.txt
	echo "vdi-linux-034.ccs.neu.edu" >> hostnames.txt

clean:
	rm -f $(TARGET)
	rm -f *.txt
