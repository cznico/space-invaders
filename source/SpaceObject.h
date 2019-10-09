#pragma once
#include "Point.h"

namespace SpaceInvaders
{
	class SpaceObject : public Point
	{
	protected:
		float collisionRadius = 0;

	public:
		SpaceObject();
		~SpaceObject();
	
		void SetCollisionRadius(float newRadius) { collisionRadius = newRadius; };
		bool IsColliding(const SpaceObject * another) const;
	};

}

