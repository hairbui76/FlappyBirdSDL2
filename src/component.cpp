#include "component.h"
#include "entity.h"
#include "renderer.h"
#include <iostream>

PositionComponent::PositionComponent(double x, double y) {
	this->tag = POSITION;
	this->x = x;
	this->y = y;
}

SpriteComponent::SpriteComponent(texture_e tName, double scale, int layer, SDL_RendererFlip flip_flag) {
	this->tag = SPRITE;
	this->tName = tName;
	this->scale = scale;
	this->layer = layer;
	this->flip_flag = flip_flag;
}

RotateComponent::RotateComponent(double angle) {
	this->tag = ANGLE;
	this->angle = angle;
}

ScoreComponent::ScoreComponent(int maxScore) {
	this->tag = SCORE;
	this->score = 0;
	this->maxScore = maxScore;
}

SizeComponent::SizeComponent(double w, double h) {
	this->tag = SIZE;
	this->w = w;
	this->h = h;
}

ScoreListenerComponent::ScoreListenerComponent(ScoreComponent* scr) {
	this->tag = SCORELISTENER;
	this->scr = scr;
}

void ScoreListenerComponent::Responder(Event* event) {
	if (event->type == INC_SCORE) {
		this->scr->score += 1;
	}
}

ClickableComponent::ClickableComponent() {
	this->tag = CLICKABLE;
	this->isClicked = false;
}

ClickListenerComponent::ClickListenerComponent(Entity* entity) {
	this->tag = CLICKLISTENER;
	this->entity = entity;
}

void ClickListenerComponent::Responder(Event* event) {
	if (event->type == MOUSE_BUTT) {
		if (entity->clickable && entity->position && entity->size) {
			PositionComponent* position = (PositionComponent*)entity->position;
			SizeComponent* size = (SizeComponent*)entity->size;
			ClickableComponent* clickable = (ClickableComponent*)entity->clickable;
			if (event->x >= position->x && event->x <= position->x + size->w && event->y >= position->y && event->y <= position->y + size->h) {
				clickable->isClicked = true;
			}
		}
	}
}

MovableComponent::MovableComponent() {
	this->tag = MOVABLE;
	this->state = RIGHT;
}

MoveListenerComponent::MoveListenerComponent(Entity* entity) {
	this->tag = MOVELISTENER;
	this->entity = entity;
}

void MoveListenerComponent::Responder(Event* event) {
	if (event->type == KEYDOWN && (!strcmp(event->data, "LEFT") || !strcmp(event->data, "RIGHT"))) {
		if (entity->movable) {
			MovableComponent* movable = (MovableComponent*)entity->movable;
			entity->size = new SizeComponent(87, 107);
			// render new texture (lumber_holding)
			if (!strcmp(event->data, "LEFT")) {
				movable->state = LEFT;
				entity->sprite = new SpriteComponent(TEX_LUMBER_CUTTING, 1.0, 0, SDL_FLIP_HORIZONTAL);
				entity->position = new PositionComponent((WIN_X - 50) / 2 - 70, WIN_Y - 350);
			} else if (!strcmp(event->data, "RIGHT")) {
				movable->state = RIGHT;
				entity->sprite = new SpriteComponent(TEX_LUMBER_CUTTING, 1.0, 0);
				entity->position = new PositionComponent((WIN_X - 50) / 2 + 30, WIN_Y - 350);
			}
		}
	}
}

SpawnerComponent::SpawnerComponent() {
	this->tag = SPAWNER;
}

SpawnerListenerComponent::SpawnerListenerComponent(Entity* entity) {
	this->tag = SPAWNERLISTENER;
	this->entity = entity;
}

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

CuttableComponent::CuttableComponent(int origin_x, int origin_y, int cut_width, int cut_height) {
	this->tag = CUTTABLE;
	this->origin_x = origin_x;
	this->origin_y = origin_y;
	this->cut_width = cut_width;
	this->cut_height = cut_height;
};

AnimationComponent::AnimationComponent() {
	this->tag = ANIMATION;
}