#pragma once

#include "utils.h"
#include "rng.h"
#include "bubbles.h"

#include <vector>
#include <deque>
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

inline uint8 operator& (const HideBubblesContainerType& t1, const HideBubblesContainerType& t2) { return static_cast<uint8>(t1) & static_cast<uint8>(t2); }

inline bool IsFinalState(const ScenarioState& state) { return state == ScenarioState::Won || state == ScenarioState::Lost; }
__forceinline bool IsRunningState(const ScenarioState& state) { return !IsFinalState(state); }
inline bool IsActiveRunningState(const ScenarioState& state) { return state == ScenarioState::InGame; }
inline bool IsPasiveRunningState(const ScenarioState& state) { return state == ScenarioState::Winning || state == ScenarioState::Losing || state == ScenarioState::Paused; }
inline bool IsFinalizingState(const ScenarioState& state) { return state == ScenarioState::Winning || state == ScenarioState::Losing; }


inline bool IsDiscrete(HideBubblesContainerType type) { return (type & HideBubblesContainerType::Discrete) == static_cast<uint8>(HideBubblesContainerType::Discrete); }
inline bool IsRandom(HideBubblesContainerType type) { return (type & HideBubblesContainerType::Random_Continuous) == static_cast<uint8>(HideBubblesContainerType::Random_Continuous); }
inline bool IsEndless(HideBubblesContainerType type) { return (type & HideBubblesContainerType::Endless_Continuous) == static_cast<uint8>(HideBubblesContainerType::Endless_Continuous); }


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

constexpr uint8  MAX_TIMER_TURN_TIME = 99;
constexpr uint16 MAX_TIMER_END_TIME  = 60 * 100 - 1;

template<class __Ty>
using RawBubbleBoard = std::vector<std::vector<__Ty>>;

template<class _Ty>
constexpr bool check_paired_column(_Ty value) { return (value % 2) != 0 ? value - 1 : value; }


class BinaryBubbleBoard
{
private:
	DynMatrix2<BubbleIdentifier> _board;
	const uint32 _columns;

public:
	BinaryBubbleBoard(const column_t& columns);
	~BinaryBubbleBoard();

	inline uint32 getColumns() const { return _columns; }
	__forceinline uint32 getColumns(const uint32& row) const { return check_paired_column(_columns); }

	void insertBubble(size_t row, size_t column, const BubbleIdentifier& bid);
	inline void insertBubble(size_t row, size_t column, const std::string& model, const BubbleColor& color)
	{
		insertBubble(row, column, {model, color});
	}

	BubbleIdentifier getBubble(const size_t& row, const size_t& column) const;
};


#define BUBGEN_MAX static_cast<uint16>(1000)
#define BUBGEN_MIN static_cast<uint16>(0)
class RandomBubbleTypeGenerator
{
private:
	std::vector<std::pair<std::string, uint16>> _percents;
	uint32 _count;

public:
	RandomBubbleTypeGenerator() : _percents(), _count(0) {}

	std::string generate(RNG& rand);
	void setPercent(const std::string& modelName, uint16 percent);
	uint16 getPercent(const std::string& modelName) const;
};

class MetaScenarioGoals
{
private:
	std::map<BubbleIdentifier, uint32> _bubgoals;
	bool _boardEmpty;
	uint32 _boardCount;

public:
	MetaScenarioGoals() : _bubgoals(), _boardEmpty(false), _boardCount(0) {}

	inline void setBoardEmpty(bool flag) { _boardEmpty = flag; }
	inline bool getBoardEmpty() { return _boardEmpty; }

	inline void setBoardEmptyCount(const uint32& count) { _boardCount = count; }
	inline uint32 getBoardEmptyCount() { return _boardCount; }

	inline size_t getBubbleGoalsCount() { return _bubgoals.size(); }

	void setBubbleGoal(const BubbleIdentifier& bid, uint32 amount);
	uint32 getBubbleGoal(const BubbleIdentifier& bid);
	inline uint32 getBubbleGoal(const std::string& model, const BubbleColor& color) { return getBubbleGoal({ model, color }); }

	void forEachBubbleGoal(std::function<void(const BubbleIdentifier&, const uint32&, size_t)> action);
};


class ScenarioProperties
{
private:
	uint32 _columns = MIN_COLUMNS;
	PlayerId _playerid = PlayerId::Single;
	std::vector<BinaryBubbleBoard> _bubbles;
	HideBubblesContainerType _hideType = HideBubblesContainerType::Continuous;
	uint32 _clearBoardsRequired = 0U;
	color_mask_t _availableColors = 0xFFU;
	seed_t _seed = 0;
	uint32 _initialBubbles = 0;
	bool _generateUpBubbles = false;
	RandomBubbleTypeGenerator _arrowBubbleTypes;
	RandomBubbleTypeGenerator _boardBubbleTypes;
	bool _roof = false;
	bool _remote = false;
	bool _enableTimer = true;
	bool _hideTimer = true;
	uint32 _timerTurnTime = 10;
	uint32 _timerEndTime = 90;
	TimerMode _timerMode = TimerMode::TURN;
	bool _enableBubbleSwap = true;
	Texture* _background = nullptr;
	std::string _music = "";
	MetaScenarioGoals _goals;

public:
	ScenarioProperties();
	~ScenarioProperties();

