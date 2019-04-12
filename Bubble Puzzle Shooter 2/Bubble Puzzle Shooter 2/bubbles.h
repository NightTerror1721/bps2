#pragma once

#include "assets.h"

#include "audio.h"

#include <string>
#include <functional>
#include <map>
#include <set>

#define BUBBLE_WIDTH 30
#define BUBBLE_HEIGHT 26
#define BUBBLE_RADIUS 15

class Bubble;
class BubbleManager;

class Scenario;

class BubbleBoard;
class BoardRow;

using color_mask_t = uint8;

class BubbleColor
{
private:
	uint8 _id;

private:
	explicit inline BubbleColor(const uint8 id) : _id(id) {}
	

public:
	inline BubbleColor() : BubbleColor(BubbleColor::Colorless._id) {}

	static const BubbleColor Red;
	static const BubbleColor Orange;
	static const BubbleColor Yellow;
	static const BubbleColor Green;
	static const BubbleColor Blue;
	static const BubbleColor Purple;
	static const BubbleColor Gray;
	static const BubbleColor Black;
	static const BubbleColor Multicolor;
	static const BubbleColor Colorless;

	static std::vector<BubbleColor> normalColors();

	bool match(const BubbleColor& other) const;

	inline bool operator== (const BubbleColor& bc) const { return _id == bc._id; }
	inline bool operator!= (const BubbleColor& bc) const { return _id != bc._id; }
	inline bool operator> (const BubbleColor& bc) const { return _id > bc._id; }
	inline bool operator>= (const BubbleColor& bc) const { return _id >= bc._id; }
	inline bool operator< (const BubbleColor& bc) const { return _id < bc._id; }
	inline bool operator<= (const BubbleColor& bc) const { return _id <= bc._id; }
	inline bool operator! () const { return _id == BubbleColor::Colorless._id; }

	inline uint8 id() const { return _id; }

	int8 singleColorCode() const;

	inline bool isNormalColor() const { return _id < 8; }
	inline bool isMulticolor() const { return _id == BubbleColor::Multicolor._id; }
	inline bool isColorless() const { return _id == BubbleColor::Colorless._id; }

	std::string name() const;

	inline color_mask_t addToMask(const color_mask_t& mask) const { return isNormalColor() ? mask | (0x1 << _id) : mask; }
	inline color_mask_t removeToMask(const color_mask_t& mask) const { return isNormalColor() ? mask & ~(0x1 << _id) : mask; }
	inline bool hasInMask(const color_mask_t& mask) const { return isNormalColor() && (mask & (0x1 << _id)) != 0; }
};

color_mask_t operator+ (const color_mask_t& mask, const BubbleColor& color);
color_mask_t operator- (const color_mask_t& mask, const BubbleColor& color);
bool operator& (const color_mask_t& mask, const BubbleColor& color);



struct BubbleModel
{
	std::string name;

	std::function<void(Bubble*, Bubble*)> onCollide;
	std::function<void(Bubble*)> onInserted;
	std::function<void(Bubble*)> onExplode;
	std::function<void(Bubble*, Bubble*)> onNeighborInserted;
	std::function<void(Bubble*, Bubble*)> onNeighborExplode;

	std::function<void(Bubble*, uint8, bool)> init;

	/* Properties */
	bool requireColor;
	bool floating = false;
	bool destroyInBottom = false;
	bool requireDestroyToClear = true;

	int8 resistence;
	float pointsOfTurnsToDown;
};





enum class BounceEdge { None, Top, Bottom, Left, Right };

class BouncingBounds
{
private:
	Bubble* const _bubble;
	sf::IntRect _bounds;
	bool _top;
	bool _bottom;

public:
	BouncingBounds(Bubble* const bubble);

	BounceEdge checkBounce();

	inline void setBounds(const sf::IntRect& bounds) { _bounds = bounds; }
	inline void setTop(const bool& top) { _top = top; }
	inline void setBottom(const bool& bottom) { _bottom = bottom; }
};





class BoardCell
{
private:
	BoardRow* _parent;
	const uint8 _column;
	int32 _row;

	BoardCell(BoardRow* const& parent, int32 row, uint8 column);

public:
	inline BoardCell() : BoardCell(nullptr, 0, 0) {}

	BoardRow* getBoardRow() const;
	BubbleBoard* getBoard() const;
	Scenario* getScenario() const;
	uint8 getLittle() const;
	int32 getRow() const;
	uint8 getColumn() const;
	bool isInBottomRow() const;

	bool operator! () const;

	friend class BubbleBoard;
	friend class BoardRow;
};



struct BubbleIdentifier;

class Bubble : public Unique<>, public LocalAttrAllocator, public sf::Transformable
{
private:
	BubbleModel *const _model;

	const TextureManager* _texs;
	Scenario* _allocScenario;
	Vec2f _allocPosition;
	Vec2i _allocCell;
	bool _floatingCheckPhase;
	Vec2f _speed;
	Vec2f _acceleration;

	BubbleColor _bcolor;

	AnimatedSprite _sprite;

	BouncingBounds _bounce;

	BoardCell _cell;

	sf::Sound* _arrowFireSound;

	bool _exploited;

	Bubble(BubbleModel *const modelName, TextureManager* texs);

public:
	~Bubble();

	void setSpeed(const Vec2f& speed);
	const Vec2f& getSpeed() const;

	void setAcceleration(const Vec2f& acceleration);
	const Vec2f& getAcceleration() const;

