#pragma once
#include "defs.h"
#include "event.h"

struct Renderer;
struct EntityManager;

struct Scene {
	virtual void DoFrame(Renderer* renderer) = 0;
	virtual void Responder(Event* event, EventManager* eventManager) = 0;
	virtual ~Scene() = default;

	EntityManager* entMan;
	EventManager* eventManager;
};

class SceneManager : public EventListener {
	void ChangeScene(scene_e sceneTag);
	Scene* currentScene;
	Renderer* renderer;
	EventManager* eventManager;

public:
	virtual ~SceneManager();
	SceneManager(Renderer* renderer, EventManager* eventManager);

	void DoFrame();
	void Responder(Event* event);
};
