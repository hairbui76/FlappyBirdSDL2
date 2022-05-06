#include "endscene.h"
#include "component.h"
#include "defs.h"
#include "entity.h"
#include "system.h"

EndScene::EndScene(EventManager* eventManager, move_tag_e move_state) {
	this->eventManager = eventManager;

	entMan = new EntityManager;
	maxScore = 0;
	// restartFlag = false;
	populateEntity(entMan, move_state);
}

void EndScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		renderSpriteSystem(entity, renderer, 0);
	}
}

void EndScene::populateEntity(EntityManager* entMan, move_tag_e move_state) {
	//! NOTICE that ALL position are self calculated
	//! BUT ALL size are shrinked a half to fit the scene

	// background
	Entity* ent1 = new Entity;
	ent1->position = new PositionComponent(0.0, 0.0);
	ent1->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	ent1->size = new SizeComponent(WIN_X, WIN_Y - 200);
	entMan->entities.push_back(ent1);

	// the log
	Entity* ent2 = new Entity;
	ent2->position = new PositionComponent((WIN_X - 50) / 2, WIN_Y - 307);
	ent2->sprite = new SpriteComponent(TEX_LOG, 1.0, 0);
	ent2->size = new SizeComponent(50, 50);
	entMan->entities.push_back(ent2);

	// the stone
	Entity* ent3 = new Entity;
	ent3->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent3->sprite = new SpriteComponent(TEX_STONE, 1.0, 0);
	ent3->size = new SizeComponent(75, 36);
	entMan->entities.push_back(ent3);

	// lumberjack dead
	Entity* ent4 = new Entity;
	if (move_state == RIGHT) {
		ent4->position = new PositionComponent((WIN_X - 50) / 2 + 60, WIN_Y - 340);
		ent4->sprite = new SpriteComponent(TEX_LUMBER_DEAD, 1.0, 0);
	} else {
		ent4->position = new PositionComponent((WIN_X - 50) / 2 - 80, WIN_Y - 340);
		ent4->sprite = new SpriteComponent(TEX_LUMBER_DEAD, 1.0, 0, SDL_FLIP_HORIZONTAL);
	}
	ent4->size = new SizeComponent(70, 85);
	entMan->entities.push_back(ent4);
}

void EndScene::Responder(Event* event, EventManager* eventManager) {
}