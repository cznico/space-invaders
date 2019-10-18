#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {

	class Invader : public SpaceObject
	{

	public:
		Point startPosition;

		float GetSize() const { return drawSize; };
	};
}
