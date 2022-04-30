#pragma once
#include "defs.h"
#include "event.h"

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
	SpriteComponent(texture_e tName, double scale, int layer);

	texture_e tName;
	double scale;
	int layer;
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

	ScoreComponent* scr;
};

struct ClickableComponent : Component {
	ClickableComponent(int layer);

	bool isClicked;
	int layer;
};