#pragma once

#include "utils.h"
#include "rng.h"
#include "bubbles.h"

#include <vector>
#include <fstream>

enum class PlayerId { First, Second, Single = First };

enum class ScenarioState { InGame, Paused, Winning, Losing, Won, Lost };

enum class column_t : uint32 { C8 = 8, C9, C10, C11, C12, C13, C14, C15, C16 };

enum class TimerMode : uint8 { TURN = 0x1, END = 0x2, TURN_AND_END = TURN | END };

enum class HideBubblesContainerType : uint8 {
	Continuous = 0x0,
	Discrete = 0x1,
	Random_Continuous = Continuous | 0x2,
	Random_Discrete = Random_Continuous | Discrete,
	Endless_Continuous = Continuous | 0x4,
	Endless_Discrete = Endless_Continuous | Discrete,
	Endless_Random_Continuous = Random_Continuous | Endless_Continuous,
	Endless_Random_Discrete = Endless_Random_Continuous | Discrete
};

inline bool IsFinalState(const ScenarioState& state) { return state == ScenarioState::Won || state == ScenarioState::Lost; }
__forceinline bool IsRunningState(const ScenarioState& state) { return !IsFinalState(state); }
inline bool IsActiveRunningState(const ScenarioState& state) { return state == ScenarioState::InGame; }
inline bool IsPasiveRunningState(const ScenarioState& state) { return state == ScenarioState::Winning || state == ScenarioState::Losing || state == ScenarioState::Paused; }
inline bool IsFinalizingState(const ScenarioState& state) { return state == ScenarioState::Winning || state == ScenarioState::Losing; }


constexpr uint32 col_cast(const column_t& col) { return static_cast<uint32>(col); }
constexpr column_t col_cast(const uint32& col) { return static_cast<column_t>(col); }

__forceinline column_t operator"" _c(unsigned long long int value) { return static_cast<column_t>(value); }

constexpr uint32 VISIBLE_ROWS = 14;
constexpr uint32 BOTTOM_ROWS = 3;
constexpr uint32 TOP_ROWS = 1;
constexpr uint32 HIDDEN_ROWS = (BOTTOM_ROWS + TOP_ROWS);
constexpr uint32 ROWS = (VISIBLE_ROWS + HIDDEN_ROWS);
constexpr uint32 MIN_COLUMNS = 8;
constexpr uint32 MAX_COLUMNS = 16;

template<class __Ty>
using RawBubbleBoard = std::vector<std::vector<__Ty>>;

template<class _Ty>
constexpr bool check_paired_column(_Ty value) { return (value % 2) != 0 ? value - 1 : value; }


class BinaryBubbleBoard
{
private:
	DynMatrix2<bubble_code_t> _board;
	const uint32 _columns;

public:
	BinaryBubbleBoard(const column_t& columns);
	~BinaryBubbleBoard();

	inline uint32 getColumns() const { return _columns; }
	__forceinline uint32 getColumns(const uint32& row) const { return check_paired_column(_columns); }

	void insertBubble(size_t row, size_t column, const bubble_code_t& code);

	bubble_code_t getBubble(const size_t& row, const size_t& column) const;
};


#define BUBGEN_MAX static_cast<uint16>(1000)
#define BUBGEN_MIN static_cast<uint16>(0)
class RandomBubbleTypeGenerator
{
private:
	std::vector<std::pair<const BubbleModel*, uint16>> _percents;
	uint32 _count;

public:
	RandomBubbleTypeGenerator() : _percents(), _count(0) {}

	const BubbleModel* generate(RNG& rand);
	void setPercent(const BubbleModel* model, uint16 percent);
	uint16 getPercent(const BubbleModel* model) const;
};

class MetaScenarioGoals
{
private:

};


class ScenarioProperties
{
private:
	uint32 _columns = MIN_COLUMNS;
	PlayerId _playerid = PlayerId::Single;
	std::vector<BinaryBubbleBoard> _bubbles;
	HideBubblesContainerType _hideType;
	uint32 _clearBoardsRequired = 0U;
	color_mask_t _availableColors = 0xFFU;
	unsigned int _seed = 0;
	uint32 _initialBubbles = 0;
	bool _generateUpBubbles = false;
	RandomBubbleTypeGenerator _arrowBubbleTypes;
	RandomBubbleTypeGenerator _boardBubbleTypes;
	bool _roof = false;
	bool _remove = false;
	bool _enableTimer = true;
	bool _hideTimmer;
	uint32 _timerTurnTime = 10;
	uint32 _timer_endTime = 90;
	TimerMode _timerMode = TimerMode::TURN;
	bool _enableBubbleSwap = true;
	Texture* _background = nullptr;
	std::string _music = "";


};

