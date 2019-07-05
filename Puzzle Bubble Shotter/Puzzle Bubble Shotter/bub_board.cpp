#include "bub_board.h"

#include <set>

#include "scenario.h"


BubbleCell::BubbleCell() :
	_bubble(nullptr),
	_row(),
	_column()
{}

BubbleCell::~BubbleCell()
{
	Ptr<Bubble>::free(_bubble);
}

const row_t& BubbleCell::row() const { return _row->_row; }
const column_t& BubbleCell::column() const { return _column; }

BubbleRow* BubbleCell::bubbleRow() { return _row; }
const BubbleRow* BubbleCell::bubbleRow() const { return _row; }

bool BubbleCell::empty() const { return !_bubble; }
bool BubbleCell::operator! () const { return !_bubble; }
BubbleCell::operator bool() const { return _bubble; }

Ptr<Bubble>& BubbleCell::operator* () { return _bubble; }
Ptr<Bubble> BubbleCell::operator* () const { return _bubble; }

Bubble* BubbleCell::operator-> () { return !_bubble ? nullptr : &_bubble; }
const Bubble* BubbleCell::operator-> () const { return !_bubble ? nullptr : &_bubble; }





BubbleRow::BubbleRow() :
	_row{},
	_columns{},
	_cells{ nullptr },
	_top{ nullptr },
	_bottom{ nullptr }
{}

BubbleRow::~BubbleRow()
{
	if(_cells)
		delete[] _cells;
}

BubbleRow::BubbleRow(BubbleRow&& br) :
	_row{ std::move(br._row) },
	_columns{ std::move(br._columns) },
	_cells{ std::move(br._cells) }
{
	br._cells = nullptr;
}

column_t BubbleRow::getColumnsCount() const { return _columns; }
row_t BubbleRow::getRow() const { return _row; }

bool BubbleRow::isLarge() const { return is_large_row(_row); }
bool BubbleRow::isSmall() const { return is_small_row(_row); }

BubbleCell& BubbleRow::operator[] (const column_t& column) { return _cells[column]; }
const BubbleCell& BubbleRow::operator[] (const column_t& column) const { return _cells[column]; }

bool BubbleRow::operator! () const
{
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++)
		if (!(cell++)->empty())
			return false;
	return true;
}
BubbleRow::operator bool() const
{
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++)
		if ((cell++)->empty())
			return false;
	return true;
}

Ptr<BubbleRow> BubbleRow::top() { return _top; }
ConstPtr<BubbleRow> BubbleRow::top() const { return _top; }

Ptr<BubbleRow> BubbleRow::bottom() { return _bottom; }
ConstPtr<BubbleRow> BubbleRow::bottom() const { return _bottom; }

bool BubbleRow::validColumn(const column_t& column) const { return column < _columns; }

u32 BubbleRow::count() const
{
	u32 amount = 0;
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++)
		if (*(cell++))
			amount++;
	return amount;
}

void BubbleRow::initiate(const row_t& row, const column_t& columns)
{
	_row = row;
	_columns = is_small_row(_row) ? columns - 1 : columns;
	if (_cells)
		delete[] _cells;
	_cells = new BubbleCell[_columns];
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++, cell++)
	{
		cell->_row = this;
		cell->_column = i;
	}
}

bool BubbleRow::isInitiated() const { return _cells; }

void BubbleRow::forEachBubble(std::function<void(Bubble*)>& action)
{
	for (u8 i = 0; i < _columns; i++)
	{
		BubbleCell& cell = _cells[i];
		if (cell)
			action(&(*cell));
	}
}

void BubbleRow::draw(sf::RenderTarget* const& g)
{
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++, cell++)
	{
		if (*cell)
			(**cell)->draw(g);
	}

}

void BubbleRow::update(const delta_t& delta)
{
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++, cell++)
		if (*cell)
			(**cell)->update(delta);
}



BubbleRow::iterator BubbleRow::begin() { return { _cells, _columns }; }
BubbleRow::const_iterator BubbleRow::begin() const { return { _cells, _columns }; }
BubbleRow::iterator BubbleRow::end() { return { _cells, _columns, _columns }; }
BubbleRow::const_iterator BubbleRow::end() const { return { _cells, _columns, _columns }; }






