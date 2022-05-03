#pragma once
#include <SDL.h>
#include <cstring>
#include <queue>
#include <random>
#include <time.h>
#include <vector>

#define WIN_X 540
#define WIN_Y 765
#define TICK_RATE 60.0
#define PIPE_GAP 312.0
#define MAX_LAYER 3

int getRandom(int a, int b);
void logSDLError();

enum component_tag_e {
	POSITION,
	SPRITE,
	ANGLE,
	SIZE,
	MASK_TICK,
	FLAPPY_PHYSICS,
	FLAPPY_INPUT,
	SPRITE_SPAN,
	ANIM,
	PIPE_SPAWN,
	PIPE,
	PIPE_SPRITE,
	COLLIDABLE,
	SCORE,
	SCORELISTENER
};

enum texture_e {
	TEX_BCKGND,
	TEX_FLAP,
	TEX_MENU,
	TEX_PIPE,
	TEX_GND,
	DUNNO
};

enum scene_e {
	TITLE,
	GAME
};
