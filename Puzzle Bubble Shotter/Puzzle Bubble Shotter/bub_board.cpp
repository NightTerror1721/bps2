#include "bub_board.h"

#include <set>


BubbleCell::BubbleCell() :
	_bubble(nullptr),
	_row(),
	_column()
{}

const row_t& BubbleCell::row() const { return _row; }
const column_t& BubbleCell::column() const { return _column; }

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

bool BubbleRow::validColumn(const column_t& column) const { return column < _columns; }

u32 BubbleRow::count() const
{
	u32 amount = 0;
	BubbleCell* cell = _cells;
	for (u8 i = 0; i < _columns; i++)
		if (!(cell++)->empty())
			amount++;
	return amount;
}

void BubbleRow::initiate(const row_t& row, const column_t& columns)
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




BubbleBoard::BubbleBoard(const column_t& columns) :
	_rows(),
	_columns(columns),
	_bubblesCount()
{}

column_t BubbleBoard::getColumnCount() const { return _columns; }
row_t BubbleBoard::getRowCount() const { return { static_cast<u32>(_rows.size()) }; }

void BubbleBoard::insertBubble(const row_t& row, const column_t& column, Ptr<Bubble> bubble)
{
	auto& cell =_rows[row][column];
	if (cell)
	{
		cell->destroy();
	}
	else _bubblesCount++;
	*cell = bubble;
}

void BubbleBoard::destroyBubble(const row_t& row, const column_t& column)
{
	auto& cell = _rows[row][column];
	cell->destroy();
	*cell = nullptr;
	_bubblesCount--;
}

void BubbleBoard::addNewRow(BubbleHeap* const& bheap, TextureManager* const& tm, const BinBubbleRow& binRow, bool editorMode)
{
	const u32 rowid = _rows.size();
	const u8 cols = is_small_row(rowid) ? static_cast<u8>(_columns) - 1 : static_cast<u8>(_columns);
	BubbleRow& row = _rows.emplace_back();
	row.initiate(rowid, _columns);
	for (u8 i = 0; i < cols && i < binRow.size(); i++)
	{
		const BinBubble& binbub = binRow[i];
		if (!binbub.model.empty())
			*row[i] = bheap->createNew(binbub.model, tm, editorMode, binbub.color);
		else *row[i] = nullptr;
	}
}

void BubbleBoard::addRows(BubbleHeap* const& bheap, TextureManager* const& tm, const ScenarioProperties& props, bool editorMode)
{
	u32 rowid = _rows.size();
	for (const BinBubbleRow& binRow : props.getBubbleRows())
	{
		const u8 cols = is_small_row(rowid) ? static_cast<u8>(_columns) - 1 : static_cast<u8>(_columns);
		BubbleRow& row = _rows.emplace_back();
		row.initiate(rowid, _columns);
		for (u8 i = 0; i < cols && i < binRow.size(); i++)
		{
			const BinBubble& binbub = binRow[i];
			if (!binbub.model.empty())
				*row[i] = bheap->createNew(binbub.model, tm, editorMode, binbub.color);
			else *row[i] = nullptr;
		}
		rowid++;
	}
}

BubbleRow& BubbleBoard::operator[] (const row_t& row) { return _rows[row]; }
const BubbleRow& BubbleBoard::operator[] (const row_t& row) const { return _rows[row]; }


#define DELTA(value, delt) ((value) + (delt))
#define delta_get(rows, row, column, d_row, d_column) ((rows)[DELTA(row, d_row)][DELTA(column, d_column)])
#define insert_nei(vec, s_rows, s_columns, rows, row, column, d_row, d_column) { \
	if(DELTA(row, d_row) >= s_rows || DELTA(column, d_column) >= s_columns) { \
		const BubbleCell& __cell = delta_get(rows, row, column, d_row, d_column); \
		if(__cell) (vec).push_back(&__cell); \
	} \
}


std::vector<const BubbleCell*> BubbleBoard::findNeighbors(const row_t& row, const column_t& column) const
{
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

void findConnectedAlg(const BubbleBoard* const& rows, std::vector<const BubbleCell*>& connected, std::set<const BubbleCell*>& visited, const Ptr<Bubble>& bub, const row_t& row, const column_t& column)
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

std::vector<const BubbleCell*> BubbleBoard::findConnected(const row_t& row, const column_t& column) const
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

void BubbleBoard::forEachBubbleInRange(const row_t& fromRow, const row_t& toRow, std::function<void(Bubble*)> action)
{
	row_t from{ std::min(fromRow, toRow) };
	row_t to{ std::max(fromRow, toRow) };

	if (to < _rows.size())
		for (; from <= to; from++)
			_rows[from].forEachBubble(action);
}
