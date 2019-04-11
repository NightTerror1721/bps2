#include "scenario.h"

#include <cstdarg>

BinaryBubbleBoard::BinaryBubbleBoard(const column_t& columns) : _columns(col_cast(columns)), _board(VISIBLE_ROWS, _columns)
{
	_board.fill({"", BubbleColor::Colorless});
}

BinaryBubbleBoard::~BinaryBubbleBoard()
{
}

void BinaryBubbleBoard::insertBubble(size_t row, size_t column, const BubbleIdentifier& bid)
{
	if (row >= VISIBLE_ROWS)
		throw BPSException("Illegal argument exception");
	
	if(column >= getColumns(row))
		throw BPSException("Illegal argument exception");
	_board[row][column] = bid;
}

BubbleIdentifier BinaryBubbleBoard::getBubble(const size_t& row, const size_t& column) const
{
	if (row >= VISIBLE_ROWS)
		throw BPSException("Illegal argument exception");

	if (column >= getColumns(row))
		throw BPSException("Illegal argument exception");
	return _board[row][column];
}




std::string RandomBubbleTypeGenerator::generate(RNG& rand)
{
	if (_percents.empty() || !_count)
		return bubman_getDefaultModel()->name;

	uint32 value = static_cast<uint32>(rand(BUBGEN_MIN, BUBGEN_MAX));
	for (auto&& p : _percents)
	{
		if (value < p.second)
			return p.first;
		value -= p.second;
	}
	return bubman_getDefaultModel()->name;
}

void RandomBubbleTypeGenerator::setPercent(const std::string& modelName, uint16 percent)
{
	if (!bubman_hasBubbleModel(modelName))
		return;
	percent = minmax_range(BUBGEN_MIN, BUBGEN_MAX, percent);
	for (auto it = _percents.begin(); it != _percents.end(); it++)
		if (it->first == modelName)
		{
			_count += percent - it->second;
			if (!percent)
				_percents.erase(it);
			else it->second = percent;
		}

	if (!percent)
		return;

	_count += percent;
	_percents.push_back({ modelName, percent });
}

uint16 RandomBubbleTypeGenerator::getPercent(const std::string& modelName) const
{
	for (auto it = _percents.cbegin(); it != _percents.cend(); it++)
		if (it->first == modelName)
			return it->second;
	return 0;
}



void MetaScenarioGoals::setBubbleGoal(const BubbleIdentifier& bid, uint32 amount)
{
	auto it = _bubgoals.find(bid);
	if (it == _bubgoals.end())
	{
		if (!amount)
			return;
		_bubgoals[bid] = amount;
	}
	else
	{
		if (!amount)
			_bubgoals.erase(it);
		else it->second = amount;
	}
}

uint32 MetaScenarioGoals::getBubbleGoal(const BubbleIdentifier& bid)
{
	auto it = _bubgoals.find(bid);
	return it == _bubgoals.end() ? 0 : it->second;
}

void MetaScenarioGoals::forEachBubbleGoal(std::function<void(const BubbleIdentifier&, const uint32&, size_t)> action)
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








BubbleColorRandomizer::BubbleColorRandomizer(const RNG& rand) : _rand(rand), _colors(), _map(8) {}

void BubbleColorRandomizer::setAvailableColors(const color_mask_t& colors)
{
	_colors = colors;
	updateMap();
}

void BubbleColorRandomizer::setColorEnabled(const BubbleColor& color, bool enabled)
{
	_colors = enabled ? _colors + color : _colors - color;
	updateMap();
}

BubbleColor BubbleColorRandomizer::generateRandom()
{
	return _map.empty()
		? BubbleColor::Blue
		: _map[_rand(_map.size())];
}

BubbleColor BubbleColorRandomizer::generateRandom(BubbleBoard* board)
{
	//TODO:
	return {};
}

void BubbleColorRandomizer::updateMap()
{
	_map.clear();
	for (auto color : BubbleColor::normalColors())
		if (_colors & color)
			_map.push_back(color);
}







BubbleGenerator::BubbleGenerator(
	Scenario *const scenario,
	BubbleColorRandomizer* const colors,
	RandomBubbleTypeGenerator* const arrowTypes,
	RandomBubbleTypeGenerator* const boardTypes,
	seed_t seed) :
	_sc(scenario),
	_arrowRand(seed),
	_boardRand(_arrowRand()),
	_colors(colors),
	_arrowTypes(arrowTypes),
	_boardTypes(boardTypes),
	_lastColor(BubbleColor::Blue)
{}

Bubble* BubbleGenerator::generateFromIdentifier(const BubbleIdentifier& bid, TextureManager* textureManager) { return bid.createBubble(textureManager, false); }

Bubble* BubbleGenerator::generate(const std::string& modelName, TextureManager* textureManager, bool isArrow)
{
	if (!bubman_hasBubbleModel(modelName))
		return nullptr;

	BubbleModel* model = bubman_getBubbleModel(modelName);
	BubbleColor color = model->requireColor ? selectColor(isArrow) : BubbleColor::Colorless;

	return CreateNewBubble(model, color, textureManager, false);
}

BubbleColor BubbleGenerator::selectColor(bool isArrow)
{
	//TODO:
	return {};
}








uint8 HideBubbleContainer::HiddenRow::count()
{
	if (_count >= 0)
		return static_cast<uint8>(_count);

	_count = 0;
	for (auto bid : _ids)
		if (bid.isValid())
			_count++;
	return _count;
}

