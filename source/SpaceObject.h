#pragma once
#include "Point.h"

#include "../lib/leetlib.h"

namespace SpaceInvaders
{
	class SpaceObject : public Point
	{
	protected:
		float collisionRadius = 0;
		float drawSize = 0;

		IDirect3DTexture9 * sprite = nullptr;

	public:
		bool enabled = true;
	
		void SetCollisionRadius(float newRadius) { collisionRadius = newRadius; };
		void SetupDrawProps(IDirect3DTexture9 * spriteToSet, float size);
		bool IsColliding(const SpaceObject &another) const;

		virtual void Draw(double animationTime) const;
	};

}

