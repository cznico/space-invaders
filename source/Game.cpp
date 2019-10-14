#include "Game.h"

#include <math.h>
#include <iostream>
#include "../lib/leetlib.h" // TODO remove dep
#include "Leaderboard.h"

#include <vector>
#include <random>
#include <algorithm>
#include <iterator>


using namespace std;
using namespace SpaceInvaders;

vector<int> GetRandomVector(int maxNumber, int count)
{
	vector<int> result;

	for (int i = 0; i <= maxNumber; i++)
	{
		result.push_back(i);
	}

	random_device rd;
	mt19937 g(rd());

	shuffle(result.begin(), result.end(), g);
	result.resize(count);

	return result;
}

void Game::Initialize(SpriteSet * spriteSet, AudioSet * audioSet, Leaderboard * highscores)
{
	int invaderSpreadX = 70;
	int invaderSpreadY = 60;
	int invadersPerLine = 10;
	int leftOffset = (maxX - (invaderSpreadX * (invadersPerLine - 1))) / 2;

	for (int n = 0; n < ENEMIES_COUNT; ++n)
	{
		Invader * enemy = &enemies[n];
		enemy->startPosition.x = (n % invadersPerLine) * invaderSpreadX + leftOffset;
		enemy->x = enemies[n].startPosition.x;
		enemy->startPosition.y = (n / invadersPerLine) * invaderSpreadY + 70;
		enemy->y = enemies[n].startPosition.y;
		enemy->size = 10 + ((n) % 17);
		enemy->SetCollisionRadius(enemy->size);
	}

	ship = Ship(60, maxX - 60);
	ship.x = maxX / 2;
	ship.y = maxY - 50;
	ship.SetCollisionRadius(40);

	sprites = spriteSet;
	audio = audioSet;

	for (Bullet &bullet : bullets)
	{
		bullet.SetCollisionRadius(15);
		bullet.enabled = false;
	}

	ui = UserInterface(maxX, maxY);
	ui.SetSpriteSet(spriteSet);

	elapsedTime = 0;
	SetGameState(GameState::INTRO);

	PlayMusic(audio->music.c_str());

	leaderboard = highscores;

	ResetLoot();
}

void Game::CaptureName()
{
	if (IsKeyHitSinceLastFlip(VK_BACK) && playerName.length() > 0)
	{
		playerName.pop_back();
	}

	for (char i = 'a'; i <= 'z'; ++i)
	{
		if (IsKeyHitSinceLastFlip(i - 32) && playerName.length() < 12)
		{
			playerName += i;
		}
	}
}

void Game::ResolvePlayerHit()
{
	for (Invader &invader : enemies)
	{
		if (invader.IsColliding(&ship))
		{
			invader.Kill(gameTime);
			lives--;
			PlaySnd(audio->shipHit, 1);
		}
	}

	for (auto &lootIt : loot)
	{
		auto lootItem = &lootIt.second;
		if (lootItem->IsColliding(&ship))
		{
			lootItem->enabled = false;
			score += lootItem->value;

			PlaySnd(audio->pickup, 0.5);
		}
	}
}

void Game::ResolveEnemyHits()
{
	int n = 0;
	for (Invader &invader : enemies)
	{
		for (Bullet &bullet : bullets) {
			if (invader.IsColliding(&bullet)) {
				bullet.enabled = false;
				invader.Kill(gameTime);
				score += 10 + (level - 1) * 2; // Every level increases score points by 20%
 				PlaySnd(audio->hit, 1);

				// Do we need to enable loot drop
				auto lootIt = loot.find(n);
				
				if (lootIt != loot.end()) {
					lootIt->second.x = invader.x;
					lootIt->second.y = invader.y;
					lootIt->second.enabled = true;
				}

			};
		}
		n++;
	}
}

void Game::ResolveInteractions()
{
	if (state != GameState::IN_GAME) return;

	ResolvePlayerHit();
	ResolveEnemyHits();
}

void Game::SetupLevel(int newLevel)
{
	lives = 3;
	level = newLevel;

	for (auto &e : enemies)
	{
		e.enabled = true;
	}

	for (auto &b : bullets)
	{
		b.enabled = false;
	}

	ResetLoot();
}

void Game::ResetGame()
{
	score = 0;
	SetupLevel(1);
}

