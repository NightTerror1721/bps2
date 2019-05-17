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
	TextureManager* const _tm;

	BubbleBoard _bubbles;
	BubbleHeap _bheap;

	ScenarioCamera _cam;
	BubblesCamera _bcam;

	const sf::FloatRect _bounds;

public:
	Scenario(GameController* const& gc, TextureManager* const& tm, const ScenarioProperties& prop);
	~Scenario();

	Scenario(const Scenario&) = delete;
	Scenario(Scenario&&) = delete;

	Scenario& operator= (const Scenario&) = delete;
	Scenario& operator= (Scenario&&) = delete;

	const sf::FloatRect getBounds() const;

	void draw(sf::RenderTarget* const& g);
	void update(const delta_t& delta);
	void dispatchEvent(const InputEvent& event);

private:
	void init(const ScenarioProperties& prop);
	void drawBoardLines(sf::RenderTarget* const& g, const bool& grid = false);

	void drawBubbles(sf::RenderTarget* const& g);

	sf::FloatRect computeBoardBounds() const;
};
