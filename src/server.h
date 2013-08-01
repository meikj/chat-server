#ifndef SOCKET_H

#define SOCKET_H

#define MAX_BACKLOG 10

#ifdef _WIN32
// Windows headers and definitions
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define WINSOCK_MAJOR 2
#define WINSOCK_MINOR 2

#define close(s) closesocket(s)
#define err WSAGetLastError()
#define inet_pton(af, src, dst) InetPton(af, src, dst)
#define inet_ntop(af, addr, buf, len) InetNtop(af, addr, buf, len)

#else
// Linux headers and definitions
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define err errno
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

int server_init();
void server_cleanup();
int server_init_addr(char *port);
int server_init_socket();

#endif
