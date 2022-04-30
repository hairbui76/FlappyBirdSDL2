#pragma once
#include "defs.h"

struct Component;

struct Entity {
	Component* position;
	Component* sprite;
	Component* angle;
	Component* size;
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
