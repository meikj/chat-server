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
    unsigned clients_id_gen();
	client *clients_get(const unsigned id);
	int clients_add(client *c);
	int clients_remove(const unsigned id);
	client **clients_list();
	char *clients_get_ip(const unsigned id);

#endif