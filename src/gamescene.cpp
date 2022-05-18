#include "gamescene.h"
#include "component.h"
#include "defs.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

GameScene::GameScene(EventManager* eventManager) {
	this->eventManager = eventManager;

	entMan = new EntityManager();
	maxScore = 0;
	restartFlag = false;
	populateEntity(entMan);
}

GameScene::~GameScene() {
	delete entMan;
}

void GameScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		// background (layer 0)
		renderSpriteSystem(entity, renderer, 0);
		// branch and trunk (layer 1 & 2)
		spawnerSystem(entity, renderer, 1);
		// stone (layer 3)
		renderSpriteSystem(entity, renderer, 3);
		// lumberjack (layer 3)
		handAnimationSystem(entity, renderer, eventManager, 3);
		// timeline
		timelineTickSystem(entity, renderer, eventManager);
		// timeline bar (layer 4)
		renderSpriteSystem(entity, renderer, 4);
		// timeline (layer 5)
		renderSpriteSystem(entity, renderer, 5);
		// score
		HudSystem(entity, renderer);
	}
}

void GameScene::Responder(Event* event, EventManager* eventManager) {
	if (event->type == BUTT_CLICK) {
		for (auto entity : entMan->entities) {
			if (entity->clickable) {
				ClickableComponent* clickable = (ClickableComponent*)entity->clickable;
				if (clickable->isClicked) {
					if (entity->sprite) {
						SpriteComponent* sprite = (SpriteComponent*)entity->sprite;
						if (sprite->tName == TEX_LEFT)
							eventManager->Post(new Event(KEYDOWN, "LEFT"));
						else if (sprite->tName == TEX_RIGHT)
							eventManager->Post(new Event(KEYDOWN, "RIGHT"));
						clickable->isClicked = false;
						break;
					}
				}
			}
		}
	}
	if (event->type == KEYDOWN && (!strcmp(event->data, "LEFT") || !strcmp(event->data, "RIGHT"))) {
		bool dead_state = false;
		for (auto entity : entMan->entities) {
			if (entity->spawner) {
				SpawnerComponent* spawner = (SpawnerComponent*)entity->spawner;
				if (spawner->spawner_entities[0].second) {
					SpriteComponent* sprite = (SpriteComponent*)spawner->spawner_entities[0].second->sprite;
					if (!strcmp(event->data, "LEFT") && (sprite->flip_flag == SDL_FLIP_HORIZONTAL)) {
						dead_state = true;
					} else if (!strcmp(event->data, "RIGHT") && (sprite->flip_flag == SDL_FLIP_NONE)) {
						dead_state = true;
					} else
						eventManager->Post(new Event(SPAWN_BRANCH, ""));
				} else if (spawner->spawner_entities[1].second) {
					SpriteComponent* sprite = (SpriteComponent*)spawner->spawner_entities[1].second->sprite;
					if (!strcmp(event->data, "LEFT") && (sprite->flip_flag == SDL_FLIP_HORIZONTAL)) {
						dead_state = true;
					} else if (!strcmp(event->data, "RIGHT") && (sprite->flip_flag == SDL_FLIP_NONE)) {
						dead_state = true;
					}
					eventManager->Post(new Event(SPAWN_BRANCH, ""));
				} else {
					eventManager->Post(new Event(SPAWN_BRANCH, ""));
				}
				break;
			}
		}
		if (dead_state) {
			// find dead component lumberjack
			for (auto entity : entMan->entities) {
				if (entity->dead) {
					DeadComponent* dead = (DeadComponent*)entity->dead;
					dead->is_dead = true;
					break;
				}
			}
		}
	}
	if (event->type == SPAWN_BRANCH) {
		eventManager->Post(new Event(INC_SCORE, ""));
		// find timeline bar
		for (auto entity : entMan->entities) {
			if (entity->shrinkable) {
				ShrinkableComponent* shrinkable = (ShrinkableComponent*)entity->shrinkable;
				if (entity->size) {
					SizeComponent* size = (SizeComponent*)entity->size;
					size->w += 5;
					if (size->w > 88) size->w = 88;
				}
				if (entity->cuttable) {
					CuttableComponent* cuttable = (CuttableComponent*)entity->cuttable;
					cuttable->cut_width += 5;
					if (cuttable->cut_width > Renderer::GetTexture(TEX_TIMELINE_BAR)->w) cuttable->cut_width = Renderer::GetTexture(TEX_TIMELINE_BAR)->w;
				}
				break;
			}
		}
	}
	if (event->type == GAME_OVER) {
		// find dead component
		for (auto entity : entMan->entities) {
			if (entity->dead) {
				DeadComponent* dead = (DeadComponent*)entity->dead;
				dead->is_dead = true;
				break;
			}
		}
	}
}

