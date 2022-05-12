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
	Component* clickListener;
	Component* clickable;
	Component* movable;
	Component* moveListener;
	Component* spawner;
	Component* spawnerListener;
	Component* cuttable;
	Component* animation;
	Component* animationListener;
	Component* dead;
	Component* deadListener;

	Entity();
	~Entity();
};

struct EntityManager {
	EntityManager() = default;
	~EntityManager();

	std::vector<Entity*> entities;
};