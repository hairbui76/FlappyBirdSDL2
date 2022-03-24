#pragma once
#include "defs.h"

struct Component;
struct ComponentContainer;
struct PositionComponent;
struct SpriteComponent;

struct Entity {
	Component* position;
	Component* sprite;
	Component* angle;
	Component* size;
	Component* flappyPhysics;
	Component* flappyInput;
	Component* spriteSpan;
	Component* anim;
	Component* pipeSpawn;
	Component* pipe;
	Component* pipeSprite;
	Component* collidable;
	Component* score;
	Component* scoreListener;

	Entity() = default;
	~Entity();
};

struct EntityManager {
	EntityManager() = default;
	~EntityManager();

	std::vector<Entity*> entities;
};
