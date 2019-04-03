#pragma once

#include "engine.h"
#include "assets.h"

#include <string>
#include <functional>
#include <map>

#define BUBBLE_WIDTH 30
#define BUBBLE_HEIGHT 26
#define BUBBLE_RADIUS 15

class Bubble;

using bubble_code_t = std::string;
using color_mask_t = uint8;

class BubbleColor
{
private:
	uint8 _id;

private:
	explicit inline BubbleColor(const uint8 id) : _id(id) {}

public:
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

	std::function<void(Bubble*)> init;
};








class Bubble : public PhysicalEntity
{
private:
	const TextureManager* _texs;
	Vec2f _allocScenario;
	Vec2i _allocCell;
	bool _floatingCheckPhase;
	Vec2f _speed;
	Vec2f _acceleration;

	BubbleColor _bcolor;
	uint8 _resistence;

	AnimatedSprite _sprite;

	bool _exploited;

	bubble_code_t _code;

protected:
	Bubble(const std::string& tag, TextureManager* texs);

public:
	~Bubble();

	inline void setCode(const bubble_code_t& code) { _code = code; }
	inline bubble_code_t getCode() { return _code; }

	void draw(sf::RenderTarget *const (&g));
	//void update(delta_t delta);


};




class BubbleManager : singleton
{
private:
	std::map<const std::string, BubbleModel> _models;
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


/*Color Bubble*/
/*class ColorBubble : public Bubble
{
public:
	inline ColorBubble(TextureManager* texs, BubbleColor color) : Bubble("ColorBubble", texs) { _bcolor = color; }
};*/

