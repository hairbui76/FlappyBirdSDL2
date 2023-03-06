#include "renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>

static Mix_Music* hit_sound;
static Mix_Music* die_sound;

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

static Texture* tex_timeline;
static Texture* tex_timeline_bar;
static Texture* tex_timeline_warn;

static Texture* tex_text_title;
static Texture* tex_text_end;

static TTF_Font* font;
static TTF_Font* title_font;

static SDL_Color white{255, 255, 255, 255};
static SDL_Color grey{77, 77, 77, 255};

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath, bool text_mode = false, const char* text = "");
// static Texture* LoadPngTex(SDL_Renderer* renderer, char const* fPath);

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
	// init sdl image
	int mixer_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(mixer_flags) != mixer_flags) logSDLError("Unable to initialize SDL_image: %s", IMG_GetError());
	// init sdl mixer
	int image_flags = MIX_INIT_MP3;
	if (Mix_Init(image_flags) != image_flags) logSDLError("Unable to initialize SDL_mixer: %s", Mix_GetError());
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	// load sound;
	hit_sound = Mix_LoadMUS("assets/hit.mp3");
	if (!hit_sound) logSDLError("Unable to load hit_sound: %s", Mix_GetError());
	die_sound = Mix_LoadMUS("assets/die.mp3");
	if (!die_sound) logSDLError("Unable to load die_sound: %s", Mix_GetError());
	// create window
	window = SDL_CreateWindow("Branch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, SDL_WINDOW_RESIZABLE);
	if (!window) logSDLError("Unable to create window: %s", SDL_GetError());
	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) logSDLError("Unable to create renderer: %s", SDL_GetError());
	// set fixed size
	SDL_RenderSetLogicalSize(renderer, WIN_X, WIN_Y);
	target = nullptr;

	title_font = TTF_OpenFont("assets/Charter Bold.ttf", 60);
	if (!title_font) logSDLError("Unable to load font: %s", TTF_GetError());
	font = TTF_OpenFont("assets/Charter Bold.ttf", 30);
	if (!font) logSDLError("Unable to load font: %s", TTF_GetError());

	// resources background
	tex_bg = LoadTex(renderer, "assets/bg.bmp");
	if (!tex_bg) logSDLError("Unable to load tex_bg: %s", SDL_GetError());
	tex_branch = LoadTex(renderer, "assets/branch.bmp");
	if (!tex_branch) logSDLError("Unable to load tex_branch: %s", SDL_GetError());
	tex_log = LoadTex(renderer, "assets/log.bmp");
	if (!tex_log) logSDLError("Unable to load tex_log: %s", SDL_GetError());
	tex_stone = LoadTex(renderer, "assets/stone.bmp");
	if (!tex_stone) logSDLError("Unable to load tex_stone: %s", SDL_GetError());
	tex_trunk = LoadTex(renderer, "assets/trunk.bmp");
	if (!tex_trunk) logSDLError("Unable to load tex_trunk: %s", SDL_GetError());
	// button
	tex_play = LoadTex(renderer, "assets/play.bmp");
	if (!tex_play) logSDLError("Unable to load tex_play: %s", SDL_GetError());
	tex_refresh = LoadTex(renderer, "assets/refresh.bmp");
	if (!tex_refresh) logSDLError("Unable to load tex_refresh: %s", SDL_GetError());
	tex_left = LoadTex(renderer, "assets/left.bmp");
	if (!tex_left) logSDLError("Unable to load tex_left: %s", SDL_GetError());
	tex_right = LoadTex(renderer, "assets/right.bmp");
	if (!tex_right) logSDLError("Unable to load tex_right: %s", SDL_GetError());
	// lumberjack
	tex_lumber_holding = LoadTex(renderer, "assets/lumber_holding.bmp");
	if (!tex_lumber_holding) logSDLError("Unable to load tex_lumber_holding: %s", SDL_GetError());
	tex_lumber_cutting = LoadTex(renderer, "assets/lumber_cutting.bmp");
	if (!tex_lumber_cutting) logSDLError("Unable to load tex_lumber_cutting: %s", SDL_GetError());
	tex_lumber_dead = LoadTex(renderer, "assets/lumber_dead.bmp");
	if (!tex_lumber_dead) logSDLError("Unable to load tex_lumber_dead: %s", SDL_GetError());
	// timeline
	tex_timeline = LoadTex(renderer, "assets/timeline.bmp");
	if (!tex_timeline) logSDLError("Unable to load tex_timeline: %s", SDL_GetError());
	tex_timeline_bar = LoadTex(renderer, "assets/timeline_bar.bmp");
	if (!tex_timeline_bar) logSDLError("Unable to load tex_timeline_bar: %s", SDL_GetError());
	tex_timeline_warn = LoadTex(renderer, "assets/timeline_warn.bmp");
	if (!tex_timeline_warn) logSDLError("Unable to load tex_timeline_warn: %s", SDL_GetError());
	// title text
	tex_text_title = LoadTex(renderer, " ", true, "Lumberjack");
	if (!tex_text_title) logSDLError("Unable to load tex_text_title: %s", TTF_GetError());
	// end text
	tex_text_end = LoadTex(renderer, " ", true, "You score");
	if (!tex_text_end) logSDLError("Unable to load tex_text_title: %s", TTF_GetError());
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
	SDL_DestroyTexture(tex_timeline->tex);
	SDL_DestroyTexture(tex_timeline_bar->tex);
	SDL_DestroyTexture(tex_timeline_warn->tex);
	SDL_DestroyTexture(tex_text_title->tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	Mix_FreeMusic(die_sound);
	Mix_FreeMusic(hit_sound);
	TTF_Quit();
	SDL_Quit();
}

