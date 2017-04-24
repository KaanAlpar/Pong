#include "mka_util.h"

#include <stdio.h>
#include <math.h>

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
	return (((x1 + wt1 / 2 > x2) && (x1 + wt1 / 2 < x2 + wt2)) && ((y1 <= y2 + ht2) && (y1 + ht1 >= y2)));
}
