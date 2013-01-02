#include <stdio.h>
#include <winsock.h>

#include "logger.h"

void cleanup(int socket) {
	if(socket != 0) {
		// Socket needs to be closed
		if(closesocket(socket) == SOCKET_ERROR) {
			log_entry("closesocket() failed", WSAGetLastError(), LOGGER_ERROR);
			exit(1);
		}
	}
	if(WSACleanup() == SOCKET_ERROR) {
		// WSACleanup() failed
		log_entry("WSACleanup() failed", WSAGetLastError(), LOGGER_ERROR);
		exit(1);
	}
	
	log_entry("cleanup() succeeded", 0, LOGGER_DEBUG);
}

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
	
	log_entry("WSAStartup() succeeded with WinSock 1.1", 0, LOGGER_DEBUG);
	
	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {		
		// Some error occured with socket()
		log_entry("socket() failed", WSAGetLastError(), LOGGER_ERROR);
		cleanup(0); // No need to cleanup socket as it failed
		exit(1);
	}
	
	log_entry("socket() succeeded", l_socket, LOGGER_DEBUG);
	
	cleanup(l_socket);

	return 0;
}
