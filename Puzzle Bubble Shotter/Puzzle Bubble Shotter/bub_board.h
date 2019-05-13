#pragma once

#include "bubbles.h"
#include "iterators.h"

#define is_large_row(row_idx) (((row_idx) % 2) != 0)
#define is_small_row(row_idx) (((row_idx) % 2) == 0)

class BubbleCell;
class BubbleRow;
class BubbleBoard;


class BubbleCell
{
private:
	Ptr<Bubble> _bubble;
	u16 _row;
	u8 _column;

public:
	const u16& row() const;
	const u8& column() const;

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

	Ptr<Bubble>& operator* ();

	Bubble* operator-> ();
};


class BubbleRow
{
public:
	using iterator = ArrayForwardIterator<BubbleCell>;
	using const_iterator = const ArrayForwardIterator<BubbleCell>;

private:
	u16 _row;
	u8 _columns;
	BubbleCell* _cells;

public:
	~BubbleRow();

	u8 getColumnsCount() const;
	u16 getRow() const;

	bool isLarge() const;
	bool isSmall() const;

	const BubbleCell& operator[] (const u8& column) const;

	bool operator! () const;
	operator bool() const;

	bool validColumn(const u8& column) const;

	u32 count() const;
	

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	friend class BubbleBoard;

private:
	BubbleRow();

	void initiate(const u16& row, const u8& columns);
	bool isInitiated() const;

	BubbleCell& operator[] (const u8& column);

	void forEachBubble(std::function<void(Bubble*)>& action);
};


class BubbleBoard
{
	std::vector<BubbleRow> _rows;
	u8 _columns;
	u32 _bubblesCount;

public:
	BubbleBoard(const u8& columns);

	void insertBubble(const u16& row, const u8& column, Ptr<Bubble> bubble);

	void destroyBubble(const u16& row, const u8& column);

	BubbleRow& operator[] (const u16& row);

	std::vector<const BubbleCell*> findNeighbors(const u16& row, const u8& column) const;

	std::vector<const BubbleCell*> findConnected(const u16& row, const u8& column) const;

	void forEachBubbleInRange(const u16& fromRow, const u16& toRow, std::function<void(Bubble*)> action);


private:
	const BubbleRow& operator[] (const u16& row) const;
};
