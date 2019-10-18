#include "Bullet.h"

#include "../lib/leetlib.h"

using namespace SpaceInvaders;

int Bullet::ShotsCount = 0;

void Bullet::Draw(double animationTime)
{
	if (sprite == nullptr) return;

	DrawSprite(sprite, x, y, drawSize, drawSize, spin, 0xffffffff);
}

Bullet Bullet::CreateBullet(const Point &point)
{
	Bullet bullet;

	bullet.x = point.x;
	bullet.y = point.y;

	return bullet;
}