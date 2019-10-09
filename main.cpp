#include "lib/leetlib.h"
#include <math.h>
#include <stdio.h>

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
	sprites.text['s'] = LoadSprite("gfx/slet.png");
	sprites.text['p'] = LoadSprite("gfx/plet.png");
	sprites.text['a'] = LoadSprite("gfx/alet.png");
	sprites.text['c'] = LoadSprite("gfx/clet.png");
	sprites.text['e'] = LoadSprite("gfx/elet.png");
	sprites.text['i'] = LoadSprite("gfx/ilet.png");
	sprites.text['n'] = LoadSprite("gfx/nlet.png");
	sprites.text['v'] = LoadSprite("gfx/vlet.png");
	sprites.text['d'] = LoadSprite("gfx/dlet.png");
	sprites.text['r'] = LoadSprite("gfx/rlet.png");

	game.Initialize(sprites);
end:
	if(WantQuit()) return;
	if(IsKeyDown(VK_ESCAPE)) return;
	
	game.Animate();
	game.ResolveInteractions();

	Flip();
    
	goto end;
}

void OldGame()
{
	void *sprite = LoadSprite("sprite.png");
	float size=10;
	float angle=0;
	while (!WantQuit() && !IsKeyDown(VK_ESCAPE))
	{
		angle+=0.01f;
		float mx,my;
		GetMousePos(mx,my);
		DWORD col=0xffffffff; // white
		if (IsKeyDown(VK_LBUTTON)) col=0xffff0000; // solid red
		if (IsKeyDown(VK_RBUTTON)) col=0x8000ff00; // 50% transparent green
		if (IsKeyDown(VK_UP)) size++;
		if (IsKeyDown(VK_DOWN)) size--;
		
		DrawSprite(sprite,400,300,100,100, angle);
		DrawSprite(sprite, mx,my, size,size, 0, col);	
		Flip();
	}
}
