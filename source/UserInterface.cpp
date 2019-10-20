#include "UserInterface.h"

#include <string>
#include "../lib/leetlib.h"

#include "Leaderboard.h"

using namespace SpaceInvaders;
using namespace std;

void UserInterface::AnimateString(string text, const TextOptions &options) const
{
	int posIndex = 0;
	int letterSize = 20 * options.scale;
	int letterSpacing = 2 * letterSize;
	int alignmentOffset = 0;
	int textSize = text.length() * letterSpacing;

	switch (options.alignment) {
	case TextAlignment::CENTER:
		alignmentOffset -= textSize / 2 - letterSpacing / 2;
		break;
	case TextAlignment::RIGHT:
		alignmentOffset -= textSize;
		break;
	}

	float phase = sin(animationTime * 10);

	for (char &ch : text) {
		auto spriteIt = sprites.font.find(ch);
		if (spriteIt != sprites.font.end()) DrawSprite(spriteIt->second, posIndex * letterSpacing + options.x + alignmentOffset, options.y, letterSize, letterSize, phase * posIndex * 0.01);
		posIndex++;
	}
}

void UserInterface::RenderIntroScreen() const
{
	TextOptions titleTextOptions;
	titleTextOptions.x = screenWidth / 2;
	titleTextOptions.y = 50;
	titleTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("space invaders", titleTextOptions);

	TextOptions youTextOptions;
	youTextOptions.x = 250;
	youTextOptions.y = 150;
	youTextOptions.scale = 0.5;

	AnimateString("you are", youTextOptions);
	if (sprites.ship != nullptr)
	{
		DrawSprite(sprites.ship, screenWidth - 200, 150, 40, 40, sin(animationTime * 10) * 0.1, 0xffffffff);
	}

	TextOptions shootTextOptions;
	shootTextOptions.x = 250;
	shootTextOptions.y = 250;
	shootTextOptions.scale = 0.5;

	AnimateString("shoot at", shootTextOptions);
	if (sprites.enemy != nullptr)
	{
		DrawSprite(sprites.enemy, screenWidth - 200, 250 + sin(animationTime * 12) * 3, 30, 30, 0, 0xffffffff);
	}

	TextOptions collectTextOptions;
	collectTextOptions.x = 250;
	collectTextOptions.y = 350;
	collectTextOptions.scale = 0.5;

	AnimateString("collect", collectTextOptions);

	if (sprites.loot != nullptr)
	{
		DrawSprite(sprites.loot, screenWidth - 200, 350 + sin(animationTime * 10) * 3, 20, 20, 0, 0xffffffff);
	}

	TextOptions pauseTextOptions;
	pauseTextOptions.x = 250;
	pauseTextOptions.y = 450;
	pauseTextOptions.scale = 0.5;

	AnimateString("to pause press", pauseTextOptions);

	DrawSprite(sprites.font.at('p'), screenWidth - 200, 450 + sin(animationTime * 8) * 3, 30, 30, 0, 0xffffffff);

	TextOptions ctaTextOptions;
	ctaTextOptions.x = screenWidth / 2;
	ctaTextOptions.y = 550;
	ctaTextOptions.alignment = TextAlignment::CENTER;
	ctaTextOptions.scale = 0.75;

	AnimateString("press enter to start", ctaTextOptions);
}

void UserInterface::RenderDeadScreen(int score) const
{
	TextOptions deadTextOptions;
	deadTextOptions.x = screenWidth / 2;
	deadTextOptions.y = screenHeight / 2 - 50;
	deadTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("game over", deadTextOptions);

	TextOptions scoredTextOptions;
	scoredTextOptions.x = screenWidth / 2;
	scoredTextOptions.y = screenHeight / 2 + 50;
	scoredTextOptions.alignment = TextAlignment::CENTER;
	scoredTextOptions.scale = 0.75;

	AnimateString("score " + to_string(score), scoredTextOptions);

	TextOptions ctaTextOptions;
	ctaTextOptions.x = screenWidth / 2;
	ctaTextOptions.y = screenHeight / 2 + 130;
	ctaTextOptions.alignment = TextAlignment::CENTER;
	ctaTextOptions.scale = 0.5;

	AnimateString("press enter to start again", ctaTextOptions);
}

