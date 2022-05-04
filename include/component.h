#pragma once
#include "defs.h"
#include "event.h"

struct Entity;

struct Component {
	component_tag_e tag;
	~Component() = default;
};

struct PositionComponent : Component {
	PositionComponent(double x, double y);

	double x;
	double y;
};

struct SpriteComponent : Component {
	SpriteComponent(texture_e tName, double scale, int layer, SDL_RendererFlip flip_flag = SDL_FLIP_NONE);

	texture_e tName;
	double scale;
	int layer;
	SDL_RendererFlip flip_flag;
};

struct RotateComponent : Component {
	RotateComponent(double angle);

	double angle;
};

struct SizeComponent : Component {
	SizeComponent(double w, double h);

	double w;
	double h;
};

struct ScoreComponent : Component {
	ScoreComponent(int maxScore);

	int score;
	int maxScore;
};

struct ScoreListenerComponent : Component, EventListener {
	ScoreListenerComponent(ScoreComponent* scr);
	void Responder(Event* event);

	ScoreComponent* scr = nullptr;
};

struct ClickableComponent : Component {
	ClickableComponent();

	bool isClicked;
};

struct ClickListenerComponent : Component, EventListener {
	ClickListenerComponent(Entity* entity);
	void Responder(Event* event);

	Entity* entity = nullptr;
};