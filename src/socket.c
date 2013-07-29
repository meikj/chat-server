/*
 * socket.h
 *
 * Contains functions for socket management.
 *
 */

#include <stdlib.h>
#include <winsock.h>

#include "logger.h"
#include "socket.h"

/*
 * Initialises the required socket startup code, including WSAStartup().

 * Returns:
 *	If the function is successful 0 is returned, otherwise -1 is returned upon
 *	error.
 */
int socket_init() {
	WSADATA wsaData;
	int error;

	error = WSAStartup(MAKEWORD(WINSOCK_MAJOR, WINSOCK_MINOR), &wsaData);

	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		log_error("socket_init(): WSAStartup() failed: %d\n", error);
		return -1;
	}

	// Check if WinSock version conforms to the requested version
	if(LOBYTE(wsaData.wVersion) != WINSOCK_MAJOR || HIBYTE(wsaData.wVersion) != WINSOCK_MINOR) {
		// Version wasn't found, therefore library isn't compatible
		log_error("socket_init(): WinSck: requested = %d.%d, received = %d.%d",
			WINSOCK_MAJOR, WINSOCK_MINOR,
			LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

		WSACleanup();
		return -1;
	}

	return 0;
}

/*
 * Initialise a server socket address structure.
 *
 * Params:
 *	host = Host name/IP address of computer to host on
 *	port = Port number to host on
 *
 * Returns:
 *	The socket address structure.
 */
struct sockaddr_in socket_init_addr(const char *host, int port) {
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host);
	addr.sin_port = htons(port);

	return addr;
}

/*
 * Initialise a server socket.
 *
 * Params:
 *	addr = The socket address structure
 *
 * Returns:
 *	The server socket descriptor, or -1 upon error.
 */
int socket_init_server(struct sockaddr_in addr) {
	int s;

	if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		// Some error occured with socket()
		log_error("socket_init_server(): socket() failed: %d",
			WSAGetLastError());
		closesocket(s);
		return -1;
	}

	if((bind(s, (struct sockaddr *)&addr, sizeof addr)) == SOCKET_ERROR) {
		log_error("socket_init_server: bind() failed: %d",
			WSAGetLastError());
		closesocket(s);
		return -1;
	}

	if(listen(s, MAX_BACKLOG) == SOCKET_ERROR) {
		log_error("socket_init_server(): listen() failed: %d",
			WSAGetLastError());
		closesocket(s);
		return -1;
	}

	return s;
}
