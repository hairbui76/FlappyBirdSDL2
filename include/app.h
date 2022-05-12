#pragma once
#include "defs.h"
#include "event.h"

class Renderer;
class EventManager;
class SceneManager;

class App : public EventListener {
	void PumpSystemEvents();

	Renderer* renderer;
	EventManager* eventManager;
	SceneManager* sceneManager;

	bool running;

public:
	App();
	~App();
	void Run();
	void Responder(Event* event);
};
