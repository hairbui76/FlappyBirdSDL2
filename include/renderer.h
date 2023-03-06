#pragma once
#include "defs.h"

struct Texture {
	Texture(SDL_Texture* tex, int w, int h);

	SDL_Texture* tex;
	int w;
	int h;
};

class Renderer {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* target;

public:
	Renderer();
	~Renderer();

	void renderSprite(SDL_Rect sRect, SDL_FRect dRect, double angle, SDL_FPoint* center, double alpha, Texture* tex, texture_e tag, bool full = false, SDL_RendererFlip flip_flag = SDL_FLIP_NONE);
	void Print(char const* text);
	void Clear();
	void Present();
	static Texture* GetTexture(texture_e tag);
	static void PlaySound(sound_e sName);
};
