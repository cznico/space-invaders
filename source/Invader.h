#pragma once
#include "SpaceObject.h"
#include "Effect.h"

namespace SpaceInvaders {

	class Invader : public SpaceObject
	{
	protected:
		Effect explosion;

	public:
		Point startPosition;
		
	public:
		void Kill(double time);
		Effect * GetExplosion();

	};
}
