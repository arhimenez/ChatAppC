#include <WinSock2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LOCAL_HOST "127.0.0.1"
#define SERVER_PORT 5000
#define BACKLOG 10
//2000
int main() {

	WSADATA wsaData;
	int is_WSA = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (is_WSA != 0) {
		perror("WSA failed to init\n");
		return 1;
	}

	SOCKET server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (server_descriptor == -1) {
		perror("couldn't init sock");
		WSACleanup();
		return 1;
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(SERVER_PORT);

	if (bind(server_descriptor, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		perror("Couldn't bind server socket\n");
		WSACleanup();
		return 1;
	}

	if (listen(server_descriptor, SOMAXCONN) == SOCKET_ERROR) {
		perror("Couldn't listen server socket\n");
		WSACleanup();
		return 1;
	}

	printf("listening for connections\n");
	char buffer[1024];
	while (1) {
		struct sockaddr_in clientAddr;
		int clientAddrLen = sizeof(clientAddr);

		// Accept incoming connections
		SOCKET clientSocket = accept(server_descriptor, (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (clientSocket == INVALID_SOCKET) {
			perror("Error accepting connection");
			closesocket(server_descriptor);
			WSACleanup();
			return 1;
		}

		printf("CONNECTED");
		int recivedbytes = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (recivedbytes > 0) {
			buffer[recivedbytes] = '\0';
			printf("Received data: %s\n", buffer);
		}
		else {
			printf("hello");
		}

		

		// Handle the connection...
	}
	
	closesocket(server_descriptor);
	WSACleanup();
	return 0;
}