BoardManager::BoardManager(column_t columns) :
	_rowsAllocator{},
	_rows{},
	_columns{ columns },
	_rowIdGen{ 0 },
	_bottomRow{ 0 },
	_realBottomRow{ 0 }
{}

const row_t& BoardManager::getBottomRow() const { return _bottomRow; }
size_t BoardManager::size() const { return _rows.size(); }
bool BoardManager::empty() const { return _rows.empty(); }

bool BoardManager::isValidRow(const row_t& row) const { return row + (_bottomRow - _realBottomRow) < _rows.size(); }

const column_t& BoardManager::columns() const { return _columns; }

Ptr<BubbleRow> BoardManager::createRow()
{
	Ptr<BubbleRow> old{ _rows.empty() ? nullptr : _rows.back() };
	Ptr<BubbleRow> row{ _rowsAllocator.malloc() };
	
	row->initiate(_rowIdGen++, _columns);
	if (old)
	{
		old->_top = row;
		row->_bottom = old;
	}
	else _realBottomRow = row->_row;
	_rows.push_back(row);
	return row;
}

void BoardManager::fillUntilMaxVisible()
{
	if (_rows.size() < BubbleBoard::VisibleRows)
	{
		const size_t max = BubbleBoard::VisibleRows - _rows.size();
		for (size_t i = 0; i < max; ++i)
			createRow();
	}
}

Ptr<BubbleRow> BoardManager::bottom() { return _rows.front(); }
ConstPtr<BubbleRow> BoardManager::bottom() const { return _rows.front(); }

Ptr<BubbleRow> BoardManager::top() { return _rows.back(); }
ConstPtr<BubbleRow> BoardManager::top() const { return _rows.back(); }

Ptr<BubbleRow> BoardManager::operator[] (const row_t& row) { return _rows[row + (_bottomRow - _realBottomRow)]; }
ConstPtr<BubbleRow> BoardManager::operator[] (const row_t& row) const { return _rows[row + (_bottomRow - _realBottomRow)]; }

Ptr<BubbleRow> BoardManager::firstVisible()
{
	const row_t first = (_bottomRow - _realBottomRow) + BubbleBoard::VisibleRows - 1;
	if (first >= _rows.size())
		return _rows.back();
	return _rows[first];
}
ConstPtr<BubbleRow> BoardManager::firstVisible() const
{
	const row_t first = (_bottomRow - _realBottomRow) + BubbleBoard::VisibleRows - 1;
	if (first >= _rows.size())
		return _rows.back();
	return _rows[first];
}


#define DELTA(value, delt) ((value) + (delt))
#define delta_get(rows, row, column, d_row, d_column) ((*((rows)[DELTA(row, d_row)]))[DELTA(column, d_column)])
#define insert_nei(vec, s_rows, s_columns, rows, row, column, d_row, d_column) { \
	if(DELTA(row, d_row) >= s_rows || DELTA(column, d_column) >= s_columns) { \
		const BubbleCell& __cell = delta_get(rows, row, column, d_row, d_column); \
		if(__cell) (vec).push_back(&__cell); \
	} \
}

std::vector<const BubbleCell*> BoardManager::findNeighbors(row_t row, column_t column) const
{
	row = row + (_bottomRow - _realBottomRow);
	std::vector<const BubbleCell*> neis(6);
	row_t rowsCount{ static_cast<u16>(_rows.size()) };
	const column_t& columnsCount = _columns;
	insert_nei(neis, rowsCount, columnsCount, _rows, row, column, 0, -1)
		insert_nei(neis, rowsCount, columnsCount, _rows, row, column, 0, 1)

		if (is_small_row(row))
		{
			insert_nei(neis, rowsCount, columnsCount, _rows, row, column, -1, 0)
				insert_nei(neis, rowsCount, columnsCount, _rows, row, column, -1, 1)
				insert_nei(neis, rowsCount, columnsCount, _rows, row, column, 1, 0)
				insert_nei(neis, rowsCount, columnsCount, _rows, row, column, 1, 1)
		}
		else
		{
			insert_nei(neis, rowsCount, columnsCount, _rows, row, column, -1, -1)
				insert_nei(neis, rowsCount, columnsCount, _rows, row, column, -1, 0)
				insert_nei(neis, rowsCount, columnsCount, _rows, row, column, 1, -1)
				insert_nei(neis, rowsCount, columnsCount, _rows, row, column, 1, 0)
		}

	return std::move(neis);
}

