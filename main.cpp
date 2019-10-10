#include "lib/leetlib.h"
#include <math.h>
#include <stdio.h>
#include <string>

#include "source/Game.h"
#include "source/Ship.h"
#include "source/Bullet.h"
#include "source/Invader.h"

using namespace std;
using namespace SpaceInvaders;

void Game()
{
	SpaceInvaders::Game game;

	SpriteSet sprites;
	sprites.enemy = LoadSprite("gfx/Little Invader.png");
	sprites.ship = LoadSprite("gfx/Big Invader.png");
	sprites.bullet = LoadSprite("gfx/bullet.png");

	for (char i = 'a'; i <= 'z'; ++i) {
		string fileName = "gfx/";
		fileName = fileName + i;
		fileName = fileName + "let.png";
		sprites.font.emplace(i, LoadSprite(fileName.c_str()));
	}
	
	for (char i = '0'; i <= '9'; ++i) {
		string fileName = "gfx/num";
		fileName = fileName + i;
		fileName = fileName + ".png";
		sprites.font.emplace(i, LoadSprite(fileName.c_str()));
	}

	game.Initialize(sprites);
	while (!WantQuit() && !IsKeyDown(VK_ESCAPE))
	{
		game.Animate();
		game.ResolveInteractions();

		Flip();
	}
}