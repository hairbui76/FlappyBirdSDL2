#include "entity.h"
#include "component.h"
#include <iostream>

Entity::Entity() {
	this->position = nullptr;
	this->sprite = nullptr;
	this->angle = nullptr;
	this->size = nullptr;
	this->score = nullptr;
	this->scoreListener = nullptr;
	this->clickListener = nullptr;
	this->clickable = nullptr;
	this->movable = nullptr;
	this->moveListener = nullptr;
	this->spawner = nullptr;
	this->spawnerListener = nullptr;
	this->cuttable = nullptr;
	this->animation = nullptr;
	this->animationListener = nullptr;
	this->dead = nullptr;
	this->deadListener = nullptr;
}

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
	delete spawnerListener;
	delete cuttable;
	delete animation;
	delete animationListener;
	delete dead;
	delete deadListener;
}

// EntityManager::~EntityManager() {
// 	for (auto entity : entities) {
// 		delete entity;
// 	}
// 	entities.clear();
// }