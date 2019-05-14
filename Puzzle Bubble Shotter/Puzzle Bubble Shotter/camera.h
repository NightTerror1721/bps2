#pragma once

#include <SFML/Graphics/View.hpp>

#include "config.h"

class BoardCamera : sf::View
{
private:
	u16 _row;

public:
	BoardCamera();

	void setRow(const u16& row);
	const u16& getRow() const;

	void increaseRow(const u16& amount = 1);
};
