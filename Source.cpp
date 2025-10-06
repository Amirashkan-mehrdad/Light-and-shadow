#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

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

	float length() const
	{
		return sqrtf(x * x + y * y);
	}

};

class Line
{
public:
	// define both ends of the line
	Vec2 startPoint = { 0,0 };
	Vec2 endPos = { 0,0 };

	Line() {}
	Line(Vec2 start, Vec2 end)
		: startPoint(start)
		, endPos(end)
	{

	}
};

// define a ray of light
class RayLight
{
public:
	// define source of the light and the angle of its ray
	// the angle is in radian
	Vec2  source = { 0,0 };
	float angle  = 0.0;
	// how far a ray of light can reach
	int length = 1500;
	float visibleLength = length;
	Line r;


	RayLight() {}
	RayLight(Vec2 start, float a, int l)
		: source(start)
		, angle(a)
		, length(l)
	{

	}

	// return invisible ending point
	const Vec2& getEndPos() const
	{
		Vec2  end = { source.x + length * cos(angle), source.y + length * sin(angle) };
		return end;
	}

	// return visible ending point
	const Vec2& getVisibleEndPos() const
	{
		Vec2  end = { source.x + visibleLength * cos(angle), source.y + visibleLength * sin(angle) };
		return end;
	}
};

// creat a light source with its rays
class LightSource
{
	float Pi = 3.14159265359;
	int numberOfRays = 20;
	Vec2 sourcePos = { 400, 350 };

public:
	LightSource() {}
	LightSource(Vec2 start, int n)
		: sourcePos(start)
		, numberOfRays(n)
	{

	}
	float Theta = 2 * Pi / numberOfRays;

	std::shared_ptr<RayLight> r;
	std::vector<std::shared_ptr<RayLight>> RayLines;

	void createLight()
	{
		for (int i = 1; i <= numberOfRays; i++)
		{
			r = std::make_shared<RayLight>(sourcePos, Theta*i, 1500);
			RayLines.push_back(r);
		}
	}

	void update(const Vec2& sourcePos)
	{
		for (auto a : RayLines)
		{
			a->source.x = sourcePos.x;
			a->source.y = sourcePos.y;

		}
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
	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return { true, Vec2(a.x + t * r.x, a.y + t * r.y) };
	}
	else
	{
		return { false, Vec2(0, 0) };
	}
};



int main(int argc, char* argv[])
{
	// extract data from file to create blocks
	// START of data extraction
	int NumberOfVertics, startX, endX, startY, endY;
	std::shared_ptr<Line> l;
	std::vector<std::shared_ptr<Line>> blocks;

	LightSource light;
	light.createLight();


	const std::string& FileName = "Data/Blocks.txt";
	std::ifstream fin(FileName);

	while (fin >> NumberOfVertics)
	{
		fin >> startX >> startY;
		for (int i = 1; i <= NumberOfVertics; i++)
		{
			fin >> endX >> endY;
			l = std::make_shared<Line>(Vec2(startX, startY), Vec2(endX, endY));
			blocks.push_back(l);
			startX = endX;
			startY = endY;
		}
	}

	// draw a window
	const int wWidth = 800;
	const int wHight = 700;
	sf::RenderWindow window(sf::VideoMode(wWidth, wHight), "SFML works!");
	window.setFramerateLimit(60);




	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseMoved)
			{
				float mmx = event.mouseMove.x;
				float mmy = event.mouseMove.y;
				light.update({ mmx, mmy });
			}
		}


		// Implementation of intersection between light and blocks
		for (auto& l : light.RayLines)
		{
			l->visibleLength = l->length;
			Vec2 a = { l->source.x, l->source.y };
			Vec2 b = { l->getEndPos().x, l->getEndPos().y};

			for (auto& s : blocks)
			{
				Vec2 c = { s->startPoint.x, s->startPoint.y };
				Vec2 d = { s->endPos.x, s->endPos.y };

				bool isIt = LineIntersect(a, b, c, d).result;
				Vec2 point = LineIntersect(a, b, c, d).pos;

				if (isIt)
				{
					Vec2 len = { point.x - l->source.x, point.y - l->source.y };
					if (len.length() < l->visibleLength)
					{
						l->visibleLength = len.length();
					}
				}
			}
		}

		//std::cout << blocks.size() << std::endl;

		window.clear();
		// draw blocks in the window
		for (auto& a : blocks)
		{
			sf::Vertex line[]
			{
				sf::Vertex(sf::Vector2f(a->startPoint.x, a->startPoint.y), sf::Color::White),
				sf::Vertex(sf::Vector2f(a->endPos.x, a->endPos.y), sf::Color::White)
			};
			window.draw(line, 2, sf::Lines);
		}

		// draw a default light source
		for (auto& a : light.RayLines)
		{
			sf::Vertex line[]
			{
			
				sf::Vertex(sf::Vector2f(a->source.x, a->source.y), sf::Color::Yellow),
				sf::Vertex(sf::Vector2f(a->getVisibleEndPos().x, a->getVisibleEndPos().y), sf::Color::Yellow)
			};
			window.draw(line, 2, sf::Lines);
		}
		window.display();
	}
}