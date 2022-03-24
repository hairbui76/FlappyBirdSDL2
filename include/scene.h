#pragma once
#include "defs.h"
#include "event.h"

struct Renderer;
struct EntityManager;

struct Scene {
	virtual void DoFrame(Renderer* renderer) = 0;
	virtual void Tick() = 0;
	virtual void Responder(Event* event, EventManager* eventManager) = 0;

	EntityManager* entMan;
	EventManager* eventManager;
	Renderer* renderer;
};

class SceneManager : public EventListener {
	void ChangeScene(scene_e sceneTag);
	std::vector<Scene*> sceneStack;
	Renderer* renderer;
	EventManager* eventManager;

  public:
	SceneManager(Renderer* renderer, EventManager* eventManager);
	virtual ~SceneManager() = default;

	void DoFrame();
	void Tick();
	void Responder(Event* event);
};
