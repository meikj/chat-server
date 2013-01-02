#include <stdio.h>
#include <winsock.h>

#include "logger.h"

void cleanup(int sockets[], int socket_count) {
	if((socket_count == 0) || (sockets == NULL)) {
		// No need to cleanup any sockets
		
	}
	if(WSACleanup() == SOCKET_ERROR) {
		// WSACleanup() failed
		
	}
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
	
	log_entry("WSAStartup() succeeded", 0, LOGGER_DEBUG);
	
0	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {		// Some error occured with socket()
		log_entry("socket() failed", WSAGetLastError(), LOGGER_ERROR);
		WSACleanup();
		exit(1);
	}
	
	log_entry("socket() succeeded", l_socket, LOGGER_DEBUG);
	
	if(closesocket(l_socket) == SOCKET_ERROR) {
		// Closing socket resulted in an error
		log_entry("closesocket() failed", WSAGetLastError(), LOGGER_ERROR);
		WSACleanup();
		exit(1);
	}
	
	log_entry("closesocket() succeeded", 0, LOGGER_DEBUG);
	
	if(WSACleanup() == SOCKET_ERROR) {
		// Cleanup resulted in an error
		log_entry("WSACleanup() failed", WSAGetLastError(), LOGGER_ERROR);
		exit(1);
	}
	
	log_entry("WSACleanup() succeeded", 0, LOGGER_DEBUG);

	return 0;
}
