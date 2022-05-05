#include "renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

static Texture* tex_bg;
static Texture* tex_branch;
static Texture* tex_log;
static Texture* tex_stone;
static Texture* tex_trunk;

static Texture* tex_play;
static Texture* tex_refresh;
static Texture* tex_left;
static Texture* tex_right;

static Texture* tex_lumber_holding;
static Texture* tex_lumber_cutting;
static Texture* tex_lumber_dead;

static Texture* tex_text_title;

static TTF_Font* font;
static SDL_Color red;
static SDL_Color white;
static SDL_Color black;
static SDL_Color yellow;

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath, bool text_mode = false, const char* text = "");

Texture::Texture(SDL_Texture* tex, int w, int h) {
	this->tex = tex;
	this->w = w;
	this->h = h;
}

Renderer::Renderer() {
	// init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError("Unable to initialize SDL: %s", SDL_GetError());
	// init sdl ttf
	if (TTF_Init() != 0) logSDLError("Unable to initialize SDL_ttf: %s", TTF_GetError());
	// create window
	window = SDL_CreateWindow("Branch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, SDL_WINDOW_RESIZABLE);
	if (window == nullptr) logSDLError("Unable to create window: %s", SDL_GetError());
	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) logSDLError("Unable to create renderer: %s", SDL_GetError());
	// set fixed size
	SDL_RenderSetLogicalSize(renderer, WIN_X, WIN_Y);
	target = nullptr;

	font = TTF_OpenFont("assets/charter-sc-itc-tt.ttf", 80);
	if (font == nullptr) logSDLError("Unable to load font: %s", TTF_GetError());
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

	// resources background
	tex_bg = LoadTex(renderer, "assets/bg.bmp");
	if (tex_bg == nullptr) logSDLError("Unable to load tex_bg: %s", SDL_GetError());
	tex_branch = LoadTex(renderer, "assets/branch.bmp");
	if (tex_branch == nullptr) logSDLError("Unable to load tex_branch: %s", SDL_GetError());
	tex_log = LoadTex(renderer, "assets/log.bmp");
	if (tex_log == nullptr) logSDLError("Unable to load tex_log: %s", SDL_GetError());
	tex_stone = LoadTex(renderer, "assets/stone.bmp");
	if (tex_stone == nullptr) logSDLError("Unable to load tex_stone: %s", SDL_GetError());
	tex_trunk = LoadTex(renderer, "assets/trunk.bmp");
	if (tex_trunk == nullptr) logSDLError("Unable to load tex_trunk: %s", SDL_GetError());
	// button
	tex_play = LoadTex(renderer, "assets/play.bmp");
	if (tex_play == nullptr) logSDLError("Unable to load tex_play: %s", SDL_GetError());
	tex_refresh = LoadTex(renderer, "assets/refresh.bmp");
	if (tex_refresh == nullptr) logSDLError("Unable to load tex_refresh: %s", SDL_GetError());
	tex_left = LoadTex(renderer, "assets/left.bmp");
	if (tex_left == nullptr) logSDLError("Unable to load tex_left: %s", SDL_GetError());
	tex_right = LoadTex(renderer, "assets/right.bmp");
	if (tex_right == nullptr) logSDLError("Unable to load tex_right: %s", SDL_GetError());
	// lumberjack
	tex_lumber_holding = LoadTex(renderer, "assets/lumber_holding.bmp");
	if (tex_lumber_holding == nullptr) logSDLError("Unable to load tex_lumber_holding: %s", SDL_GetError());
	tex_lumber_cutting = LoadTex(renderer, "assets/lumber_cutting.bmp");
	if (tex_lumber_cutting == nullptr) logSDLError("Unable to load tex_lumber_cutting: %s", SDL_GetError());
	tex_lumber_dead = LoadTex(renderer, "assets/lumber_dead.bmp");
	if (tex_lumber_dead == nullptr) logSDLError("Unable to load tex_lumber_dead: %s", SDL_GetError());
	// title text
	tex_text_title = LoadTex(renderer, " ", true, "Lumberjack");
	if (tex_text_title == nullptr) logSDLError("Unable to load tex_text_title: %s", TTF_GetError());
}

Renderer::~Renderer() {
	SDL_DestroyTexture(tex_bg->tex);
	SDL_DestroyTexture(tex_branch->tex);
	SDL_DestroyTexture(tex_log->tex);
	SDL_DestroyTexture(tex_stone->tex);
	SDL_DestroyTexture(tex_trunk->tex);
	SDL_DestroyTexture(tex_play->tex);
	SDL_DestroyTexture(tex_refresh->tex);
	SDL_DestroyTexture(tex_left->tex);
	SDL_DestroyTexture(tex_right->tex);
	SDL_DestroyTexture(tex_lumber_holding->tex);
	SDL_DestroyTexture(tex_lumber_cutting->tex);
	SDL_DestroyTexture(tex_lumber_dead->tex);
	SDL_DestroyTexture(tex_text_title->tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

void Renderer::renderSprite(SDL_Rect sRect, SDL_FRect dRect, double angle, Texture* tex, texture_e tag, bool full, SDL_RendererFlip flip_flag) {
	if (full) {
		if (SDL_RenderCopyExF(renderer, tex->tex, &sRect, NULL, angle, NULL, flip_flag) != 0) logSDLError("Unable to render %s texture: %s", getTextureTag(tag).c_str(), SDL_GetError());
	} else {
		if (SDL_RenderCopyExF(renderer, tex->tex, &sRect, &dRect, angle, NULL, flip_flag) != 0) logSDLError("Unable to render %s texture: %s", getTextureTag(tag).c_str(), SDL_GetError());
	}
}

void Renderer::Print(int x, int y, char const* text) {
	SDL_Rect r{x, y, 0, 0};
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
	target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_X, WIN_Y);
	// Set render target to the blank texture
	SDL_SetRenderTarget(renderer, target);
	// Set render draw color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// Clear the render target (means clear the texture)
	SDL_RenderClear(renderer);
}

void Renderer::Present() {
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, target, NULL, NULL);
	SDL_RenderPresent(renderer);
}

Texture* Renderer::GetTexture(texture_e tag) {
	switch (tag) {
		case TEX_BG:
			return tex_bg;

		case TEX_LOG:
			return tex_log;

		case TEX_STONE:
			return tex_stone;

		case TEX_TRUNK:
			return tex_trunk;

		case TEX_BRANCH:
			return tex_branch;

		case TEX_PLAY:
			return tex_play;

		case TEX_REFRESH:
			return tex_refresh;

		case TEX_LEFT:
			return tex_left;

		case TEX_RIGHT:
			return tex_right;

		case TEX_LUMBER_DEAD:
			return tex_lumber_dead;

		case TEX_LUMBER_HOLDING:
			return tex_lumber_holding;

		case TEX_LUMBER_CUTTING:
			return tex_lumber_cutting;

		case TEX_TEXT_TITLE:
			return tex_text_title;

		default:
			return nullptr;
	}
}

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath, bool text_mode, const char* text) {
	SDL_Surface* surf;
	if (text_mode)
		surf = TTF_RenderText_Blended(font, text, black);
	else
		surf = SDL_LoadBMP(fPath);
	if (!surf) return nullptr;
	SDL_SetColorKey(surf, true, SDL_MapRGB(surf->format, 0, 255, 255));
	SDL_Texture* new_tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_SetTextureBlendMode(new_tex, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surf);

	int w, h;
	SDL_QueryTexture(new_tex, NULL, NULL, &w, &h);
	Texture* tex = new Texture(new_tex, w, h);

	return tex;
}