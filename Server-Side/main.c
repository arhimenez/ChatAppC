#include <WinSock2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>

#define LOCAL_HOST "127.0.0.1"
#define SERVER_PORT 5000
#define BACKLOG 10
#define MAX_CLIENTS 10
//2000

typedef struct {
	SOCKET socket;
	struct sockaddr_in address;
} Client;

Client clientList[MAX_CLIENTS];
int client_num = 0;

DWORD WINAPI handleClient(int clientId);
void broadcastMessage(char buffer[]);

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

	struct sockaddr_in server_address = { .sin_family = AF_INET, .sin_addr = INADDR_ANY, .sin_port = htons(SERVER_PORT) };
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
	while (1) {
		if (client_num < MAX_CLIENTS) {
			if ((clientList[client_num].socket = accept(server_descriptor, (struct sockaddr *)&clientList[client_num].address, NULL)) == SOCKET_ERROR) {
				printf("Connection failed with: %d\n", WSAGetLastError());
			}

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleClient, (LPVOID)client_num, 0, NULL);
			client_num++;
		}
	}
	
	closesocket(server_descriptor);
	WSACleanup();
	return 0;
}

DWORD WINAPI handleClient(int clientId) {
	int receivedbytes;
	char buffer[1024];
	char ipBuffer[INET_ADDRSTRLEN];
	while (1) { // I want the threads to continue listening for inc messages.
		if ((receivedbytes = recv(clientList[clientId].socket, buffer, sizeof(buffer), 0)) <= 0) {

			inet_ntop(AF_INET, &(clientList[clientId].address.sin_addr), ipBuffer, INET_ADDRSTRLEN);
			printf("Client number %s:%d has disconnected.\n", ipBuffer, ntohs(clientList[clientId].address.sin_port));
			closesocket(clientList[clientId].socket);
			for (int i = clientId; i < client_num - 1; ++clientId) {
				clientList[i] = clientList[i + 1];
			}
			--client_num;
			break;
		}
		else if (receivedbytes < sizeof(buffer)) { // RECEIVED A MESSAGE
			buffer[receivedbytes] = '\0';
			broadcastMessage(buffer);
		}
	}
}

void broadcastMessage(char buffer[]) {
	for (int i = 0; i < client_num; ++i) {
		send(clientList[i].socket, buffer, strlen(buffer) + 1, SOCK_STREAM);
		printf("Broadcasting: %s | to client: %d\n", buffer, i);
	}
}