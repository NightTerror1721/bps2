#include "arrow.h"

#include <exception>

#define MAX_SPEED 120.f
#define MIN_SPEED 15.f
#define MAX_DEGREES 84.f
#define MIN_DEGREES -MAX_DEGREES

#define SPEED(delta) (this->_slowly ? MIN_SPEED : MAX_SPEED)


Arrow::Arrow(Scenario* const& sc, TextureManager* const& tm, const ScenarioProperties& props) :
	_sc{ sc },
	_bounds{},
	_base{},
	_player{ props.getPlayerId() },
	_slowly{ false },
	_doFire{ false },
	_lockFire{ false },
	_lock{ false },
	_degrees{ 0 },
	_moveTimes{ 0 },
	_currentBubble{ nullptr },
	_nextBubble{ nullptr },
{}

void Arrow::startBubbles();

void Arrow::unlockFire();

bool Arrow::isFireLocked() const;
bool Arrow::isLocked() const;

const sf::FloatRect& Arrow::getBaseBounds() const;

void Arrow::fireBubble();

bool Arrow::isBubbleFired() const;

void Arrow::draw(sf::RenderTarget* const& g);
void Arrow::update(const delta_t& delta);
void Arrow::dispatchEvent(const InputEvent& event);

void Arrow::generateNext()
{
	_currentBubble = _nextBubble;
	if (_currentBubble)
	{
		_currentBubble->setPosition(_bounds.left + (_bounds.width / 2.f), _bounds.top + (_bounds.height / 2.f));
		//_nextBubble = _bgen.generate(true);
		//_nextBubble->set
	}
}

void Arrow::swapBubble();

const InputMask& Arrow::mask(const u8& maskId)
{
	switch (maskId)
	{
		case ArrowLeft: return _player == PlayerId::Player_One ? InputMask::P1_Left : InputMask::P2_Left;
		case ArrowRight: return _player == PlayerId::Player_One ? InputMask::P1_Right : InputMask::P1_Right;
		case ArrowUp: return _player == PlayerId::Player_One ? InputMask::P1_Up : InputMask::P2_Up;
		case Fire: return _player == PlayerId::Player_One ? InputMask::P1_Fire : InputMask::P2_Fire;
		case Slow: return _player == PlayerId::Player_One ? InputMask::P1_Slow : InputMask::P2_Slow;
		case Change: return _player == PlayerId::Player_One ? InputMask::P1_Swap : InputMask::P2_Swap;
		default: throw std::exception{ "Invalid maskId" };
	}
}
