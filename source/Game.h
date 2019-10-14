#pragma once
#include "Invader.h"
#include "Bullet.h"
#include "Ship.h"
#include "Loot.h"
#include "UserInterface.h"

#include "../lib/leetlib.h"

#include <map>
#include <string>

using namespace std;

namespace SpaceInvaders {
	const int ENEMIES_COUNT = 50;

	class Leaderboard;

	enum GameState {
		IN_GAME_PREPARE = 0,
		IN_GAME = 1,
		HIGHSCORED = 2,
		LEVEL_FINISHED = 3,
		DEAD = 4,
		PAUSED = 5,
		LEADERBOARD = 6
	};

	class Game
	{
		protected:
			int maxX = 0;
			int maxY = 0;

			UserInterface ui{0, 0};
			
			Invader enemies[ENEMIES_COUNT];
			Bullet bullets[10];
			Ship ship{0,0};
			
			map<int, Loot> loot;

			double startTime = 0;
			double elapsedTime = 0;
			double gameTime = 0;

			int score = 0;
			int lives = 3;
			int level = 1;
			GameState state;

			SpriteSet * sprites;
			AudioSet * audio;
			Leaderboard * leaderboard;

			string playerName;

			void CaptureName();

			void ResolveEnemyHits();
			void ResolvePlayerHit();

			void AnimateEnemies();
			void AnimateExplosions();
			void AnimateLoot(double timeDiff);
			void AnimateShip(double timeDiff);
			void AnimateFiring(double timeDiff);

			void AnimateGame(double timeDiff);

			void ResetLoot();

			void ResolveInteractions();

			void SetupLevel(int newLevel);
			void ResetGame();

			void SetGameState(GameState newState);
			void ResolveGameState();

			double UpdateTime(double time);
			void ResetTime();

		public:
			Game(int screenWidth, int screenHeight, double startTime) : maxX(screenWidth), maxY(screenHeight), startTime(startTime) {};

			void Tick(double elapsedSeconds);
			void Initialize(SpriteSet * spriteSet, AudioSet * audioSet, Leaderboard * leaderboard);
	};
}
