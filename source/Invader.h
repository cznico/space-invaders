#pragma once
#include "SpaceObject.h"
#include "Effect.h"

namespace SpaceInvaders {

	class Invader : public SpaceObject
	{

	public:
		Point startPosition;
		int size = 0;
	};
}
