#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders
{
	class Loot : public SpaceObject
	{
	public:
		// Value to add to score
		int value = 0;
		unsigned int invaderIndex;

		Loot(int value, unsigned int invaderIndex) : value(value), invaderIndex(invaderIndex) { enabled = false; };
	};

}

