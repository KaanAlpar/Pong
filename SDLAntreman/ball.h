#pragma once

typedef struct
{
	float x, y, w, h;
	float dx, dy;
}Ball;

void updateBallX(Ball* ball);
