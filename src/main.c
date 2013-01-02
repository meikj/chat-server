/*
 * main.c
 *
 * Chat server programmed in C. This is done with educational purposes as an
 * aid to learn both C and network programming (at the moment Windows network
 * programming). There is no license attached with this software.
 *
 */

#include <stdio.h>
#include <winsock.h>

#include "logger.h"

/*
 * Used to clean up socket(s) and/or invoke WSACleanup()
 */
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

/*
 * Program is run as follows:
 *		<prog_name> <port_number>
 *
 * If arguments don't conform as such, then program will exit
 */
int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: %s <port_number>\n", argv[0]);
		exit(1);
	}
		
	WSADATA wsaData;
	int error;
	int l_socket;
	int l_port;
	
	error = WSAStartup(MAKEWORD(1, 1), &wsaData);
	l_port = argv[1];
	
	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		log_entry("main(): WSAStartup() failed", error, LOGGER_ERROR);
		exit(1);
	}
	
	log_entry("main(): WSAStartup() succeeded with WinSock 1.1", 0, LOGGER_DEBUG);
	
	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {		
		// Some error occured with socket()
		log_entry("main(): socket() failed", WSAGetLastError(), LOGGER_ERROR);
		cleanup(0); // No need to cleanup socket as it failed
		exit(1);
	}
	
	log_entry("main(): socket() succeeded", l_socket, LOGGER_DEBUG);
	
	cleanup(l_socket);

	return 0;
}
