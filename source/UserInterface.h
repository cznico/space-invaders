#pragma once
#include <map>
#include <string>
#include "../lib/leetlib.h"

using namespace std;

namespace SpaceInvaders
{
	class Leaderboard;

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
		IDirect3DTexture9 * explosion;
		IDirect3DTexture9 * loot;
		map<char, IDirect3DTexture9 *> font;
	};

	struct AudioSet {
		FSOUND_SAMPLE * fire;
		FSOUND_SAMPLE * hit;
		FSOUND_SAMPLE * shipHit;
		FSOUND_SAMPLE * ready;
		FSOUND_SAMPLE * dead;
		FSOUND_SAMPLE * pickup;
		string music;
	};

	class UserInterface
	{
	protected:
		int screenWidth;
		int screenHeight;
		double animationTime;

		SpriteSet * sprites;

	public:
		UserInterface(int screenWidth, int screenHeight) : screenHeight(screenHeight), screenWidth(screenWidth) {};

		void AnimateString(string text, const TextOptions &options) const;

		void RenderIntroScreen();
		void RenderDeadScreen(int score);
		void RenderHighscoreScreen(string name);
		void RenderLeaderboardScreen(Leaderboard * leaderboard);
		void RenderPrepareOverlay(int level);
		void RenderPausedOverlay();
		void RenderGameOverlay(int lives, int score, int level);
		
		void SetSpriteSet(SpriteSet * spriteSet) { sprites = spriteSet; };
		void SetAnimationTime(double time) { animationTime = time; };
	};
}



