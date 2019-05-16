#pragma once

#include "camera.h"
#include "bub_board.h"
#include "inputs.h"
#include "scenario_props.h"

class GameController;

class Scenario
{
private:
	GameController* const _gc;

	BubbleBoard _bubbles;
	ScenarioCamera _cam;

public:
	Scenario(GameController* const& gc, const ScenarioProperties& prop);

	void draw(sf::RenderTarget* const& g);
	void update(const delta_t& delta);
	void dispatchEvent(const InputEvent& event);

private:
	void init(const ScenarioProperties& prop);
	void drawBoardLines(sf::RenderTarget* const& g, const bool& grid = false);

	sf::FloatRect computeBoardBounds() const;
};
