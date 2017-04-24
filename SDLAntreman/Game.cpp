#include <time.h>
#include <stdlib.h>

#include "Game.h"
#include "ball.h"
#include "mka_util.h"

const float MAX_BOUNCE_ANGLE = 5 * 3.14 / 10;

void initGame(Game* game)
{
	srand((int)time(NULL));

	game->time = 0;
	game->scoreL = 0;
	game->scoreR = 0;

	game->paddles[0].x = 10;
	game->paddles[0].y = 640/2 - 50;
	game->paddles[0].w = 15;
	game->paddles[0].h = 100;
	game->paddles[0].direction = 0;

	game->paddles[1].x = 995;
	game->paddles[1].y = 640/2 - 50;
	game->paddles[1].w = 15;
	game->paddles[1].h = 100;
	game->paddles[1].direction = 0;

	game->ball.w = 10;
	game->ball.h = 10;
	game->ballSpeed.x = 10;
	game->ballSpeed.y = 5;
	game->ball.x = 1024 / 2 - game->ball.w;
	game->ball.y = 640 / 2;
	throwBall(game, 3);

	game->resetCountdown = -1;
}

void resetBall(Game* game)
{
	if (game->resetCountdown < 0)
	{
		game->resetCountdown = 100;
	}
	game->ball.dx = 0;
	game->ball.dy = 0;
	game->ball.x = 1024 / 2 - game->ball.w;
	game->ball.y = 640 / 2;
}

void throwBall(Game* game, int randY)
{
	//Start the ball randomly//
new_num:
	int u = ((double)rand() / (RAND_MAX + 1) * (2 - -2) + -2);
	if (u == 0) goto new_num;
	game->ball.dx = u * 5;

	u = ((double)rand() / (RAND_MAX + 1) * (10 - -10) + -10);
	if (u < 0) u = -1; else if (u > 0) u = 1; else if (u == 0) u = 1;
	game->ball.dy = u * rand() % randY;
	game->resetCountdown = -1;
}

void resetScore(Game* game)
{
	game->scoreL = 0;
	game->scoreR = 0;
}

void process(Game* game)
{
	if (game->scoreL >= 10 || game->scoreR >= 10)
	{
		resetScore(game);
	}

	//Hold the ball for a second when new round starts
	if (game->resetCountdown > 0)
	{
		game->resetCountdown--;
		if (game->resetCountdown <= 0)
		{
			throwBall(game, 7);
		}
	}

	//Ball restriction
	if (game->ball.y <= 0 || game->ball.y >= 640 - game->ball.h)
	{
		game->ball.dy *= -1;
	}
	if (game->ball.x <= -50)
	{
		Mix_PlayChannel(-1, game->missSound, 0);
		game->scoreR++;
		resetBall(game);
	}
	if (game->ball.x >= 1024 - game->ball.w / 2 + 50)
	{
		Mix_PlayChannel(-1, game->missSound, 0);
		game->scoreL++;
		resetBall(game);
	}

	//Paddle restrictions
	for (int i = 0; i < 2; i++)
	{
		if (game->paddles[i].y <= -80)
		{
			game->paddles[i].y = -80;
		}
		if (game->paddles[i].y >= 620)
		{
			game->paddles[i].y = 620;
		}
	}
		
	//Score
	SDL_Color white = { 255, 255, 255, 255 };

	char score[128];
	sprintf_s(score, "%d|%d", game->scoreL, game->scoreR);

	SDL_Surface* textSurface = TTF_RenderText_Blended(game->font, score, white);
	game->labelW = textSurface->w;
	game->labelH = textSurface->h;
	game->label = SDL_CreateTextureFromSurface(game->renderer, textSurface);
	SDL_FreeSurface(textSurface);

	//update ball
	updateBallX(&game->ball);

	game->time++;
}

void collisionDetect(Game* game)
{
	//paddel left
	if (collide2d(game->paddles[0].x, game->paddles[0].y, game->ball.x, game->ball.y, game->paddles[0].w, game->paddles[0].h, game->ball.w, game->ball.h))
	{
		Mix_PlayChannel(-1, game->bounceSound, 0);

		float relativeIntersect = (game->paddles[0].y + (game->paddles[0].h / 2)) - game->ball.y;
		float normalizedRelativeIntersection = (relativeIntersect / (game->paddles[0].h));
		float bounceAngle = normalizedRelativeIntersection * MAX_BOUNCE_ANGLE;

		game->ball.dx = game->ballSpeed.x * cos(bounceAngle);
		game->ball.dy = game->ballSpeed.y * -sin(bounceAngle);
	}

	//paddel right
	if (collide2d(game->paddles[1].x, game->paddles[1].y, game->ball.x, game->ball.y, game->paddles[1].w, game->paddles[1].h, game->ball.w, game->ball.h))
	{
		Mix_PlayChannel(-1, game->bounceSound, 0);
	
		float relativeIntersect = (game->paddles[1].y + (game->paddles[1].h / 2)) - game->ball.y;
		float normalizedRelativeIntersection = (relativeIntersect / (game->paddles[1].h));
		float bounceAngle = normalizedRelativeIntersection * MAX_BOUNCE_ANGLE;

		game->ball.dx = -game->ballSpeed.x * cos(bounceAngle);
		game->ball.dy = game->ballSpeed.y * -sin(bounceAngle);
	}
}