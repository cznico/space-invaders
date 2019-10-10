#include "SpaceObject.h"
#include <math.h>

using namespace SpaceInvaders;

bool SpaceObject::IsColliding(const SpaceObject * another) const
{
	if (!enabled || !another->enabled) return false;

	float offsetX = another->x - x;
	float offsetY = another->y - y;

	float dist = sqrt((offsetX * offsetX) + (offsetY * offsetY));

	return dist < collisionRadius + another->collisionRadius;
}