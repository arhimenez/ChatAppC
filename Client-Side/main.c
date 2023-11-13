#include <string.h>
#include <WinSock2.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "gui.h"
#include "message.h"

//2000
#define DEBUG 1
#define LOCAL_HOST "127.0.0.1"
#define CONNECT_PORT 5000
#define LINE_LENGTH 24

void SendMessageFont(Message* message_p, SOCKET clientfd);
void PrintReceivedMessages(SDL_Renderer* renderer, SDL_Surface* textSurface, SDL_Texture* textTexture, TTF_Font* font);
DWORD WINAPI ReceiveMessage(LPVOID serverfd);

Message receivedMessageList[256];
static int receivedMessagesCount = 0;

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("SDL2 Couldn't init\n");
		exit(1);
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed\n");
		SDL_Quit();
		return 1;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		WSACleanup();
		SDL_Quit();
		perror("Couldn't init socket");
		exit(1);
	}

	SDL_Window* window = NULL;
	window = SDL_CreateWindow("chaTapp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_DestroyWindow(window);
		WSACleanup();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!window) {
		SDL_DestroyWindow(window);
		WSACleanup();
		SDL_Quit();
		return 1;
	}

	if (TTF_Init() < 0) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		WSACleanup();
		SDL_Quit();
		return 1;
	}

	TTF_Font* font = NULL;
	font = TTF_OpenFont("C:/font/Pixellettersfull-BnJ5.ttf", 24);
	if (!font) {
		perror("counldn't oepn font'nd ");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		WSACleanup();
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_StartTextInput();

	SDL_Event event_var;
	int quit = 0;
	struct sockaddr_in serverAddress = { .sin_family = AF_INET, .sin_port = htons(5000) };
	if (inet_pton(AF_INET, LOCAL_HOST, &serverAddress.sin_addr) != 1) {
		perror("couldn't translate ip into host type");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		WSACleanup();
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		perror("couldn't establish connection");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		WSACleanup();
		TTF_Quit();
		SDL_Quit();
		return 1;
	}


	// Create a surface and texture for rendering text
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;

	Message init_mes = { .length = 0, .text[0] = '\0', .timestamp = time(NULL) };
	Message* user_message = &init_mes;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveMessage, (LPVOID) clientSocket, 0, NULL);

	while (!quit) {
		while (SDL_PollEvent(&event_var)) {
			switch (event_var.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event_var.key.keysym.sym == SDLK_BACKSPACE) {
					goto SDLINPUT;
				}
				break;
			SDLINPUT: // most friendly way of dealing the case of backspace and many key presses that SDL_TEXTINPUT does not handle.
			case SDL_TEXTINPUT:
				GetUserMessage(event_var, user_message);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (DEBUG) {
					int i = 0;
					//printf("x: %d | y: %d\n", event_var.motion.x, event_var.motion.y);
					for (; i < user_message->length; ++i) {
						putchar(user_message->text[i]);
					}
					//printf("%d", user_message->text[++i]);
				}
				int clickx = event_var.motion.x;
				int clicky = event_var.motion.y;
				if ((800 <= clickx && clickx <= 800 + BUTTON_WIDTH) && (600 <= clicky && clicky <= 600 + BUTTON_HEIGHT)) {
					SendMessageFont(user_message, clientSocket);
					FlushMessage(user_message);
				}
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		DrawTextFont(renderer, textSurface, textTexture, font, user_message);
		DrawButton(renderer);
		PrintReceivedMessages(renderer, textSurface, textTexture, font);

		SDL_RenderPresent(renderer);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}

	SDL_StopTextInput();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}

void SendMessageFont(Message* message_p, SOCKET clientfd) {
	send(clientfd, &(message_p->text[0]), message_p->length + 1, SOCK_STREAM);
}

DWORD WINAPI ReceiveMessage(LPVOID serverfd) {
	SOCKET server_fd = (SOCKET)serverfd;
	while (1) {
		char buffer[1024];
		Message receivedMessage = { .length = NULL, .text = NULL, .timestamp = NULL };
		int receivedbytes = recv(server_fd, buffer, sizeof(buffer), 0);
		if (receivedbytes > 0) {
			receivedMessageList[receivedMessagesCount].length = receivedbytes;
			if (receivedbytes < sizeof(buffer)) {
				buffer[receivedbytes] = '\0';
				printf("gotten: %s\n", buffer);
				memcpy(receivedMessageList[receivedMessagesCount].text, buffer, receivedbytes);
				//receivedMessageList[receivedMessagesCount].text[receivedbytes] = '\0';
				printf("into list: %s\n", receivedMessageList[receivedMessagesCount].text);
				receivedMessageList[receivedMessagesCount].timestamp = time(NULL);

				receivedMessagesCount++;
			}
			else {
				perror("buffer overflow, from ReceiveMessageAndShow");
				exit(1);
			}
		}
	}
}

void PrintReceivedMessages(SDL_Renderer* renderer, SDL_Surface* textSurface, SDL_Texture* textTexture, TTF_Font* font) {
	for (int i = 0; i < receivedMessagesCount; ++i) {
		SDL_Color TEXT_COLOR = { 0, 0, 0xA5 };
		textSurface = TTF_RenderText_Solid(font, receivedMessageList[i].text, TEXT_COLOR);
		SDL_Rect textRect = { INIT_TEXT_POSX, 48 + i * LINE_LENGTH, textSurface->w, textSurface->h };
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	}
}