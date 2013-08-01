/*
 * protocol.c
 *
 * Contains functionality for parsing client messages against a very basic
 * chat protocol.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "clients.h"

/*
 * Parse a received client message.
 *
 */
void proto_parse(int id, const char *data, size_t size) {
    char *buf, *tok;

    // Make a copy of the data for manipulation purposes
    buf = malloc(size);
    memcpy(buf, data, size);

    tok = strtok(buf, " ");
    while(tok != NULL) {
        printf("tok = %s\n", tok);
        tok = strtok(NULL, " ");
    }
}

/*
 * The client has requested to see a list of connected clients.
 *
 */
void proto_get_clients(int id) {
	client c;

    clients_get(id, &c);
    if(c != NULL) {
        // send(c.socket);
    } else {
        // Client ID doesn't exist...
    }
}
