/*
 * main.c
 *
 * Basic server being programmed in C for educational purposes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "socket.h"

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 5000
#define DEFAULT_BUF_SIZE 256

// Basic data structure storing all connected clients
socket_client *clients[MAX_CLIENTS];
int count = 0;

/*
 * Handle a client connection.
 *
 * Params:
 *	s = The client socket descriptor
 *	addr = The client socket address
 */
void handle_client(int s, struct sockaddr_in addr) {
	char buf[DEFAULT_BUF_SIZE];
	int res;
	socket_client c;
	int c_pos = count++;

	// Populate the client structure using the socket and address details
	c.socket = s;
	memcpy(&c.addr, &addr, sizeof(struct sockaddr_in));

	// Add client to list
	clients[c_pos] = malloc(sizeof(socket_client));
	memcpy(&clients[c_pos], &c, sizeof(socket_client));

	do {
		res = recv(s, buf, DEFAULT_BUF_SIZE, 0);

		if(res > 0) {
			log_info("Client(%d): recv = %d bytes\n", s, res);
		} else if(res == 0) {
			log_info("Client(%d): connection closed\n", s);
		} else {
			log_error("handle_client(%d): recv() failed: %d\n",
				s, WSAGetLastError());
		}
	} while(res > 0);

	// Clean up client
	free(clients[c_pos]);
}

/*
 * The following arguments are accepted:
 *	host = The host to listen on
 *	port = The port number to listen on
 *
 * Example usage: ./chat-server.exe 127.0.0.1 5000
 *
 * If no arguments are passed, the default values from DEFAULT_HOST and
 * DEFAULT_HOST are used instead.
 */
int main(int argc, char *argv[]) {
	char *host;
	int port;
	int l_socket; // Server socket
	struct sockaddr_in l_addr;
	int c_socket; // Client socket
	struct sockaddr_in c_addr;
	socket_client *c;

	// This is used for the address size in accept()
	socklen_t socksize = sizeof(struct sockaddr_in);

	log_info("Validating server information... ", NULL);
	if(argc >= 3) {
		host = argv[1];
		port = atoi(argv[2]);
	} else {
		host = DEFAULT_HOST;
		port = DEFAULT_PORT;
	}

	// Do some primitive port checking
	if(port == 0) {
		log("failed\n", NULL);
		exit(1);
	} else {
		log("success\n", NULL);
	}

	// Initialise socket address structure and server socket
	if(socket_init() == -1) {
		exit(1);
	}
	l_addr = socket_init_addr(host, port);
	if((l_socket = socket_init_server(l_addr)) == -1) {
		exit(1);
	}

	// Main program loop
	log_info("Server is ready to accept client connections on %s:%d\n",
		host, port);
	for(;;) {
		if((c_socket = accept(l_socket, (struct sockaddr *)&c_addr,
			&socksize)) == INVALID_SOCKET) {
			log_error("Client accept() failed: %d\n", c_socket);
			continue;
		} else {
			log_info("Client connection accepted, passed on to handler: %d\n", c_socket);
			handle_client(c_socket, c_addr);
		}
	}

	// Perform final clean up code
	closesocket(l_socket);

	return 0;
}
