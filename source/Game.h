#pragma once
#include "Invader.h"
#include "Bullet.h"
#include "Ship.h"

#include <d3d9.h>

#include <map>
#include <string>

using namespace std;

namespace SpaceInvaders {
	enum GameState {
		IN_GAME = 1,
		HIGHSCORED = 2,
		LEVEL_FINISHED = 3,
		DEAD = 4,
		PAUSED = 5
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


	class Game
	{
		protected:
			int maxX = 0;
			int maxY = 0;

			Invader enemies[50];
			Bullet bullets[10];
			Ship ship{0,0};

			double elapsedTime = 0;
			int score = 0;
			int lives = 3;
			int level = 1;
			GameState state;

			SpriteSet sprites;

			void ResolveEnemyHits();
			void ResolvePlayerHit();

			void AnimateString(string text, const TextOptions &options) const;

			void AnimateEnemies();
			void AnimateShip(double timeDiff);
			void AnimateFiring(double timeDiff);

			void AnimateGameScreen(double timeDiff);
			void AnimateDeadScreen();

			void ResolveInteractions();

			void SetupLevel(int level);
			void ResetGame();

			void ResolveGameState();

		public:
			Game(int screenWidth, int screenHeight) : maxX(screenWidth), maxY(screenHeight) {};

			void Tick(double elapsedMicroseconds);
			void Initialize(const SpriteSet spriteSet);
	};
}
