#include "entity.h"
#include "component.h"

Entity::~Entity() {
	delete position;
	delete sprite;
	delete angle;
	delete size;
	delete score;
	delete scoreListener;
}

EntityManager::~EntityManager() {
	for (auto entity : entities) {
		delete entity;
	}
	entities.clear();
}