#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>

#ifndef BUTTON_H
#define BUTTON_H

#pragma once

class Button {
public:
	bool isSelected = false;
	SDL_Rect rect;

	Button();
	Button(SDL_Renderer* renderer, SDL_Surface* gSurface, SDL_Texture* gTexture, TTF_Font* bFont, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, const char* name);

	virtual ~Button();

	void update(int x, int y);
};

#endif