#include "Ship.h"
#include <math.h>

using namespace SpaceInvaders;

void Ship::MoveHorizontally(int offset)
{
	x += offset;

	x = fmax(x, maxLeft);
	x = fmin(x, maxRight);
}

void Ship::Draw(double animationTime) const
{
	DrawSprite(sprite, x, y, drawSize, drawSize, sin(animationTime * 10) * 0.1, 0xffffffff);
}
