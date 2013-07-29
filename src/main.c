/*
 * main.c
 *
 * Chat server programmed in C. This is done with educational purposes as an
 * aid to learn both C and network programming (at the moment Windows network
 * programming). There is no license attached with this software.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>

#include "logger.h"
#include "socket.h"

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 5000
#define BUFFER_LEN 512

/*
 * Handle an accepted client socket connection until it dies. This function
 * receives data from the client and outputs the number of bytes received.
 *
 * Params:
 *		s = Client socket descriptor
 */
void handle_client(int s) {
	char buf[BUFFER_LEN];
	int res;

	do {
		res = recv(s, buf, BUFFER_LEN, 0);

		if(res > 0) {
			log_entry(LOGGER_INFO, 0,
				"handle_client(%d): recv() = %d bytes", s, res);
		} else if(res == 0) {
			log_entry(LOGGER_INFO, 0,
				"handle_client(%d): connection closed", s);
		} else {
			log_entry(LOGGER_ERROR, WSAGetLastError(),
				"handle_client(%d): recv() failed", s);
		}
	} while(res > 0);
}

/*
 * Begins the main program loop of accepting incoming client connections. The
 * function takes a function pointer to a client socket handler function. This
 * function is executed when a client successfully connects.
 *
 * Params:
 *		l_socket	= The listener socket descriptor
 *		handler		= Function pointer to the client socket handler function
 */
void wait_for_client(int l_socket, void(*handler)(int)) {
	for(;;) {
		int c_socket;

		if((c_socket = accept(l_socket, NULL, NULL)) == INVALID_SOCKET) {
			log_entry(LOGGER_ERROR, WSAGetLastError(),
				"wait_for_client(): accept() failed");
			socket_cleanup(l_socket, 1);
			exit(1);
		}

		log_entry(LOGGER_INFO, 0,
			"wait_for_client(): accept() was successful: c_socket = %d", c_socket);
		log_entry(LOGGER_INFO, 0,
			"wait_for_client(): passing client on to handler...");
		handler(c_socket);
	}
}

/*
 * Initialise the server socket.
 *
 * Params:
 *	host = The host to listen on
 *	port = The port to listen on
 *
 * Returns:
 *	The listening socket descriptor
 */
int init_server_socket(char *host, int port) {
	int s;
	struct sockaddr_in addr;

	log_entry(LOGGER_DEBUG, 0, "init_server_socket(): s = socket_gen()");
	s = socket_gen();

	log_entry(LOGGER_DEBUG, 0,
		"init_server_socket(): addr = socket_init_addr()");
	addr = socket_init_addr(host, port);

	if((bind(s, (struct sockaddr *)&addr, sizeof addr)) == SOCKET_ERROR) {
		log_entry(LOGGER_ERROR, WSAGetLastError(),
			"init_server_socket(): bind() failed");
		socket_cleanup(s, 1);
		exit(1);
	}

	log_entry(LOGGER_INFO, 0,
		"init_server_socket(): bind(): host = %s, port = %i",
		host, port);

	if(listen(s, MAX_BACKLOG) == SOCKET_ERROR) {
		log_entry(LOGGER_ERROR, WSAGetLastError(),
			"init_server_socket(): listen() failed");
		socket_cleanup(s, 1);
		exit(1);
	}

	log_entry(LOGGER_INFO, 0,
		"init_server_socket(): listen(): socket = %d, backlog = %d",
		s, MAX_BACKLOG);

	return s;
}

/*
 * The following arguments are accepted:
 *	host = The host to listen on
 *	port = The port number to listen on
 *
 * Example usage: ./chat-server.exe 127.0.0.1 5000
 *
 * If no arguments are passed, the default values from DEFAULT_HOST and
 * DEFAULT_HOST are used
 */
int main(int argc, char *argv[]) {
	int l_socket;
	char *l_host;
	int l_port;

	if(argc >= 3) {
		l_host = argv[1];
		l_port = atoi(argv[2]);
	} else {
		l_host = DEFAULT_HOST;
		l_port = DEFAULT_PORT;
	}

	// Do some primitive port checking
	if(l_port == 0) {
		log_entry(LOGGER_ERROR, 0, "main(): invalid port specified");
		socket_cleanup(l_socket, 1);
		exit(1);
	}

	// Output address and port for debugging purposes
	log_entry(LOGGER_DEBUG, 0, "main(): l_host = %s", l_host);
	log_entry(LOGGER_DEBUG, 0, "main(): l_port = %d", l_port);

	log_entry(LOGGER_DEBUG, 0, "main(): l_socket = init_server_socket()");
	l_socket = init_server_socket(l_host, l_port);

	log_entry(LOGGER_INFO, 0, "main(): wait_for_client() loop initiated");
	wait_for_client(l_socket, &handle_client);

	// Perform final clean up code
	socket_cleanup(l_socket, 1);

	return 0;
}
