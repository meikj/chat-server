#ifndef SOCKET_H

	#define SOCKET_H

	#define WINSOCK_MAJOR	1
	#define WINSOCK_MINOR	1
	#define MAX_BACKLOG 	10

    #ifdef _WIN32
        // Windows headers and definitions
        #include <winsock.h>

        #define close(s) closesocket(s)
        #define errno WSAGetLastError()
    #else
        // Linux headers and definitions
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <unistd.h>
        #include <errno.h>

        #define INVALID_SOCKET -1
        #define SOCKET_ERROR -1
    #endif

    int socket_init();
	int socket_init_addr(const char *host, int port, struct sockaddr_in *addr);
    int socket_init_server(struct sockaddr_in);

#endif
