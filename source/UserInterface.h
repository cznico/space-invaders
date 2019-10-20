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
		IDirect3DTexture9 * enemy = nullptr;
		IDirect3DTexture9 * ship = nullptr;
		IDirect3DTexture9 * bullet = nullptr;
		IDirect3DTexture9 * explosion = nullptr;
		IDirect3DTexture9 * loot = nullptr;
		IDirect3DTexture9 * background = nullptr;
		IDirect3DTexture9 * backgroundLand = nullptr;
		map<char, IDirect3DTexture9 *> font;
	};

	struct AudioSet {
		FSOUND_SAMPLE * fire = nullptr;
		FSOUND_SAMPLE * hit = nullptr;
		FSOUND_SAMPLE * shipHit = nullptr;
		FSOUND_SAMPLE * ready = nullptr;
		FSOUND_SAMPLE * dead = nullptr;
		FSOUND_SAMPLE * pickup = nullptr;
		string music;
	};

	class UserInterface
	{
	protected:
		int screenWidth;
		int screenHeight;
		double animationTime;

		SpriteSet sprites;

	public:
		UserInterface(int screenWidth, int screenHeight) : screenHeight(screenHeight), screenWidth(screenWidth) {};

		void AnimateString(string text, const TextOptions &options) const;
		
		void RenderBackground() const;
		void RenderBackgroundLand() const;
		void RenderIntroScreen() const;
		void RenderDeadScreen(unsigned int score) const;
		void RenderHighscoreScreen(const string &name) const;
		void RenderLeaderboardScreen(const Leaderboard & leaderboard) const;
		void RenderPrepareOverlay(unsigned int level) const;
		void RenderPausedOverlay() const;
		void RenderGameOverlay(unsigned int lives, unsigned int score, unsigned int level) const;
		
		void SetSpriteSet(const SpriteSet &spriteSet) { sprites = spriteSet; };
		void SetAnimationTime(double time) { animationTime = time; };
	};
}



