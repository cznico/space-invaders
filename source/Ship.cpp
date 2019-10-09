#include "Ship.h"
#include <math.h>

using namespace SpaceInvaders;

Ship::Ship()
{
}

void Ship::MoveHorizontally(int Offset)
{
	x += Offset;

	x = fmax(x, MaxLeft);
	x = fmin(x, MaxRight);
}

Ship::~Ship()
{
}