void Game::SetGameState(GameState newState)
{
	switch (newState)
	{
	case GameState::IN_GAME_PREPARE:
		ResetTime();
		PlaySnd(audio->ready, 0.7);
		break;
	case GameState::DEAD:
		PlaySnd(audio->dead, 0.7);
		break;
	}

	state = newState;
}

void Game::ResolveGameState()
{
	if (state == GameState::IN_GAME)
	{
		if (lives <= 0)
		{
			if (leaderboard->HitLeaderboard(score))
			{
				SetGameState(GameState::HIGHSCORED);
				return;
			}
			SetGameState(GameState::DEAD);
			return;
		}
		
		int aliveEnemies = 0;
		int activeLoot = 0;

		for (auto &e : enemies) {
			if (e.enabled) aliveEnemies++;
		}

		for (auto &l : loot) {
			if (l.second.enabled) activeLoot++;
		}

		if (aliveEnemies == 0 && activeLoot == 0)
		{
			SetGameState(GameState::LEVEL_FINISHED);
			return;
		}

		if (IsKeyHitSinceLastFlip('P')) {
			SetGameState(GameState::PAUSED);
			return;
		}

	}

	if (state == GameState::IN_GAME_PREPARE && elapsedTime > 3)
	{
		SetGameState(GameState::IN_GAME);
		return;
	}

	if (state == GameState::DEAD && IsKeyHitSinceLastFlip(VK_RETURN))
	{
		ResetGame();
		SetGameState(GameState::IN_GAME_PREPARE);
		return;
	}

	if (state == GameState::PAUSED && IsKeyHitSinceLastFlip('P'))
	{
		SetGameState(GameState::IN_GAME);
		return;
	}

	if (state == GameState::LEVEL_FINISHED)
	{
		SetupLevel(++level);
		SetGameState(GameState::IN_GAME_PREPARE);
		return;
	}

	if (state == GameState::HIGHSCORED && IsKeyHitSinceLastFlip(VK_RETURN))
	{
		leaderboard->Update(score, playerName);
		leaderboard->Save();

		SetGameState(GameState::LEADERBOARD);
		return;
	}

	if (state == GameState::LEADERBOARD && IsKeyHitSinceLastFlip(VK_RETURN))
	{
		ResetGame();
		SetGameState(GameState::IN_GAME_PREPARE);
		return;
	}

	if (state == GameState::INTRO && IsKeyHitSinceLastFlip(VK_RETURN))
	{
		SetGameState(GameState::IN_GAME_PREPARE);
		return;
	}
}

void Game::ResetLoot()
{
	loot.clear();

	int lootValue = 20 + (5 * level);
	auto indices = GetRandomVector(ENEMIES_COUNT - 1, 5);

	for (int index : indices)
	{
		loot.emplace(index, Loot(lootValue, index));
	}
	
}

void Game::AnimateGame(double timeDiff)
{
	AnimateEnemies();	
	AnimateFiring(timeDiff);
	AnimateShip(timeDiff);
	AnimateLoot(timeDiff);
	AnimateExplosions();

	ResolveInteractions();
}


void Game::AnimateEnemies()
{
	int phase = gameTime * (100 + (level - 1) * 10); // Every level increases enemy speed by 10%

	for (int n = 0; n < ENEMIES_COUNT; ++n)
	{
		Invader * enemy = &enemies[n];

		if (enemy->enabled)
		{
			int xo = 0;
			int yo = min((int)(phase / 1000) * 30, 200); // Descending behaviour

			int n1 = phase + n * n + n * n * n;
			
			if (((n1 >> 6) & 0x7) == 0x7) // Rotating behaviour
			{
				xo += (1 - cos((n1 & 0x7f) / 64.0f * 2.f * PI)) * (20 + ((n * n) % 9));
				yo += (sin((n1 & 0x7f) / 64.0f * 2.f * PI)) * (20 + ((n * n) % 9));
			}

			int n2 = phase + n + n * n + n * n * n * 3;

			if (((n2 >> 8) & 0xf) == 0xf) // Swooping behaviour
			{
				yo += (1 - cos((n2 & 0xff) / 256.0f * 2.f * PI)) * (150 + ((n * n) % 9));
			}

			enemy->x = enemy->startPosition.x + xo;
			enemy->y = enemy->startPosition.y + yo;

			DrawSprite(sprites->enemy, enemy->x, enemy->y, enemy->size, enemy->size, 0, 0xffffffff);
		}
	}
}

