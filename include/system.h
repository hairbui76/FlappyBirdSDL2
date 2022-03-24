#pragma once
#include "defs.h"

class Renderer;
struct EntityManager;
class EventManager;
class Entity;

void BlitSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full = false, SDL_RendererFlip flip_flag = SDL_FLIP_NONE);
void AngleTickSystem(Entity* entity);
void FlappyPhysicsSystem(Entity* entity);
void FlappyInputSystem(Entity* entity);
void PipeSpawnerTickSystem(Entity* entity, EventManager* eventManager);
void PipeTickSystem(Entity* entity);
void PipeSpriteSystem(Entity* entity, Renderer* renderer);
void CollisionHandlerSystem(EntityManager* entMan, Entity* entity, EventManager* eventManager);
void HudSystem(Entity* entity, Renderer* renderer);
