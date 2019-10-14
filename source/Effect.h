#pragma once
#include "Point.h"

namespace SpaceInvaders {

	class Effect : public Point
	{
	public:
		double startTime = 0;
		float duration = 0.25f;

		float GetEffectPhase(double time);
	};

}