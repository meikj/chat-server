#ifndef SOCKET_H
	
	#define SOCKET_H
	
	#define WINSOCK_MAJOR	1
	#define WINSOCK_MINOR	1
	#define MAX_BACKLOG 	10
	
	void cleanup(int socket, int wsa_cleanup);
	struct sockaddr_in init_server_addr(const char *host, int port);
	int init_socket();
	
#endif
