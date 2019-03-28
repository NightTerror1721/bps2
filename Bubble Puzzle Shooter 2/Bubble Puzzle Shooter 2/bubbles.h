#pragma once

#include "engine.h"
#include "assets.h"

#include <string>

#define BUBBLE_WIDTH 30
#define BUBBLE_HEIGHT 26
#define BUBBLE_RADIUS 15

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
};

class Bubble : public PhysicalEntity
{
private:
	bool _exploited;

protected:
	const TextureManager* _texs;
	Vec2f _allocScenario;
	Vec2i _allocCell;
	bool _floatingCheckPhase;
	Vec2f _speed;
	Vec2f _acceleration;

	BubbleColor _bcolor;
	uint8 _resistence;

protected:
	Bubble(const std::string& tag, TextureManager* texs);

public:
	~Bubble();

	virtual void draw(sf::RenderTarget *const (&g));
};



/*Color Bubble*/
class ColorBubble : public Bubble
{
public:
	inline ColorBubble(TextureManager* texs, BubbleColor color) : Bubble("ColorBubble", texs) { _bcolor = color; }
};
