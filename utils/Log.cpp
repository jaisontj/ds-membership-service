#include "Log.h"
#include <string>
#include <iostream>

void Log::v(std::string message) {
	if (LOG_LEVEL == VERBOSE)
		std::cout<<message<<std::endl;
}

void Log::d(std::string message) {
	if (LOG_LEVEL == DEBUG || LOG_LEVEL == VERBOSE)
		std::cout<<message<<std::endl;
}

void Log::e(std::string message) {
	if (LOG_LEVEL != NONE) {
		std::cout<<"######################################################################"<<std::endl;
		std::cout<<message<<std::endl;
		std::cout<<"######################################################################"<<std::endl;
	}
}

void Log::i(std::string message) {
	if (LOG_LEVEL == INFO || LOG_LEVEL == VERBOSE || LOG_LEVEL == DEBUG) {
		std::cout<<message<<std::endl;
	}
}

void Log::p(std::string message) {
	std::cout<<message<<std::endl;
}

void Log::f(std::string message) {
	std::cout<<"FATAL ERROR: "<<message<<std::endl;
	std::cout<<"Exiting Program."<<std::endl;
	exit(1);
}
