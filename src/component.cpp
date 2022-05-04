#include "component.h"
#include "entity.h"

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

ScoreListenerComponent::ScoreListenerComponent(ScoreComponent* scr) {
	this->tag = SCORELISTENER;
	this->scr = scr;
}

SizeComponent::SizeComponent(double w, double h) {
	this->tag = SIZE;
	this->w = w;
	this->h = h;
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