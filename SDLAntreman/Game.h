#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>

#include "ball.h"
#include "paddle.h"

typedef struct
{
	float x, y;
}fvector2;

typedef struct
{
	Ball ball;

	Paddle paddles[2];

	SDL_Window* window;
	SDL_Renderer* renderer;

	fvector2 ballSpeed;
	int time;
	int resetCountdown;

	//score
	int scoreL, scoreR;
	SDL_Texture* label;
	int labelW, labelH;

	//Font
	TTF_Font* font;

	//Sounds
	Mix_Chunk* bounceSound;
	Mix_Chunk* missSound;
}Game;

void initGame(Game* game);
void resetBall(Game* game);
void resetScore(Game* game);
void process(Game* game);
void collisionDetect(Game* game); 
void throwBall(Game* game, int randY);
