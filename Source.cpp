#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2() {}
	Vec2(float xin, float yin)
		: x(xin)
		, y(yin)
	{

	}

	Vec2 operator - (const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	void add(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

};

struct Intersection
{
	// if 2 lines intercect each other or not
	bool result;

	// return the coordinate of the point, where 2 lines intersect
	Vec2 pos;
};

// one line starts from position ((a)) to position ((b))
// the othe line starts from position ((c)) to position ((d))
Intersection LineIntersect(Vec2 a, Vec2 b, Vec2 c, Vec2 d)
{
	// calcuate the vector of each line
	Vec2 r = b - a;
	Vec2 s = d - c;

	// cross product of two lines
	float rxs = r.x * s.y - s.x * r.y;

	// position c minus position a
	Vec2 cma = c - a;

	// calculate the line scaler, where 2 lines intersect
	float t = (cma.x * s.y - s.x * cma.y) / rxs;
	float u = (cma.x * r.y - r.x * cma.y) / rxs;

	// check if they are between zero and one
	if (t>=0 && t<=1 && u>=0 && u<=1)
	{
		return { true, Vec2(a.x + t * r.x, a.y + t * r.y) };
	}
	else
	{
		return { false, Vec2(0, 0) };
	}
}



int main(int argc, char* argv[])
{
	Vec2 a(1, 4), b(4, 1), c(1, 1), d(4, 4);
	bool isIt = LineIntersect(a, b, c, d).result;
	Vec2 point = LineIntersect(a, b, c, d).pos;
	std::cout << isIt << " the point is in " << "(" << point.x << "," << point.y << ")" << std::endl;
}