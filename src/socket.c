/*
 * socket.h
 *
 * Contains the relevant socket code for the chat server.
 *
 */

#include <stdlib.h>
#include <winsock.h>

#include "logger.h"
#include "socket.h"

 /*
 * Used to clean up socket(s) and/or invoke WSACleanup()
 *
 * Params:
 *	socket		= Socket descriptor
 *	wsa_socket_cleanup	= Invoke WSACleanup() or not (0 = false, 1 = true)
 *
 */
void socket_cleanup(int socket, int wsa_cleanup) {
	if(socket != 0) {
		// Socket needs to be closed
		if(closesocket(socket) == SOCKET_ERROR) {
			log_entry(LOGGER_ERROR, WSAGetLastError(),
				"socket_cleanup(): closesocket(%d) failed", socket);
			exit(1);
		}
	}
	if(wsa_cleanup != 0) {
		if(WSACleanup() == SOCKET_ERROR) {
			// WSACleanup() failed
			log_entry(LOGGER_ERROR, WSAGetLastError(),
				"socket_cleanup(): WSACleanup() failed");
			exit(1);
		}
	}

	log_entry(LOGGER_DEBUG, 0, "socket_cleanup(%d): success", socket);
}

/*
 * Initialise a server socket address structure.
 *
 * Params:
 *	host = Host name/IP address of computer to host on
 *	port = Port number to host on
 *
 * Returns:
 *	The newly initialised server socket address structure
 *	that conforms to the passed arguments
 *
 */
struct sockaddr_in socket_init_addr(const char *host, int port) {
	struct sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(host);
	server.sin_port = htons(port);

	log_entry(LOGGER_DEBUG, 0,
		"socket_init_addr(): server structure created");

	return server;
}

/*
 * Generate a socket descriptor that is of type TCP and IPv4
 *
 * Returns:
 *	The newly generated socket descriptor
 *
 */
int socket_gen() {
	WSADATA wsaData;
	int error;
	int l_socket;

	error = WSAStartup(MAKEWORD(WINSOCK_MAJOR, WINSOCK_MINOR), &wsaData);

	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		log_entry(LOGGER_ERROR, error, "socket_gen(): WSAStartup() failed");
		exit(1);
	}

	// Check if WinSock version conforms to the requested version
	if(LOBYTE(wsaData.wVersion) != WINSOCK_MAJOR || HIBYTE(wsaData.wVersion) != WINSOCK_MINOR) {
		// Version wasn't found, therefore library isn't compatible
		log_entry(LOGGER_ERROR, 0,
			"socket_gen(): WinSock lib requested wasn't found: requested = %d.%d, received = %d.%d",
			WINSOCK_MAJOR, WINSOCK_MINOR,
			LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

		socket_cleanup(0, 1);
		exit(1);
	}

	log_entry(LOGGER_DEBUG, 0,
		"socket_gen(): WSAStartup(): WinSock %d.%d",
		WINSOCK_MAJOR, WINSOCK_MINOR);

	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		// Some error occured with socket()
		log_entry(LOGGER_ERROR, WSAGetLastError(), "socket_gen(): socket() failed");
		socket_cleanup(0, 1); // No need to socket_cleanup socket as it failed
		exit(1);
	}

	log_entry(LOGGER_DEBUG, l_socket, "socket_gen(): socket()");

	return l_socket;
}
