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
		SDL_RendererFlip flip_flag = spr->flip_flag;

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

		if (entity->cuttable) {
			CuttableComponent* cuttable = (CuttableComponent*)entity->cuttable;
			sRect = {cuttable->origin_x, cuttable->origin_y, cuttable->cut_width, cuttable->cut_height};
			dRect = {(float)x, (float)y, (float)(w * scale), (float)(h * scale)};
			renderer->renderSprite(sRect, dRect, angle, tex, spr->tName, full, flip_flag);
			return;
		}
		renderer->renderSprite(sRect, dRect, angle, tex, spr->tName, full, flip_flag);
		return;
	}
	if (entity->spawner) {
		SpawnerComponent* spawner = (SpawnerComponent*)entity->spawner;
		for (auto spawner_entity : spawner->spawner_entities) {
			if (spawner_entity.second) {
				renderSpriteSystem(spawner_entity.second, renderer, layer, full);
				renderSpriteSystem(spawner_entity.first, renderer, layer + 1, full);
			} else {
				renderSpriteSystem(spawner_entity.first, renderer, layer + 1, full);
			}
		}
		return;
	}
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