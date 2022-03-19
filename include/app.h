#pragma once
#include "defs.h"
#include "event.h"

struct Renderer;
struct EventManager;
struct SceneManager;

struct App : EventListener {
  private:
	double GetDelta();
	void PumpSystemEvents();

	Renderer* renderer;
	EventManager* eventManager;
	SceneManager* sceneManager;

	bool running;
	double lastTicks;
	double tickRate;
	double lag;

  public:
	App();
	~App();

	void Run();
	void Responder(Event* event);
};
