#pragma once
#include "Invader.h"
#include "Bullet.h"
#include "Ship.h"

#include <map>
#include <string>

using namespace std;

namespace SpaceInvaders {
	struct SpriteSet {
		void * enemy;
		void * ship;
		void * bullet;
		map<char, void *> text;
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

		public:
			Game();

			void Animate();
			void ResolveInteractions();
			void Initialize(const SpriteSet spriteSet);
	};
}
