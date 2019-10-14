#include "Effect.h"

using namespace SpaceInvaders;

float Effect::GetEffectPhase(double time)
{
	if (startTime > time) return 0;

	return (time - startTime) / duration;
}