void Renderer::renderSprite(SDL_Rect sRect, SDL_FRect dRect, double angle, SDL_FPoint* center, double alpha, Texture* tex, texture_e tag, bool full, SDL_RendererFlip flip_flag) {
	if (SDL_SetTextureAlphaMod(tex->tex, alpha * 255) != 0) logSDLError("Unable to set texture alpha: %s", SDL_GetError());
	// std::cout << center->x << " " << center->y << std::endl;
	if (full) {
		if (SDL_RenderCopyExF(renderer, tex->tex, &sRect, NULL, angle, center, flip_flag) != 0) logSDLError("Unable to render %s texture: %s", getTextureTag(tag).c_str(), SDL_GetError());
	} else {
		if (SDL_RenderCopyExF(renderer, tex->tex, &sRect, &dRect, angle, center, flip_flag) != 0) logSDLError("Unable to render %s texture: %s", getTextureTag(tag).c_str(), SDL_GetError());
	}
}

void Renderer::Print(char const* text) {
	SDL_Rect r{0, 0, 0, 0};
	TTF_SizeText(font, text, &r.w, &r.h);
	SDL_Surface* surf = TTF_RenderText_Solid(font, text, white);
	if (!surf) logSDLError("Unable to render text: %s", TTF_GetError());
	r.x = (WIN_X - r.w) / 2;
	r.y = 20.0;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	if (!tex) logSDLError("Unable to create texture: %s", SDL_GetError());
	SDL_RenderCopyEx(renderer, tex, NULL, &r, 0.0, NULL, SDL_FLIP_NONE);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
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

		case TEX_TIMELINE:
			return tex_timeline;

		case TEX_TIMELINE_BAR:
			return tex_timeline_bar;

		case TEX_TIMELINE_WARN:
			return tex_timeline_warn;

		case TEX_TEXT_TITLE:
			return tex_text_title;

		case TEX_TEXT_END:
			return tex_text_end;

		default:
			return nullptr;
	}
}

static Texture* LoadTex(SDL_Renderer* renderer, char const* fPath, bool text_mode, const char* text) {
	SDL_Surface* surf;
	if (text_mode)
		surf = TTF_RenderText_Blended(title_font, text, grey);
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

// static Texture* LoadPngTex(SDL_Renderer* renderer, char const* fPath) {
// 	SDL_Surface* surf = IMG_Load(fPath);
// 	if (!surf) return nullptr;
// 	Uint32 colorkey = SDL_MapRGB(surf->format, 0, 0, 0);
// 	SDL_SetColorKey(surf, SDL_TRUE, colorkey);
// 	SDL_Texture* new_tex = SDL_CreateTextureFromSurface(renderer, surf);
// 	SDL_FreeSurface(surf);
// 	int w, h;
// 	SDL_QueryTexture(new_tex, NULL, NULL, &w, &h);
// 	Texture* tex = new Texture(new_tex, w, h);

// 	return tex;
// }

void Renderer::PlaySound(sound_e sName) {
	switch (sName) {
		case HIT:
			Mix_PlayMusic(hit_sound, 1);
			break;

		case DIE:
			Mix_PlayMusic(die_sound, 1);
			break;

		default:
			break;
	}
}