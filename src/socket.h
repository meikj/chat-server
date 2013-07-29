#ifndef SOCKET_H

	#define SOCKET_H

	#define WINSOCK_MAJOR	1
	#define WINSOCK_MINOR	1
	#define MAX_BACKLOG 	10
    #define MAX_CLIENTS     20

    #ifdef _WIN32
        #include <winsock.h>
    #else
        // UNIX variants here
    #endif

    int socket_init();
	struct sockaddr_in socket_init_addr(const char *host, int port);
    int socket_init_server(struct sockaddr_in);

    #ifndef _WIN32
        #define closesocket(s) close(s)
    #endif

    // Represents a client in the server
    typedef struct {
        int socket;
        struct sockaddr_in addr;
    } socket_client;

#endif
