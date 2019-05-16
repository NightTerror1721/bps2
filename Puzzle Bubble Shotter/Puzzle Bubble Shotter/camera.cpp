#include "camera.h"
#include "bubbles.h"
#include "engine.h"

#define ROW_SIZE(row) static_cast<float>((row) * Bubble::HitboxHeight)


ScenarioCamera::ScenarioCamera() :
	View{ computeCenter(), { static_cast<float>(ScenarioCamera::Width), static_cast<float>(ScenarioCamera::Height) } }
{}

sf::View ScenarioCamera::bind(GameController* const& gc) const
{
	sf::View old = gc->getWindow()->getView();
	gc->getWindow()->setView(*this);
	return std::move(old);
}

void ScenarioCamera::unbind(GameController* const& gc, const sf::View& oldView) const
{
	gc->getWindow()->setView(oldView);
}


vec2f ScenarioCamera::computeCenter(const u32& row)
{
	return {
		static_cast<float>(ScenarioCamera::Width) / 2.f,
		static_cast<float>(ScenarioCamera::Height - ROW_SIZE(row)) / 2.f
	};
}

BubblesCamera::BubblesCamera() :
	ScenarioCamera(),
	_row{ 0 }
{}


void BubblesCamera::setRow(const row_t& row)
{
	_row = row;
	setCenter(computeCenter(_row));
}

const row_t& BubblesCamera::getRow() const
{
	return _row;
}

void BubblesCamera::increaseRow(const row_t& amount)
{
	if (amount > 0)
	{
		_row += amount;
		setCenter(computeCenter(_row));
	}
}
