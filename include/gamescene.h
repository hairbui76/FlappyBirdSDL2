#pragma once
#include "defs.h"
#include "scene.h"

class GameScene : public Scene {
	void PopulateWithInitEnts(EntityManager* entMan);
	void Restart(EventManager* eventManager);
	void SpawnPipe();
	bool restartFlag;
	void DoPreRestart(EntityManager* entMan);
	int maxScore;

  public:
	GameScene(Renderer* renderer, EventManager* eventManager);
	~GameScene();

	void DoFrame(Renderer* renderer);
	void Tick();
	void Responder(Event* event, EventManager* eventManager);
};
