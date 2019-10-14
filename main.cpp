#include "lib/leetlib.h"
#include <math.h>
#include <stdio.h>
#include <string>

#include "source/Game.h"
#include "source/Leaderboard.h"
#include "lib/leetlib.h"

using namespace std;
using namespace SpaceInvaders;

double timeInSeconds() {
	return GetTimeInMS() / (double)1000;
}

SpriteSet LoadSpriteSet()
{
	SpriteSet sprites;
	sprites.enemy = LoadSprite("gfx/Little Invader.png");
	sprites.ship = LoadSprite("gfx/donkey-kong.png");
	sprites.bullet = LoadSprite("gfx/barrel.png");
	sprites.explosion = LoadSprite("gfx/explosion.png");

	Leaderboard highscores;

	highscores.Load("leaderboard.txt");

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

	return sprites;
}

AudioSet LoadAudioSet()
{
	AudioSet audioSet;
	audioSet.hit = LoadSnd("sfx/hit.ogg");
	audioSet.shipHit = LoadSnd("sfx/ship-hit.ogg");
	audioSet.ready = LoadSnd("sfx/get-ready.ogg");
	audioSet.dead = LoadSnd("sfx/game-over.ogg");
	audioSet.fire = LoadSnd("sfx/fire.ogg");
	audioSet.music = "sfx/Automation.mp3";

	return audioSet;
}

void Game()
{
	auto startTime = timeInSeconds();
	SpaceInvaders::Game game = SpaceInvaders::Game(800, 600, startTime);

	SpriteSet spriteSet = LoadSpriteSet();
	AudioSet audioSet = LoadAudioSet();
	
	Leaderboard highscores;

	highscores.Load("leaderboard.txt");

	game.Initialize(&spriteSet, &audioSet, &highscores);

	while (!WantQuit() && !IsKeyDown(VK_ESCAPE))
	{
		game.Tick(timeInSeconds());

		Flip();
	}
}