#pragma once
#include <SDL.h>
#include <cstring>
#include <queue>
#include <random>
#include <time.h>
#include <vector>

#define WIN_X 600
#define WIN_Y 757

enum component_tag_e {
	SPRITE,
	SCORE,
	SCORELISTENER,
	POSITION,
	SIZE,
	ANGLE,
	CLICKABLE,
};

enum texture_e {
	TEX_BG,
	TEX_BRANCH,
	TEX_LOG,
	TEX_STONE,
	TEX_TRUNK,
	TEX_PLAY,
	TEX_REFRESH,
	TEX_LEFT,
	TEX_RIGHT,
	TEX_LUMBER_DEAD,
	TEX_LUMBER_BODY,
	TEX_HAND_DOWN,
	TEX_HAND_UP,
	TEX_TEXT_TITLE
};

enum scene_e {
	TITLE,
	GAME
};

int getRandom(int a, int b);
void logSDLError(const char* msg, const char* err, ...);
std::string getTextureTag(texture_e tag);