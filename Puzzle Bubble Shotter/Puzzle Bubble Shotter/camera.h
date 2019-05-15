#pragma once

#include <SFML/Graphics/View.hpp>

#include "bub_board.h"

class GameController;

class ScenarioCamera : sf::View
{
private:
	row_t _row;

public:
	ScenarioCamera();

	void setRow(const row_t& row);
	const row_t& getRow() const;

	void increaseRow(const row_t& amount = 1);

	void bind(GameController* const& gc);

	static constexpr u32 Width = 2304;
	static constexpr u32 Height = 1296;
};
