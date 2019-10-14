#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {

	class Ship : public SpaceObject
	{
	protected:
		int maxLeft;
		int maxRight;

		public:
			Ship(int maxLeft, int maxRight) : maxLeft(maxLeft), maxRight(maxRight) { SpaceObject(); };

			void MoveHorizontally(int offset);
	};

}