void BoardManager::clear()
{
	_rows.clear();
	_rowsAllocator.clear();
	_rowIdGen = 0;
	_bottomRow = 0;
	_realBottomRow = 0;

}






BubbleBoard::BubbleBoard(Scenario* const& sc, const column_t& columns) :
	_sc(sc),
	_board{ columns },
	_bubblesCount()
{}

BubbleBoard::~BubbleBoard()
{

}

column_t BubbleBoard::getColumnCount() const { return _board.columns(); }
row_t BubbleBoard::getRowCount() const { return { static_cast<u32>(_board.size()) }; }

const row_t& BubbleBoard::getBottomRow() const { return _board.getBottomRow(); }

void BubbleBoard::insertBubble(const row_t& row, const column_t& column, Ptr<Bubble> bubble)
{
	if (bubble)
	{
		auto& cell = (*_board[row])[column];
		if (cell)
		{
			cell->destroy();
		}
		else _bubblesCount++;
		*cell = bubble;
		bubble->setBoardCell(&cell);
		situateBubble(bubble, row, column);
	}
}

void BubbleBoard::destroyBubble(const row_t& row, const column_t& column)
{
	auto& cell = (*_board[row])[column];
	cell->destroy();
	*cell = nullptr;
	_bubblesCount--;
}

void BubbleBoard::destroyAll()
{
	_board.clear();
	_bubblesCount = 0;
}

void BubbleBoard::addNewRow(BubbleHeap* const& bheap, TextureManager* const& tm, const BinBubbleRow& binRow, bool editorMode)
{
	BubbleRow& row = *_board.createRow();
	for (u8 i = 0; i < row._columns && i < binRow.size(); i++)
	{
		const BinBubble& binbub = binRow[i];
		if (!binbub.model.empty())
		{
			Ptr<Bubble> bub{ bheap->createNew(binbub.model, tm, editorMode, binbub.color) };
			if (bub)
			{
				*row[i] = bub;
				situateBubble(bub, row._row - 1, i);
				_bubblesCount++;
			}
			else *row[i] = nullptr;
		}
		else *row[i] = nullptr;
	}
}

void BubbleBoard::addRows(BubbleHeap* const& bheap, TextureManager* const& tm, const ScenarioProperties& props, bool editorMode)
{
	for (const BinBubbleRow& binRow : props.getBubbleRows())
	{
		BubbleRow& row = *_board.createRow();
		for (u8 i = 0; i < row._columns && i < binRow.size(); i++)
		{
			const BinBubble& binbub = binRow[i];
			if (!binbub.model.empty())
			{
				Ptr<Bubble> bub{ bheap->createNew(binbub.model, tm, editorMode, binbub.color) };
				if (bub)
				{
					*row[i] = bub;
					situateBubble(bub, row._row, i);
					_bubblesCount++;
				}
				else *row[i] = nullptr;
			}
			else *row[i] = nullptr;
		}
	}
}

void BubbleBoard::fillPanel()
{
	_board.fillUntilMaxVisible();
}

Ptr<BubbleRow> BubbleBoard::operator[] (const row_t& row) { return _board[row]; }
ConstPtr<BubbleRow> BubbleBoard::operator[] (const row_t& row) const { return _board[row]; }

ConstPtr<BubbleRow> BubbleBoard::getFirstVisibleRow() const { return _board.firstVisible(); }

std::vector<const BubbleCell*> BubbleBoard::findNeighbors(const row_t& row, const column_t& column) const
{
	return _board.findNeighbors(row, column);
}

