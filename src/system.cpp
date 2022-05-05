#include "system.h"
#include "component.h"
#include "entity.h"
#include "event.h"
#include "renderer.h"
#include <cmath>
#include <iostream>

void renderSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full) {
	if (entity->position && entity->sprite) {
		PositionComponent* pos = (PositionComponent*)entity->position;
		SpriteComponent* spr = (SpriteComponent*)entity->sprite;
		Texture* tex = Renderer::GetTexture(spr->tName);

		if (spr->layer != layer) {
			return;
		}

		double x = pos->x;
		double y = pos->y;
		double w = tex->w;
		double h = tex->h;
		double scale = spr->scale;
		double angle = 0.0;
		SDL_RendererFlip flip_flag = SDL_FLIP_NONE;

		if (entity->angle) {
			RotateComponent* r = (RotateComponent*)entity->angle;
			angle = r->angle;
		}

		if (entity->size) {
			SizeComponent* s = (SizeComponent*)entity->size;
			w = s->w;
			h = s->h;
		}

		if (entity->movable) {
			MovableComponent* m = (MovableComponent*)entity->movable;
			flip_flag = m->state == LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}

		SDL_Rect sRect{0, 0, 0, 0};
		SDL_FRect dRect{(float)x, (float)y, (float)(w * scale), (float)(h * scale)};
		if (SDL_QueryTexture(tex->tex, NULL, NULL, &sRect.w, &sRect.h))
			logSDLError("Unable to query texture: %s", SDL_GetError());

		if (entity->spawner) {
			SpawnerComponent* spawner = (SpawnerComponent*)entity->spawner;
			for (auto const& e : spawner->flip_map) {
				if (e.second == 2) {
				}
			}
			// if (spawner->flip_flags[i] == SDL_FLIP_HORIZONTAL)
			// 	dRect = {(float)(x - w * scale - 20), (float)(y - (h * i)), (float)(w * scale), (float)(h * scale)};
			// else
			// 	dRect = {(float)x, (float)(y - (h * i)), (float)(w * scale), (float)(h * scale)};
			// renderer->renderSprite(sRect, dRect, angle, tex, spr->tName, full, spawner->flip_flags[i]);
		}
		return;
	}
	renderer->renderSprite(sRect, dRect, angle, tex, spr->tName, full, flip_flag);
}

void HudSystem(Entity* entity, Renderer* renderer) {
	try {
		if (entity->score) {
			PositionComponent* pos = (PositionComponent*)entity->position;
			ScoreComponent* scr = (ScoreComponent*)entity->score;

			char buff[10];
			sprintf(buff, "%d / %d", scr->score, scr->maxScore);
			renderer->Print(pos->x, pos->y, buff);
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}