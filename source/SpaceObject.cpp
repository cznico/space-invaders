#include "SpaceObject.h"

using namespace SpaceInvaders;

bool SpaceObject::IsColliding(const SpaceObject &another) const
{
	if (!enabled || !another.enabled) return false;
	
	// Can be optimized using bounding box and quad trees in the future

	return Distance(another) < collisionRadius + another.collisionRadius;
}

void SpaceObject::SetupDrawProps(IDirect3DTexture9 * spriteToSet, float size)
{
	sprite = spriteToSet;
	drawSize = size;
};

void SpaceObject::Draw(double animationTime)
{
	if (sprite == nullptr) return;

	DrawSprite(sprite, x, y, drawSize, drawSize, 0, 0xffffffff);
}