void Game::AnimateExplosions()
{
	for (int n = 0; n < ENEMIES_COUNT; ++n)
	{
		Invader * enemy = &enemies[n];

		if (!enemy->enabled)
		{
			Effect * explosion = enemy->GetExplosion();
			float explosionPhase = explosion->GetEffectPhase(gameTime);

			if (explosionPhase < 1.f)
			{
				DrawSprite(sprites->explosion, explosion->x, explosion->y, enemy->size + explosionPhase * 30, enemy->size + explosionPhase * 30, gameTime, 0xffffffff);
			}

		}
	}
}

void Game::AnimateLoot(double timeDiff)
{
	for (auto &lootIt : loot)
	{
		auto lootItem = &lootIt.second;
		if (!lootItem->enabled) continue;
 
		lootItem->y = lootItem->y + timeDiff * 100;

		if (lootItem->y > maxY) lootItem->enabled = false;

		DrawSprite(sprites->loot, lootItem->x, lootItem->y, 15, 15, 0, 0xffffffff);
	}
}

void Game::AnimateShip(double timeDiff)
{
	int speed = timeDiff * 700;
	ship.MoveHorizontally(IsKeyDown(VK_LEFT) ? -speed : IsKeyDown(VK_RIGHT) ? speed : 0);

	DrawSprite(sprites->ship, ship.x, ship.y, 50, 50, sin(elapsedTime * 10) * 0.1, 0xffffffff);
}

void Game::AnimateFiring(double timeDiff)
{
	static int b = 0; // TODO move this to class/object space
	static double shotDelay = 0;  // TODO move this to class/object space

	int speed = timeDiff * 400;

	if (shotDelay > 0)
	{
		shotDelay -= timeDiff;
	}
	if (!IsKeyDown(VK_SPACE)) shotDelay = 0;
	if (IsKeyDown(VK_SPACE) && shotDelay <= 0 && state == GameState::IN_GAME)
	{
		bullets[b].x = ship.x;
		bullets[b].y = ship.y - 20;
		bullets[b].enabled = true;
		b = (b + 1) % 10;
		shotDelay = .25f; // 250ms rate of fire

		PlaySnd(audio->fire, 0.5);
	}

	for (int n = 0; n<10; ++n)
	{
		Bullet &bullet = bullets[n];
		if (bullet.enabled)
		{
			bullet.y -= speed;
			bullet.rotation += timeDiff * 2;

			DrawSprite(sprites->bullet, bullets[n].x, bullets[n].y, 15, 15, bullets[n].rotation, 0xffffffff);
		}		
	}
}

double Game::UpdateTime(double elapsedSeconds)
{
	double newElapsedTime = elapsedSeconds - startTime;
	double timeDiff = newElapsedTime - elapsedTime;
	elapsedTime = newElapsedTime;

	ui.SetAnimationTime(elapsedTime);

	return timeDiff;
}

void Game::ResetTime()
{
	startTime += elapsedTime;
	gameTime = 0;
}

void Game::Tick(double elapsedSeconds)
{
	double timeDiff = UpdateTime(elapsedSeconds);

	ResolveGameState();

	switch (state)
	{
	case GameState::INTRO:
		ui.RenderIntroScreen();
		break;
	case GameState::PAUSED:
		AnimateGame(0);
		ui.RenderGameOverlay(lives, score, level);
		ui.RenderPausedOverlay();
		break;
	case GameState::IN_GAME_PREPARE:
		AnimateGame(timeDiff);
		ui.RenderGameOverlay(lives, score, level);
		ui.RenderPrepareOverlay(level);
		break;
	case GameState::IN_GAME:
		gameTime += timeDiff;
		AnimateGame(timeDiff);
		ui.RenderGameOverlay(lives, score, level);
		break;
	case GameState::LEVEL_FINISHED:
		// player is moved to new level immediately
		break;
	case GameState::DEAD:
		ui.RenderDeadScreen(score);
		break;
	case GameState::HIGHSCORED:
		ui.RenderHighscoreScreen(playerName);
		CaptureName();
		break;
	case GameState::LEADERBOARD:
		ui.RenderLeaderboardScreen(leaderboard);
		break;
	}
}