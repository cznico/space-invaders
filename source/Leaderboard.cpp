#include "Leaderboard.h"

using namespace SpaceInvaders;

bool compareLeaderboardItems(LeaderboardItem first, LeaderboardItem second)
{
	return first.score > second.score;
}

void Leaderboard::Load(string name)
{
	fileName = name;

	// TODO load from file
}

void Leaderboard::Update(unsigned int score, string name)
{
	LeaderboardItem newItem;
	newItem.name = name;
	newItem.score = score;

	boardItems.push_back(newItem);
	sort(boardItems.begin(), boardItems.end(), compareLeaderboardItems);

	boardItems.resize(count);
}

bool Leaderboard::HitLeaderboard(unsigned int score)
{
	if (boardItems.size() >= count)
	{
		return score > boardItems.back().score;
	}
	return true;
}

void Leaderboard::Save()
{

}