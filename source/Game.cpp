#include "Game.h"

#include <math.h>
#include "../lib/leetlib.h"
#include "Leaderboard.h"

#include <vector>
#include <random>
#include <algorithm>
#include <iterator>


using namespace std;
using namespace SpaceInvaders;

vector<unsigned int> GetRandomVector(unsigned int maxNumber, unsigned int count)
{
	vector<unsigned int> result;

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

void Game::Initialize(const SpriteSet &spriteSet, const AudioSet &audioSet, const Leaderboard &highscores)
{
	int invaderSpreadX = 70;
	int invaderSpreadY = 60;
	int invadersPerLine = 10;
	int leftOffset = (maxX - (invaderSpreadX * (invadersPerLine - 1))) / 2;

	sprites = spriteSet;
	audio = audioSet;

	for (unsigned int n = 0; n < ENEMIES_COUNT; ++n)
	{
		Invader &enemy = enemies[n];
		unsigned int size = 10 + ((n) % 17);

		enemy.startPosition.x = (n % invadersPerLine) * invaderSpreadX + leftOffset;
		enemy.x = enemies[n].startPosition.x;
		enemy.startPosition.y = (n / invadersPerLine) * invaderSpreadY + 70;
		enemy.y = enemies[n].startPosition.y;

		enemy.SetCollisionRadius(size);
		enemy.SetupDrawProps(sprites.enemy, size);
	}

	ship = Ship(60, maxX - 60);
	ship.x = maxX / 2;
	ship.y = maxY - 60;
	ship.SetCollisionRadius(40);
	ship.SetupDrawProps(sprites.ship, 40);

	ui = UserInterface(maxX, maxY);
	ui.SetSpriteSet(spriteSet);

	elapsedTime = 0;
	SetGameState(GameState::INTRO);

	if (!muted)
	{
		PlayMusic(audio.music.c_str());
	}	

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
		if (invader.IsColliding(ship))
		{
			invader.enabled = false;
			lives--;
			if (audio.shipHit != nullptr && !muted)
			{
				PlaySnd(audio.shipHit, 1.f);
			}
			
		}
	}

	for (auto &lootIt : loot)
	{
		auto lootItem = &lootIt.second;
		if (lootItem->IsColliding(ship))
		{
			lootItem->enabled = false;
			score += lootItem->value;

			if (audio.pickup != nullptr && !muted)
			{
				PlaySnd(audio.pickup, 0.5f);
			}
		}
	}
}

