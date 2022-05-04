#pragma once
#include "defs.h"

struct Component;

struct Entity {
	Component* position = nullptr;
	Component* sprite = nullptr;
	Component* angle = nullptr;
	Component* size = nullptr;
	Component* score = nullptr;
	Component* scoreListener = nullptr;
	Component* clickListener = nullptr;
	Component* clickable = nullptr;

	Entity() = default;
	~Entity();
};

struct EntityManager {
	EntityManager() = default;
	~EntityManager();

	std::vector<Entity*> entities;
};
