#include "gamescene.h"
#include "component.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

GameScene::GameScene(EventManager*& eventManager) {
	this->eventManager = eventManager;

	entMan = new EntityManager;
	maxScore = 0;
	restartFlag = false;
	populateEntity(entMan);
}

GameScene::~GameScene() {
	delete (entMan);
}

void GameScene::DoFrame(Renderer*& renderer) {
	for (auto entity : entMan->entities) {
		// tile scene (layer 0)
		renderSpriteSystem(entity, renderer, 0, true);
	}

	// for (auto entity : entMan->entities) {
	// 	// status bar (the score bar)
	// 	HudSystem(entity, renderer);
	// }
}

void GameScene::Tick() {
}

void GameScene::Responder(Event*& event, EventManager*& eventManager) {
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

void GameScene::populateEntity(EntityManager*& entMan) {
	Entity* ent = new Entity;
	// bckgnd
	ent->position = new PositionComponent(0.0, 0.0);
	ent->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	entMan->entities.push_back(ent);

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
