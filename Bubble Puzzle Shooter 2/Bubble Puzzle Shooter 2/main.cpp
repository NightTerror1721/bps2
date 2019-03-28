#include <SFML/Graphics.hpp>

#include "engine.h"

int main(int argc, char** argv)
{
	

	/*sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	bool first = true;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();

	}*/

	GameController gc("Bubble Puzzle Shooter");
	gc.setVideoMode({ 640, 480 }, false);
	gc.start();


	return 0;
}