#include "Bullet.h"

#include "../lib/leetlib.h"

using namespace SpaceInvaders;

void Bullet::Draw(double animationTime) const
{
	if (sprite == nullptr) return;

	DrawSprite(sprite, x, y, drawSize, drawSize, rotation, 0xffffffff);
}