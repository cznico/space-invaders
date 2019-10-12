#include "Point.h"
#include <math.h>

using namespace SpaceInvaders;

float Point::Distance(const Point * another) const
{
	float offsetX = another->x - x;
	float offsetY = another->y - y;

	return sqrt((offsetX * offsetX) + (offsetY * offsetY));
}