	inline void setColumns(column_t columns) { _columns = col_cast(columns); }
	inline column_t getColumns() const { return col_cast(_columns); }

	inline void setPlayerId(PlayerId id) { _playerid = id; }
	inline PlayerId getPlayerId() const { return _playerid; }

	void setBubbleBoardCount(uint8 count);
	inline uint8 getBubbleBoardCount() const { return static_cast<uint8>(_bubbles.size()); }
	inline BinaryBubbleBoard* getBubbleBoard(uint8 idx) { return &_bubbles[idx]; }

	inline void setHideBubblesContainerType(const HideBubblesContainerType& type) { _hideType = type; }
	inline HideBubblesContainerType getHideBubblesContainerType() const { return _hideType; }

	inline void setClearBoardsRequired(const uint32& amount) { _clearBoardsRequired = amount; }
	inline uint32 getClearBoardsRequired() const { return _clearBoardsRequired; }

	inline void setEnabledColor(const BubbleColor& color, bool enabled) { _availableColors = enabled ? _availableColors - color : _availableColors + color; }
	inline bool isEnablledColor(const BubbleColor& color) const { return _availableColors & color; }
	inline color_mask_t getEnabledColors() const { return _availableColors; }

	inline void setSeed(seed_t seed) { _seed = seed; }
	inline bool isRandomSeed() const { return _seed == 0; }
	inline seed_t getSeed() const { return _seed; }

	inline void setInitialFilledRows(uint32 count) { _initialBubbles = std::max(count, VISIBLE_ROWS); }
	inline uint32 getInitialFilledRows() const { return _initialBubbles; }

	inline void setEnabledGenerateUpBubbles(bool flag) { _generateUpBubbles = flag; }
	inline bool isEnabledGenerateUpBubbles() const { return _generateUpBubbles; }

	inline RandomBubbleTypeGenerator* getArrowBubbleTypes() { return &_arrowBubbleTypes; }
	inline RandomBubbleTypeGenerator* getBoardBubbleTypes() { return &_boardBubbleTypes; }

	inline void setEnabledRoof(bool enabled) { _roof = enabled; }
	inline bool isEnabledRoof() const { return _roof; }

	inline void setEnabledRemoteBubbles(bool enabled) { _remote = enabled; }
	inline bool isEnabledRemoteBubbles() const { return _remote; }

	inline void setEnabledTimer(bool enabled) { _enableTimer = enabled; }
	inline bool isEnabledTimer() const { return _enableTimer; }

	inline void setHideTimer(bool enabled) { _hideTimer = enabled; }
	inline bool isHideTimer() const { return _hideTimer; }

	inline void setTimerTurnTime(uint8 seconds) { _timerTurnTime = std::max(seconds, MAX_TIMER_TURN_TIME); }
	inline uint8 getTimerTurnTime() const { return _timerTurnTime; }

	inline void setTimerEndTime(uint16 seconds) { _timerEndTime = std::max(seconds, MAX_TIMER_END_TIME); }
	inline uint16 getTimerEndTurn() const { return _timerEndTime; }

	inline void setTimerMode(const TimerMode& mode) { _timerMode = mode; }
	inline TimerMode getTimerMode() const { return _timerMode; }

	inline void setBackground(Texture* background) { _background = background; }
	inline Texture* getBackground() const { return _background; }

	inline MetaScenarioGoals* getGoals() { return &_goals; }

};


class Scenario;
class BubbleBoard;
class BoardRow;
class Boardcell;

class BubbleColorRandomizer
{
private:
	RNG _rand;
	color_mask_t _colors;
	std::vector<BubbleColor> _map;

public:
	explicit BubbleColorRandomizer(const RNG& rand);

	void setAvailableColors(const color_mask_t& colors);

	void setColorEnabled(const BubbleColor& color, bool enabled);

	BubbleColor generateRandom();
	BubbleColor generateRandom(BubbleBoard* board);

	inline bool isColorEnabled(const BubbleColor& color) const { return _colors & color; }

private:
	void updateMap();
};


class BubbleGenerator
{
private:
	Scenario *const _sc;
	BubbleColorRandomizer* _colors;
	RNG _arrowRand;
	RNG _boardRand;
	RandomBubbleTypeGenerator* _arrowTypes;
	RandomBubbleTypeGenerator* _boardTypes;
	BubbleColor _lastColor;

public:
	explicit BubbleGenerator(
		Scenario *const scenario,
		BubbleColorRandomizer* const colors,
		RandomBubbleTypeGenerator* const arrowTypes,
		RandomBubbleTypeGenerator* const boardTypes,
		seed_t seed);

