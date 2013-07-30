#ifndef CLIENTS_H

	#define CLIENTS_H
	
	#include "socket.h"
	#define MAX_CLIENTS 20

	// Represents a client in the server
    typedef struct {
        int socket;
        struct sockaddr_in addr;
    } client;

    void clients_init();
	client *clients_get(const unsigned id);
	int clients_add(client *c);
	int clients_remove(const unsigned id);

#endif