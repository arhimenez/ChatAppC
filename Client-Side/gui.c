#include "gui.h"
#include "message.h"

void DrawButton(SDL_Renderer* renderer) {
	SDL_Rect button = { .x = 800, .y = 600, .w = BUTTON_WIDTH, .h = BUTTON_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0);
	SDL_RenderFillRect(renderer, &button);
}

void DrawTextFont(SDL_Renderer* renderer, SDL_Surface* textSurface, SDL_Texture* textTexture, TTF_Font* font, Message* message_p) {
	SDL_Color TEXT_COLOR = { 0, 0, 0 };
	if (message_p->length < 1) {
		textSurface = TTF_RenderText_Solid(font, "Enter a message", TEXT_COLOR);
	}
	else {
		textSurface = TTF_RenderText_Solid(font, message_p->text, TEXT_COLOR);
	}
	SDL_Rect textRect = { INIT_TEXT_POSX, INIT_TEXT_POSY, textSurface->w, textSurface->h };

	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}