#include "entity.h"
#include "component.h"

Entity::~Entity() {
	delete position;
	delete sprite;
	delete angle;
	delete size;
	delete flappyPhysics;
	delete flappyInput;
	delete spriteSpan;
	delete anim;
	delete pipeSpawn;
	delete pipe;
	delete pipeSprite;
	delete collidable;
	delete score;
	delete scoreListener;
}

EntityManager::~EntityManager() {
	for (auto entity : entities) {
		delete entity;
	}
	entities.clear();
}