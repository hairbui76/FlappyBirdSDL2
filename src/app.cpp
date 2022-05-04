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

			case SDL_KEYDOWN:
				if (event.key.repeat == 1)
					break;

				switch (event.key.keysym.sym) {
					case SDLK_RIGHT:
						eventManager->Post(new Event(KEYDOWN, "RIGHT"));
						break;

					case SDLK_LEFT:
						eventManager->Post(new Event(KEYDOWN, "LEFT"));
						break;

					case SDLK_SPACE:
						eventManager->Post(new Event(KEYDOWN, "SPACE"));
						break;

					default:
						break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					eventManager->Post(new Event(MOUSE_BUTT, " ", event.motion.x, event.motion.y));
				break;

			default:
				break;
		}
	}
}

void App::Responder(Event* event) {
	if (event->type == QUIT)
		running = false;
}
