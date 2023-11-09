#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 1
#define INIT_TEXT_POSX 20
#define INIT_TEXT_POSY 520
// #define ENTERKEY '\n'


static int WINDOW_WIDTH = 940;
static int WINDOW_HEIGHT = 721;
static SDL_Color TEXT_COLOR = { 0, 0, 0 }; // White text color

int TextPosX = INIT_TEXT_POSX;
int TextPosY = INIT_TEXT_POSY;

typedef struct {
	int length;
	char text[1024];
	time_t timestamp;
} Message;

void GetUserMessage(SDL_Event event, Message *message_p);
void SendMessage();
void FlushMessage(Message* message_p);
void DrawText(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* textRect, SDL_Surface* textSurface, Message* message_p);
void DrawButton(SDL_Renderer* renderer);
void ButtonInput(SDL_Renderer* renderer, SDL_Event event);

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		perror("SDL2 Couldn't init\n");
		exit(1);
	}

	SDL_Window* window = NULL;
	window = SDL_CreateWindow("chaTapp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!window) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	if (TTF_Init() < 0) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	TTF_Font* font = NULL;
	font = TTF_OpenFont("C:/font/Pixellettersfull-BnJ5.ttf", 24);
	if (!font) {
		perror("counldn't oepn font'nd ");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_StartTextInput();
	
	SDL_Event event_var;
	int quit = 0;

	// Create a surface and texture for rendering text
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;

	Message init_mes = { .length = 0, .text[0] = '\0', .timestamp = time(NULL) };
	Message* user_message = &init_mes;


	while (!quit) {
		while (SDL_PollEvent(&event_var)) {
			switch (event_var.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event_var.key.keysym.sym == SDLK_BACKSPACE ){
					goto SDLINPUT;
				}
				else if (event_var.key.keysym.sym == SDLK_RETURN) {
					TextPosY += 24;
				}
				break;
			SDLINPUT: // most friendly way of dealing the case of backspace and many key presses that SDL_TEXTINPUT does not handle.
			case SDL_TEXTINPUT:
				GetUserMessage(event_var, user_message);
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		DrawText(renderer, textTexture, textSurface, textTexture, font, user_message);

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

	return 0;
}

void GetUserMessage(SDL_Event event, Message* message_p) {
	char key_val = event.text.text[0];
	printf("\n%d\n", key_val);
	if (key_val == 1 && message_p->length > 0) {
		if (DEBUG) {
			printf("you son of bitch, im in\n");
		}
		message_p->text[--message_p->length] = '\0';
	}
	else if (key_val != 1) {
		message_p->text[message_p->length++] = key_val;
		message_p->text[message_p->length] = '\0';
	}
	printf("length: %d", message_p->length);
}

void DrawText(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface *textSurface, SDL_Texture *textTexture, TTF_Font *font, Message* message_p) {
	if (message_p->length < 1) {
		textSurface = TTF_RenderText_Solid(font, "Enter a message", TEXT_COLOR);
	}
	else {
		textSurface = TTF_RenderText_Solid(font, message_p->text, TEXT_COLOR);
	}
	SDL_Rect textRect = { TextPosX, TextPosY, textSurface->w, textSurface->h }; // Position of the text

	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}