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

void logSDLError(const char* msg, const char* err, ...) {
	va_list args;
	va_start(args, err);
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, msg, err, va_arg(args, const char*));
	va_end(args);
	exit(0);
}

std::string getTextureTag(texture_e tag) {
	switch (tag) {
		case TEX_BG:
			return "TEX_BG";
		case TEX_BRANCH:
			return "TEX_BRANCH";
		case TEX_LOG:
			return "TEX_LOG";
		case TEX_STONE:
			return "TEX_STONE";
		case TEX_TRUNK:
			return "TEX_TRUNK";
		case TEX_PLAY:
			return "TEX_PLAY";
		case TEX_REFRESH:
			return "TEX_REFRESH";
		case TEX_LEFT:
			return "TEX_LEFT";
		case TEX_RIGHT:
			return "TEX_RIGHT";
		case TEX_LUMBER_DEAD:
			return "TEX_LUMBER_DEAD";
		case TEX_LUMBER_HOLDING:
			return "TEX_LUMBER_HOLDING";
		case TEX_LUMBER_CUTTING:
			return "TEX_LUMBER_CUTTING";
		case TEX_TEXT_TITLE:
			return "TEX_TEXT_TITLE";
		case TEX_TEXT_END:
			return "TEX_TEXT_END";
		default:
			return "";
	}
}
