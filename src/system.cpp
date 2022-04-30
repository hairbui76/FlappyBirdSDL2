#include "system.h"
#include "component.h"
#include "entity.h"
#include "event.h"
#include "renderer.h"
#include <cmath>
#include <iostream>

void renderSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full, SDL_RendererFlip flip_flag) {
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
		int repeat = 1;

		if (entity->angle) {
			RotateComponent* r = (RotateComponent*)entity->angle;
			angle = r->angle;
		}

		if (entity->size) {
			SizeComponent* s = (SizeComponent*)entity->size;
			w = s->w;
			h = s->h;
		}

		for (int j = 0; j < repeat; ++j) {
			renderer->renderSprite(x + (j * w), y, w, h, angle, tex, scale, spr->tName, full, flip_flag);
		}
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
