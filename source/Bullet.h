#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {
	class Bullet : public SpaceObject
	{
	public:
		static int ShotsCount;
		float spin = 0;

	public:
		void Draw(double animationTime) override;

		static Bullet CreateBullet(const Point &point);
	};
}
