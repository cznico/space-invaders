#include "Game.h"

#include <math.h>
#include <iostream>
#include "../lib/leetlib.h" // TODO remove dep

using namespace std;
using namespace SpaceInvaders;

void Game::Initialize(SpriteSet spriteSet)
{
	for (int n = 0; n < 50; ++n)
	{
		enemies[n].startPosition.x = (n % 10) * 60 + 120;
		enemies[n].x = enemies[n].startPosition.x;
		enemies[n].startPosition.y = (n / 10) * 60 + 70;
		enemies[n].y = enemies[n].startPosition.y;
		enemies[n].SetCollisionRadius(20);
	}

	ship = Ship();
	ship.x = 400;
	ship.y = 550;
	ship.SetCollisionRadius(40);

	sprites = spriteSet;
	time = 0;
}

void Game::ResolvePlayerHit()
{
	for (Invader &invader : enemies)
	{
		if (invader.IsColliding(&ship))
		{
			invader.enabled = false;
			lives--;
		}
	}
}


void Game::ResolveEnemyHits()
{
	for (Invader &invader : enemies)
	{
		for (Bullet &bullet : bullets) {
			if (invader.IsColliding(&bullet)) {
				bullet.enabled = false;
				invader.enabled = false;
				score++;
			};
		}

	}
}

void Game::ResolveInteractions()
{
	ResolvePlayerHit();
	ResolveEnemyHits();
}

void Game::Animate()
{
	++time;
	AnimateEnemies();
	AnimateShip();
	AnimateFiring();

	AnimateString("space invaders", 150, 30);
	AnimateString(std::to_string(score), 20, 560);
	AnimateString(std::to_string(lives), 780, 560);
}

void Game::AnimateString(string text, int x, int y)
{
	int posIndex = 0;
	for (char &ch : text) {
		auto sprite = sprites.font.find(ch);
		if (sprite != sprites.font.end()) DrawSprite(sprite->second, posIndex * 40 + x, y, 20, 20, sin(time * 0.1) * posIndex * 0.01);
		posIndex++;
	}
}

void Game::AnimateEnemies()
{
	for (int n = 0; n<50; ++n)
	{
		int xo = 0, yo = 0;
		int n1 = time + n * n + n * n*n;
		int n2 = time + n + n * n + n * n*n * 3;
		if (((n1 >> 6) & 0x7) == 0x7)xo += (1 - cos((n1 & 0x7f) / 64.0f * 2.f * PI))*(20 + ((n*n) % 9));
		if (((n1 >> 6) & 0x7) == 0x7)yo += (sin((n1 & 0x7f) / 64.0f * 2.f * PI))*(20 + ((n*n) % 9));
		if (((n2 >> 8) & 0xf) == 0xf)yo += (1 - cos((n2 & 0xff) / 256.0f * 2.f * PI))*(150 + ((n*n) % 9));

		Invader * enemy = &enemies[n];
		enemy->x = enemy->startPosition.x + xo;
		enemy->y = enemy->startPosition.y + yo;

		if (enemy->enabled)
		{
			DrawSprite(sprites.enemy, enemy->x, enemy->y, (10 + ((n) % 17)), (10 + ((n) % 17)), 0, 0xffffffff);
		}		
	}
}

void Game::AnimateShip()
{
	ship.MoveHorizontally(IsKeyDown(VK_LEFT) ? -7 : IsKeyDown(VK_RIGHT) ? 7 : 0);

	DrawSprite(sprites.ship, ship.x, ship.y, 50, 50, 3.141592 + sin(time*0.1)*0.1, 0xffffffff);
}

void Game::AnimateFiring()
{
	static int b = 0;
	static int count = 0;
	if (count) --count;
	if (!IsKeyDown(VK_SPACE)) count = 0;
	if (IsKeyDown(VK_SPACE) && count == 0)
	{
		bullets[b].x = ship.x;
		bullets[b].y = ship.y;
		bullets[b].enabled = true;
		b = (b + 1) % 10;
		count = 15;
	}

	for (int n = 0; n<10; ++n)
	{
		if (bullets[n].enabled)
		{
			DrawSprite(sprites.bullet, bullets[n].x, bullets[n].y -= 4, 10, 10, bullets[n].rotation += 0.1f, 0xffffffff);
		}		
	}
}
