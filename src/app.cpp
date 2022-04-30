#include "app.h"
#include "event.h"
#include "renderer.h"
#include "scene.h"
#include <SDL2/SDL.h>
#include <iostream>

App::App() {
	renderer = new Renderer;
	eventManager = new EventManager;
	eventManager->AddListener(this);
	sceneManager = new SceneManager(renderer, eventManager);
	eventManager->AddListener(sceneManager);

	running = true;
}

App::~App() {
	delete (renderer);
	delete (eventManager);
	delete (sceneManager);
}

void App::Run() {
	while (running) {
		PumpSystemEvents();
		eventManager->PumpEvents();

		// Clear renderer
		renderer->Clear();
		sceneManager->DoFrame();
		renderer->Present();
	}
}

void App::PumpSystemEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				eventManager->Post(new Event(QUIT, ""));
				break;
		}
	}
}

void App::Responder(Event* event) {
	if (event->type == QUIT)
		running = false;
}
