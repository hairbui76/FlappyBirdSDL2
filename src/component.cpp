#include "component.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

Component::Component(component_tag_e tag) : tag(tag) {}

PositionComponent::PositionComponent(double x, double y) : Component(POSITION), x(x), y(y) {}

SpriteComponent::SpriteComponent(texture_e tName, double scale, int layer, SDL_RendererFlip flip_flag, double alpha) : alpha(alpha), flip_flag(flip_flag), Component(SPRITE), tName(tName), scale(scale), layer(layer) {}

RotateComponent::RotateComponent(double angle, float origin_x, float origin_y) : Component(ANGLE), angle(angle), origin_x(origin_x), origin_y(origin_y) {}

ScoreComponent::ScoreComponent(int maxScore) : Component(SCORE), maxScore(maxScore), score(0) {}

SizeComponent::SizeComponent(double w, double h) : Component(SIZE), w(w), h(h) {}

ScoreListenerComponent::ScoreListenerComponent(ScoreComponent* scr, EventManager* eventManager) : Component(SCORELISTENER), scr(scr), eventManager(eventManager) {}

void ScoreListenerComponent::Responder(Event* event) {
	if (event->type == INC_SCORE) {
		this->scr->score += 1;
		if (this->scr->score % 20 == 0 && this->scr->score != 0)
			eventManager->Post(new Event(LEVEL_UP, ""));
	}
}

ClickableComponent::ClickableComponent() : Component(CLICKABLE), isClicked(false) {}

ClickListenerComponent::ClickListenerComponent(Entity* entity, EventManager* eventManager) : Component(CLICKLISTENER), entity(entity), eventManager(eventManager) {}

void ClickListenerComponent::Responder(Event* event) {
	if (event->type == MOUSE_BUTT) {
		if (entity->clickable && entity->position && entity->size) {
			PositionComponent* position = (PositionComponent*)entity->position;
			SizeComponent* size = (SizeComponent*)entity->size;
			ClickableComponent* clickable = (ClickableComponent*)entity->clickable;
			if (event->x >= position->x && event->x <= position->x + size->w && event->y >= position->y && event->y <= position->y + size->h) {
				clickable->isClicked = true;
				eventManager->Post(new Event(BUTT_CLICK, ""));
			}
		}
	}
}

MovableComponent::MovableComponent() : state(RIGHT), Component(MOVABLE) {}

MoveListenerComponent::MoveListenerComponent(Entity* entity) : entity(entity), Component(MOVELISTENER) {}

void MoveListenerComponent::Responder(Event* event) {
	if (event->type == KEYDOWN && (!strcmp(event->data, "LEFT") || !strcmp(event->data, "RIGHT"))) {
		if (entity->movable) {
			MovableComponent* movable = (MovableComponent*)entity->movable;
			if (!strcmp(event->data, "LEFT")) {
				movable->state = LEFT;
			} else if (!strcmp(event->data, "RIGHT")) {
				movable->state = RIGHT;
			}
		}
	}
}

SpawnerComponent::SpawnerComponent() : Component(SPAWNER) {}

SpawnerListenerComponent::SpawnerListenerComponent(Entity* entity) : entity(entity), Component(SPAWNERLISTENER) {}

