#ifndef SOCKET_H
	
	#define SOCKET_H
	
	#define WINSOCK_MAJOR 1
	#define WINSOCK_MINOR 1
	
	void cleanup(int socket, int wsa_cleanup);
	int init_socket();
	
#endif
