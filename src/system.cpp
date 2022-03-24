#include "system.h"
#include "component.h"
#include "entity.h"
#include "event.h"
#include "renderer.h"
#include <cmath>
#include <iostream>

void BlitSpriteSystem(Entity* entity, Renderer* renderer, int layer, bool full, SDL_RendererFlip flip_flag) {
	try {
		if (entity->position && entity->sprite) {
			PositionComponent* pos = (PositionComponent*)entity->position;
			SpriteComponent* spr = (SpriteComponent*)entity->sprite;
			Texture* tex = renderer->GetTexture(spr->tName);

			if (spr->layer != layer) {
				// std::cout << spr->layer << std::endl;
				return;
			}

			double x = pos->x;
			double y = pos->y;
			int w = tex->w;
			int h = tex->h;
			double scale = spr->scale;
			double angle = 0.0;
			int repeat = 1;
			int frame = 0;
			int offset = -1;

			if (entity->angle) {
				RotateComponent* r = (RotateComponent*)entity->angle;
				angle = r->angle;
			}

			if (entity->spriteSpan) {
				SpriteSpanComponent* spn = (SpriteSpanComponent*)entity->spriteSpan;

				repeat = spn->repeat;
			}

			if (entity->anim) {
				AnimComponent* anm = (AnimComponent*)entity->anim;
				w = anm->w;
				h = anm->h;

				anm->value += anm->decay;
				if (anm->value <= 0.0) {
					anm->value = 1.0;
					anm->currFrame = (anm->currFrame + 1) % anm->fCount;
				}

				frame = anm->currFrame;
				if (anm->offset != -1)
					offset = anm->offset;
			}

			for (int j = 0; j < repeat; ++j) {
				renderer->Blit(x + (j * w), y, w, h, angle, tex, scale, frame, offset, full, flip_flag);
			}
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void AngleTickSystem(Entity* entity) {
	if (entity->angle) {
		RotateComponent* r = (RotateComponent*)entity->angle;
		r->angle += r->angleAcc;
	}
}

void FlappyPhysicsSystem(Entity* entity) {
	if (entity->flappyPhysics) {
		FlappyPhysicsComponent* flap = (FlappyPhysicsComponent*)entity->flappyPhysics;
		PositionComponent* pos = (PositionComponent*)entity->position;
		RotateComponent* rot = (RotateComponent*)entity->angle;
		SizeComponent* size = (SizeComponent*)entity->size;

		flap->yAcc += flap->grav;
		pos->y += flap->yAcc;
		if ((pos->y + size->h) > (WIN_Y - 160))
			pos->y = WIN_Y - 160 - size->h;
		rot->angle = flap->yAcc;
	}
}

void FlappyInputSystem(Entity* entity) {
	if (entity->flappyInput) {
		FlappyInputComponent* fIn = (FlappyInputComponent*)entity->flappyInput;
		FlappyPhysicsComponent* flap = (FlappyPhysicsComponent*)entity->flappyPhysics;

		flap->yAcc += fIn->lift;
		if (flap->yAcc < -20.0) {
			flap->yAcc = -20.0;
		}

		if (flap->yAcc > 2.0) {
			flap->yAcc = 2.0;
		}
	}
}

void PipeSpawnerTickSystem(Entity* entity, EventManager* eventManager) {
	if (entity->pipeSpawn) {
		PipeSpawnerComponent* pip = (PipeSpawnerComponent*)entity->pipeSpawn;

		pip->value += pip->decay;
		if (pip->value <= 0.0) {
			pip->value = 1.0;
			eventManager->Post(new Event(SPAWN_PIPE, "q"));
		}
	}
}

void PipeTickSystem(Entity* entity) {
	if (entity->pipe) {
		PipeComponent* pip = (PipeComponent*)entity->pipe;
		PositionComponent* pos = (PositionComponent*)entity->position;

		pos->y = pip->offset; // top of gap
		pos->x += pip->xAcc;

		if (pos->x + 160.0 < 0.0)
			delete entity;
	}
}

void PipeSpriteSystem(Entity* entity, Renderer* renderer) {
	try {
		if (entity->pipeSprite) {
			PositionComponent* pos = (PositionComponent*)entity->position;
			PipeSpriteComponent* psp = (PipeSpriteComponent*)entity->pipeSprite;
			Texture* tex = renderer->GetTexture(psp->tName);
			// pipe top
			renderer->Blit(pos->x, pos->y - 800, tex->w, tex->h, 180.0, tex, 1.0, 0, -1, false, SDL_FLIP_HORIZONTAL);
			// pipe bottom
			renderer->Blit(pos->x, pos->y + PIPE_GAP, tex->w, tex->h, 0.0, tex, 1.0, 0, -1);
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void CollisionHandlerSystem(EntityManager* entMan, Entity* entity, EventManager* eventManager) {
	// find flappy
	if (entity->flappyPhysics) {
		PositionComponent* aPos = (PositionComponent*)entity->position;
		SizeComponent* aSize = (SizeComponent*)entity->size;

		if (aPos->y + aSize->h >= WIN_Y - 160.0) {
			eventManager->Post(new Event(GAME_RESTART, "q"));
			return;
		}

		// find pipes
		for (auto entity : entMan->entities) {
			if (entity->collidable) {
				PositionComponent* bPos = (PositionComponent*)entity->position;
				SizeComponent* bSize = (SizeComponent*)entity->size;

				// top pipe
				double topX = bPos->x;							   // origin x
				double topY = 0.0;								   // origin y
				double topW = 160.0;							   // width
				double topH = bPos->y;							   // height
				double tCnX = topX + (topW / 2.0);				   // centre x
				double tCnY = topY + (topH / 2.0);				   // centre y
				double tSpX = tCnX - (aPos->x + (aSize->w / 2.0)); // seperation x
				double tSpY = tCnY - (aPos->y + (aSize->h / 2.0)); // seperation y
				double tHfX = tCnX - topX;						   // half width origin x
				double tHfY = tCnY - topY;						   // half width origin y

				// bottom pipe
				double botX = bPos->x;
				double botY = bPos->y + bSize->h;
				double botW = 160.0;
				double botH = (WIN_Y - 160.0) - (botY);
				double bCnX = botX + (botW / 2.0);
				double bCnY = botY + (botH / 2.0);
				double bSpX = bCnX - (aPos->x + (aSize->w / 2.0)); // seperation x
				double bSpY = bCnY - (aPos->y + (aSize->h / 2.0)); // seperation y
				double bHfX = bCnX - botX;
				double bHfY = bCnY - botY;

				double aHalfX = (aPos->x + (aSize->w / 2.0)) - aPos->x;
				double aHalfY = (aPos->y + (aSize->h / 2.0)) - aPos->y;

				double topPenX = abs(tSpX) - (aHalfX + tHfX);
				if (topPenX > 0.0)
					topPenX = 0.0;
				double topPenY = abs(tSpY) - (aHalfY + tHfY);
				if (topPenY > 0.0)
					topPenY = 0.0;

				double botPenX = abs(bSpX) - (aHalfX + bHfX);
				if (botPenX > 0.0)
					botPenX = 0.0;
				double botPenY = abs(bSpY) - (aHalfY + bHfY);
				if (botPenY > 0.0)
					botPenY = 0.0;

				if (topPenX != 0.0 && topPenY != 0.0) {
					eventManager->Post(new Event(GAME_RESTART, "q"));
					return;
				}

				if (botPenX != 0.0 && botPenY != 0.0) {
					eventManager->Post(new Event(GAME_RESTART, "q"));
					return;
				}

				// check for score trigger collision
				if ((aPos->x + aSize->w) > bPos->x + ((160.0 / 3.0) * 2)) {
					PipeComponent* pip = (PipeComponent*)entity->pipe;

					if (pip->hasScore) {
						pip->hasScore = false;
						eventManager->Post(new Event(INC_SCORE, " "));
					}
				}

				// prevent flappy going over pipes
				if (aPos->y + aSize->h <= 0) {
					if ((aPos->x + aSize->w > bPos->x) &&
						(aPos->x + aSize->w < bPos->x + bSize->w))
						eventManager->Post(new Event(GAME_RESTART, "q"));
				}

				return;
			}
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
