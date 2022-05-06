#pragma once
#include "defs.h"
#include "scene.h"

class GameScene : public Scene {
	void populateEntity(EntityManager* entMan);
	void Restart(EventManager* eventManager);
	bool restartFlag;
	void DoPreRestart(EntityManager* entMan);
	int maxScore;

public:
	~GameScene() = default;
	GameScene(EventManager* eventManager);

	void DoFrame(Renderer* renderer);
	void Responder(Event* event, EventManager* eventManager);
};
