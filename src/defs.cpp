#include "defs.h"
#include <SDL.h>
#include <cstdlib>
#include <ctime>

void InitRandom() {
	srand(time(NULL));
}

int GetRandom(int a, int b) {
	return (rand() + a) % b;
}

void logSDLError(const char* msg, const char* err) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, msg, err);
	exit(0);
}
