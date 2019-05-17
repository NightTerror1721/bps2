#include "scenario_props.h"

#include "bub_board.h"

BinBubble::BinBubble(const std::string& model, const BubbleColor& color) :
	model{ model },
	color{ color }
{}

BinBubbleRow::BinBubbleRow(const std::vector<BinBubble>& bubs) :
	_bubbles{ bubs }
{}

BinBubbleRow::BinBubbleRow(const u8& columns) :
	_bubbles{ columns }
{}

BinBubbleRow& BinBubbleRow::operator= (const std::vector<BinBubble>& bubs) { _bubbles = bubs; return *this; }

BinBubble& BinBubbleRow::operator[] (const size_t& index) { return _bubbles[index]; }
const BinBubble& BinBubbleRow::operator[] (const size_t& index) const { return _bubbles[index]; }

size_t BinBubbleRow::size() const { return _bubbles.size(); }

void BinBubbleRow::checkColumns(const u8& columns)
{
	if (_bubbles.size() != columns)
		_bubbles.resize(columns, { "", BubbleColor::defaultColor() });
}

void BinBubbleRow::setBubble(const u8& column, const BinBubble& bub)
{
	u8 col = column;
	if (col >= _bubbles.size())
		return;

	_bubbles[col] = bub;
}







ScenarioProperties::ScenarioProperties() :
	_player{ PlayerId::Single },
	_columns{ 8 },
	_bubbleRows{},
	_hideType{ HideRowsType::None },
	_availableColors{ 0xff },
	_seed{ 0 }
{}

void ScenarioProperties::setPlayerId(const PlayerId& player) { _player = player; }
const PlayerId& ScenarioProperties::getPlayerId() const { return _player; }

void ScenarioProperties::setColumns(const u8& columns)
{
	_columns = columns;
	for (BinBubbleRow& row : _bubbleRows)
		row.checkColumns(_columns);
}
const u8& ScenarioProperties::getColumns() const { return _columns; }

void ScenarioProperties::setBubbleRows(const std::vector<BinBubbleRow>& rows)
{
	_bubbleRows = rows;
	for (BinBubbleRow& row : _bubbleRows)
		row.checkColumns(_columns);
}
const std::vector<BinBubbleRow>& ScenarioProperties::getBubbleRows() const { return _bubbleRows; }
u32 ScenarioProperties::getBubbleRowsCount() const { return static_cast<u32>(_bubbleRows.size()); }
void ScenarioProperties::addBubbleRow(const std::vector<BinBubble>& bubs)
{
	_bubbleRows.push_back(BinBubbleRow{ bubs });
	_bubbleRows[_bubbleRows.size() - 1].checkColumns(_columns);
}
BinBubbleRow& ScenarioProperties::addNewBubbleRow() { _bubbleRows.push_back(BinBubbleRow{ _columns }); return _bubbleRows[_bubbleRows.size() - 1]; }
BinBubbleRow& ScenarioProperties::getBubbleRow(const u32& rowIndex) { return _bubbleRows[rowIndex]; }
const BinBubbleRow& ScenarioProperties::getBubbleRow(const u32& rowIndex) const { return _bubbleRows[rowIndex]; }
void ScenarioProperties::setBubbleToRow(const u32& row, const u8& column, const BinBubble& bub)
{
	if (row < _bubbleRows.size())
		_bubbleRows[row].setBubble(column, bub);
}
void ScenarioProperties::setNewBubbleToRow(const u32& row, const u8& column, const std::string& model, const BubbleColor& color)
{
	if (row < _bubbleRows.size())
		_bubbleRows[row].setBubble(column, { model, color });
}

void ScenarioProperties::setHideType(const HideRowsType& flags) { _hideType = flags; }
const HideRowsType& ScenarioProperties::getHideType() const { return _hideType; }
bool ScenarioProperties::hasHideRowsType(const HideRowsType& flags) const
{
	return (static_cast<u8>(_hideType) & ~static_cast<u8>(flags)) != 0;
}

void ScenarioProperties::setAvailableColors(const colormask_t& colors) { _availableColors = colors; }
const colormask_t& ScenarioProperties::getAvailableColors() const { return _availableColors; }
void ScenarioProperties::setColorEnabled(const BubbleColor& color, const bool& enabled)
{
	_availableColors = enabled ? _availableColors + color : _availableColors - color;
}
bool ScenarioProperties::hasColorAvailable(const BubbleColor& color) const { return _availableColors & color; }

void ScenarioProperties::setSeed(const seed_t& seed) { _seed = seed; }
const seed_t& ScenarioProperties::getSeed() const { return _seed; }
