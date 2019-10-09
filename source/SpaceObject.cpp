#include "SpaceObject.h"
#include <math.h>

using namespace SpaceInvaders;

SpaceObject::SpaceObject()
{
}


SpaceObject::~SpaceObject()
{
}

bool SpaceObject::IsColliding(const SpaceObject * another) const
{
	float offsetX = another->x - x;
	float offsetY = another->y - y;

	float dist = sqrt((offsetX * offsetX) + (offsetY * offsetY));

	return dist < collisionRadius + another->collisionRadius;
}