	inline bool isExploited() const { return _exploited; }

	inline const BubbleColor& getColor() const { return _bcolor; }

	inline int8 getResistence() const { return _model->resistence < 0 ? 127 : _model->resistence; }
	inline bool isIndestructible() const { return _model->resistence < 0; }

	inline void translate(float dx, float dy) { translate({ dx, dy }); }
	inline void move(float speedx, float speedy, float accelerationx, float accelerationy) { move({ speedx, speedy }, { accelerationx, accelerationy }); }
	inline void move(float speedx, float speedy) { move({ speedx, speedy }); }
	inline void stop() { move({ 0, 0 }, { 0, 0 }); }
	inline void fall() { move({ 0, 120.f }, { 0, 45.f }); }

	inline bool isMoving() const { return !(!_speed && !_acceleration); }

	inline const BoardCell& getBoardCell() const { return _cell; }
	inline BoardCell& boardCell() { return _cell; }
	inline bool containsBoardCell() const { return !(!_cell); }

	inline bool isFloating() const { return _model->floating; }
	inline bool destroyInBottom() const { return _model->destroyInBottom; }
	inline bool requireDestroyToClear() const { return _model->requireDestroyToClear; }

	inline float getPointsOfTurnsToDown() const { return _model->pointsOfTurnsToDown; }

	inline bool isAllocatingPosition() const { return _allocScenario != nullptr; }


	BubbleIdentifier getIdentifier() const;


	void translate(const Vec2f& dp);
	void move(const Vec2f& speed, const Vec2f& acceleration);
	void move(const Vec2f& speed);

	void setBounce(const sf::IntRect& bounds, bool top, bool bottom);

	bool colorMatch(Bubble* other) const;

	void setBoardCell(const BoardCell& cell);
	void removeBoardCell();

	void fire(Scenario* scenario, float degrees, sf::Sound* fireSound);

	void stopFireSound();

	void explode(Scenario* scenario);

	void destroy();

	void draw(sf::RenderTarget *const (&g));
	void draw(sf::RenderTarget *const (&g), float dx);

	void update(delta_t delta);

	void collideWithNeighbors(BubbleBoard* board);

	void reallocatePosition(std::vector<Bubble*> neighbors, delta_t delta);

	void allocate(Scenario* scenario);


	void callOnCollide(Scenario* scenario, Bubble* other);


	

	friend Bubble* CreateNewBubble(BubbleModel* model, const BubbleColor& color, TextureManager* textureManager, bool editorMode);

private:
	void draw(sf::RenderTarget *const (&g), float x, float y);

	bool findAllocationPosition(const Vec2i& pcell, Scenario* scenario, BubbleBoard* board, std::set<Vec2i>& visited);

	void updateAllocation(delta_t delta);
};



struct BubbleIdentifier
{
	std::string model;
	BubbleColor color;

	static BubbleIdentifier invalid();

	bool operator== (const BubbleIdentifier& bi2) const;
	bool operator!= (const BubbleIdentifier& bi2) const;
	bool operator> (const BubbleIdentifier& bi2) const;
	bool operator>= (const BubbleIdentifier& bi2) const;
	bool operator< (const BubbleIdentifier& bi2) const;
	bool operator<= (const BubbleIdentifier& bi2) const;
	bool operator! () const;

	operator bool() const;

	bool isValid() const;
	void invalidate();

	Bubble* createBubble(TextureManager* textureManager, bool editorMode) const;
};




class BubbleManager : singleton
{
private:
	std::map<std::string, BubbleModel> _models;
	BubbleModel* _default;

	static BubbleManager _instance;

	inline BubbleManager() : _models() {}

public:
	~BubbleManager();

	BubbleModel* registerBubbleModel(const std::string& name);

	void setDefaultModel(const std::string& name);

	inline BubbleModel* getBubbleModel(const std::string& name) { return &_models[name]; }

	inline BubbleModel* getDefaultModel() { return _default; }

	inline bool hasBubbleModel(const std::string& name) const { return _models.find(name) != _models.cend(); }

	void deleteBubbleModel(const std::string& name);

	void clear();

	friend BubbleManager& GetBubbleManager();
};

__forceinline BubbleManager& GetBubbleManager() { return BubbleManager::_instance; }

#define BUBBLE_MANAGER GetBubbleManager()
#define bubman_registerBubbleModel(name) BUBBLE_MANAGER.registerBubbleModel((name))
#define bubman_setDefaultModel(name) BUBBLE_MANAGER.setDefaultModel((name))
#define bubman_getBubbleModel(name) BUBBLE_MANAGER.getBubbleModel((name))
#define bubman_getDefaultModel() BUBBLE_MANAGER.getDefaultModel()
#define bubman_hasBubbleModel(name) BUBBLE_MANAGER.hasBubbleModel((name))
#define bubman_deleteBubbleModel(name) BUBBLE_MANAGER.deleteBubbleModel((name))
#define bubman_clear() BUBBLE_MANAGER.clear()

Bubble* CreateNewBubble(const std::string& model, const BubbleColor& color, TextureManager* textureManager, bool editorMode);
Bubble* CreateNewBubble(BubbleModel* model, const BubbleColor& color, TextureManager* textureManager, bool editorMode);
void DestroyBubble(const Bubble* bub);



/*Color Bubble*/
/*class ColorBubble : public Bubble
{
public:
	inline ColorBubble(TextureManager* texs, BubbleColor color) : Bubble("ColorBubble", texs) { _bcolor = color; }
};*/

