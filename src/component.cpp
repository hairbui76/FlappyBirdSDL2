#include "component.h"
#include "entity.h"
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
	this->state = getRandom(0, 1) ? LEFT : RIGHT;
	this->is_moved = false;
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

CuttableComponent::CuttableComponent(int origin_x, int origin_y, int cut_width, int cut_height) {
	this->tag = CUTTABLE;
	this->origin_x = origin_x;
	this->origin_y = origin_y;
	this->cut_width = cut_width;
	this->cut_height = cut_height;
};