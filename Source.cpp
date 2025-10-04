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
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	std::shared_ptr<sf::Shape>              s;
	std::vector<float>                      XSpeeds, YSpeeds;
	std::vector<float>                      XPositions, YPositions;
	std::vector<int>                        Reds, Blues, Greens;
	std::vector<int>                        WidthSize, HeightSize;
	std::vector<sf::Text>                   names;


	const std::string& FileName = "Data/Assignment1.txt";
	std::ifstream fin(FileName);

	sf::Font myFont;
	if (!myFont.loadFromFile("fonts/tech.ttf"))
	{
		std::cerr << "Could not load font!\n";
		exit(-1);
	}


	std::string Shape, Name;
	int Red, Green, Blue;
	float InitialPositionX, InitialPositionY;
	float InitialSpeedX, InitialSpeedY;
	float Width, Hight, Radius;

	while (fin >> Shape)
	{
		fin >> Name >> InitialPositionX >> InitialPositionY >> InitialSpeedX >> InitialSpeedY;
		fin >> Red >> Green >> Blue;

		if (Shape == "Rectangle")
		{
			fin >> Width >> Hight;
			s = std::make_shared<sf::RectangleShape>(sf::Vector2f(Width, Hight));
			WidthSize.push_back(Width);
			HeightSize.push_back(Hight);
		}
		else if (Shape == "Circle")
		{
			fin >> Radius;
			s = std::make_shared<sf::CircleShape>(Radius);
			WidthSize.push_back(2*Radius);
			HeightSize.push_back(2*Radius);
		}

		sf::Text n(Name, myFont, 14);


		shapes.push_back(s);
		XPositions.push_back(InitialPositionX);
		YPositions.push_back(InitialPositionY);
		XSpeeds.push_back(InitialSpeedX);
		YSpeeds.push_back(InitialSpeedY);
		Reds.push_back(Red);
		Greens.push_back(Green);
		Blues.push_back(Blue);
		names.push_back(n);
	}

	const int wWidth = 450;
	const int wHight = 450;
	sf::RenderWindow window(sf::VideoMode(wWidth, wHight), "SFML works!");
	window.setFramerateLimit(180);


	int i = 0;
	for (auto& c : shapes)
	{
		int r = Reds[i];
		int b = Blues[i];
		int g = Greens[i];
		c->setFillColor(sf::Color(r, g, b));
		c->setPosition(XPositions[i], YPositions[i]);
		i += 1;
	}


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key pressed with code = " << event.key.code << "\n";

				if (event.key.code == sf::Keyboard::X)
				{
					//circleMoveSpeed *= -1.0f;
				}
			}
		}

		i = 0;
		for (auto& c : shapes)
		{
			c->setPosition(c->getPosition().x + XSpeeds[i], c->getPosition().y + YSpeeds[i]);

			if (c->getPosition().x < 0)
			{
				XSpeeds[i] *= -1.0f;
			}

			if (c->getPosition().y < 0)
			{
				YSpeeds[i] *= -1.0f;
			}

			if (c->getPosition().x > wWidth - WidthSize[i])
			{
				XSpeeds[i] *= -1.0f;
			}

			if (c->getPosition().y > wHight - HeightSize[i])
			{
				YSpeeds[i] *= -1.0f;
			}

			names[i].setPosition(c->getPosition().x + WidthSize[i] / 2 - names[i].getLocalBounds().width / 2,
			  c->getPosition().y + HeightSize[i] / 2 - names[i].getLocalBounds().height / 2);

			i += 1;
		}


		window.clear();
		for (auto& c : shapes)
		{
			window.draw(*c);
		}
		for (auto& c : names)
		{
			window.draw(c);
		}
		window.display();
	}

	return 0;

}