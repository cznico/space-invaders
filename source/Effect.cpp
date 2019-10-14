#include "Effect.h"

using namespace SpaceInvaders;

float Effect::GetEffectPhase(double time)
{
	//if (startTime > time) 0;

	return (time - startTime) / duration;
}