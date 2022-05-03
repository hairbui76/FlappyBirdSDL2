#pragma once
#include "defs.h"

struct Component;
struct ComponentContainer;
struct PositionComponent;
struct SpriteComponent;

struct Entity {
	Component* position = nullptr;
	Component* sprite = nullptr;
	Component* angle = nullptr;
	Component* size = nullptr;
	Component* flappyPhysics = nullptr;
	Component* flappyInput = nullptr;
	Component* spriteSpan = nullptr;
	Component* anim = nullptr;
	Component* pipeSpawn = nullptr;
	Component* pipe = nullptr;
	Component* pipeSprite = nullptr;
	Component* collidable = nullptr;
	Component* score = nullptr;
	Component* scoreListener = nullptr;

	Entity() = default;
	~Entity();
};

struct EntityManager {
	EntityManager() = default;
	~EntityManager();

	std::vector<Entity*> entities;
};