std::vector<Bubble*> HideBubbleContainer::HiddenRow::generateRow(TextureManager* textureManager)
{
	if (!empty())
	{
		std::vector<Bubble*> bubs(_ids.size());

		for (auto& bid : _ids)
			if (bid.isValid())
				bubs.push_back(__parent->_bgen->generateFromIdentifier(bid, textureManager));
		return bubs;
	}
	return {};
}




HideBubbleContainer::HiddenBoard::HiddenBoard(const HiddenBoard& base) :
	__parent(base.__parent),
	_rows(base._rows),
	_count(0),
	_modified(true)
{}

void HideBubbleContainer::HiddenBoard::createRow(const std::vector<BubbleIdentifier>& ids, uint8 row, uint8 boardId)
{
	if (_rows.size() < VISIBLE_ROWS)
	{
		_rows.emplace_back(HiddenRow{__parent, ids, boardId, row});
		_modified = true;
	}
}

void HideBubbleContainer::HiddenBoard::trimTop()
{
	while (!_rows.empty() && _rows.back().empty())
		_rows.pop_back();
}

std::vector<Bubble*> HideBubbleContainer::HiddenBoard::generateRow(TextureManager* textureManager)
{
	if (!empty())
	{
		_modified = true;
		std::vector<Bubble*> bubs = _rows.front().generateRow(textureManager);
		_rows.pop_front();
		return bubs;
	}
	return {};
}

std::vector<std::vector<Bubble*>> HideBubbleContainer::HiddenBoard::generateAllRows(TextureManager* textureManager)
{
	const size_t len = _rows.size();
	std::vector<std::vector<Bubble*>> all(len);

	for (size_t i = len - 1; i >= 0; i--)
		all[i] = generateRow(textureManager);
	return all;
}

uint32 HideBubbleContainer::HiddenBoard::count()
{
	if (_modified)
	{
		_count = 0;
		for (auto& row : _rows)
			_count += row.count();
		_modified = false;
	}
	return _count;
}





HideBubbleContainer::HideBubbleContainer(ScenarioProperties *const props, column_t columns, BubbleGenerator *const bgen, seed_t seed, TextureManager* const textureManager) :
	_type(props->getHideBubblesContainerType()),
	_columns(col_cast(columns)),
	_bgen(bgen),
	_rand(seed),
	_maxBoards(props->getClearBoardsRequired() < 1 ? -1 : props->getClearBoardsRequired()),
	_boards(),
	_current(nullptr),
	_tm(textureManager)
{}

HideBubbleContainer::~HideBubbleContainer()
{
	auto it = _boards.begin();
	while (it != _boards.end())
	{
		auto hb = *it;
		_boards.erase(it++);
		delete hb;
	}
}

void HideBubbleContainer::fill(const std::vector<BinaryBubbleBoard>& bin)
{
	if (bin.empty())
		return;

	_boards.clear();
	_current = nullptr;
	uint8 max = static_cast<uint8>(bin.size() > 255 ? 255 : bin.size());

	std::vector<HiddenBoard*> aux(max);
	for (uint8 i = 0; i < max; i++)
	{
		auto& bbb = bin[i];
		HiddenBoard* hb = new HiddenBoard(this);
		for (uint8 row = VISIBLE_ROWS - 1; row >= 0; row--)
			createHiddenRow(hb, bbb, row, i);
		aux.push_back(hb);
	}
	if (!IsDiscrete(_type) && aux.size() > 1)
		aux[aux.size() - 1]->trimTop();

	bool random = IsRandom(_type);
	auto it = aux.begin();
	while (it != aux.end())
	{
		auto hb = *it;
		aux.erase(it++);
		_boards.push_back(hb);
	}
}

HideBubbleContainer::bubble_board_t HideBubbleContainer::generate()
{
	if (empty())
		return {};

	checkNext();
	if (!IsDiscrete(_type))
		return { _current->generateRow(_tm) };
	return _current->generateAllRows(_tm);
}

HideBubbleContainer::bubble_board_t HideBubbleContainer::generateBoard()
{
	if (empty())
		return {};

	checkNext();
	return _current->generateAllRows(_tm);
}

HideBubbleContainer::bubble_row_t HideBubbleContainer::generateRow()
{
	if (IsDiscrete(_type))
		throw BPSException{ "Cannot generate rows in Discrete hidden system" };

	if (empty())
		return {};

	checkNext();
	return _current->generateRow(_tm);
}

uint32 HideBubbleContainer::getBubbleCount()
{
	uint32 ct = 0;
	for (auto& hb : _boards)
		ct += hb->count();
	if (_current)
		ct += _current->count();
	return ct;
}

void HideBubbleContainer::checkNext()
{
	if (!_current || _current->empty())
	{
		if (_current && _current->empty())
			delete _current;
		_current = _boards.front();
		_boards.pop_front();
		if (IsEndless(_type))
		{
			_boards.push_back(_current);
			_current = _current->copyPtr();
		}
	}
}

void HideBubbleContainer::createHiddenRow(HiddenBoard* board, const BinaryBubbleBoard& bbb, uint8 row, uint8 boardId)
{
	uint8 pair = row % 2;
	uint8 cmax = _columns - pair;

	std::vector<BubbleIdentifier> ids(cmax);
	uint8 ct = 0;
	for (uint8 c = 0; c < cmax; c++)
	{
		BubbleIdentifier id = bbb.getBubble(row, c);
		ids.push_back(id);
		if (id.isValid())
			ct++;
	}
	if (ct > 0)
		board->createRow(ids, row, boardId);
	else board->createEmptyRow(row, boardId);
}
