#include "Ship.h"
#include <math.h>

using namespace SpaceInvaders;

void Ship::MoveHorizontally(int offset)
{
	x += offset;

	x = fmax(x, maxLeft);
	x = fmin(x, maxRight);
}
