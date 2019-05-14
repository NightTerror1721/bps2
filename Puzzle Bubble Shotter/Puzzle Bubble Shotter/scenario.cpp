#include "scenario.h"

Scenario::Scenario(GameController* const& gc, const ScenarioProperties& prop) :
	_gc(gc),
	_bubbles(prop.columns),
	_bcam()
{}

void Scenario::draw(sf::RenderTarget* const& g)
{

}

void Scenario::update(const delta_t& delta)
{

}

void Scenario::dispatchEvent(const InputEvent& event)
{

}

void Scenario::drawBoardLines(sf::RenderTarget* const& g)
{
	sf::Vertex boardSquare[4]
	{
		{  }
	};
}
