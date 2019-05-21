#pragma once

#include "bubbles.h"
#include "iterators.h"

#include "scenario_props.h"
#include "inputs.h"

#define is_large_row(row_idx) (((row_idx) % 2) != 0)
#define is_small_row(row_idx) (((row_idx) % 2) == 0)

class BubbleCell;
class BubbleRow;
class BubbleBoard;
class BoardManager;

class Scenario;

typedef u32 row_t;
typedef u8 column_t;


class BubbleCell
{
private:
	Ptr<Bubble> _bubble;
	BubbleRow* _row;
	column_t _column;

public:
	~BubbleCell();

	const row_t& row() const;
	const column_t& column() const;

	const BubbleRow* bubbleRow() const;

	bool empty() const;
	bool operator! () const;
	operator bool() const;

	Ptr<Bubble> operator* () const;

	const Bubble* operator-> () const;

	BubbleCell(const BubbleCell&) = delete;
	BubbleCell(BubbleCell&&) = delete;

	BubbleCell& operator= (const BubbleCell&) = delete;
	BubbleCell& operator= (BubbleCell&&) = delete;

	friend class BubbleRow;
	friend class BubbleBoard;

private:
	BubbleCell();

	BubbleRow* bubbleRow();

	Ptr<Bubble>& operator* ();

	Bubble* operator-> ();
};


class BubbleRow
{
public:
	using iterator = ArrayForwardIterator<BubbleCell>;
	using const_iterator = const ArrayForwardIterator<BubbleCell>;

private:
	row_t _row;
	column_t _columns;
	BubbleCell* _cells;

	BubbleRow* _top;
	BubbleRow* _bottom;

public:
	BubbleRow();
	~BubbleRow();

	BubbleRow(BubbleRow&& br);

	column_t getColumnsCount() const;
	row_t getRow() const;

	bool isLarge() const;
	bool isSmall() const;

	const BubbleCell& operator[] (const column_t& column) const;

	bool operator! () const;
	operator bool() const;

	
	const BubbleRow* top() const;
	const BubbleRow* bottom() const;

	bool validColumn(const column_t& column) const;

	u32 count() const;
	

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	friend class BubbleCell;
	friend class BoardManager;
	friend class BubbleBoard;

private:
	void initiate(const row_t& row, const column_t& columns);
	bool isInitiated() const;

	BubbleCell& operator[] (const column_t& column);

	BubbleRow* top();
	BubbleRow* bottom();

	void forEachBubble(std::function<void(Bubble*)>& action);

	void draw(sf::RenderTarget* const& g);
	void update(const delta_t& delta);
};


class BoardManager
{
private:
	std::vector<BubbleRow> _rows;
	column_t _columns;
	row_t _rowIdGen;
	row_t _bottomRow;
	row_t _realBottomRow;

public:
	const row_t& getBottomRow() const;
	size_t size() const;
	bool empty() const;

	bool isValidRow(const row_t& row) const;

	const column_t& columns() const;
	
	const BubbleRow& bottom() const;
	const BubbleRow& top() const;
	
	const BubbleRow& operator[] (const row_t& row) const;

	const BubbleRow& firstVisible() const;

	std::vector<const BubbleCell*> findNeighbors(row_t row, column_t column) const;

	friend class BubbleBoard;

private:
	BoardManager(column_t columns);

	BubbleRow& createRow();

	void fillUntilMaxVisible();

	BubbleRow& bottom();
	BubbleRow& top();

	BubbleRow& operator[] (const row_t& row);

	BubbleRow& firstVisible();

	void clear();
};


class BubbleBoard
{
private:
	Scenario* const _sc;

	BoardManager _board;

	u32 _bubblesCount;

public:
	BubbleBoard(Scenario* const& sc, const column_t& columns);
	~BubbleBoard();

	column_t getColumnCount() const;
	row_t getRowCount() const;

	const row_t& getBottomRow() const;

	void insertBubble(const row_t& row, const column_t& column, Ptr<Bubble> bubble);

	void destroyBubble(const row_t& row, const column_t& column);

	void destroyAll();

	void addNewRow(BubbleHeap* const& bheap, TextureManager* const& tm, const BinBubbleRow& binRow, bool editorMode);

	void addRows(BubbleHeap* const& bheap, TextureManager* const& tm, const ScenarioProperties& props, bool editorMode);

	void fillPanel();

	BubbleRow& operator[] (const row_t& row);

	const BubbleRow* getFirstVisibleRow() const;

	std::vector<const BubbleCell*> findNeighbors(const row_t& row, const column_t& column) const;

	std::vector<const BubbleCell*> findConnected(const row_t& row, const column_t& column) const;

	bool isBubbleInaccesible(Ptr<Bubble> bub) const;

	void draw(sf::RenderTarget* const& g);
	void update(const delta_t& delta);
	void dispatchEvent(const InputEvent& event);


private:
	const BubbleRow& operator[] (const row_t& row) const;

	void situateBubble(Ptr<Bubble>& bub, const row_t& row, const column_t& column);

public:
	static constexpr u32 VisibleRows = 14;
};
