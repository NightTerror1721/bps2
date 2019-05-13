#pragma once

#include "config.h"
#include "mem_alloc.h"
#include "assets.h"

typedef u8 colormask_t;
class BubbleColor;
class Bubble;
class BubbleHeap;

colormask_t operator+ (const colormask_t& mask, const BubbleColor& color);
colormask_t operator- (const colormask_t& mask, const BubbleColor& color);
bool operator& (const colormask_t& mask, const BubbleColor& color);

class BubbleColor
{
private:
	u8 _id;

public:
	bool operator== (const BubbleColor& color) const;
	bool operator!= (const BubbleColor& color) const;
	bool operator> (const BubbleColor& color) const;
	bool operator< (const BubbleColor& color) const;
	bool operator>= (const BubbleColor& color) const;
	bool operator<= (const BubbleColor& color) const;

	u8 id() const;

	std::string name() const;

	colormask_t addToMask(const colormask_t& mask) const;
	colormask_t removeFromMask(const colormask_t& mask) const;
	bool hasInMask(const colormask_t& mask) const;

	friend colormask_t operator+ (const colormask_t& mask, const BubbleColor& color);
	friend colormask_t operator- (const colormask_t& mask, const BubbleColor& color);
	friend bool operator& (const colormask_t& mask, const BubbleColor& color);

private:
	BubbleColor(const u8& id);

public:
	static const BubbleColor Red;
	static const BubbleColor Orange;
	static const BubbleColor Yellow;
	static const BubbleColor Green;
	static const BubbleColor Blue;
	static const BubbleColor Purple;
	static const BubbleColor Gray;
	static const BubbleColor Black;

	static std::vector<BubbleColor> all();
	static const BubbleColor& defaultColor();
};

enum class ColorType : u8 { Colorless, NormalColor, Multicolor };




struct BubbleModel
{
	/* PROPERTIES */
	std::string name;

	ColorType colorType;

	bool floating;
	bool destroyInBottom;
	bool requireDestroyToClear;

	int8 resistence;

	float pointsOfTurnsToDown;

	u8 localInts;
	u8 localFloats;
	u8 localStrings;


	/* FUNCTIONS */
	std::function<void(Bubble*, BubbleColor, bool)> init;

	std::function<void(Bubble*, Bubble*)> onCollide;
	std::function<void(Bubble*)> onInserted;
	std::function<void(Bubble*)> onExplode;
	std::function<void(Bubble*, Bubble*)> onNeighborInserted;
	std::function<void(Bubble*, Bubble*)> onNeighborExplode;
};

bool operator== (const BubbleModel& bm0, const BubbleModel& bm1);
bool operator!= (const BubbleModel& bm0, const BubbleModel& bm1);




enum class BounceEdge { None, Top, Bottom, Left, Right };

class BouncingBounds
{
private:
	Bubble* const _bubble;
	sf::IntRect _bounds;
	bool _top;
	bool _bottom;

public:
	BouncingBounds(Bubble* const& bubble);

	void setBounds(const sf::IntRect& bounds);
	void setTopEnabled(const bool& flag);
	void setBottomEnabled(const bool& flag);

	BounceEdge check();
};




class Bubble : public UniqueObject, public sf::Transformable
{
	/* STATIC ELEMENTS */
public:
	static constexpr u32 Radius = 32;
	static constexpr u32 HitboxWith = 64;
	static constexpr u32 HitboxHeight = 54;

	/* NORMAL ELEMENTS */
private:
	Ptr<BubbleModel> _model;

	bool _destroyed;
	bool _exploited;

	vec2f _speed;
	vec2f _acceleration;

	vec2f _allocPosition;
	vec2i _allocCell;
	bool _floatingCheckPhase;

	BubbleColor _color;

	AnimatedSprite _sprite;

	BouncingBounds _bounce;

	std::vector<int32> _localInts;
	std::vector<float> _localFloats;
	std::vector<std::string> _localStrings;

public:
	~Bubble();

	Ptr<BubbleModel> getModel() const;

	bool hasDestroyed() const;
	void destroy();

	bool hasExploited() const;
	void explode();

	void setSpeed(const vec2f& speed);
	const vec2f& getSpeed() const;

	void setAcceleration(const vec2f& acceleration);
	const vec2f& getAcceleration() const;

	void translate(const vec2f& dp);
	void translate(const float& dx, const float& dy);
	void move(const vec2f& speed, const vec2f& acceleration = {});

	BubbleColor getColor() const;
	ColorType getColorType() const;

	bool colorMatch(const Ptr<Bubble>& other) const;

	AnimatedSprite* getSprite();
	const AnimatedSprite* getSprite() const;


	/* Model functions */
	int8 getResistence() const;
	bool isIndestructible() const;
	bool isFloating() const;
	bool destroyInBottom() const;
	bool requireDestroyToClear() const;
	float getPointsOfTurnsToDown() const;

	int32 getLocalInt(const u8& index) const;
	float getLocalFloat(const u8& index) const;
	std::string getLocalString(const u8& index) const;

	void setLocalInt(const u8& index, const int32& value);
	void setLocalFloat(const u8& index, const float& value);
	void setLocalString(const u8& index, const std::string& value);

	friend class BubbleHeap;
	ALLOCATOR_FRIEND;

private:
	Bubble(const Ptr<BubbleModel>& model, TextureManager* texs);
};




class BubbleModelManager : public Manager<BubbleModel>, Singleton
{
private:
	BubbleModelManager();

public:
	Ptr<BubbleModel> createNew(const std::string& name);

	friend Ptr<BubbleModel> RegisterBubbleModel(const std::string& name);
	friend Ptr<BubbleModel> GetBubbleModel(const std::string& name);
	friend bool HasBubbleModel(const std::string& name);

private:
	static BubbleModelManager _Instance;
};

Ptr<BubbleModel> RegisterBubbleModel(const std::string& name);
Ptr<BubbleModel> GetBubbleModel(const std::string& name);
bool HasBubbleModel(const std::string& name);




class BubbleHeap : MemoryAllocator<Bubble>
{
private:
	BubbleModelManager* const _models;

public:
	BubbleHeap(BubbleModelManager* const& models);

	Ptr<Bubble> createNew(const std::string& modelName, TextureManager* const& textures, bool editorMode, const BubbleColor& color = BubbleColor::defaultColor());

	void destroy(Ptr<Bubble> bubble);

	void clearDestroyeds();
};

