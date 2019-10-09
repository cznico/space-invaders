#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders {

	class Ship : public SpaceObject
	{
	protected:
		int MaxLeft = 0;
		int MaxRight = 800;

		public:
			Ship();
			~Ship();

			void MoveHorizontally(int Offset);
	};

}

