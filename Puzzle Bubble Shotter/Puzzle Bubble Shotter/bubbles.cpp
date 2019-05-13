#include "bubbles.h"

const BubbleColor BubbleColor::Red{ 0x1 << 0 };
const BubbleColor BubbleColor::Orange{ 0x1 << 1 };
const BubbleColor BubbleColor::Yellow{ 0x1 << 2 };
const BubbleColor BubbleColor::Green{ 0x1 << 3 };
const BubbleColor BubbleColor::Blue{ 0x1 << 4 };
const BubbleColor BubbleColor::Purple{ 0x1 << 5 };
const BubbleColor BubbleColor::Gray{ 0x1 << 6 };
const BubbleColor BubbleColor::Black{ 0x1 << 7 };

BubbleColor::BubbleColor(const u8& id) :
	_id(id)
{}

bool BubbleColor::operator== (const BubbleColor& color) const { return _id == color._id; }
bool BubbleColor::operator!= (const BubbleColor& color) const { return _id != color._id; }
bool BubbleColor::operator> (const BubbleColor& color) const { return _id > color._id; }
bool BubbleColor::operator< (const BubbleColor& color) const { return _id < color._id; }
bool BubbleColor::operator>= (const BubbleColor& color) const { return _id >= color._id; }
bool BubbleColor::operator<= (const BubbleColor& color) const { return _id <= color._id; }

u8 BubbleColor::id() const { return _id; }

std::string BubbleColor::name() const
{
	switch (_id)
	{
		case 0: return "red";
		case 1: return "orange";
		case 2: return "yellow";
		case 3: return "green";
		case 4: return "blue";
		case 5: return "purple";
		case 6: return "gray";
		case 7: return "black";
		default: return "<unknown_color>";
	}
}

colormask_t BubbleColor::addToMask(const colormask_t& mask) const { return mask | _id; }
colormask_t BubbleColor::removeFromMask(const colormask_t& mask) const { return mask & ~_id; }
bool BubbleColor::hasInMask(const colormask_t& mask) const { return mask & _id; }

std::vector<BubbleColor> BubbleColor::all()
{
	return {
		BubbleColor::Red,
		BubbleColor::Orange,
		BubbleColor::Yellow,
		BubbleColor::Green,
		BubbleColor::Blue,
		BubbleColor::Purple,
		BubbleColor::Gray,
		BubbleColor::Black
	};
}

const BubbleColor& BubbleColor::defaultColor() { return BubbleColor::Blue; }


colormask_t operator+ (const colormask_t& mask, const BubbleColor& color) { return mask | color._id; }
colormask_t operator- (const colormask_t& mask, const BubbleColor& color) { return mask & ~color._id; }
bool operator& (const colormask_t& mask, const BubbleColor& color) { return mask & color._id; }



bool operator== (const BubbleModel& bm0, const BubbleModel& bm1) { return bm0.name == bm1.name; }
bool operator!= (const BubbleModel& bm0, const BubbleModel& bm1) { return bm0.name == bm1.name; }




BouncingBounds::BouncingBounds(Bubble* const& bubble) :
	_bubble(bubble),
	_bounds(),
	_top(false),
	_bottom(false)
{}

void BouncingBounds::setBounds(const sf::IntRect& bounds) { _bounds = bounds; }
void BouncingBounds::setTopEnabled(const bool& flag) { _top = flag; }
void BouncingBounds::setBottomEnabled(const bool& flag) { _bottom = flag; }

BounceEdge BouncingBounds::check()
{
	vec2f pos = _bubble->getPosition();
	vec2f speed = _bubble->getSpeed();
	if (_top && pos.y - Bubble::Radius <= _bounds.top)
	{
		pos.y = static_cast<float>(_bounds.top + Bubble::Radius);
		speed.y *= -1;
		_bubble->setPosition(pos);
		_bubble->setSpeed(speed);
		return BounceEdge::Top;
	}
	if (_bottom && pos.y + Bubble::Radius >= _bounds.top + _bounds.height)
	{
		pos.y = static_cast<float>(_bounds.top + _bounds.height - Bubble::Radius);
		speed.y *= -1;
		_bubble->setPosition(pos);
		_bubble->setSpeed(speed);
		return BounceEdge::Bottom;
	}
	if (pos.x - Bubble::Radius <= _bounds.left)
	{
		pos.x = static_cast<float>(_bounds.left + Bubble::Radius);
		speed.x *= -1;
		_bubble->setPosition(pos);
		_bubble->setSpeed(speed);
		return BounceEdge::Left;
	}
	if (pos.x + Bubble::Radius >= _bounds.left + _bounds.width)
	{
		pos.x = static_cast<float>(_bounds.left + _bounds.width - Bubble::Radius);
		speed.x *= -1;
		_bubble->setPosition(pos);
		_bubble->setSpeed(speed);
		return BounceEdge::Right;
	}
	return BounceEdge::None;
}






