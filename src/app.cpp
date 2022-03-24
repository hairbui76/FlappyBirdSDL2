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

	startTime = SDL_GetTicks();
	tickRate = 1.0 / TICK_RATE;
	lag = 0.0;
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

		for (lag += GetDelta(); lag >= tickRate; lag -= tickRate) {
			sceneManager->Tick();
		}

		// Clear renderer
		renderer->Clear();
		sceneManager->DoFrame();
		renderer->Present();
	}
}

double App::GetDelta() {
	uint32_t currTime = SDL_GetTicks();
	uint32_t delta = currTime - startTime;
	startTime = currTime;

	return delta / 1000.0;
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
					case SDLK_SPACE:
						eventManager->Post(new Event(KEYDOWN, " "));
						break;

					case SDLK_q:
						eventManager->Post(new Event(KEYDOWN, "q"));
						break;

					case SDLK_RETURN:
						eventManager->Post(new Event(KEYDOWN, "ENTER"));
						break;

					default:
						break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				eventManager->Post(new Event(MOUSE_BUTT, " "));
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
