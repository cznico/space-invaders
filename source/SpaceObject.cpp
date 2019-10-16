#include "SpaceObject.h"

using namespace SpaceInvaders;

bool SpaceObject::IsColliding(const SpaceObject &another) const
{
	if (!enabled || !another.enabled) return false;

	return Distance(another) < collisionRadius + another.collisionRadius;
}