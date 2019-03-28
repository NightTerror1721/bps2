#include <SFML/Graphics.hpp>

#include "engine.h"
#include "assets.h"
#include "bubbles.h"

#include <filesystem>

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

	//std::cout << std::filesystem::current_path() << std::endl;
	//std::system("pause");

	/*Texture tex;
	if (tex.loadFromFile("assets/bubbles/color.png", sf::IntRect(0, 0, 384, 256)))
		std::cout << "Texture " << "assets/bubbles/color.png" << " loaded correctly" << std::endl;*/


	TextureManager texs;

	texs.loadTexture("assets/bubbles/color.png", "bubble.colors", 0, 0, 384, 256);

	ColorBubble* cb = new ColorBubble(&texs, BubbleColor::Red);
	cb->setTexture(*texs["bubble.colors"], true);
	cb->setTextureRect(sf::IntRect(0, 0, 32, 32));

	cb->setPosition(50, 50);
	

	Frame* frame = new Frame();

	frame->addEntity(cb);
	


	GameController gc("Bubble Puzzle Shooter");
	gc.setVideoMode({ 640, 480 }, false);
	gc.addEntity(frame);
	gc.start();


	return 0;
}