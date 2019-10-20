#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {

	class Invader : public SpaceObject
	{

	public:
		// Base position of invader without animation offset
		Point startPosition;

		float GetSize() const { return drawSize; };
	};
}
