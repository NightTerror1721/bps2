#include "camera.h"
#include "bubbles.h"
#include "engine.h"

#define ROW_SIZE(row) static_cast<float>((row) * Bubble::HitboxHeight)


ScenarioCamera::ScenarioCamera() :
	View{ { 0, 0 }, { static_cast<float>(ScenarioCamera::Width), static_cast<float>(ScenarioCamera::Height) } },
	_row{ 0 }
{}

void ScenarioCamera::setRow(const row_t& row)
{
	_row = row;
	setCenter({ 0, -ROW_SIZE(_row) });
}

const row_t& ScenarioCamera::getRow() const
{
	return _row;
}

void ScenarioCamera::increaseRow(const row_t& amount)
{
	if (amount > 0)
	{
		_row += amount;
		setCenter({ 0, -ROW_SIZE(_row) });
	}
}

void ScenarioCamera::bind(GameController* const& gc)
{
	gc->getWindow()->setView(*this);
}
