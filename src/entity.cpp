#include "entity.h"
#include "component.h"

Entity::~Entity() {
	delete position;
	delete sprite;
	delete angle;
	delete size;
	delete score;
	delete scoreListener;
	delete clickable;
	delete clickListener;
	delete movable;
	delete moveListener;
	delete spawner;
}

EntityManager::~EntityManager() {
	for (auto entity : entities) {
		delete entity;
	}
	entities.clear();
}