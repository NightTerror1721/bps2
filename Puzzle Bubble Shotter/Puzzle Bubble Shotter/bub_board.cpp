#include "bub_board.h"

#include <set>


BubbleCell::BubbleCell() :
	_bubble(nullptr),
	_row(),
	_column()
{}

const u16& BubbleCell::row() const { return _row; }
const u8& BubbleCell::column() const { return _column; }

bool BubbleCell::empty() const { return !_bubble; }
bool BubbleCell::operator! () const { return !_bubble; }
BubbleCell::operator bool() const { return _bubble; }

Ptr<Bubble>& BubbleCell::operator* () { return _bubble; }
Ptr<Bubble> BubbleCell::operator* () const { return _bubble; }

Bubble* BubbleCell::operator-> () { return !_bubble ? nullptr : &_bubble; }
const Bubble* BubbleCell::operator-> () const { return !_bubble ? nullptr : &_bubble; }




BubbleRow::BubbleRow() :
	_row(),
	_columns(),
	_cells(nullptr)
{}

BubbleRow::~BubbleRow()
{
	if(_cells)
		delete[] _cells;
}

u8 BubbleRow::getColumnsCount() const { return _columns; }
u16 BubbleRow::getRow() const { return _row; }

bool BubbleRow::isLarge() const { return is_large_row(_row); }
bool BubbleRow::isSmall() const { return is_small_row(_row); }

BubbleCell& BubbleRow::operator[] (const u8& column) { return _cells[column]; }
const BubbleCell& BubbleRow::operator[] (const u8& column) const { return _cells[column]; }

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

bool BubbleRow::validColumn(const u8& column) const { return column < _columns; }

u32 BubbleRow::count() const
{
	u32 amount = 0;
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++)
		if (!(cell++)->empty())
			amount++;
	return amount;
}

void BubbleRow::initiate(const u16& row, const u8& columns)
{
	auto cell = _cells;
	_row = row;
	_columns = is_small_row(_row) ? columns - 1 : columns;
	for (u8 i = 0; i < _columns; i++, cell++)
	{
		cell->_row = _row;
		cell->_column = 0;
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


BubbleRow::iterator BubbleRow::begin() { return { _cells, _columns }; }
BubbleRow::const_iterator BubbleRow::begin() const { return { _cells, _columns }; }
BubbleRow::iterator BubbleRow::end() { return { _cells, _columns, _columns }; }
BubbleRow::const_iterator BubbleRow::end() const { return { _cells, _columns, _columns }; }




BubbleBoard::BubbleBoard(const u8& columns) :
	_rows(),
	_columns(columns),
	_bubblesCount()
{}

void BubbleBoard::insertBubble(const u16& row, const u8& column, Ptr<Bubble> bubble)
{
	auto& cell =_rows[row][column];
	if (cell)
		cell->destroy();
	*cell = bubble;
}

void BubbleBoard::destroyBubble(const u16& row, const u8& column)
{
	auto& cell = _rows[row][column];
	cell->destroy();
	*cell = nullptr;
}

BubbleRow& BubbleBoard::operator[] (const u16& row) { return _rows[row]; }
const BubbleRow& BubbleBoard::operator[] (const u16& row) const { return _rows[row]; }


#define DELTA(value, delt) ((value) + (delt))
#define delta_get(rows, row, column, d_row, d_column) ((rows)[DELTA(row, d_row)][DELTA(column, d_column)])
#define insert_nei(vec, s_rows, s_columns, rows, row, column, d_row, d_column) { \
	if(DELTA(row, d_row) >= s_rows || DELTA(column, d_column) >= s_columns) { \
		const BubbleCell& __cell = delta_get(rows, row, column, d_row, d_column); \
		if(__cell) (vec).push_back(&__cell); \
	} \
}


std::vector<const BubbleCell*> BubbleBoard::findNeighbors(const u16& row, const u8& column) const
{
	std::vector<const BubbleCell*> neis(6);
	u16 rowsCount = static_cast<u16>(_rows.size());
	const u8& columnsCount = _columns;
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

void findConnectedAlg(const BubbleBoard* const& rows, std::vector<const BubbleCell*>& connected, std::set<const BubbleCell*>& visited, const Ptr<Bubble>& bub, const u16& row, const u8& column)
{
	std::vector<const BubbleCell*> neis = rows->findNeighbors(row, column);
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
						findConnectedAlg(rows, connected, visited, foundBub, cell->row(), cell->column());
				}
			}
		}
	}
}

std::vector<const BubbleCell*> BubbleBoard::findConnected(const u16& row, const u8& column) const
{
	std::vector<const BubbleCell*> connected{ 6 };
	std::set<const BubbleCell*> visited{};

	if (row < _rows.size())
	{
		const BubbleRow& brow = _rows[row];
		if (brow.validColumn(column))
		{
			const BubbleCell& cell = brow[column];
			if (cell)
			{
				connected.push_back(&cell);
				visited.insert(&cell);
				findConnectedAlg(this, connected, visited, *cell, row, column);
			}
		}
	}
	return std::move(connected);
}

void BubbleBoard::forEachBubbleInRange(const u16& fromRow, const u16& toRow, std::function<void(Bubble*)> action)
{
	u16 from{ std::min(fromRow, toRow) };
	u16 to{ std::max(fromRow, toRow) };

	if (to < _rows.size())
		for (; from <= to; from++)
			_rows[from].forEachBubble(action);
}
