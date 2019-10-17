#include "Effect.h"

using namespace SpaceInvaders;

float Effect::GetEffectPhase(double time)
{
	if (startTime > time) return 0;

	return (time - startTime) / duration;
}

void Effect::StartAnimation(double start, double durationToSet)
{
	enabled = true;
	startTime = start;
	duration = durationToSet;
}

void Effect::Draw(double animationTime)
{
	if (!enabled) return;

	float phase = GetEffectPhase(animationTime);

	if (phase > 1.f)
	{
		enabled = false;
	}
	else
	{
		DrawSprite(sprite, x, y, drawSize + phase * 30, drawSize + phase * 30, animationTime, 0xffffffff);
	}
}

Effect Effect::CreateEffect(const Point &point, double startTime)
{
	Effect effect;

	effect.StartAnimation(startTime, 1.f);
	effect.x = point.x;
	effect.y = point.y;

	return effect;
}