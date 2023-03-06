#include "endscene.h"
#include "component.h"
#include "defs.h"
#include "entity.h"
#include "system.h"
#include <iostream>

EndScene::EndScene(EventManager* eventManager, move_tag_e move_state, int score) {
	this->eventManager = eventManager;

	entMan = new EntityManager();
	populateEntity(entMan, move_state, score);
}

// EndScene::~EndScene() {
// 	delete entMan;
// }

void EndScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		renderSpriteSystem(entity, renderer, 0);
		HudSystem(entity, renderer);
	}
}

void EndScene::populateEntity(EntityManager* entMan, move_tag_e move_state, int score) {
	//! NOTICE that ALL position are self calculated
	//! BUT ALL size are shrinked a half to fit the scene

	// background
	Entity* ent1 = new Entity();
	ent1->position = new PositionComponent(0.0, 0.0);
	ent1->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	ent1->size = new SizeComponent(WIN_X, WIN_Y - 200);
	entMan->entities.push_back(ent1);

	// the log
	Entity* ent2 = new Entity();
	ent2->position = new PositionComponent((WIN_X - 50) / 2, WIN_Y - 307);
	ent2->sprite = new SpriteComponent(TEX_LOG, 1.0, 0);
	ent2->size = new SizeComponent(50, 50);
	entMan->entities.push_back(ent2);

	// the stone
	Entity* ent3 = new Entity();
	ent3->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent3->sprite = new SpriteComponent(TEX_STONE, 1.0, 0);
	ent3->size = new SizeComponent(75, 36);
	entMan->entities.push_back(ent3);

	// lumberjack dead
	Entity* ent4 = new Entity();
	if (move_state == RIGHT) {
		ent4->position = new PositionComponent((WIN_X - 50) / 2 + 60, WIN_Y - 340);
		ent4->sprite = new SpriteComponent(TEX_LUMBER_DEAD, 1.0, 0);
	} else {
		ent4->position = new PositionComponent((WIN_X - 50) / 2 - 80, WIN_Y - 340);
		ent4->sprite = new SpriteComponent(TEX_LUMBER_DEAD, 1.0, 0, SDL_FLIP_HORIZONTAL);
	}
	ent4->size = new SizeComponent(70, 85);
	entMan->entities.push_back(ent4);

	// refresh button
	Entity* ent5 = new Entity();
	ent5->position = new PositionComponent((WIN_X - 120) / 2, WIN_Y - 160);
	ent5->sprite = new SpriteComponent(TEX_REFRESH, 1.0, 0);
	ent5->size = new SizeComponent(120, 120);
	ent5->clickable = new ClickableComponent();
	ClickListenerComponent* clc = new ClickListenerComponent(ent5, eventManager);
	eventManager->AddListener(clc);
	ent5->clickListener = clc;
	entMan->entities.push_back(ent5);

	// score
	// Entity* ent6 = new Entity();
	// ent6->sprite = new SpriteComponent(TEX_TEXT_END, 1.0, 0);
	// ent6->position = new PositionComponent(PositionComponent((WIN_X - Renderer::GetTexture(TEX_TEXT_END)->w) / 2, WIN_Y / 3));
	// entMan->entities.push_back(ent6);
}

void EndScene::Responder(Event* event, EventManager* eventManager) {
	if (event->type == BUTT_CLICK) {
		for (auto entity : entMan->entities) {
			if (entity->clickable) {
				ClickableComponent* clickable = (ClickableComponent*)entity->clickable;
				if (clickable->isClicked) {
					if (entity->sprite) {
						SpriteComponent* sprite = (SpriteComponent*)entity->sprite;
						if (sprite->tName == TEX_REFRESH)
							eventManager->Post(new Event(KEYDOWN, "SPACE"));
						clickable->isClicked = false;
						break;
					}
				}
			}
		}
	}
	if (event->type == KEYDOWN && !strcmp(event->data, "SPACE")) {
		eventManager->Post(new Event(CHANGE_SCENE, "GAME_SCENE"));
	}
}
