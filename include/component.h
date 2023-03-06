#pragma once
#include "defs.h"
#include "event.h"

struct Entity;

struct Component {
	Component() = default;
	Component(component_tag_e tag);
	~Component() = default;

	component_tag_e tag;
};

struct PositionComponent : Component {
	PositionComponent(double x, double y);

	double x;
	double y;
};

struct SpriteComponent : Component {
	SpriteComponent(texture_e tName, double scale, int layer, SDL_RendererFlip flip_flag = SDL_FLIP_NONE, double alpha = 1.0);

	texture_e tName;
	double alpha;
	double scale;
	int layer;
	SDL_RendererFlip flip_flag;
};

struct RotateComponent : Component {
	RotateComponent(double angle, float origin_x, float origin_y);

	double angle;
	float origin_x;
	float origin_y;
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
	ScoreListenerComponent(ScoreComponent* scr, EventManager* eventManager);

	void Responder(Event* event);
	ScoreComponent* scr = nullptr;
	EventManager* eventManager = nullptr;
};

struct ClickableComponent : Component {
	ClickableComponent();

	bool isClicked;
};

struct ClickListenerComponent : Component, EventListener {
	ClickListenerComponent(Entity* entity, EventManager* eventManager);

	void Responder(Event* event);
	Entity* entity = nullptr;
	EventManager* eventManager = nullptr;
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

struct HandAnimationComponent : Component {
	HandAnimationComponent();

	std::vector<Entity*> hand_animation_entities;
	size_t current_frame;
};

struct HandAnimationListenerComponent : Component, EventListener {
	HandAnimationListenerComponent(Entity* entity);

	void Responder(Event* event);
	Entity* entity = nullptr;
};

struct DeadComponent : Component {
	DeadComponent();

	bool is_dead;
	bool is_over;
};

struct ShrinkableComponent : Component {
	ShrinkableComponent();

	double value;
	bool started;
};

struct ShrinkListenerComponent : Component, EventListener {
	ShrinkListenerComponent(ShrinkableComponent* shrinkable);

	void Responder(Event* event);
	ShrinkableComponent* shrinkable = nullptr;
};

struct AutoAnimationComponent : Component {
	AutoAnimationComponent(int frame_end);

	int current_frame;
	int frame_end;
	bool is_started;
};

struct AutoAnimationListenerComponent : Component, EventListener {
	AutoAnimationListenerComponent(Entity* entity);

	void Responder(Event* event);
	Entity* entity = nullptr;
};

struct DisappearComponent : Component {
	DisappearComponent();
};

struct DisappearAngleComponent : Component {
	DisappearAngleComponent(double end_angle);

	float origin_x;
	float origin_y;
	double end_angle;
};