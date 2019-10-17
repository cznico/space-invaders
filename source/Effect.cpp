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
		auto color = 0x00ffffff | (byte((1 - phase) * 255) * 0x01000000); // 2 most-left hexa positions are alpha channel
		DrawSprite(sprite, x, y, drawSize + phase * 20, drawSize + phase * 20, animationTime, color);
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