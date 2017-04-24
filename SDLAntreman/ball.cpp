#include "ball.h"

void updateBallX(Ball* ball)
{
	ball->x += ball->dx;
	ball->y += ball->dy;
}