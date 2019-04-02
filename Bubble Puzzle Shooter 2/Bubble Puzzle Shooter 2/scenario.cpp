#include "scenario.h"

BinaryBubbleBoard::BinaryBubbleBoard(const column_t& columns) : _columns(col_cast(columns)), _board(VISIBLE_ROWS, _columns)
{
	_board.fill("");
}

BinaryBubbleBoard::~BinaryBubbleBoard()
{
}

void BinaryBubbleBoard::insertBubble(size_t row, size_t column, const bubble_code_t& code)
{
	if (row >= VISIBLE_ROWS)
		throw BPSException("Illegal argument exception");
	
	if(column >= getColumns(row))
		throw BPSException("Illegal argument exception");
	_board[row][column] = code;
}

bubble_code_t BinaryBubbleBoard::getBubble(const size_t& row, const size_t& column) const
{
	if (row >= VISIBLE_ROWS)
		throw BPSException("Illegal argument exception");

	if (column >= getColumns(row))
		throw BPSException("Illegal argument exception");
	return _board[row][column];
}




const BubbleModel* RandomBubbleTypeGenerator::generate(RNG& rand)
{
	if (_percents.empty() || !_count)
		return bubman_getDefaultModel();

	uint32 value = static_cast<uint32>(rand(BUBGEN_MIN, BUBGEN_MAX));
	for (auto&& p : _percents)
	{
		if (value < p.second)
			return p.first;
		value -= p.second;
	}
	return bubman_getDefaultModel();
}

void RandomBubbleTypeGenerator::setPercent(const BubbleModel* model, uint16 percent)
{
	percent = minmax_range(BUBGEN_MIN, BUBGEN_MAX, percent);
	auto it = std::find(_percents.begin(), _percents.end(), model);
	if (it == _percents.end())
	{
		if (!percent)
			return;
		_count += percent;
		it->second = percent;
	}
	else
	{
		_count += percent - it->second;
		if (!percent)
			_percents.erase(it);
		else it->second = percent;
	}
}

uint16 RandomBubbleTypeGenerator::getPercent(const BubbleModel* model) const
{
	auto it = std::find(_percents.begin(), _percents.end(), model);
	return it == _percents.end() ? 0 : it->second;
}

