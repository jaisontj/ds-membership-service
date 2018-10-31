# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
CFLAGS  = -std=c++11 -Wall -pthread

TARGET = prj2
MAIN = main.cpp

UTILS_FILES = utils/Log.cpp utils/SocketImpl.cpp utils/TcpListener.cpp utils/TcpSender.cpp utils/UdpSender.cpp utils/UdpListener.cpp

COMPONENTS = components/CommandArgParser.cpp components/MembershipList.cpp components/ReqMessageOkCollecter.cpp components/ProcessInfoHelper.cpp components/MessageHandler.cpp components/MessageSender.cpp components/RequestIdProvider.cpp components/FailureDetector.cpp

buildccs: clean build

build: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) $(UTILS_FILES) $(COMPONENTS) $(MAIN) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) -p $(PORT) -h $(HOSTFILE) -v verbose

clean:
	rm -f $(TARGET)
	rm -f *.txt
