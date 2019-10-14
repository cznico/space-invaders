#include "Invader.h"

using namespace SpaceInvaders;

void Invader::Kill(double time)
{
	explosion.startTime = time;
	explosion.x = x;
	explosion.y = y;

	enabled = false;
}

Effect * Invader::GetExplosion()
{
	return &explosion;
}