/*
 * server.c
 *
 * Contains functionality for starting up the server.
 *
 */


// TODO: Clean this fucking mess up

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "server.h"

#ifdef _WIN32
WSADATA wsaData;
#endif

int serverfd = 0; // Server socket descriptor
struct addrinfo *serverinfo; // Server address structure

/*
 * Initialises the required server startup code.

 * Returns:
 *	If the function was successful 0 is returned, otherwise -1 is returned.
 */
int server_init() {
#ifdef _WIN32
	// Windows specific WSAStartup code
	int error;

	error = WSAStartup(MAKEWORD(WINSOCK_MAJOR, WINSOCK_MINOR), &wsaData);

	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		log_error("socket_init(): WSAStartup() failed: %d\n", error);
		return -1;
	}
#endif

	return 0;
}

/*
 * Clean up server resources.
 */
void server_cleanup() {
	if(serverinfo != NULL)
		freeaddrinfo(serverinfo);

	close(serverfd);
#ifdef _WIN32
	WSACleanup();
#endif
}

/*
 * Initialise the server address structure.
 *
 * Params:
 *	port     = The port number to host on
 *
 * Returns:
 *	If the function was successful 0 is returned, otherwise -1 is returned.
 */
int server_init_addr(char *port) {
	int status;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((status = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0) {
		log_error("server_init_addr(): getaddrinfo() error: %s\n",
			gai_strerror(status));
		return -1;
	}

	return 0;
}

/*
 * Initialise the server socket.
 *
 * Returns:
 *	The server socket descriptor, or -1 upon error.
 */
int server_init_socket() {
	struct addrinfo *p;

	for(p = serverinfo; p != NULL; p = p->ai_next) {
		log_info("server_init_socket(): Attempting socket creation...\n",
			NULL);
		if((serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET) {
			// Some error occured with socket()
			log_error("server_init_socket(): socket() failed: %d\n",
				errno);
			close(serverfd);
			continue;
		}

		if((bind(serverfd, p->ai_addr, p->ai_addrlen)) == SOCKET_ERROR) {
			log_error("server_init_socket(): bind() failed: %d\n",
				errno);
			close(serverfd);
			continue;
		}

		if(listen(serverfd, MAX_BACKLOG) == SOCKET_ERROR) {
			log_error("server_init_socket(): listen() failed: %d\n",
				errno);
			close(serverfd);
			continue;
		}

		// If we reach this point then socket creation was a success
		break;
	}

	if(p == NULL) {
		// If we reach this point when p is NULL, then address is invalid
		log_error("server_init_socket(): servinfo contains no valid address\n",
			NULL);
		return -1;
	}

	return serverfd;
}