void UserInterface::RenderHighscoreScreen(const string &name) const
{
	TextOptions highscoreTextOptions;
	highscoreTextOptions.x = screenWidth / 2;
	highscoreTextOptions.y = screenHeight / 2 - 50;
	highscoreTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("high score", highscoreTextOptions);

	TextOptions promptTextOptions;
	promptTextOptions.x = screenWidth / 2;
	promptTextOptions.y = screenHeight / 2 + 50;
	promptTextOptions.alignment = TextAlignment::CENTER;
	promptTextOptions.scale = 0.75;

	AnimateString("insert your name", promptTextOptions);

	TextOptions nameTextOptions;
	nameTextOptions.x = screenWidth / 2;
	nameTextOptions.y = screenHeight / 2 + 130;
	nameTextOptions.alignment = TextAlignment::CENTER;
	nameTextOptions.scale = 0.5;

	AnimateString(name, nameTextOptions);

}

void UserInterface::RenderLeaderboardScreen(const Leaderboard &leaderboard) const
{
	int row = 150;

	TextOptions headlineTextOptions;
	headlineTextOptions.x = screenWidth / 2;
	headlineTextOptions.y = row - 70;
	headlineTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("leaderboard", headlineTextOptions);

	for (LeaderboardItem item : leaderboard.GetItems())
	{
		TextOptions nameTextOptions;
		nameTextOptions.x = 100;
		nameTextOptions.y = row;
		nameTextOptions.scale = 0.75;

		AnimateString(item.name, nameTextOptions);

		TextOptions scoreTextOptions;
		scoreTextOptions.x = screenWidth - 100;
		scoreTextOptions.y = row;
		scoreTextOptions.alignment = TextAlignment::RIGHT;
		scoreTextOptions.scale = 0.75;

		AnimateString(to_string(item.score), scoreTextOptions);

		row += 40;
	}

	TextOptions ctaTextOptions;
	ctaTextOptions.x = screenWidth / 2;
	ctaTextOptions.y = row;
	ctaTextOptions.alignment = TextAlignment::CENTER;
	ctaTextOptions.scale = 0.5;

	AnimateString("press enter to continue", ctaTextOptions);
}

void UserInterface::RenderPrepareOverlay(int level) const
{
	TextOptions levelTextOptions;
	levelTextOptions.x = screenWidth / 2;
	levelTextOptions.y = screenHeight / 2 - 25;
	levelTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("level " + to_string(level), levelTextOptions);

	TextOptions prepareTextOptions;
	prepareTextOptions.x = screenWidth / 2;
	prepareTextOptions.y = screenHeight / 2 + 25;
	prepareTextOptions.alignment = TextAlignment::CENTER;
	prepareTextOptions.scale = 1.2;

	AnimateString("get ready", prepareTextOptions);
}

void UserInterface::RenderPausedOverlay() const
{
	TextOptions pauseTextOptions;
	pauseTextOptions.x = screenWidth / 2;
	pauseTextOptions.y = screenHeight / 2;
	pauseTextOptions.alignment = TextAlignment::CENTER;
	pauseTextOptions.scale = 1.2;

	AnimateString("pause", pauseTextOptions);
}

void UserInterface::RenderGameOverlay(int lives, int score, int level) const
{
	TextOptions headlineTextOptions;
	headlineTextOptions.x = screenWidth / 2;
	headlineTextOptions.y = 30;
	headlineTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("space invaders", headlineTextOptions);

	TextOptions scoreTextOptions;
	scoreTextOptions.x = 20;
	scoreTextOptions.y = screenHeight - 40;
	scoreTextOptions.scale = 0.5;

	AnimateString(to_string(score), scoreTextOptions);

	TextOptions levelTextOptions;
	levelTextOptions.x = 20;
	levelTextOptions.y = screenHeight - 70;
	levelTextOptions.scale = 0.5;

	AnimateString("level " + to_string(level), levelTextOptions);

	TextOptions livesTextOptions;
	livesTextOptions.x = screenWidth - 40;
	livesTextOptions.y = screenHeight - 40;
	livesTextOptions.alignment = TextAlignment::RIGHT;
	livesTextOptions.scale = 0.75;

	AnimateString(to_string(lives) + "x", livesTextOptions);

	if (sprites.ship != nullptr)
	{
		DrawSprite(sprites.ship, screenWidth - 30, screenHeight - 40, 18, 18, sin(animationTime * 10)*0.1, 0xffffffff);
	}
}