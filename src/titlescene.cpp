#include "titlescene.h"
#include "component.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

TitleScene::TitleScene(EventManager* eventManager) {
	entMan = new EntityManager;
	this->eventManager = eventManager;
	// background
	Entity* entity = new Entity;
	entity->position = new PositionComponent(0.0, 0.0);
	entity->sprite = new SpriteComponent(TEX_BCKGND, 1.0, 0);
	entMan->entities.push_back(entity);
}

TitleScene::~TitleScene() {
	delete (entMan);
}

void TitleScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		BlitSpriteSystem(entity, renderer, 0, true);
	}
}

void TitleScene::Tick() {
	for (auto entity : entMan->entities) {
		AngleTickSystem(entity);
	}
}

void TitleScene::Responder(Event* event, EventManager* eventManager) {
	if (event->type == KEYDOWN && !strcmp(event->data, " "))
		eventManager->Post(new Event(CHANGE_SCENE, "GAME_SCENE"));

	if (event->type == MOUSE_BUTT)
		eventManager->Post(new Event(CHANGE_SCENE, "GAME_SCENE"));
}
