#pragma once

#include <vector>
#include <string>

#include "bubbles.h"
#include "rng.h"

class ScenarioProperties;

enum class PlayerId
{
	Player_One,
	Player_Two,
	Single = Player_One
};

struct BinBubble
{
	std::string model;
	BubbleColor color;

	BinBubble(const std::string& model = "", const BubbleColor& color = BubbleColor::defaultColor());
};

class BinBubbleRow
{
private:
	std::vector<BinBubble> _bubbles;

public:
	explicit BinBubbleRow(const std::vector<BinBubble>& bubs);
	explicit BinBubbleRow(const u8& columns);

	BinBubbleRow& operator= (const std::vector<BinBubble>& bubs);

	BinBubble& operator[] (const size_t& index);
	const BinBubble& operator[] (const size_t& index) const;

	size_t size() const;

	friend class ScenarioProperties;

private:
	void checkColumns(const u8& columns);
	void setBubble(const u8& column, const BinBubble& bub);
};


enum class HideRowsType : u8
{
	None     = 0x0,
	Discrete = 0x1 << 0,
	Random	 = 0x1 << 1,
	Endless  = 0x1 << 2
};



class ScenarioProperties
{
private:
	PlayerId _player{ PlayerId::Single };
	u8 _columns{ 8 };
	std::vector<BinBubbleRow> _bubbleRows{};
	HideRowsType _hideType{ HideRowsType::None };
	colormask_t _availableColors{ 0xff };
	seed_t _seed{ 0 };


public:
	ScenarioProperties();

	void setPlayerId(const PlayerId& player);
	const PlayerId& getPlayerId() const;

	void setColumns(const u8& columns);
	const u8& getColumns() const;

	void setBubbleRows(const std::vector<BinBubbleRow>& rows);
	const std::vector<BinBubbleRow>& getBubbleRows() const;
	u32 getBubbleRowsCount() const;
	void addBubbleRow(const std::vector<BinBubble>& bubs);
	BinBubbleRow& addNewBubbleRow();
	BinBubbleRow& getBubbleRow(const u32& rowIndex);
	const BinBubbleRow& getBubbleRow(const u32& rowIndex) const;
	void setBubbleToRow(const u32& row, const u8& column, const BinBubble& bub);
	void setNewBubbleToRow(const u32& row, const u8& column, const std::string& model, const BubbleColor& color);

	void setHideType(const HideRowsType& flags);
	const HideRowsType& getHideType() const;
	bool hasHideRowsType(const HideRowsType& flags) const;

	void setAvailableColors(const colormask_t& colors);
	const colormask_t& getAvailableColors() const;
	void setColorEnabled(const BubbleColor& color, const bool& enabled);
	bool hasColorAvailable(const BubbleColor& color) const;

	void setSeed(const seed_t& seed);
	const seed_t& getSeed() const;

};
