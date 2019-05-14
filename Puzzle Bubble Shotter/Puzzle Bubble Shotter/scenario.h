#pragma once

#include "camera.h"
#include "bub_board.h"
#include "inputs.h"

class GameController;

enum class PlayerId
{
	Player_One,
	Player_Two,
	Single = Player_One
};

struct ScenarioProperties
{
	PlayerId player;
	u8 columns;


};

class Scenario
{
private:
	GameController* const _gc;

	BubbleBoard _bubbles;
	BoardCamera _bcam;

public:
	Scenario(GameController* const& gc, const ScenarioProperties& prop);

	void draw(sf::RenderTarget* const& g);
	void update(const delta_t& delta);
	void dispatchEvent(const InputEvent& event);

private:
	void drawBoardLines(sf::RenderTarget* const& g);
};
