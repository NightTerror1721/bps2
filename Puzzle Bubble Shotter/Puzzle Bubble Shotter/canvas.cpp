#include "canvas.h"

sf::View Canvas::createCanvasView(const sf::RenderWindow& window)
{
	const sf::Vector2u& size = window.getSize();
	
	sf::View view{ { 0, 0 }, { CANVAS_WIDTH, CANVAS_HEIGHT } };
	
	unsigned int v_min = std::min(CANVAS_WIDTH, CANVAS_HEIGHT);
	unsigned int w_min = std::min(size.x, size.y);
	float factor = static_cast<float>(w_min) / static_cast<float>(v_min);
	
	return std::move(view);
}