void findConnectedAlg(const BoardManager& board, std::vector<const BubbleCell*>& connected, std::set<const BubbleCell*>& visited, const Ptr<Bubble>& bub, const row_t& row, const column_t& column)
{
	std::vector<const BubbleCell*> neis = board.findNeighbors(row, column);
	if (!neis.empty())
	{
		for (const BubbleCell* (&cell) : neis)
		{
			if (*cell)
			{
				if (visited.find(cell) == visited.end())
				{
					visited.insert(cell);
					Ptr<Bubble> foundBub = **cell;
					if (bub->colorMatch(foundBub))
						findConnectedAlg(board, connected, visited, foundBub, cell->row(), cell->column());
				}
			}
		}
	}
}

std::vector<const BubbleCell*> BubbleBoard::findConnected(const row_t& row, const column_t& column) const
{
	std::vector<const BubbleCell*> connected{ 6 };
	std::set<const BubbleCell*> visited{};

	if (_board.isValidRow(row))
	{
		const BubbleRow& brow = *_board[row];
		if (brow.validColumn(column))
		{
			const BubbleCell& cell = brow[column];
			if (cell)
			{
				connected.push_back(&cell);
				visited.insert(&cell);
				findConnectedAlg(_board, connected, visited, *cell, row, column);
			}
		}
	}
	return std::move(connected);
}

bool BubbleBoard::isBubbleInaccesible(Ptr<Bubble> bub) const
{
	if (!bub->hasBoardCell())
		return false;

	const BubbleCell& cell = *bub->getBoardCell();
	if (!cell.bubbleRow()->bottom())
		return false;

	const BubbleRow& bottomRow = *cell.bubbleRow()->bottom();
	if (!bottomRow[cell._column])
		return false;

	if (bottomRow.isSmall())
		return (cell._column + 1 >= bottomRow._columns || bottomRow[cell._column + 1]) && bottomRow[cell._column];
	return (cell._column <= 0 || bottomRow[cell._column - 1]) && bottomRow[cell._column];
}


void BubbleBoard::draw(sf::RenderTarget* const& g)
{
	/*if (!_rows.empty())
	{
		const u32 rows = BubbleBoard::VisibleRows + _currentRow;
		const u32 last = _rows.front()._row;
		if (rows > last)
		{
			u32 len = rows - last;
			if (len > 16)
				len = 16;
			for (u32 i = 0, rowid = rows - 1; i < len; ++i, --rowid)
				_rows[rowid].draw(g);
		}
	}*/
	if (!_board.empty())
	{
		for (Ptr<BubbleRow> row = _board.firstVisible(); row; row = row->bottom())
			row->draw(g);
	}
}

void BubbleBoard::update(const delta_t& delta)
{
	/*if (!_rows.empty())
	{
		const u32 rows = BubbleBoard::VisibleRows + _currentRow;
		const u32 last = _rows.front()._row;
		if (rows > last)
		{
			u32 len = rows - last;
			if (len > 16)
				len = 16;
			for (u32 i = 0, rowid = rows - 1; i < len; ++i, --rowid)
				_rows[rowid].update(delta);
		}
	}*/
	if (!_board.empty())
	{
		for (Ptr<BubbleRow> row = _board.firstVisible(); row; row = row->bottom())
			row->update(delta);
	}
}

void BubbleBoard::dispatchEvent(const InputEvent& event)
{

}



void BubbleBoard::situateBubble(Ptr<Bubble>& bub, const row_t& row, const column_t& column)
{
	const sf::FloatRect& bounds = _sc->getBounds();
	const float minX = (is_small_row(row) ? bounds.left + (Bubble::HitboxWith / 2) : bounds.left) + (Bubble::HitboxWith / 2);
	const float minY = (bounds.top + bounds.height) - (Bubble::HitboxHeight / 2);
	bub->setPosition({
		minX + (Bubble::HitboxWith * static_cast<u8>(column)),
		minY - (Bubble::HitboxHeight * static_cast<u32>(row))
	});
}
