#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {

	class Effect : public SpaceObject
	{
	protected:
		double startTime = 0;
		float duration = 0.25f;

	public:
		float GetEffectPhase(double time);
		void StartAnimation(double start, double durationToSet);
		void Draw(double animationTime) override;
		static Effect CreateEffect(const Point &point, double startTime);
	};

}