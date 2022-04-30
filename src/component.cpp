#include "component.h"

PositionComponent::PositionComponent(double x, double y) {
	this->tag = POSITION;
	this->x = x;
	this->y = y;
}

SpriteComponent::SpriteComponent(texture_e tName, double scale, int layer) {
	this->tag = SPRITE;
	this->tName = tName;
	this->scale = scale;
	this->layer = layer;
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
