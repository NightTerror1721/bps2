#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#define CANVAS_WIDTH 1200
#define CANVAS_HEIGHT 1200

namespace Canvas
{
	sf::View createCanvasView(const sf::RenderWindow& window);
}
