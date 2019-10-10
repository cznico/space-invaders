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

			int time = 0;

			SpriteSet sprites;

			bool IsShipHit();

			void AnimateEnemies();
			void AnimateShip();
			void AnimateFiring();
			void AnimateHeadline();
			void AnimateScore();

		public:
			Game();

			void Animate();
			void ResolveInteractions();
			void Initialize(const SpriteSet spriteSet);
	};
}
