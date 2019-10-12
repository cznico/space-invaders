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

	ship = Ship(40, maxX - 40);
	ship.x = maxX / 2;
	ship.y = maxY - 50;
	ship.SetCollisionRadius(40);

	sprites = spriteSet;
	elapsedTime = 0;
	state = GameState::IN_GAME;
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
				score += 10 + (level - 1) * 2; // Every level increases score points by 20%
			};
		}

	}
}

void Game::ResolveInteractions()
{
	ResolvePlayerHit();
	ResolveEnemyHits();
}

void Game::SetupLevel(int level)
{
	state = GameState::IN_GAME;
	lives = 3;
	level = level;

	for (auto &e : enemies) {
		e.enabled = true;
	}
}

void Game::ResetGame()
{
	score = 0;
	SetupLevel(1);
}

void Game::ResolveGameState()
{
	if (state == GameState::IN_GAME)
	{
		if (lives <= 0)
		{
			// TODO check if made to high scores
			state = GameState::DEAD;
			return;
		}
		
		int aliveEnemies = 0;
		for (auto &e : enemies) {
			if (e.enabled) aliveEnemies++;
		}

		if (aliveEnemies == 0)
		{
			state = GameState::LEVEL_FINISHED;
			return;
		}

	}
}

void Game::Animate(double timeDiff)
{
	AnimateEnemies();
	AnimateShip(timeDiff);
	AnimateFiring(timeDiff);

	AnimateString("space invaders", maxX / 2, 30, TextAlignment::CENTER);
	AnimateString(std::to_string(score), 20, maxY - 20);
	AnimateString(std::to_string(level), maxX - 20, 20);
	AnimateString(std::to_string(lives), maxX - 20, maxY - 20, TextAlignment::RIGHT);
}

void Game::AnimateString(string text, int x, int y)
{
	AnimateString(text, x, y, TextAlignment::LEFT);
}

void Game::AnimateString(string text, int x, int y, TextAlignment alignment)
{
	int posIndex = 0;
	int letterSize = 20;
	int letterSpacing = 2 * letterSize;
	int alignmentOffset = 0;
	int textSize = text.length() * letterSpacing;

	switch (alignment) {
	case TextAlignment::CENTER:
		alignmentOffset -= textSize / 2;
		break;
	case TextAlignment::RIGHT:
		alignmentOffset -= textSize;
		break;
	}

	float phase = sin(elapsedTime * 10);

	for (char &ch : text) {
		auto sprite = sprites.font.find(ch);
		if (sprite != sprites.font.end()) DrawSprite(sprite->second, posIndex * letterSpacing + x + alignmentOffset, y, letterSize, letterSize, phase * posIndex * 0.01);
		posIndex++;
	}
}

void Game::AnimateEnemies()
{
	int phase = elapsedTime * (100 + (level - 1) * 10); // Every level increases enemy speed by 10%
	for (int n = 0; n<50; ++n)
	{
		Invader * enemy = &enemies[n];

		if (enemy->enabled)
		{
			int xo = 0, yo = 0;
			int n1 = phase + n * n + n * n*n;
			int n2 = phase + n + n * n + n * n*n * 3;
			if (((n1 >> 6) & 0x7) == 0x7)xo += (1 - cos((n1 & 0x7f) / 64.0f * 2.f * PI))*(20 + ((n*n) % 9));
			if (((n1 >> 6) & 0x7) == 0x7)yo += (sin((n1 & 0x7f) / 64.0f * 2.f * PI))*(20 + ((n*n) % 9));
			if (((n2 >> 8) & 0xf) == 0xf)yo += (1 - cos((n2 & 0xff) / 256.0f * 2.f * PI))*(150 + ((n*n) % 9));

			enemy->x = enemy->startPosition.x + xo;
			enemy->y = enemy->startPosition.y + yo;

			DrawSprite(sprites.enemy, enemy->x, enemy->y, (10 + ((n) % 17)), (10 + ((n) % 17)), 0, 0xffffffff);
		}		
	}
}

void Game::AnimateShip(double timeDiff)
{
	int speed = timeDiff * 700;
	ship.MoveHorizontally(IsKeyDown(VK_LEFT) ? -speed : IsKeyDown(VK_RIGHT) ? speed : 0);

	DrawSprite(sprites.ship, ship.x, ship.y, 50, 50, 3.141592 + sin(elapsedTime * 10)*0.1, 0xffffffff);
}

void Game::AnimateFiring(double timeDiff)
{
	static int b = 0; // TODO move this to class/object space
	static int count = 0;  // TODO move this to class/object space

	int speed = timeDiff * 400;

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
			DrawSprite(sprites.bullet, bullets[n].x, bullets[n].y -= speed, 10, 10, bullets[n].rotation += timeDiff * 15, 0xffffffff);
		}		
	}
}

void Game::Tick(double elapsedMicroseconds)
{
	double timeDiff = elapsedMicroseconds - elapsedTime;
	elapsedTime = elapsedMicroseconds;

	ResolveGameState();

	switch (state)
	{
	case GameState::IN_GAME:
		Animate(timeDiff);
		ResolveInteractions();
		break;
	case GameState::LEVEL_FINISHED:
		SetupLevel(++level);
		break;
	case GameState::DEAD:
		AnimateString("you are dead", maxX / 2, maxY / 2, TextAlignment::CENTER);

		string scoredText = "you scored ";
		scoredText = scoredText + to_string(score);
		AnimateString(scoredText, maxX / 2, maxY / 2 + 50, TextAlignment::CENTER);
		if (IsKeyDown(VK_RETURN)) ResetGame();
		break;
	}
}