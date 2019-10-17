#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {
	class Bullet : public SpaceObject
	{
	public:
		float rotation = 0;

		void Draw(double animationTime) const override;
	};
}
