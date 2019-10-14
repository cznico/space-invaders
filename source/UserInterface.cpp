#include "UserInterface.h"

#include <string>
#include "../lib/leetlib.h"

#include "Leaderboard.h"

using namespace SpaceInvaders;
using namespace std;

void UserInterface::AnimateString(string text, const TextOptions &options) const
{
	if (sprites == nullptr) return;

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
		auto spriteIt = sprites->font.find(ch);
		if (spriteIt != sprites->font.end()) DrawSprite(spriteIt->second, posIndex * letterSpacing + options.x + alignmentOffset, options.y, letterSize, letterSize, phase * posIndex * 0.01);
		posIndex++;
	}
}

void UserInterface::RenderDeadScreen(int score)
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

void UserInterface::RenderHighscoreScreen(string name)
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

void UserInterface::RenderLeaderboardScreen(Leaderboard * leaderboard)
{
	int row = 150;

	TextOptions headlineTextOptions;
	headlineTextOptions.x = screenWidth / 2;
	headlineTextOptions.y = row - 70;
	headlineTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("leaderboard", headlineTextOptions);

	for (auto item : (*leaderboard->getItems()))
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

void UserInterface::RenderPrepareOverlay(int level)
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

void UserInterface::RenderPausedOverlay()
{
	TextOptions pauseTextOptions;
	pauseTextOptions.x = screenWidth / 2;
	pauseTextOptions.y = screenHeight / 2;
	pauseTextOptions.alignment = TextAlignment::CENTER;
	pauseTextOptions.scale = 1.2;

	AnimateString("pause", pauseTextOptions);
}

void UserInterface::RenderGameOverlay(int lives, int score, int level)
{
	TextOptions headlineTextOptions;
	headlineTextOptions.x = screenWidth / 2;
	headlineTextOptions.y = 30;
	headlineTextOptions.alignment = TextAlignment::CENTER;

	AnimateString("space invaders", headlineTextOptions);

	TextOptions scoreTextOptions;
	scoreTextOptions.x = 20;
	scoreTextOptions.y = screenHeight - 20;
	scoreTextOptions.scale = 0.75;

	AnimateString(to_string(score), scoreTextOptions);

	TextOptions levelTextOptions;
	levelTextOptions.x = 20;
	levelTextOptions.y = screenHeight - 50;
	levelTextOptions.scale = 0.5;

	AnimateString("level " + to_string(level), levelTextOptions);

	TextOptions livesTextOptions;
	livesTextOptions.x = screenWidth - 40;
	livesTextOptions.y = screenHeight - 20;
	livesTextOptions.alignment = TextAlignment::RIGHT;

	AnimateString(to_string(lives) + "x", livesTextOptions);

	DrawSprite(sprites->ship, screenWidth - 30, screenHeight - 20, 20, 20, sin(animationTime * 10)*0.1, 0xffffffff);
}