#ifndef SOCKET_H

	#define SOCKET_H

	#define WINSOCK_MAJOR	1
	#define WINSOCK_MINOR	1
	#define MAX_BACKLOG 	10

    int socket_init();
	struct sockaddr_in socket_init_addr(const char *host, int port);
    int socket_init_server(struct sockaddr_in);

#endif
