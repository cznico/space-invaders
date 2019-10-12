#include "lib/leetlib.h"
#include <math.h>
#include <stdio.h>
#include <string>
#include <chrono>

#include "source/Game.h"
#include "source/Ship.h"
#include "source/Bullet.h"
#include "source/Invader.h"

using namespace std;
using namespace SpaceInvaders;

double timeInSeconds() {
	return (double(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) / double(1000000));
}

void Game()
{
	auto startTime = timeInSeconds();
	SpaceInvaders::Game game = SpaceInvaders::Game(800, 600, startTime);

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

	AudioSet audios;
	audios.hit = LoadSnd("sfx/hit.ogg");
	audios.shipHit = LoadSnd("sfx/ship-hit.ogg");
	audios.ready = LoadSnd("sfx/get-ready.ogg");
	audios.dead = LoadSnd("sfx/game-over.ogg");
	audios.fire = LoadSnd("sfx/fire.ogg");

	game.Initialize(sprites, audios);

	while (!WantQuit() && !IsKeyDown(VK_ESCAPE))
	{
		game.Tick(timeInSeconds());

		Flip();
	}
}