#include "camera.h"
#include "bubbles.h"

#define ROW_SIZE(row) static_cast<float>((row) * Bubble::HitboxHeight)

BoardCamera::BoardCamera() :
	View{ { 0, 0 }, { 1024, 1024 } },
	_row{ 0 }
{}

void BoardCamera::setRow(const u16& row)
{
	_row = row;
	setCenter({ 0, -ROW_SIZE(_row) });
}

const u16& BoardCamera::getRow() const
{
	return _row;
}

void BoardCamera::increaseRow(const u16& amount)
{
	if (amount > 0)
	{
		_row += amount;
		setCenter({ 0, -ROW_SIZE(_row) });
	}
}
