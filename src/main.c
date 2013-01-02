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

#include "logger.h"
#include "socket.h"

/*
 * Program is run as follows:
 *		<prog_name> <port_number>
 *
 * If arguments don't conform as such, then program will exit
 */
int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: %s <port_number>\n", argv[0]);
		exit(1);
	}
	
	int l_socket;
	int l_port;
	
	l_socket = init_socket();
	l_port = atoi(argv[1]);
	
	// Do some port checking
	if(l_port == 0) {
		log_entry(LOGGER_ERROR, 0, "main(): invalid port specified");
		exit(1);
	}
	
	log_entry(LOGGER_DEBUG, 0, "main(): l_port = %d", l_port);
	
	cleanup(l_socket, 1);

	return 0;
}
