#pragma once
#include "defs.h"
#include "scene.h"

class EndScene : public Scene {
	void populateEntity(EntityManager* entMan, move_tag_e move_state, int score);
	int maxScore;

public:
	EndScene(EventManager* eventManager, move_tag_e move_state, int score);
	~EndScene() = default;

	void DoFrame(Renderer* renderer);
	void Responder(Event* event, EventManager* eventManager);
};