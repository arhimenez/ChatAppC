#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "message.h"

#define WINDOW_WIDTH 940
#define WINDOW_HEIGHT 721
#define BUTTON_WIDTH 65
#define BUTTON_HEIGHT 65
#define INIT_TEXT_POSX 20
#define INIT_TEXT_POSY 520


void DrawButton(SDL_Renderer* renderer);
void DrawTextFont(SDL_Renderer* renderer, SDL_Surface* textSurface, SDL_Texture* textTexture, TTF_Font* font, Message* message_p);



#endif // !GUI_H
