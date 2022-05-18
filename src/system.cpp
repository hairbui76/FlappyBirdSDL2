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
	}
}

void handAnimationSystem(Entity* entity, Renderer* renderer, EventManager* eventManager, int layer) {
	if (entity->handAnimation) {
		HandAnimationComponent* handAnimation = (HandAnimationComponent*)entity->handAnimation;
		if (handAnimation->hand_animation_entities.size() > 0) {
			Entity* hand_animation_entity = handAnimation->hand_animation_entities[handAnimation->current_frame];
			SDL_Delay(60);
			renderSpriteSystem(hand_animation_entity, renderer, layer);
			// check for game over state => update animation system to wait for game over change scene
			if (entity->dead) {
				DeadComponent* dead = (DeadComponent*)entity->dead;
				// check if already dead but not finished rendering
				if (dead->is_dead && !dead->is_over && handAnimation->current_frame == handAnimation->hand_animation_entities.size() - 1) {
					dead->is_over = true;
					// check if already dead and finished rendering => post event to change scene
				} else if (dead->is_dead && dead->is_over && handAnimation->current_frame == 0) {
					if (entity->movable) {
						MovableComponent* movable = (MovableComponent*)entity->movable;
						if (movable->state == LEFT)
							eventManager->Post(new Event(CHANGE_SCENE, "END_SCENE_LEFT"));
						else
							eventManager->Post(new Event(CHANGE_SCENE, "END_SCENE_RIGHT"));
					}
				}
			}
			if (handAnimation->current_frame == handAnimation->hand_animation_entities.size() - 1)
				handAnimation->current_frame = 0;
		}
	}
}

void spawnerSystem(Entity* entity, Renderer* renderer, int layer) {
	if (entity->spawner) {
		SpawnerComponent* spawner = (SpawnerComponent*)entity->spawner;
		for (auto spawner_entity : spawner->spawner_entities) {
			if (spawner_entity.second) {
				renderSpriteSystem(spawner_entity.second, renderer, layer);
				renderSpriteSystem(spawner_entity.first, renderer, layer + 1);
			} else {
				renderSpriteSystem(spawner_entity.first, renderer, layer + 1);
			}
		}
	}
}

void timelineTickSystem(Entity* entity, Renderer* renderer, EventManager* eventManager) {
	if (entity->shrinkable) {
		ShrinkableComponent* shrinkable = (ShrinkableComponent*)entity->shrinkable;
		if (shrinkable->started) {
			if (entity->sprite) {
				SpriteComponent* spr = (SpriteComponent*)entity->sprite;
				if (entity->cuttable) {
					CuttableComponent* cuttable = (CuttableComponent*)entity->cuttable;
					cuttable->cut_width -= shrinkable->value;
					if (cuttable->cut_width < 0) cuttable->cut_width = 0;
				}
				if (entity->size) {
					SizeComponent* size = (SizeComponent*)entity->size;
					size->w -= shrinkable->value;
					if (size->w < 25)
						spr->tName = TEX_TIMELINE_WARN;
					else
						spr->tName = TEX_TIMELINE_BAR;
					if (size->w < 0) eventManager->Post(new Event(GAME_OVER, ""));
				}
			}
		}
	}
}

void HudSystem(Entity* entity, Renderer* renderer) {
	if (entity->score) {
		ScoreComponent* scr = (ScoreComponent*)entity->score;
		renderer->Print(std::to_string(scr->score).c_str());
	}
}