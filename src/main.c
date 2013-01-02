#include <stdio.h>
#include <winsock.h>

#include "logger.h"

int main(void) {
	WSADATA wsaData;
	int error;
	
	int l_socket;
	
	error = WSAStartup(MAKEWORD(1, 1), &wsaData);
	
	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		log_entry("WSAStartup() failed", error, LOGGER_ERROR);
		exit(1);
	}
	
	log_entry("WSAStartup() succeeded", 0, LOGGER_DEBUG);
	
	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		// Some error occured with socket()
		log_entry("socket() failed", WSAGetLastError(), LOGGER_ERROR);
		exit(1);
	}
	
	log_entry("socket() succeeded", l_socket, LOGGER_DEBUG);

	return 0;
}
