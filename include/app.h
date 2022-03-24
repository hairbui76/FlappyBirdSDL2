#pragma once
#include "defs.h"
#include "event.h"

class Renderer;
class EventManager;
class SceneManager;

class App : public EventListener {
	double GetDelta();
	void PumpSystemEvents();

	Renderer* renderer;
	EventManager* eventManager;
	SceneManager* sceneManager;

	bool running;
	uint32_t startTime;
	double tickRate;
	double lag;

  public:
	App();
	~App();

	void Run();
	void Responder(Event* event);
};
