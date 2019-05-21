#include "scenario.h"

#include <SFML/Graphics.hpp>

#include "engine.h"

Scenario::Scenario(GameController* const& gc, TextureManager* const& tm, const ScenarioProperties& prop) :
	_gc(gc),
	_tm(tm),
	_bounds{ computeBoardBounds() },
	_bubbles(this, prop.getColumns()),
	_bheap(),
	_cam(),
	_bcam()
{
	init(prop);
}

Scenario::~Scenario()
{
	_bubbles.destroyAll();
}

void Scenario::init(const ScenarioProperties& prop)
{
	pylib::bindTextureManager(_tm);

	_bubbles.addRows(&_bheap, _tm, prop, false);

	pylib::unbindTextureManager();
}

void Scenario::draw(sf::RenderTarget* const& g)
{
	sf::View oldView = _cam.bind(_gc);

	drawBoardLines(g, true);
	drawBubbles(g);

	_cam.unbind(_gc, oldView);
}

void Scenario::update(const delta_t& delta)
{
	pylib::bindTextureManager(_tm);

	updateBubbles(delta);

	_bheap.clearDestroyeds();

	pylib::unbindTextureManager();
}


void Scenario::updateBubbles(const delta_t& delta)
{
	_bubbles.update(delta);
}





void Scenario::dispatchEvent(const InputEvent& event)
{

}

void Scenario::drawBubbles(sf::RenderTarget* const& g)
{
	_bcam.bind(_gc);

	_bubbles.draw(g);

	_cam.bind(_gc);
}

void Scenario::drawBoardLines(sf::RenderTarget* const& g, const bool& grid)
{
	sf::RectangleShape board{ { _bounds.width, _bounds.height + (Bubble::HitboxHeight * 2.75f) } };
	board.setPosition(_bounds.left, _bounds.top);
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
					_bounds.left + delta_x + (Bubble::HitboxWith * column),
					(_bounds.top + _bounds.height) - (Bubble::HitboxHeight * row) - Bubble::HitboxHeight
				});
				g->draw(board);
			}
		}
	}
}

const sf::FloatRect Scenario::getBounds() const { return _bounds; }

sf::FloatRect Scenario::computeBoardBounds() const
{
	const float width = static_cast<float>(_bubbles.getColumnCount() * Bubble::HitboxWith);
	return {
		(_cam.getSize().x / 2) - (width / 2),
		static_cast<float>(Bubble::HitboxWith),
		width,
		BubbleBoard::VisibleRows * Bubble::HitboxHeight
	};
}
