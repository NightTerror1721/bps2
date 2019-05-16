#include "scenario.h"

#include <SFML/Graphics.hpp>

#include "engine.h"

Scenario::Scenario(GameController* const& gc, const ScenarioProperties& prop) :
	_gc(gc),
	_bubbles(prop.getColumns()),
	_cam()
{
	init(prop);
}

void Scenario::init(const ScenarioProperties& prop)
{
	
}

void Scenario::draw(sf::RenderTarget* const& g)
{
	sf::View oldView = _cam.bind(_gc);

	drawBoardLines(g, true);

	_cam.unbind(_gc, oldView);
}

void Scenario::update(const delta_t& delta)
{

}

void Scenario::dispatchEvent(const InputEvent& event)
{

}

void Scenario::drawBoardLines(sf::RenderTarget* const& g, const bool& grid)
{
	sf::FloatRect bounds{ computeBoardBounds() };
	sf::RectangleShape board{ { bounds.width, bounds.height + (Bubble::HitboxHeight * 2.75f) } };
	board.setPosition(bounds.left, bounds.top);
	board.setFillColor(sf::Color::Transparent);
	board.setOutlineColor(sf::Color::Blue);
	board.setOutlineThickness(2.f);

	g->draw(board);

	if (grid)
	{
		board.setOutlineThickness(2.f);
		board.setSize({ static_cast<float>(Bubble::HitboxWith), static_cast<float>(Bubble::HitboxHeight) });
		const u8 maxColumns = _bubbles.getColumnCount();

		for (u32 row = 0; row < BubbleBoard::VisibleRows; row++)
		{
			u8 columns = is_small_row(row) ? maxColumns - 1 : maxColumns;
			const float delta_x = is_small_row(row) ? Bubble::HitboxWith / 2.f : 0.f;
			for (u8 column = 0; column < columns; column++)
			{
				board.setPosition({
					bounds.left + delta_x + (Bubble::HitboxWith * column),
					(bounds.top + bounds.height) - (Bubble::HitboxHeight * row) - Bubble::HitboxHeight
				});
				g->draw(board);
			}
		}
	}
}

sf::FloatRect Scenario::computeBoardBounds() const
{
	const float width = static_cast<float>(column_t::base(_bubbles.getColumnCount()) * Bubble::HitboxWith);
	return {
		(_cam.getSize().x / 2) - (width / 2),
		static_cast<float>(Bubble::HitboxWith),
		width,
		BubbleBoard::VisibleRows * Bubble::HitboxHeight
	};
}
