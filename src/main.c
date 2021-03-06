/*
 * main.c
 *
 * Basic chat server programmed in C for educational purposes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "server.h"
#include "clients.h"
#include "protocol.h"

#define DEFAULT_PORT "27015"
#define BUF_SIZE 256

// Debug use
void list_clients() {
	int i;
	client **list = clients_list();

	for(i = 1; i < MAX_CLIENTS; i++) {
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
void handle_client(int s, struct sockaddr_in *addr) {
	char buf[BUF_SIZE];
	char ip[INET_ADDRSTRLEN];
	int res;
	int c_id;
	client c;

	// Populate the client structure using the socket and address details
	memset(&c, 0, sizeof c);
	c.socket = s;
	memcpy(&c.addr, addr, sizeof(struct sockaddr_in));
	if((c_id = clients_add(&c)) == -1) {
		close(s);
		return;
	}

	clients_get_ip(c_id, ip, INET_ADDRSTRLEN);
	log_info("Client(id:%d): connected from %s\n", c_id, ip);

	do {
		memset(buf, 0, BUF_SIZE);
		res = recv(s, buf, BUF_SIZE, 0);

		if(res > 0) {
			log_info("Client(id:%d): recv = %d bytes\n", c_id, res);
			proto_parse(c_id, buf, BUF_SIZE);
		} else if(res == 0) {
			log_info("Client(id:%d): connection closed\n", c_id);
		} else {
			log_error("handle_client(id:%d): recv() failed: %d\n",
				c_id, errno);
		}
	} while(res > 0);

	// Clean up client
	clients_remove(c_id);
}

/*
 * The following argument is accepted:
 *	port = The port number to listen on
 *
 * Example usage: ./chat-server 5000
 *
 * If no argument is passed, the default value from DEFAULT_PORT is used.
 */
int main(int argc, char *argv[]) {
	char *port;
	int l_socket; // Server socket
	int c_socket; // Client socket
	struct sockaddr_in c_addr;

	// This is used for the address size in accept()
	socklen_t socksize = sizeof(struct sockaddr_in);

	if(argc == 2) {
		port = argv[1];
	} else {
		port = DEFAULT_PORT;
	}

	// Initialise socket address structure and server socket
	log_info("Initialising server...\n", NULL);
	if(server_init() == -1) {
		server_cleanup();
		exit(1);
	}
	if(server_init_addr(port) == -1) {
		server_cleanup();
		exit(1);
	}
	if((l_socket = server_init_socket()) == -1) {
		server_cleanup();
		exit(1);
	}

	// Main program loop
	log_info("Ready to accept client connections on port %s\n", port);
	for(;;) {
		clients_init();
		if((c_socket = accept(l_socket, (struct sockaddr *)&c_addr,
			&socksize)) == INVALID_SOCKET) {
			log_error("Client accept() failed: %d\n", c_socket);
			continue;
		} else {
			log_info("Client connection accepted, passed on to handler: %d\n", c_socket);
			handle_client(c_socket, &c_addr);
		}
	}

	server_cleanup();

	return 0;
}
