#pragma once
#include "Invader.h"
#include "Bullet.h"
#include "Ship.h"

#include <d3d9.h>

#include <map>
#include <string>

using namespace std;

namespace SpaceInvaders {
	struct SpriteSet {
		IDirect3DTexture9 * enemy;
		IDirect3DTexture9 * ship;
		IDirect3DTexture9 * bullet;
		map<char, IDirect3DTexture9 *> font;
	};


	class Game
	{
		protected:
			Invader enemies[50];
			Bullet bullets[10];
			Ship ship;

			double elapsedTime = 0;
			int score = 0;
			int lives = 3;

			SpriteSet sprites;

			void ResolveEnemyHits();
			void ResolvePlayerHit();

			void AnimateString(string text, int x, int y);

			void AnimateEnemies();
			void AnimateShip(double timeDiff);
			void AnimateFiring(double timeDiff);

			void Animate(double timeDiff);
			void ResolveInteractions();

		public:
			void Tick(double elapsedMicroseconds);

			void Initialize(const SpriteSet spriteSet);
	};
}
