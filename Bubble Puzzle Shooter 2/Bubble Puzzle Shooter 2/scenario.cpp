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
	for (auto it = _percents.begin(); it != _percents.end(); it++)
		if (it->first == model)
		{
			_count += percent - it->second;
			if (!percent)
				_percents.erase(it);
			else it->second = percent;
		}

	if (!percent)
		return;

	_count += percent;
	_percents.push_back({ model, percent });
}

uint16 RandomBubbleTypeGenerator::getPercent(const BubbleModel* model) const
{
	for (auto it = _percents.cbegin(); it != _percents.cend(); it++)
		if (it->first == model)
			return it->second;
	return 0;
}



void MetaScenarioGoals::setBubbleGoal(const bubble_code_t& code, uint32 amount)
{
	auto it = _bubgoals.find(code);
	if (it == _bubgoals.end())
	{
		if (!amount)
			return;
		_bubgoals[code] = amount;
	}
	else
	{
		if (!amount)
			_bubgoals.erase(it);
		else it->second = amount;
	}
}

uint32 MetaScenarioGoals::getBubbleGoal(const bubble_code_t& code)
{
	auto it = _bubgoals.find(code);
	return it == _bubgoals.end() ? 0 : it->second;
}

void MetaScenarioGoals::forEachBubbleGoal(std::function<void(const bubble_code_t&, const uint32&, size_t)> action)
{
	size_t counter = 0;
	for (const auto& e : _bubgoals)
		action(e.first, e.second, counter++);
}





ScenarioProperties::ScenarioProperties()
	: _bubbles(), _arrowBubbleTypes(), _boardBubbleTypes(), _goals()
{

}

ScenarioProperties::~ScenarioProperties()
{
	_bubbles.clear();
}

void ScenarioProperties::setBubbleBoardCount(uint8 count)
{
	const column_t cols = getColumns();
	_bubbles.clear();
	_bubbles.reserve(count);
	for (auto i = 0; i < count; i++)
		_bubbles.emplace_back(BinaryBubbleBoard(cols));
}

