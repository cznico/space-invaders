#include "SpaceObject.h"

#include "../lib/leetlib.h"

using namespace SpaceInvaders;

bool SpaceObject::IsColliding(const SpaceObject &another) const
{
	if (!enabled || !another.enabled) return false;
	
	// TODO optimize using bounding box

	return Distance(another) < collisionRadius + another.collisionRadius;
}

void SpaceObject::SetupDrawProps(IDirect3DTexture9 * spriteToSet, float size)
{
	sprite = spriteToSet;
	drawSize = size;
};

void SpaceObject::Draw(double animationTime) const
{
	if (sprite == nullptr) return;

	DrawSprite(sprite, x, y, drawSize, drawSize, 0, 0xffffffff);
}