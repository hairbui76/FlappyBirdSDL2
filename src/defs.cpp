#include "defs.h"

// return reference to the minstd_rand class
// so that we use static
std::minstd_rand& random_engine() {
	static std::minstd_rand eng{static_cast<unsigned int>(time(0))};
	return eng;
}

int getRandom(int a, int b) {
	return random_engine()() % (b - a + 1) + a;
}

void logSDLError(const char* msg, const char* err) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, msg, err);
	exit(0);
}
