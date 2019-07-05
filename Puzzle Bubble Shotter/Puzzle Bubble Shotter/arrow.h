#pragma once

#include "scenario_props.h"
#include "inputs.h"

class Scenario;
class TextureManager;
class Arrow;

class ArrowBase
{
private:
	sf::Sprite _arrow;
	sf::Sprite _base;
	sf::Sprite _base2;
	sf::Sprite _gears[4];

	sf::FloatRect _bbase;
	sf::Image _canvas;

public:
	ArrowBase();

	void update(const delta_t& delta);
	void drawArrow(sf::RenderTarget* const& g);
	void drawBase(sf::RenderTarget* const& g);

	friend class Arrow;
};

class Arrow
{
private:
	Scenario* const _sc;

	sf::FloatRect _bounds;

	ArrowBase _base;

	PlayerId _player;

	bool _slowly;
	bool _doFire;
	bool _lockFire;
	bool _lock;
	float _degrees;

	u32 _moveTimes;

	Ptr<Bubble> _currentBubble;
	Ptr<Bubble> _nextBubble;

public:
	Arrow(Scenario* const& sc, TextureManager* const& tm, const ScenarioProperties& props);

	void startBubbles();

	void unlockFire();

	bool isFireLocked() const;
	bool isLocked() const;

	void unlockArrow();

	const sf::FloatRect& getBaseBounds() const;

	void fireBubble();

	bool isBubbleFired() const;

	void draw(sf::RenderTarget* const& g);
	void update(const delta_t& delta);
	void dispatchEvent(const InputEvent& event);

private:
	void generateNext();
	void swapBubble();



private:
	enum : u8
	{
		ArrowLeft,
		ArrowRight,
		ArrowUp,
		Fire,
		Slow,
		Change
	};

	InputMask mask(const u8& maskId);
};
