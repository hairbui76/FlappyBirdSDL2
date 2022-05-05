#pragma once
#include "defs.h"

class Renderer;
struct EntityManager;
class EventManager;
class Entity;

void renderSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full = false);
void HudSystem(Entity* entity, Renderer* renderer);