Bubble::Bubble(const Ptr<BubbleModel>& model, TextureManager* texs) :
	_model(model),
	_destroyed(false),
	_exploited(false),
	_speed(),
	_acceleration(),
	_allocPosition(),
	_allocCell(),
	_floatingCheckPhase(false),
	_color(BubbleColor::defaultColor()),
	_sprite(),
	_bounce(this),
	_localInts(model->localInts, 0),
	_localFloats(model->localFloats, 0.f),
	_localStrings(model->localStrings, "")
{}

Bubble::~Bubble() {}

Ptr<BubbleModel> Bubble::getModel() const { return _model; }

bool Bubble::hasDestroyed() const { return _destroyed; }

void Bubble::destroy()
{
	//TODO: Implement
	_destroyed = true;
}

bool Bubble::hasExploited() const { return _destroyed || _exploited; }

void Bubble::explode()
{
	//TODO: Implement
}

void Bubble::setSpeed(const vec2f& speed) { _speed = speed; }
const vec2f& Bubble::getSpeed() const { return _speed; }

void Bubble::setAcceleration(const vec2f& acceleration) { _acceleration = acceleration; }
const vec2f& Bubble::getAcceleration() const { return _acceleration; }

void Bubble::translate(const vec2f& dp) { setPosition(getPosition() + dp); }
void Bubble::translate(const float& dx, const float& dy) { translate({ dx, dy }); }
void Bubble::move(const vec2f& speed, const vec2f& acceleration) { _speed = speed; _acceleration = acceleration; }

BubbleColor Bubble::getColor() const { return _color; }
ColorType Bubble::getColorType() const { return _model->colorType; }

bool Bubble::colorMatch(const Ptr<Bubble>& other) const
{
	switch (_model->colorType)
	{
		case ColorType::Colorless:
			return false;

		case ColorType::NormalColor:
			switch(other->_model->colorType)
			{
				case ColorType::Colorless: return false;
				case ColorType::Multicolor: return true;
				case ColorType::NormalColor: return _color == other->_color;
			}
			
		case ColorType::Multicolor:
			return other->_model->colorType != ColorType::Colorless;
	}
	return false;
}

AnimatedSprite* Bubble::getSprite() { return &_sprite; }
const AnimatedSprite* Bubble::getSprite() const { return &_sprite; }


/* Model functions */
int8 Bubble::getResistence() const { return _model->resistence; }
bool Bubble::isIndestructible() const { return _model->resistence < 0; }
bool Bubble::isFloating() const { return _model->floating; }
bool Bubble::destroyInBottom() const { return _model->destroyInBottom; }
bool Bubble::requireDestroyToClear() const { return _model->requireDestroyToClear; }
float Bubble::getPointsOfTurnsToDown() const { return _model->pointsOfTurnsToDown; }

int32 Bubble::getLocalInt(const u8& index) const { return _localInts[index]; }
float Bubble::getLocalFloat(const u8& index) const { return _localFloats[index]; }
std::string Bubble::getLocalString(const u8& index) const { return _localStrings[index]; }

void Bubble::setLocalInt(const u8& index, const int32& value) { _localInts[index] = value; }
void Bubble::setLocalFloat(const u8& index, const float& value) { _localFloats[index] = value; }
void Bubble::setLocalString(const u8& index, const std::string& value) { _localStrings[index] = value; }







BubbleModelManager::BubbleModelManager() :
	Manager(),
	Singleton()
{}

Ptr<BubbleModel> BubbleModelManager::createNew(const std::string& name)
{
	auto model = create(name);
	model->name = name;
	return model;
}

BubbleModelManager BubbleModelManager::_Instance{};

Ptr<BubbleModel> RegisterBubbleModel(const std::string& name) { return BubbleModelManager::_Instance.createNew(name); }
Ptr<BubbleModel> GetBubbleModel(const std::string& name) { return BubbleModelManager::_Instance.get(name); }
bool HasBubbleModel(const std::string& name) { return BubbleModelManager::_Instance.has(name); }





BubbleHeap::BubbleHeap(BubbleModelManager* const& models) :
	MemoryAllocator(),
	_models(models)
{}

Ptr<Bubble> BubbleHeap::createNew(const std::string& modelName, TextureManager* const& textures, bool editorMode, const BubbleColor& color)
{
	if (!_models->has(modelName))
		return nullptr;

	auto model = _models->get(modelName);
	auto bubble = malloc(model, textures);
	
	model->init(&bubble, color, editorMode);
	return bubble;
}

void BubbleHeap::destroy(Ptr<Bubble> bubble)
{
	free(bubble);
}

void BubbleHeap::clearDestroyeds()
{
	clear([](Bubble& bubble) -> bool { return bubble.hasDestroyed(); });
}
