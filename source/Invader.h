#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {

	class Invader : public SpaceObject
	{
	public:
		Point startPosition;
	public:
		Invader();
		~Invader();
	};
}
