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
	PopulateWithInitEnts(entMan);
}

GameScene::~GameScene() {
	delete (entMan);
}

void GameScene::DoFrame(Renderer* renderer) {
	try {
		// std::cout << entMan->entities.size() << std::endl;
		for (auto entity : entMan->entities) {
			// tile scene (layer 0)
			BlitSpriteSystem(entity, renderer, 0, true);
		}

		for (auto entity : entMan->entities) {
			// pipe
			PipeSpriteSystem(entity, renderer);
		}

		for (auto entity : entMan->entities) {
			// ground
			BlitSpriteSystem(entity, renderer, 1);
		}

		// for (auto entity : entMan->entities) {
		// 	// bird
		// 	BlitSpriteSystem(entity, renderer, 2);
		// }

		// for (auto entity : entMan->entities) {
		// 	// status bar (the score bar)
		// 	HudSystem(entity, renderer);
		// }
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void GameScene::Tick() {
	for (auto entity : entMan->entities) {
		FlappyPhysicsSystem(entity);
		if (!restartFlag) {
			PipeSpawnerTickSystem(entity, eventManager);
			PipeTickSystem(entity);
			// CollisionHandlerSystem(entMan, entity, eventManager);
		}
	}
}

void GameScene::Responder(Event* event, EventManager* eventManager) {
	// std::cout << event->type << std::endl;
	switch (event->type) {
		case KEYDOWN:
			if (strcmp(event->data, "ENTER") == 0) {
				Restart(eventManager);
			} else if (restartFlag) {
				Restart(eventManager);
			} else {
				for (auto entity : entMan->entities) {
					FlappyInputSystem(entity);
				}
			}
			break;

		case MOUSE_BUTT:
			// if (restartFlag)
			// 	Restart(eventManager);
			// else {
			for (auto entity : entMan->entities) {
				FlappyInputSystem(entity);
			}
			// }
			break;

		case SPAWN_PIPE:
			SpawnPipe();
			break;

		case GAME_RESTART:
			restartFlag = true;
			// DoPreRestart(entMan);
			break;

		default:
			break;
	}
}

void GameScene::PopulateWithInitEnts(EntityManager* entMan) {
	Entity* ent = new Entity;
	// bckgnd
	ent->position = new PositionComponent(0.0, 0.0);
	ent->sprite = new SpriteComponent(TEX_BCKGND, 1.0, 0);
	entMan->entities.push_back(ent);

	Entity* ent1 = new Entity;
	// gnd
	ent1->position = new PositionComponent(0.0, WIN_Y - (Renderer::GetTexture(TEX_GND)->h / 2));
	ent1->sprite = new SpriteComponent(TEX_GND, 1.0, 1);
	ent1->spriteSpan = new SpriteSpanComponent(22);
	ent1->anim = new AnimComponent(50, -0.1, 48, 160, 1);
	entMan->entities.push_back(ent1);

	// Entity* ent2 = new Entity;
	// // flappy
	// ent2->position = new PositionComponent(366.0, (WIN_Y - 82) / 2);
	// ent2->angle = new RotateComponent(0.0, 0.0);
	// ent2->sprite = new SpriteComponent(TEX_FLAP, 1.0, 2);
	// ent2->flappyPhysics = new FlappyPhysicsComponent;
	// ent2->flappyInput = new FlappyInputComponent(eventManager);
	// ent2->collidable = new CollidableComponent;
	// ent2->size = new SizeComponent(110.0, 80.0);
	// entMan->entities.push_back(ent2);

	Entity* ent3 = new Entity;
	// pipe spawner
	ent3->pipeSpawn = new PipeSpawnerComponent(-0.01);
	entMan->entities.push_back(ent3);

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

void GameScene::Restart(EventManager* eventManager) {
	restartFlag = false;
	for (auto entity : entMan->entities) {
		EventListener* e = (EventListener*)(entity->scoreListener);
		eventManager->RemoveListener(e - 1); // offset due to multiple inheritance
	}

	delete (entMan);
	entMan = new EntityManager;
	PopulateWithInitEnts(entMan);
}

void GameScene::SpawnPipe() {
	double offset = (double)getRandom(10, WIN_Y - PIPE_GAP);
	Entity* entity = new Entity;
	entity->position = new PositionComponent(950.0, 0.0);
	entity->pipeSprite = new PipeSpriteComponent(TEX_PIPE);
	entity->pipe = new PipeComponent(offset, -6.0);
	entity->collidable = new CollidableComponent;
	entity->size = new SizeComponent(160.0, PIPE_GAP);
	entMan->entities.push_back(entity);
}

void GameScene::DoPreRestart(EntityManager* entMan) {
	for (auto entity : entMan->entities) {
		// find flappy
		if (entity->flappyPhysics) {
			RotateComponent* angle = (RotateComponent*)entity->angle;
			FlappyPhysicsComponent* fpy = (FlappyPhysicsComponent*)entity->flappyPhysics;

			delete (entity->flappyPhysics);
			angle->angle = 90.0;
			angle->angleAcc = 0.0;
			fpy->yAcc = 15.0;
		}

		if (entity->pipe)
			delete (entity->pipe);
		if (entity->pipeSpawn)
			delete (entity->pipeSpawn);
		if (entity->anim)
			delete (entity->anim);
		if (entity->score) {
			ScoreComponent* scr = (ScoreComponent*)entity->score;
			if (scr->score > maxScore)
				maxScore = scr->score;
		}
	}
}