	inline Bubble* generate(TextureManager* textureManager, bool isArrow) { return generate(typegen(isArrow)->generate(rand(isArrow)), textureManager, isArrow); }

	inline RandomBubbleTypeGenerator* arrowTypeGenerator() { return _arrowTypes; }
	inline RandomBubbleTypeGenerator* boardTypeGenerator() { return _boardTypes; }

	Bubble* generateFromIdentifier(const BubbleIdentifier& bid, TextureManager* textureManager);

private:
	__forceinline RandomBubbleTypeGenerator* typegen(bool isArrow) { return isArrow ? _arrowTypes : _boardTypes; }
	__forceinline RNG& rand(bool isArrow) { return isArrow ? _arrowRand : _boardRand; }

	Bubble* generate(const std::string& modelName, TextureManager* textureManager, bool isArrow);
	BubbleColor selectColor(bool isArrow);
	
};






class HideBubbleContainer
{
private:
	class HiddenRow
	{
	private:
		HideBubbleContainer* const __parent;
		std::vector<BubbleIdentifier> _ids;
		int8 _count;
		uint8 _boardId;
		uint8 _row;

	public:
		inline HiddenRow(HideBubbleContainer* const parent, std::vector<BubbleIdentifier> ids, uint8 boardId, uint8 row) : __parent(parent), _ids(ids), _boardId(boardId), _row(row) {}
		inline HiddenRow(HideBubbleContainer* const parent, uint8 boardId, uint8 row) : HiddenRow(parent, {}, boardId, row) {}

		inline bool empty() { return _ids.empty(); };
		inline std::vector<BubbleIdentifier> getIdentifiers() { return _ids; }
		uint8 count();

		std::vector<Bubble*> generateRow(TextureManager* textureManager);
	};

	class HiddenBoard
	{
	private:
		HideBubbleContainer *const __parent;
		std::deque<HiddenRow> _rows;
		uint32 _count;
		bool _modified;

	public:
		inline HiddenBoard(HideBubbleContainer *const parent) : __parent(parent), _rows(), _count(0), _modified(true) {}
		HiddenBoard(const HiddenBoard& base);

		inline bool empty() const { return _rows.empty(); }
		inline HiddenBoard* copyPtr() { return new HiddenBoard(*this); }

		void createRow(const std::vector<BubbleIdentifier>& ids, uint8 row, uint8 boardId);
		inline void createEmptyRow(uint8 row, uint8 boardId) { createRow({}, row, boardId); }

		void trimTop();

		std::vector<Bubble*> generateRow(TextureManager* textureManager);

		std::vector<std::vector<Bubble*>> generateAllRows(TextureManager* textureManager);

		uint32 count();
	};



private:
	using bubble_row_t = std::vector<Bubble*>;
	using bubble_board_t = std::vector<bubble_row_t>;

	const HideBubblesContainerType _type;
	TextureManager* const _tm;
	uint8 _columns;
	BubbleGenerator* const _bgen;
	RNG _rand;
	uint8 _maxBoards;
	std::deque<HiddenBoard*> _boards;
	HiddenBoard* _current;

public:
	HideBubbleContainer(ScenarioProperties *const props, column_t columns, BubbleGenerator *const bgen, seed_t seed, TextureManager* const textureManager);
	~HideBubbleContainer();

	void fill(const std::vector<BinaryBubbleBoard>& bin);

	bubble_board_t generate();

	bubble_board_t generateBoard();

	bubble_row_t generateRow();

	uint32 getBubbleCount();


	inline bool isDiscrete() const { return IsDiscrete(_type); }
	inline bool empty() const { return _boards.empty() && (!_current || _current->empty()); }

private:
	void checkNext();

	void createHiddenRow(HiddenBoard* board, const BinaryBubbleBoard& bbb, uint8 row, uint8 boardId);

};





class BoardRow
{
private:
	BubbleBoard* const _parent;
	uint8 _little;
	std::vector<Bubble*> _bubs;

	BoardRow(BubbleBoard* const& parent, int downs);

public:
	~BoardRow();

	Bubble* getBubble(uint8 column) const;

	bool containsBubble(uint8 column) const;

	void forEachBubble(void (*action)(Bubble*));

	friend class BubbleBoard;
	friend class BoardCell;

private:
	void setBubble(int32 row, uint8 column, Bubble* b);
	void setBubble(const BoardCell& cell, Bubble* b);

	Bubble* removeBubble(uint8 column);

	uint32 destroy();

	void down();

	uint32 count();

	bool hasInvalidBottomBubble();
};



class BubbleBoard : public sf::Transformable
{
private:
	Scenario* const _scenario;
	uint8 _columns;
	uint32 _size;

public:


	friend class BoardRow;
	friend class BoardCell;
};

