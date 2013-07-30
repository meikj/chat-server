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
#include "clients.h"

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 5000
#define DEFAULT_BUF_SIZE 256

// Debug use
void list_clients() {
	client **list = clients_list();

	for(int i = 1; i < MAX_CLIENTS; i++) {
		if(list[i] == NULL) {
			printf("[%d] = no record\n", i);
		} else {
			printf("[%d] = yep!\n", i);
		}
	}
}

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
	client c;
	int c_id;
	char *ip;

	// Populate the client structure using the socket and address details
	c.socket = s;
	memcpy(&c.addr, &addr, sizeof(struct sockaddr_in));
	if((c_id = clients_add(&c)) == -1) {
		close(s);
		return;
	}

	ip = clients_get_ip(c_id);
	log_info("Client(id:%d, s:%d): connected from %s\n", c_id, s, ip);

	do {
		res = recv(s, buf, DEFAULT_BUF_SIZE, 0);

		if(res > 0) {
			log_info("Client(id:%d, s:%d): recv = %d bytes\n", c_id, s, res);
		} else if(res == 0) {
			log_info("Client(id:%d, s:%d): connection closed\n", c_id, s);
		} else {
			log_error("handle_client(id:%d, s:%d): recv() failed: %d\n",
				c_id, s, errno);
		}
	} while(res > 0);

	// Clean up client
	clients_remove(c_id);
	free(ip);
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

	// This is used for the address size in accept()
	socklen_t socksize = sizeof(struct sockaddr_in);

	if(argc >= 3) {
		host = argv[1];
		port = atoi(argv[2]);
	} else {
		host = DEFAULT_HOST;
		port = DEFAULT_PORT;
	}

	// Do some primitive port checking
	if(port == 0) {
		log_error("Port number is invalid: %d\n", port);
		exit(1);
	}

	// Initialise socket address structure and server socket
	if(socket_init() == -1) {
		exit(1);
	}
	if(socket_init_addr(host, port, &l_addr) == -1) {
		exit(1);
	}
	if((l_socket = socket_init_server(l_addr)) == -1) {
		exit(1);
	}

	// Main program loop
	log_info("Server is ready to accept client connections on %s:%d\n",
		host, port);
	for(;;) {
		clients_init();
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
	close(l_socket);

	return 0;
}
