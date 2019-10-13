#pragma once
#include "Invader.h"
#include "Bullet.h"
#include "Ship.h"

#include "../lib/leetlib.h"

#include <map>
#include <string>

using namespace std;

namespace SpaceInvaders {
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

	enum TextAlignment {
		LEFT = 0,
		RIGHT = 1,
		CENTER = 2
	};

	struct TextOptions {
		int x;
		int y;
		TextAlignment alignment = TextAlignment::LEFT;
		float scale = 1.f;
	};

	struct SpriteSet {
		IDirect3DTexture9 * enemy;
		IDirect3DTexture9 * ship;
		IDirect3DTexture9 * bullet;
		map<char, IDirect3DTexture9 *> font;
	};

	struct AudioSet {
		FSOUND_SAMPLE * fire;
		FSOUND_SAMPLE * hit;
		FSOUND_SAMPLE * shipHit;
		FSOUND_SAMPLE * ready;
		FSOUND_SAMPLE * dead;
		string music;
	};


	class Game
	{
		protected:
			int maxX = 0;
			int maxY = 0;

			Invader enemies[50];
			Bullet bullets[10];
			Ship ship{0,0};

			double startTime = 0;
			double elapsedTime = 0;
			double gameTime = 0;

			int score = 0;
			int lives = 3;
			int level = 1;
			GameState state;

			SpriteSet sprites;
			AudioSet audio;
			Leaderboard * leaderboard;

			string playerName;

			void CaptureName();

			void ResolveEnemyHits();
			void ResolvePlayerHit();

			void AnimateString(string text, const TextOptions &options) const;

			void AnimateEnemies();
			void AnimateShip(double timeDiff);
			void AnimateFiring(double timeDiff);

			void AnimatePausedOverlay();
			void AnimatePrepareOverlay();
			void AnimateGameScreen(double timeDiff);
			void AnimateDeadScreen();
			void AnimateHighscoreScreen();
			void AnimateLeaderboardScreen();

			void ResolveInteractions();

			void SetupLevel(int newLevel);
			void ResetGame();

			void SetGameState(GameState newState);
			void ResolveGameState();

			void ResetTime();

		public:
			Game(int screenWidth, int screenHeight, double startTime) : maxX(screenWidth), maxY(screenHeight), startTime(startTime) {};

			void Tick(double elapsedSeconds);
			void Initialize(const SpriteSet spriteSet, const AudioSet audioSet, Leaderboard * leaderboard);
	};
}
