#include "renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

static Texture* tex_bckgnd;
static Texture* tex_flap;
static Texture* tex_menu;
static Texture* tex_pipe;
static Texture* tex_gnd;

static TTF_Font* font;
static SDL_Color red;
static SDL_Color white;
static SDL_Color black;
static SDL_Color yellow;

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath);
static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath, int w, int h, int frames);

Texture::Texture(SDL_Texture* tex, int w, int h, int frames) {
	this->tex = tex;
	this->w = w;
	this->h = h;
	this->frames = frames;
}

Renderer::Renderer() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &window, &renderer);
	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	target = NULL;

	TTF_Init();
	font = TTF_OpenFont("assets/arial.ttf", 80);
	red.r = 155;
	red.g = 50;
	red.b = 50;
	red.a = 255;
	white.r = 255;
	white.g = 255;
	white.b = 255;
	white.a = 255;
	black.r = 0;
	black.b = 0;
	black.g = 0;
	black.a = 255;
	yellow.r = 255;
	yellow.b = 0;
	yellow.g = 211;
	yellow.a = 255;

	tex_bckgnd = LoadTex(renderer, "assets/background.bmp");
	tex_flap = LoadTex(renderer, "assets/tex_flap.bmp");
	tex_menu = LoadTex(renderer, "assets/tex_menu.bmp");
	tex_pipe = LoadTex(renderer, "assets/pipe.bmp");
	tex_gnd = LoadTex(renderer, "assets/tex_gnd.bmp", 45, 160, 2);
}

Renderer::~Renderer() {
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Renderer::Blit(double x, double y, int w, int h, double angle, Texture* tex, double scale, int frame, int offset, bool full, SDL_RendererFlip flip_flag) {
	try {
		SDL_Rect sQuad;
		if (offset == -1) {
			sQuad.x = w * frame;
			sQuad.y = 0;
			sQuad.w = w;
			sQuad.h = h;
		} else {
			sQuad.x = frame;
			sQuad.y = 0;
			sQuad.w = w;
			sQuad.h = h;
		}

		SDL_Rect dQuad = {(int)x, (int)y, (int)(w * scale), (int)(h * scale)};
		if (full)
			SDL_RenderCopyEx(renderer, tex->tex, &sQuad, NULL, angle, NULL, flip_flag);
		else
			SDL_RenderCopyEx(renderer, tex->tex, &sQuad, &dQuad, angle, NULL, flip_flag);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Renderer::Print(int x, int y, char const* text) {
	SDL_Rect r = {x, y, 0, 0};
	TTF_SizeText(font, text, &r.w, &r.h);
	SDL_Surface* surf = TTF_RenderText_Solid(font, text, red);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_RenderCopyEx(renderer, tex, NULL, &r, 0.0, NULL, SDL_FLIP_NONE);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
}

void Renderer::DrawLine(int aX, int aY, int bX, int bY) {
	SDL_RenderDrawLine(renderer, aX, aY, bX, bY);
}

void Renderer::Clear() {
	if (target)
		SDL_DestroyTexture(target);
	// Create a blank texture for render everything onto it
	// Use SDL_TEXTUREACCESS_TARGET to use texture as the target of rendering everything
	target = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_X, WIN_Y);
	// Set render target to the blank texture
	SDL_SetRenderTarget(renderer, target);
	// Clear the render target (means clear the texture)
	SDL_RenderClear(renderer);
}

void Renderer::Present() {
	SDL_SetRenderTarget(renderer, NULL);
	double scale = 1;
	SDL_Rect dQuad = {0, 0, (int)(540 * scale), (int)(765 * scale)};
	SDL_RenderCopyEx(
		renderer,
		target,
		NULL,
		&dQuad,
		0.0,
		NULL,
		SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);
}

Texture* Renderer::GetTexture(texture_e tag) {
	switch (tag) {
		case TEX_BCKGND:
			return tex_bckgnd;

		case TEX_FLAP:
			return tex_flap;

		case TEX_MENU:
			return tex_menu;

		case TEX_PIPE:
			return tex_pipe;

		case TEX_GND:
			return tex_gnd;

		default:
			return nullptr;
	}
}

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath) {
	SDL_Surface* surf = SDL_LoadBMP(fPath);
	SDL_SetColorKey(surf, true, SDL_MapRGB(surf->format, 0, 255, 255));
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surf);

	int w, h;
	SDL_QueryTexture(text, NULL, NULL, &w, &h);
	Texture* tex = new Texture(text, w, h, 0);

	return tex;
}

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath, int w, int h, int frames) {
	SDL_Surface* surf = SDL_LoadBMP(fPath);
	SDL_SetColorKey(surf, true, SDL_MapRGB(surf->format, 0, 255, 255));
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surf);

	Texture* tex = new Texture(text, w, h, frames);

	return tex;
}
