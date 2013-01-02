#include <stdio.h>
#include <winsock.h>

int main(void) {
	WSAData wsaData;
	int error;
	
	int l_socket;
	
	error = WSAStartup(MAKEWORD(1, 1), &wsaData);
	
	if(error != 0) {
		// Some error occured with WSAStartup(), therefore we can't continue
		printf("WSAStartup() failed with error code: %d\n", error);
		exit(1);
	}
	
	if((l_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		// Some error occured with socket()
		printf("socket() failed with error code: %d\n", WSAGetLastError());
		exit(1);
	}

	return 0;
}
