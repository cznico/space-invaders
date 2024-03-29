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

		static LeaderboardItem deserialize(string input)
		{
			LeaderboardItem item;

			string::size_type sz;
			item.score = stoi(input, &sz);
			item.name = input.substr(++sz);

			return item;
		};

		string serialize()
		{
			return to_string(score) + ";" + name;
		};
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

		void LoadFromFile(const string &name);

		void Update(unsigned int score, const string &name);

		bool HitLeaderboard(unsigned int score) const;

		void SaveToFile();

		const vector<LeaderboardItem> GetItems() const { return boardItems; }
	};


}
