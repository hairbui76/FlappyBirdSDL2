#pragma once
#include "defs.h"

class Renderer;
struct EntityManager;
class EventManager;
class Entity;

void renderSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full = false);
void handAnimationSystem(Entity* entity, Renderer* renderer, EventManager* eventManager, EntityManager* entMan, int layer);
void autoAnimationSystem(Entity* entity, Renderer* renderer, int layer);
void spawnerSystem(Entity* entity, Renderer* renderer, int layer);
void timelineTickSystem(Entity* entity, EventManager* eventManager);
void HudSystem(Entity* entity, Renderer* renderer);