void GameScene::populateEntity(EntityManager* entMan) {
	//! NOTICE that ALL position are self calculated
	//! BUT ALL size are shrinked a half to fit the scene

	// background
	Entity* ent1 = new Entity();
	ent1->position = new PositionComponent(0.0, 0.0);
	ent1->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	ent1->size = new SizeComponent(WIN_X, WIN_Y - 200);
	entMan->entities.push_back(ent1);

	// branch and trunk
	Entity* ent2 = new Entity();
	std::vector<SDL_RendererFlip> flip_flags;
	SpawnerComponent* spawner = new SpawnerComponent;
	// trunk
	for (int i = 1; i <= MAX_SPAWNER_COMPONENTS; i++) {
		Entity* spawner_entity1 = new Entity();
		spawner_entity1->position = new PositionComponent((WIN_X - 50) / 2, WIN_Y - 265 - Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS * i);
		spawner_entity1->cuttable = new CuttableComponent(0, Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS * (MAX_SPAWNER_COMPONENTS - i), Renderer::GetTexture(TEX_TRUNK)->w, Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS);
		spawner_entity1->size = new SizeComponent(50, Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS);
		spawner_entity1->sprite = new SpriteComponent(TEX_TRUNK, 1.0, 2);

		spawner->spawner_entities.push_back(std::make_pair(spawner_entity1, nullptr));
	}
	// branch
	for (int i = 0; i < MAX_SPAWNER_COMPONENTS; i++) {
		if (i % 2 != 0) {
			Entity* spawner_entity2 = new Entity();
			SDL_RendererFlip flip_flag = getRandom(0, 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
			if (flip_flag == SDL_FLIP_NONE)
				spawner_entity2->position = new PositionComponent(WIN_X / 2 + 10, WIN_Y - 340 - i * 80);
			else
				spawner_entity2->position = new PositionComponent(WIN_X / 2 - 135, WIN_Y - 340 - i * 80);
			spawner_entity2->sprite = new SpriteComponent(TEX_BRANCH, 1.0, 1, flip_flag);
			spawner_entity2->size = new SizeComponent(125, 80);

			spawner->spawner_entities[i].second = spawner_entity2;
		}
	}
	ent2->spawner = spawner;
	SpawnerListenerComponent* slc = new SpawnerListenerComponent(ent2);
	ent2->spawnerListener = slc;
	eventManager->AddListener(slc);
	entMan->entities.push_back(ent2);

	// animation when cutting wood
	Entity* ent3 = new Entity();
	ent3->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent3->size = new SizeComponent(125, 80);
	ent3->autoAnimation = new AutoAnimationComponent(ent3);

	// the stone
	Entity* ent4 = new Entity();
	ent4->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent4->sprite = new SpriteComponent(TEX_STONE, 1.0, 3);
	ent4->size = new SizeComponent(75, 36);
	entMan->entities.push_back(ent4);

	// lumberjack
	Entity* ent5 = new Entity();
	ent5->dead = new DeadComponent();
	// handle moving event
	ent5->movable = new MovableComponent();
	MoveListenerComponent* mcl = new MoveListenerComponent(ent5);
	ent5->moveListener = mcl;
	eventManager->AddListener(mcl);
	// handle animation
	HandAnimationComponent* handAnimation = new HandAnimationComponent();
	// lumberjack holding axe
	Entity* hand_animation_entity1 = new Entity();
	hand_animation_entity1->position = new PositionComponent((WIN_X - 50) / 2 + 65, WIN_Y - 350);
	hand_animation_entity1->sprite = new SpriteComponent(TEX_LUMBER_HOLDING, 1.0, 3);
	hand_animation_entity1->size = new SizeComponent(70, 107);
	handAnimation->hand_animation_entities.push_back(hand_animation_entity1);
	// lumberjack cutting axe
	Entity* hand_animation_entity2 = new Entity();
	hand_animation_entity2->position = new PositionComponent((WIN_X - 50) / 2 + 30, WIN_Y - 350);
	hand_animation_entity2->sprite = new SpriteComponent(TEX_LUMBER_CUTTING, 1.0, 3);
	hand_animation_entity2->size = new SizeComponent(87, 107);
	handAnimation->hand_animation_entities.push_back(hand_animation_entity2);
	ent5->handAnimation = handAnimation;
	// handle animation event
	HandAnimationListenerComponent* alc = new HandAnimationListenerComponent(ent5);
	ent5->handAnimationListener = alc;
	eventManager->AddListener(alc);
	entMan->entities.push_back(ent5);

	// timeline bar
	Entity* ent6 = new Entity();
	ShrinkableComponent* shrinkable = new ShrinkableComponent();
	ShrinkListenerComponent* slc2 = new ShrinkListenerComponent(shrinkable);
	eventManager->AddListener(slc2);
	ent6->shrinkable = shrinkable;
	ent6->cuttable = new CuttableComponent(0, 0, Renderer::GetTexture(TEX_TIMELINE_BAR)->w / 2, Renderer::GetTexture(TEX_TIMELINE_BAR)->h);
	ent6->position = new PositionComponent((WIN_X - 88) / 2, 62);
	ent6->size = new SizeComponent(44, 9);
	ent6->sprite = new SpriteComponent(TEX_TIMELINE_BAR, 1.0, 4);
	entMan->entities.push_back(ent6);

	// timeline
	Entity* ent7 = new Entity();
	ent7->position = new PositionComponent((WIN_X - 112.5) / 2, 50);
	ent7->size = new SizeComponent(112.5, 33.5);
	ent7->sprite = new SpriteComponent(TEX_TIMELINE, 1.0, 5);
	entMan->entities.push_back(ent7);

	// left button
	Entity* ent8 = new Entity();
	ent8->position = new PositionComponent((WIN_X - 120) / 5, WIN_Y - 160);
	ent8->sprite = new SpriteComponent(TEX_LEFT, 1.0, 0);
	ent8->size = new SizeComponent(120, 120);
	ent8->clickable = new ClickableComponent();
	ClickListenerComponent* clc1 = new ClickListenerComponent(ent8, eventManager);
	eventManager->AddListener(clc1);
	ent8->clickListener = clc1;
	entMan->entities.push_back(ent8);

	// right button
	Entity* ent9 = new Entity();
	ent9->position = new PositionComponent((WIN_X - 120) / 5 * 4, WIN_Y - 160);
	ent9->sprite = new SpriteComponent(TEX_RIGHT, 1.0, 0);
	ent9->size = new SizeComponent(120, 120);
	ent9->clickable = new ClickableComponent();
	ClickListenerComponent* clc2 = new ClickListenerComponent(ent9, eventManager);
	eventManager->AddListener(clc2);
	ent9->clickListener = clc2;
	entMan->entities.push_back(ent9);

	// score
	Entity* ent10 = new Entity();
	ScoreComponent* scr = new ScoreComponent(0);
	ScoreListenerComponent* scoreListener = new ScoreListenerComponent(scr, eventManager);
	eventManager->AddListener(scoreListener);
	ent10->score = scr;
	ent10->scoreListener = scoreListener;
	entMan->entities.push_back(ent10);
}