void Game::ResolveEnemyHits()
{
	int n = 0;
	for (Invader &invader : enemies)
	{
		for (auto &bulletIt : bullets) {
			if (invader.IsColliding(bulletIt.second)) {
				bulletIt.second.enabled = false;
				invader.enabled = false;

				Explosion explosion = Explosion::CreateEffect(invader, gameTime);
				explosion.SetupDrawProps(sprites.explosion, invader.GetSize() + 10);

				explosions.emplace(n, explosion);

				score += 10 + (level - 1) * 2; // Every level increases score points by 20%
				
				if (audio.hit != nullptr && !muted)
				{
					PlaySnd(audio.hit, 1.f);
				}

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

void Game::ResolveUserInput(float timeDiff)
{
	// Bullet spawning
	if (shotDelay > 0)
	{
		shotDelay -= timeDiff;
	}
	if (!IsKeyDown(VK_SPACE)) shotDelay = 0;
	if (IsKeyDown(VK_SPACE) && shotDelay <= 0 && state == GameState::IN_GAME)
	{
		Bullet bullet = Bullet::CreateBullet(ship);
		bullet.SetCollisionRadius(15);
		bullet.SetupDrawProps(sprites.bullet, 15);

		bullets.emplace(Bullet::ShotsCount++, bullet);
		shotDelay = .25f; // 250ms rate of fire

		if (audio.fire != nullptr && !muted)
		{
			PlaySnd(audio.fire, 0.5f);
		}
	}

	// Ship movement
	int speed = timeDiff * 700;
	ship.MoveHorizontally(IsKeyDown(VK_LEFT) ? -speed : IsKeyDown(VK_RIGHT) ? speed : 0);
}

void Game::SetupLevel(int newLevel)
{
	lives = 3;
	level = newLevel;

	for (auto &e : enemies)
	{
		e.enabled = true;
	}

	ResetLoot();
	explosions.clear();
	bullets.clear();
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

		if (audio.ready != nullptr && !muted)
		{
			PlaySnd(audio.ready, 0.7f);
		}		
		break;
	case GameState::DEAD:
		if (audio.dead != nullptr && !muted)
		{
			PlaySnd(audio.dead, 0.7f);
		}
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
			if (leaderboard.HitLeaderboard(score))
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
		SetGameState(GameState::LEADERBOARD);
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
		leaderboard.Update(score, playerName);
		leaderboard.SaveToFile();

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

void Game::ResolveAudioState()
{
	if (IsKeyHitSinceLastFlip('M'))
	{
		muted = !muted;

		if (muted)
		{
			StopMusic();
		}
		else
		{
			PlayMusic(audio.music.c_str());
		}
	}
}

void Game::ResetLoot()
{
	loot.clear();

	unsigned int lootValue = 20 + (5 * level);
	auto indices = GetRandomVector(ENEMIES_COUNT - 1, 5);

	for (unsigned int index : indices)
	{
		Loot lootItem = Loot(lootValue, index);
		lootItem.SetupDrawProps(sprites.loot, 15);
		lootItem.SetCollisionRadius(15);

		loot.emplace(index, lootItem);
	}
	
}

void Game::AnimateGame(double timeDiff)
{
	ResolveUserInput(timeDiff);

	// Order matters - z-ordering in rendering
	AnimateEnemies();	
	AnimateFiring(timeDiff);
	AnimateShip(timeDiff);
	AnimateLoot(timeDiff);
	AnimateExplosions();

	ResolveInteractions();
	CleanDynamicStructures();
}

void Game::AnimateEnemies()
{
	int phase = gameTime * (100 + (level - 1) * 10); // Every level increases enemy speed by 10%

	for (unsigned int n = 0; n < ENEMIES_COUNT; ++n)
	{
		Invader &enemy = enemies[n];

		if (enemy.enabled)
		{
			int xo = 0;
			int yo = min((int)(phase / 1000) * 30, 200); // Descending behaviour - every 10s at level 1

			// Start of Rotating behaviour
			int n1 = phase + n * n + n * n * n;
			
			if (((n1 >> 6) & 0x7) == 0x7) 
			{
				auto rotationRadius = 20 + ((n * n) % 9);
				auto rotationPhase = (n1 & 0x7f) / 64.0f * 2.f * PI;

				xo += (1 - cos(rotationPhase)) * rotationRadius;
				yo += sin(rotationPhase) * rotationRadius;
			}
			// End of Rotating behaviour


			// Start of Swooping behaviour
			int n2 = phase + n + n * n + n * n * n * 3;

			if (((n2 >> 8) & 0xf) == 0xf)
			{
				auto swoopingDistance = 150 + ((n * n) % 9);
				auto swoopingPhase = (n2 & 0xff) / 256.0f * 2.f * PI;

				yo += (1 - cos(swoopingPhase)) * swoopingDistance;
			}
			// End of Swooping behaviour

			enemy.x = enemy.startPosition.x + xo;
			enemy.y = enemy.startPosition.y + yo;

			enemy.Draw(elapsedTime);
		}
	}
}

void Game::AnimateExplosions()
{
	for (auto &explosionIt : explosions)
	{
		explosionIt.second.Draw(gameTime);
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

		lootItem->Draw(elapsedTime);
	}
}

void Game::AnimateShip(double timeDiff)
{
	ship.Draw(elapsedTime);
}

void Game::AnimateFiring(double timeDiff)
{
	int speed = timeDiff * 400;

	for (auto &bulletIt : bullets)
	{
		Bullet &bullet = bulletIt.second;

		if (!bullet.enabled) continue;

		bullet.y -= speed;
		bullet.enabled = bullet.enabled && bullet.y > 0;
		bullet.spin += timeDiff * 2;

		bullet.Draw(gameTime);
	}
}

void Game::CleanDynamicStructures()
{
	for (auto it = bullets.cbegin(), next_it = it; it != bullets.cend(); it = next_it)
	{
		++next_it;
		if (!it->second.enabled)
		{
			bullets.erase(it);
		}
	}

	for (auto it = explosions.cbegin(), next_it = it; it != explosions.cend(); it = next_it)
	{
		++next_it;
		if (!it->second.enabled)
		{
			explosions.erase(it);
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
	ResolveAudioState();

	ui.RenderBackground();

	switch (state)
	{
	case GameState::INTRO:
		ui.RenderIntroScreen();
		break;
	case GameState::PAUSED:
		ui.RenderBackgroundLand();
		AnimateGame(0);
		ui.RenderGameOverlay(lives, score, level);
		ui.RenderPausedOverlay();
		break;
	case GameState::IN_GAME_PREPARE:
		ui.RenderBackgroundLand();
		AnimateGame(timeDiff);
		ui.RenderGameOverlay(lives, score, level);
		ui.RenderPrepareOverlay(level);
		break;
	case GameState::IN_GAME:
		ui.RenderBackgroundLand();
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