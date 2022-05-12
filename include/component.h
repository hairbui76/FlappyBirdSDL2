#pragma once
#include "defs.h"
#include "event.h"

struct Entity;

struct Component {
	~Component() = default;

	component_tag_e tag;
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

struct MovableComponent : Component {
	MovableComponent();

	move_tag_e state;
};

struct MoveListenerComponent : Component, EventListener {
	MoveListenerComponent(Entity* entity);
	void Responder(Event* event);

	Entity* entity = nullptr;
};

struct SpawnerComponent : Component {
	SpawnerComponent();

	std::vector<std::pair<Entity*, Entity*>> spawner_entities;
};

struct SpawnerListenerComponent : Component, EventListener {
	SpawnerListenerComponent(Entity* entity);

	void Responder(Event* event);
	Entity* entity = nullptr;
};

struct CuttableComponent : Component {
	CuttableComponent(int origin_x, int origin_y, int cut_width, int cut_height);

	int origin_x, origin_y, cut_width, cut_height;
};

struct AnimationComponent : Component {
	AnimationComponent();

	std::vector<Entity*> animation_entities;
	size_t current_frame;
};

struct AnimationListenerComponent : Component, EventListener {
	AnimationListenerComponent(Entity* entity);

	void Responder(Event* event);
	Entity* entity = nullptr;
};

struct DeadComponent : Component {
	DeadComponent();

	bool is_dead;
	bool is_over;
};