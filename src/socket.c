/*
 * socket.h
 *
 * Contains the relevant socket code for the chat server.
 *
 */

#include <winsock.h>
 
#include "logger.h"
 
 /*
 * Used to clean up socket(s) and/or invoke WSACleanup()
 *
 * Params:
 *	socket		= Socket descriptor
 *	wsa_cleanup	= Invoke WSACleanup() or not (0 = false, 1 = true)
 */
void cleanup(int socket, int wsa_cleanup) {
	if(socket != 0) {
		// Socket needs to be closed
		if(closesocket(socket) == SOCKET_ERROR) {
			log_entry(LOGGER_ERROR, WSAGetLastError(), "closesocket() failed");
			exit(1);
		}
	}
	if(wsa_cleanup != 0) {
		if(WSACleanup() == SOCKET_ERROR) {
			// WSACleanup() failed
			log_entry(LOGGER_ERROR, WSAGetLastError(), "WSACleanup() failed");
			exit(1);
		}
	}
	
	log_entry(LOGGER_DEBUG, 0, "cleanup() succeeded");
}

int init_socket() {
	WSADATA wsaData;
	int error;
	int l_socket;
	
	error = WSAStartup(MAKEWORD(1, 1), &wsaData);
	
	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		log_entry(LOGGER_ERROR, error, "main(): WSAStartup() failed");
		exit(1);
	}
	
	log_entry(LOGGER_DEBUG, 0, "main(): WSAStartup() succeeded with WinSock 1.1");
	
	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {		
		// Some error occured with socket()
		log_entry(LOGGER_ERROR, WSAGetLastError(), "main(): socket() failed");
		cleanup(0, 1); // No need to cleanup socket as it failed
		exit(1);
	}
	
	log_entry(LOGGER_DEBUG, l_socket, "main(): socket() succeeded");
	
	return l_socket;
}