void SpawnerListenerComponent::Responder(Event* event) {
	if (event->type == SPAWN_BRANCH) {
		if (entity->spawner) {
			SpawnerComponent* spawner = (SpawnerComponent*)entity->spawner;
			// make a copy of the first chunk
			Entity* temp_entity = spawner->spawner_entities[0].first;
			size_t size = spawner->spawner_entities.size();
			for (size_t i = 0; i < size - 1; i++) {
				// take the (i+1)th trunk and place it to the ith trunk
				if (spawner->spawner_entities[i + 1].first->cuttable) {
					CuttableComponent* cuttable = (CuttableComponent*)spawner->spawner_entities[i + 1].first->cuttable;
					spawner->spawner_entities[i].first->cuttable = cuttable;
				}

				// take the (i+1)th branch and place it to the ith branch
				spawner->spawner_entities[i].second = spawner->spawner_entities[i + 1].second;
				if (spawner->spawner_entities[i].second) {
					if (spawner->spawner_entities[i + 1].second->sprite && spawner->spawner_entities[i + 1].second->position) {
						SpriteComponent* sprite = (SpriteComponent*)spawner->spawner_entities[i + 1].second->sprite;
						spawner->spawner_entities[i].second->sprite = sprite;
						if (sprite->flip_flag == SDL_FLIP_NONE)
							spawner->spawner_entities[i].second->position = new PositionComponent(WIN_X / 2 + 10, WIN_Y - 340 - i * 80);
						else
							spawner->spawner_entities[i].second->position = new PositionComponent(WIN_X / 2 - 135, WIN_Y - 340 - i * 80);
					}
				}
			}
			// now place the first trunk to the end of the trunk spawner
			if (temp_entity->cuttable) {
				CuttableComponent* cuttable = (CuttableComponent*)temp_entity->cuttable;
				spawner->spawner_entities[size - 1].first->cuttable = cuttable;
			}

			if (!spawner->spawner_entities[size - 2].second) {
				// make a new branch of the last trunk if valid
				Entity* new_spawner_entity = new Entity;
				SDL_RendererFlip flip_flag = getRandom(0, 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
				if (flip_flag == SDL_FLIP_NONE)
					new_spawner_entity->position = new PositionComponent(WIN_X / 2 + 10, WIN_Y - 340 - (size - 1) * 80);
				else
					new_spawner_entity->position = new PositionComponent(WIN_X / 2 - 135, WIN_Y - 340 - (size - 1) * 80);
				new_spawner_entity->sprite = new SpriteComponent(TEX_BRANCH, 1.0, 1, flip_flag);
				new_spawner_entity->size = new SizeComponent(125, 80);

				spawner->spawner_entities[size - 1].second = new_spawner_entity;
			} else {
				spawner->spawner_entities[size - 1].second = nullptr;
			}
		}
	}
}

CuttableComponent::CuttableComponent(int origin_x, int origin_y, int cut_width, int cut_height) : origin_x(origin_x), origin_y(origin_y), cut_width(cut_width), cut_height(cut_height), Component(CUTTABLE) {}

HandAnimationComponent::HandAnimationComponent() : current_frame(0), Component(HANDANIMATION) {}

HandAnimationListenerComponent::HandAnimationListenerComponent(Entity* entity) : entity(entity), Component(HANDANIMATIONLISTENER) {}

void HandAnimationListenerComponent::Responder(Event* event) {
	if ((event->type == KEYDOWN && (!strcmp(event->data, "LEFT") || !strcmp(event->data, "RIGHT"))) || event->type == GAME_OVER) {
		if (entity->dead) {
			DeadComponent* dead = (DeadComponent*)entity->dead;
			if (entity->movable) {
				MovableComponent* movable = (MovableComponent*)entity->movable;
				if (entity->handAnimation) {
					HandAnimationComponent* handAnimation = (HandAnimationComponent*)entity->handAnimation;
					Entity* hand_animation_entity1 = handAnimation->hand_animation_entities[0];
					hand_animation_entity1->size = new SizeComponent(70, 107);
					if (movable->state == LEFT) {
						hand_animation_entity1->position = new PositionComponent((WIN_X - 50) / 2 - 90, WIN_Y - 350);
						hand_animation_entity1->sprite = new SpriteComponent(TEX_LUMBER_HOLDING, 1.0, 3, SDL_FLIP_HORIZONTAL);
						Entity* hand_animation_entity2 = handAnimation->hand_animation_entities[1];
						hand_animation_entity2->position = new PositionComponent((WIN_X - 50) / 2 - 70, WIN_Y - 350);
						hand_animation_entity2->sprite = new SpriteComponent(TEX_LUMBER_CUTTING, 1.0, 3, SDL_FLIP_HORIZONTAL);
					} else if (movable->state == RIGHT) {
						hand_animation_entity1->position = new PositionComponent((WIN_X - 50) / 2 + 65, WIN_Y - 350);
						hand_animation_entity1->sprite = new SpriteComponent(TEX_LUMBER_HOLDING, 1.0, 3);
						Entity* hand_animation_entity2 = handAnimation->hand_animation_entities[1];
						hand_animation_entity2->position = new PositionComponent((WIN_X - 50) / 2 + 28, WIN_Y - 350);
						hand_animation_entity2->sprite = new SpriteComponent(TEX_LUMBER_CUTTING, 1.0, 3);
					}
					if (dead->is_dead) {
						hand_animation_entity1->size = new SizeComponent(70, 85);
						if (movable->state == LEFT) {
							hand_animation_entity1->position = new PositionComponent((WIN_X - 50) / 2 - 80, WIN_Y - 340);
							hand_animation_entity1->sprite = new SpriteComponent(TEX_LUMBER_DEAD, 1.0, 3, SDL_FLIP_HORIZONTAL);
						} else if (movable->state == RIGHT) {
							hand_animation_entity1->position = new PositionComponent((WIN_X - 50) / 2 + 60, WIN_Y - 340);
							hand_animation_entity1->sprite = new SpriteComponent(TEX_LUMBER_DEAD, 1.0, 3);
						}
						if (event->type == GAME_OVER) {
							dead->is_over = true;
							return;
						}
					}
					handAnimation->current_frame++;
				}
			}
		}
	}
}

DeadComponent::DeadComponent() : is_dead(false), is_over(false), Component(DEAD) {}

ShrinkableComponent::ShrinkableComponent() : value(1), started(false), Component(SHRINKABLE) {}

ShrinkListenerComponent::ShrinkListenerComponent(ShrinkableComponent* shrinkable) : shrinkable(shrinkable), Component(SHRINKLISTENER) {}

void ShrinkListenerComponent::Responder(Event* event) {
	if (event->type == LEVEL_UP) {
		shrinkable->value += 0.5;
	}
	if (event->type == KEYDOWN && (!strcmp(event->data, "LEFT") || !strcmp(event->data, "RIGHT"))) {
		if (!shrinkable->started) {
			shrinkable->started = true;
		}
	}
};

AutoAnimationComponent::AutoAnimationComponent(int frame_end) : frame_end(frame_end), current_frame(0), is_started(false), Component(AUTOANIMATION) {}

AutoAnimationListenerComponent::AutoAnimationListenerComponent(Entity* entity) : entity(entity), Component(AUTOANIMATIONLISTENER) {}

void AutoAnimationListenerComponent::Responder(Event* event) {
	if (event->type == CUT_TREE) {
		AutoAnimationComponent* autoAnimation = (AutoAnimationComponent*)entity->autoAnimation;
		autoAnimation->is_started = true;
		DisappearAngleComponent* disappearAngle = (DisappearAngleComponent*)entity->disappearAngle;
		if (!strcmp(event->data, "RIGHT")) {
			disappearAngle->origin_x = 0;
			disappearAngle->origin_y = 80;
		} else if (!strcmp(event->data, "LEFT")) {
			disappearAngle->origin_x = 125;
			disappearAngle->origin_y = 80;
		}
		if (!entity->sprite) {
			entity->size = new SizeComponent(125, 80);
			if (!strcmp(event->data, "LEFT")) {
				entity->sprite = new SpriteComponent(TEX_BRANCH, 1.0, 3, SDL_FLIP_HORIZONTAL, 1.0);
				entity->position = new PositionComponent((WIN_X - 50) / 2 - 120, WIN_Y - 350);
			} else if (!strcmp(event->data, "RIGHT")) {
				entity->sprite = new SpriteComponent(TEX_BRANCH, 1.0, 3, SDL_FLIP_NONE, 1.0);
				entity->position = new PositionComponent((WIN_X - 50) / 2 + 50, WIN_Y - 350);
			}
		} else {
			SpriteComponent* sprite = (SpriteComponent*)entity->sprite;
			if (!strcmp(event->data, "LEFT")) {
				sprite->flip_flag = SDL_FLIP_HORIZONTAL;
				if (entity->position) {
					PositionComponent* position = (PositionComponent*)entity->position;
					position->x = (WIN_X - 50) / 2 - 120;
					position->y = WIN_Y - 350;
				}
			} else if (!strcmp(event->data, "RIGHT")) {
				sprite->flip_flag = SDL_FLIP_NONE;
				if (entity->position) {
					PositionComponent* position = (PositionComponent*)entity->position;
					position->x = (WIN_X - 50) / 2 + 50;
					position->y = WIN_Y - 350;
				}
			}
		}
	}
}

DisappearComponent::DisappearComponent() : Component(DISAPPEAR) {}

DisappearAngleComponent::DisappearAngleComponent(double end_angle) : end_angle(end_angle), Component(DISAPPEARANGLE) {}