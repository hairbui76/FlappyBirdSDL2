#include "scene.h"
#include "endscene.h"
#include "gamescene.h"
#include "renderer.h"
#include "titlescene.h"
#include <iostream>

SceneManager::~SceneManager() {
	delete currentScene;
}

SceneManager::SceneManager(Renderer* renderer, EventManager* eventManager) {
	this->renderer = renderer;
	this->eventManager = eventManager;

	ChangeScene(TITLE);
}

void SceneManager::DoFrame() {
	currentScene->DoFrame(renderer);
}

void SceneManager::Responder(Event* event) {
	switch (event->type) {
		case CHANGE_SCENE: {
			if (!strcmp(event->data, "GAME_SCENE")) {
				ChangeScene(GAME);
			}
			if (!strcmp(event->data, "END_SCENE")) {
				ChangeScene(END);
			}
			break;
		}

		default:
			currentScene->Responder(event, eventManager);
			break;
	}
}

void SceneManager::ChangeScene(scene_e sceneTag, move_tag_e move_state) {
	switch (sceneTag) {
		case TITLE:
			currentScene = new TitleScene(eventManager);
			break;
		case GAME:
			currentScene = new GameScene(eventManager);
			break;
		case END:
			currentScene = new EndScene(eventManager, move_state);
			break;
		default:
			break;
	};
}