#include "Leaderboard.h"

#include <fstream>

using namespace SpaceInvaders;
using namespace std;

bool compareLeaderboardItems(LeaderboardItem first, LeaderboardItem second)
{
	return first.score > second.score;
}

void Leaderboard::Load(string name)
{
	fileName = name;

	string row;
	ifstream inFile;

	inFile.open(fileName);
	if (!inFile.is_open()) return;

	while (getline(inFile, row))
	{
		auto item = LeaderboardItem::deserialize(row);
		boardItems.push_back(item);
	}

	sort(boardItems.begin(), boardItems.end(), compareLeaderboardItems);
	inFile.close();
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
	ofstream outFile;

	outFile.open(fileName);

	if (!outFile.is_open()) return;

	for (auto item : boardItems)
	{
		outFile << item.serialize() << endl;
	}
	 
}