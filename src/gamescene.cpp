#include "gamescene.h"
#include "component.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

GameScene::GameScene(EventManager* eventManager) {
	this->eventManager = eventManager;

	entMan = new EntityManager;
	maxScore = 0;
	restartFlag = false;
	populateEntity(entMan);
}

GameScene::~GameScene() {
	delete (entMan);
}

void GameScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		// background (layer 0)
		renderSpriteSystem(entity, renderer, 0);
		// branch (layer 1)
		renderSpriteSystem(entity, renderer, 1);
		// trunk (layer 2)
		renderSpriteSystem(entity, renderer, 2);
		// lumberjack (and stone) (layer 3)
		renderSpriteSystem(entity, renderer, 3);
	}

	// for (auto entity : entMan->entities) {
	// 	// status bar (the score bar)
	// 	HudSystem(entity, renderer);
	// }
}

void GameScene::Tick() {
}

void GameScene::Responder(Event* event, EventManager* eventManager) {
	// std::cout << event->type << std::endl;
	switch (event->type) {
		case KEYDOWN:
			if (strcmp(event->data, "ENTER") == 0) {
				eventManager->Post(new Event(CHANGE_SCENE, "GAME_SCENE"));
				// Restart(eventManager);
			} else if (restartFlag) {
				// Restart(eventManager);
			} else {
			}
			break;

		case MOUSE_BUTT:
			// if (restartFlag)
			// 	Restart(eventManager);
			// else {
			// }
			break;

		case GAME_RESTART:
			restartFlag = true;
			// DoPreRestart(entMan);
			break;

		default:
			break;
	}
}

void GameScene::populateEntity(EntityManager* entMan) {
	// background
	Entity* ent1 = new Entity;
	ent1->position = new PositionComponent(0.0, 0.0);
	ent1->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	ent1->size = new SizeComponent(WIN_X, WIN_Y - 200);
	entMan->entities.push_back(ent1);

	// branch

	// the trunk
	Entity* ent2 = new Entity;
	ent2->position = new PositionComponent((WIN_X - 50) / 2, 0);
	ent2->sprite = new SpriteComponent(TEX_TRUNK, 1.0, 1);
	ent2->size = new SizeComponent(50, 500);
	entMan->entities.push_back(ent2);

	// the stone
	Entity* ent3 = new Entity;
	ent3->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent3->sprite = new SpriteComponent(TEX_STONE, 1.0, 2);
	ent3->size = new SizeComponent(75, 36);
	entMan->entities.push_back(ent3);

	// lumberjack holding axe
	Entity* ent4 = new Entity;
	ent4->position = new PositionComponent((WIN_X - 50) / 2 + 60, WIN_Y - 365);
	ent4->sprite = new SpriteComponent(TEX_LUMBER_HOLDING, 1.0, 2);
	ent4->size = new SizeComponent(70, 107);
	entMan->entities.push_back(ent4);

	// left button
	Entity* ent5 = new Entity;
	ent5->position = new PositionComponent((WIN_X - 120) / 5, WIN_Y - 160);
	ent5->sprite = new SpriteComponent(TEX_LEFT, 1.0, 0);
	ent5->size = new SizeComponent(120, 120);
	ent5->clickable = new ClickableComponent();
	ClickListenerComponent* clc1 = new ClickListenerComponent(ent5);
	eventManager->AddListener(clc1);
	ent5->clickListener = clc1;
	entMan->entities.push_back(ent5);

	// right button
	Entity* ent6 = new Entity;
	ent6->position = new PositionComponent((WIN_X - 120) / 5 * 4, WIN_Y - 160);
	ent6->sprite = new SpriteComponent(TEX_RIGHT, 1.0, 0);
	ent6->size = new SizeComponent(120, 120);
	ent6->clickable = new ClickableComponent();
	ClickListenerComponent* clc2 = new ClickListenerComponent(ent6);
	eventManager->AddListener(clc2);
	ent6->clickListener = clc2;
	entMan->entities.push_back(ent6);

	// Entity* ent4 = new Entity;
	// // score
	// ent4->position = new PositionComponent(40.0, 20.0);
	// ent4->size = new SizeComponent(0.0, 0.0);
	// ScoreComponent* scr = new ScoreComponent(maxScore);
	// ScoreListenerComponent* slc = new ScoreListenerComponent(scr);
	// eventManager->AddListener(slc);
	// ent4->score = scr;
	// ent4->scoreListener = slc;
	// entMan->entities.push_back(ent4);
}

// void GameScene::Restart(EventManager* eventManager) {
// 	restartFlag = false;
// 	for (auto entity : entMan->entities) {
// 		EventListener* e = (EventListener*)(entity->scoreListener);
// 		eventManager->RemoveListener(e - 1); // offset due to multiple inheritance
// 	}

// 	delete (entMan);
// 	entMan = new EntityManager;
// 	populateEntity(entMan);
// }

// void GameScene::DoPreRestart(EntityManager* entMan) {
// 	for (auto entity : entMan->entities) {
// 		// find flappy
// 		if (entity->flappyPhysics) {
// 			RotateComponent* angle = (RotateComponent*)entity->angle;
// 			FlappyPhysicsComponent* fpy = (FlappyPhysicsComponent*)entity->flappyPhysics;

// 			delete (entity->flappyPhysics);
// 			angle->angle = 90.0;
// 			angle->angleAcc = 0.0;
// 			fpy->yAcc = 15.0;
// 		}

// 		if (entity->pipe)
// 			delete (entity->pipe);
// 		if (entity->pipeSpawn)
// 			delete (entity->pipeSpawn);
// 		if (entity->anim)
// 			delete (entity->anim);
// 		if (entity->score) {
// 			ScoreComponent* scr = (ScoreComponent*)entity->score;
// 			if (scr->score > maxScore)
// 				maxScore = scr->score;
// 		}
// 	}
// }
