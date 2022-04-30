#pragma once
#include "defs.h"

class Renderer;
struct EntityManager;
class EventManager;
class Entity;

void renderSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full = false, SDL_RendererFlip flip_flag = SDL_FLIP_NONE);
void HudSystem(Entity* entity, Renderer* renderer);
