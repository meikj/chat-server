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

#define BUFFER_LEN 512

/*
 * Handle an accepted client socket connection until it dies.
 *
 * Params:
 *		s = Client socket descriptor
 */
void handle_socket(int s) {
	char buf[BUFFER_LEN];
	int res;

	do {
		res = recv(s, buf, BUFFER_LEN, 0);

		if(res > 0) {
			log_entry(LOGGER_INFO, 0,
				"handle_socket(%d): recv() = %d bytes", s, res);
		} else if(res == 0) {
			log_entry(LOGGER_INFO, 0,
				"handle_socket(%d): connection closed", s);
		} else {
			log_entry(LOGGER_ERROR, WSAGetLastError(),
				"handle_socket(%d): recv() failed", s);
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
	int c_socket;

	if((c_socket = accept(l_socket, NULL, NULL)) == INVALID_SOCKET) {
		log_entry(LOGGER_ERROR, WSAGetLastError(),
			"wait_for_client(): accept() failed");
		cleanup(l_socket, 1);
		exit(1);
	}

	log_entry(LOGGER_INFO, 0,
		"wait_for_client(): accept() was successful: c_socket = %d", c_socket);
	handler(c_socket);
}

/*
 * The following arguments are accepted:
 *		host - The host to listen on
 *		port - The port number to listen on
 *
 * Example usage: ./chat-server.exe 127.0.0.1 5000
 */
int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("Usage: %s <host> <port_number>\n", argv[0]);
		exit(1);
	}

	// Listen socket variables
	int l_socket;
	char *l_host;
	int l_port;
	struct sockaddr_in l_addr;

	// Client socket variables
	int c_socket;

	l_socket = init_socket();
	l_host = argv[1];
	l_port = atoi(argv[2]);

	// Do some primitive port checking
	if(l_port == 0) {
		log_entry(LOGGER_ERROR, 0, "main(): invalid port specified");
		cleanup(l_socket, 1);
		exit(1);
	}

	// Output address and port for debugging purposes
	log_entry(LOGGER_DEBUG, 0, "main(): l_host = %s", l_host);
	log_entry(LOGGER_DEBUG, 0, "main(): l_port = %d", l_port);

	// Create address structure and bind socket
	l_addr = init_server_addr(l_host, l_port);

	if((bind(l_socket, (struct sockaddr *)&l_addr, sizeof l_addr)) == SOCKET_ERROR) {
		log_entry(LOGGER_ERROR, WSAGetLastError(), "main(): bind() failed");
		cleanup(l_socket, 1);
		exit(1);
	}

	log_entry(LOGGER_DEBUG, 0, "main(): bind() was successful: address = %s, port = %i",
		l_host, l_port);

	if(listen(l_socket, MAX_BACKLOG) == SOCKET_ERROR) {
		log_entry(LOGGER_ERROR, WSAGetLastError(), "main(): listen() failed");
		cleanup(l_socket, 1);
		exit(1);
	}

	log_entry(LOGGER_DEBUG, 0, "main(): listen() was successful: socket = %d, backlog = %d",
		l_socket, MAX_BACKLOG);

	log_entry(LOGGER_INFO, 0, "main(): wait_for_client() loop initiated");
	wait_for_client(l_socket, &handle_socket);

	// Perform final clean up code
	cleanup(l_socket, 1);

	return 0;
}
