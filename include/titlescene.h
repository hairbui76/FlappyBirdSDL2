#pragma once
#include "defs.h"
#include "scene.h"

struct TitleScene : Scene {
	TitleScene(EventManager* eventManager);
	~TitleScene() = default;

	void DoFrame(Renderer* renderer);
	void Responder(Event* event, EventManager* eventManager);
};
