#pragma once
namespace SpaceInvaders {

	class Point
	{
		public:
			float x;
			float y;

		public:
			float Distance(const Point &another) const;
	};

}



