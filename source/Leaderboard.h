#pragma once
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

namespace SpaceInvaders
{

	struct LeaderboardItem {
		string name;
		unsigned int score = 0;
	};

	class Leaderboard
	{
	private:
		string fileName;
		unsigned int count = 10;
		vector<LeaderboardItem> boardItems;

	public:
		Leaderboard() {};
		Leaderboard(unsigned int count) : count(count) {};

		void Load(string name);

		void Update(unsigned int score, string name);

		bool HitLeaderboard(unsigned int score);

		void Save();

		const vector<LeaderboardItem>* getItems() { return &boardItems; }
	};


}
