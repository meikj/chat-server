/*
 * clients.c
 *
 * Represents a basic data structure for storing clients.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "socket.h"
#include "clients.h"

// Basic data structure containing the connected clients (0 is NOT used)
client *clients[MAX_CLIENTS];
unsigned count = 0;

/*
 * Initialise the clients data structure.
 */
void clients_init() {
	int i;
	for(i = 1; i < MAX_CLIENTS; i++) {
		clients[i] = NULL;
	}
}

/*
 * Generate a simple client ID.
 */
unsigned clients_id_gen() {
	unsigned i;
	for(i = 1; i < MAX_CLIENTS; i++) {
		if(clients[i] == NULL) {
			return i;
		}
	}
	return 0;
}

/*
 * Get a client from the data structure.
 *
 * Params:
 *	id = The client ID
 *
 * Returns:
 *	The resulting client structure, or NULL if no such client exists.
 */
client *clients_get(const unsigned id) {
	if(id < 1 || id > MAX_CLIENTS) {
		log_error("clients_get(): invalid id passed: %d\n", id);
		return NULL;
	}
	return clients[id];
}

/*
 * Add a client to the data structure.
 *
 * Params:
 *	c = The client structure.
 *
 * Returns:
 *	If the function was successful, then 0 is returned, otherwise -1.
 */
int clients_add(client *c) {
	int c_id;

	if(c == NULL) {
		log_error("clients_add(): null client passed\n", NULL);
		return -1;
	} else if(count >= MAX_CLIENTS) {
		log_warn("clients_add(): maximum number of clients connected\n", NULL);
		return -1;
	}

	if((c_id = clients_id_gen()) == 0) {
		log_error("clients_add(): problem generating a client ID\n", NULL);
		return -1;
	}

	clients[c_id] = malloc(sizeof(client));
	memcpy(clients[c_id], c, sizeof(client));
	count++;
	return c_id;
}

/*
 * Remove a client from the database.
 *
 * Params:
 *	id = The client ID
 *
 * Returns:
 *	If the function was successful, then 0 is returned, otherwise -1.
 */
int clients_remove(const unsigned id) {
	if(id < 1 || id > MAX_CLIENTS) {
		log_error("clients_remove(): invalid id passed: %d\n", id);
		return -1;
	} else if(clients[id] == NULL) {
		log_error("clients_remove(): no record at id: %d\n", id);
		return -1;
	}
	close(clients[id]->socket);
	free(clients[id]);
	clients[id] = NULL;
	count--;
	return 0;
}

/*
 * Return a pointer to the list of clients.
 */
client **clients_list() {
	return clients;
}

/*
 * Fetch the IP address of a client.
 */
char *clients_get_ip(const unsigned id) {
	char *ip;

	if(id < 1 || id > MAX_CLIENTS) {
		log_error("clients_get_ip(): invalid id passed: %d\n", id);
		return NULL;
	} else if(clients[id] == NULL) {
		log_error("clients_get_ip(): no record at id: %d\n", id);
		return NULL;
	}

	ip = (char *)malloc(INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(clients[id]->addr.sin_addr), ip, INET_ADDRSTRLEN);
	return ip;
}
