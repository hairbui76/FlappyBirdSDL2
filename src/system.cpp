#include "system.h"
#include "component.h"
#include "entity.h"
#include "event.h"
#include "renderer.h"
#include <SDL_TTF.h>
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
		double alpha = spr->alpha;
		SDL_FPoint center{0.0, 0.0};
		SDL_RendererFlip flip_flag = spr->flip_flag;

		if (entity->angle) {
			RotateComponent* r = (RotateComponent*)entity->angle;
			angle = r->angle;
			center.x = r->origin_x;
			center.y = r->origin_y;
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
			renderer->renderSprite(sRect, dRect, angle, &center, alpha, tex, spr->tName, full, flip_flag);
			return;
		}
		renderer->renderSprite(sRect, dRect, angle, &center, alpha, tex, spr->tName, full, flip_flag);
	}
}

void handAnimationSystem(Entity* entity, Renderer* renderer, EventManager* eventManager, EntityManager* entMan, int layer) {
	if (entity->handAnimation) {
		HandAnimationComponent* handAnimation = (HandAnimationComponent*)entity->handAnimation;
		if (handAnimation->hand_animation_entities.size() > 0) {
			Entity* hand_animation_entity = handAnimation->hand_animation_entities[handAnimation->current_frame];
			SDL_Delay(60);
			renderSpriteSystem(hand_animation_entity, renderer, layer);
			if (handAnimation->current_frame == handAnimation->hand_animation_entities.size() - 1) Renderer::PlaySound(HIT);
			// check for game over state => update animation system to wait for game over change scene
			if (entity->dead) {
				DeadComponent* dead = (DeadComponent*)entity->dead;
				// check if already dead but not finished rendering
				if (dead->is_dead && !dead->is_over && handAnimation->current_frame == handAnimation->hand_animation_entities.size() - 1) {
					dead->is_over = true;
					// check if already dead and finished rendering => post event to change scene
				} else if (dead->is_dead && dead->is_over && handAnimation->current_frame == 0) {
					Renderer::PlaySound(DIE);
					if (entity->movable) {
						MovableComponent* movable = (MovableComponent*)entity->movable;
						int last_score = 0;
						// find score entity
						for (auto entity : entMan->entities) {
							if (entity->score) {
								ScoreComponent* score = (ScoreComponent*)entity->score;
								last_score = score->score;
								break;
							}
						}
						if (movable->state == LEFT)
							eventManager->Post(new Event(CHANGE_SCENE, "LEFT", 0.0, 0.0, last_score));
						else
							eventManager->Post(new Event(CHANGE_SCENE, "RIGHT", 0.0, 0.0, last_score));
					}
				}
			}
			if (handAnimation->current_frame == handAnimation->hand_animation_entities.size() - 1)
				handAnimation->current_frame = 0;
		}
	}
}

void autoAnimationSystem(Entity* entity, Renderer* renderer, int layer) {
	if (entity->autoAnimation) {
		AutoAnimationComponent* autoAnimation = (AutoAnimationComponent*)entity->autoAnimation;
		if (autoAnimation->is_started) {
			if (entity->sprite) {
				SpriteComponent* sprite = (SpriteComponent*)entity->sprite;
				if (entity->disappear) {
					sprite->alpha = 1.0 - autoAnimation->current_frame * (1.0 / autoAnimation->frame_end);
				}
				if (entity->disappearAngle) {
					DisappearAngleComponent* disappearAngle = (DisappearAngleComponent*)entity->disappearAngle;
					if (entity->angle) {
						RotateComponent* angle = (RotateComponent*)entity->angle;
						angle->origin_x = disappearAngle->origin_x;
						angle->origin_y = disappearAngle->origin_y;
						if (sprite->flip_flag == SDL_FLIP_HORIZONTAL)
							angle->angle = -autoAnimation->current_frame * (disappearAngle->end_angle / autoAnimation->frame_end);
						else if (sprite->flip_flag == SDL_FLIP_NONE)
							angle->angle = autoAnimation->current_frame * (disappearAngle->end_angle / autoAnimation->frame_end);
					} else {
						if (sprite->flip_flag == SDL_FLIP_HORIZONTAL)
							entity->angle = new RotateComponent(-autoAnimation->current_frame * (disappearAngle->end_angle / autoAnimation->frame_end), disappearAngle->origin_x, disappearAngle->origin_y);
						else if (sprite->flip_flag == SDL_FLIP_NONE)
							entity->angle = new RotateComponent(autoAnimation->current_frame * (disappearAngle->end_angle / autoAnimation->frame_end), disappearAngle->origin_x, disappearAngle->origin_y);
					}
				}
				renderSpriteSystem(entity, renderer, layer);
				if (autoAnimation->current_frame < autoAnimation->frame_end)
					autoAnimation->current_frame++;
				else {
					autoAnimation->current_frame = 0;
					autoAnimation->is_started = false;
				}
			}
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

void timelineTickSystem(Entity* entity, EventManager* eventManager) {
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