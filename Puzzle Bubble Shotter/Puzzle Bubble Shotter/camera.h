#pragma once

#include <SFML/Graphics/View.hpp>

#include "bub_board.h"

class GameController;

class ScenarioCamera : public sf::View
{
public:
	ScenarioCamera();

	sf::View bind(GameController* const& gc) const;
	void unbind(GameController* const& gc, const sf::View& oldView) const;

	static constexpr u32 Width = 1728;
	static constexpr u32 Height = 972;

protected:
	static vec2f computeCenter(const u32& row = 0);
};

class BubblesCamera : public ScenarioCamera
{
private:
	row_t _row;

public:
	BubblesCamera();

	void setRow(const row_t& row);
	const row_t& getRow() const;

	void increaseRow(const row_t& amount = 1);
};
