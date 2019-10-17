#pragma once
#include "SpaceObject.h"

namespace SpaceInvaders
{
	class Loot : public SpaceObject
	{
	public:
		int value = 0;
		int invaderIndex;

		Loot(int value, int invaderIndex) : value(value), invaderIndex(invaderIndex) { enabled = false; };
	};

}

