#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "socket.h"
#include "clients.h"

// Basic data structure containing the connected clients
client *clients[MAX_CLIENTS];
unsigned count = 0;

/*
 * Initialise the clients data structure.
 */
void clients_init() {
	// Initialise the clients list
	for(int i = 0; i < MAX_CLIENTS; i++) {
		clients[i] = NULL;
	}
}

client *clients_get(const unsigned id) {
	if(id > MAX_CLIENTS) {
		log_error("clients_get(): invalid id passed: %d\n", id);
		return NULL;
	}
	return clients[id];
}

int clients_add(client *c) {
	if(c == NULL) {
		log_error("clients_add(): null client passed\n", NULL);
		return -1;
	} else if(count >= MAX_CLIENTS) {
		log_warn("clients_add(): maximum number of clients connected\n", NULL);
		return -1;
	}

	int c_id = count++;
	clients[c_id] = malloc(sizeof(client));
	memcpy(clients[c_id], c, sizeof(client));
	return c_id;
}

int clients_remove(const unsigned id) {
	if(id > MAX_CLIENTS) {
		log_error("clients_remove(): invalid id passed: %d\n", id);
		return -1;
	}
	free(clients[id]);
	count--;
	return 0;
}
