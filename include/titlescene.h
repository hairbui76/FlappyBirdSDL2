#pragma once
#include "defs.h"
#include "scene.h"

struct TitleScene : Scene {
	TitleScene(EventManager* eventManager);
	~TitleScene();

	void DoFrame(Renderer*& renderer);
	void Tick();
	void Responder(Event*& event, EventManager*& eventManager);
};
