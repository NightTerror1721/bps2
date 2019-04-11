#include "bubbles.h"

#include <SFML/Graphics.hpp>

#define COLORID_RED 0
#define COLORID_ORANGE 1
#define COLORID_YELLOW 2
#define COLORID_GREEN 3
#define COLORID_BLUE 4
#define COLORID_PURPLE 5
#define COLORID_GRAY 6
#define COLORID_BLACK 7
#define COLORID_MULTICOLOR 8
#define COLORID_COLORLESS 9

BubbleManager BubbleManager::_instance;

const BubbleColor BubbleColor::Red(COLORID_RED);
const BubbleColor BubbleColor::Orange(COLORID_ORANGE);
const BubbleColor BubbleColor::Yellow(COLORID_YELLOW);
const BubbleColor BubbleColor::Green(COLORID_GREEN);
const BubbleColor BubbleColor::Blue(COLORID_BLUE);
const BubbleColor BubbleColor::Purple(COLORID_PURPLE);
const BubbleColor BubbleColor::Gray(COLORID_GRAY);
const BubbleColor BubbleColor::Black(COLORID_BLACK);
const BubbleColor BubbleColor::Multicolor(COLORID_MULTICOLOR);
const BubbleColor BubbleColor::Colorless(COLORID_COLORLESS);

std::vector<BubbleColor> BubbleColor::normalColors()
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

bool BubbleColor::match(const BubbleColor& other) const
{
	switch (_id)
	{
		case COLORID_RED:
		case COLORID_ORANGE:
		case COLORID_YELLOW:
		case COLORID_GREEN:
		case COLORID_BLUE:
		case COLORID_PURPLE:
		case COLORID_GRAY:
		case COLORID_BLACK:
			return _id == other._id || other._id == COLORID_MULTICOLOR;
		case COLORID_MULTICOLOR: return other._id != COLORID_COLORLESS;
		case COLORID_COLORLESS: return false;
	}
	return false;
}

int8 BubbleColor::singleColorCode() const
{
	return isNormalColor() ? 0x1 << _id : -1;
}

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
		case 8: return "multicolor";
		case 9: return "colorless";
		default: return "<unknown_color>";
	}
}

color_mask_t operator+ (const color_mask_t& mask, const BubbleColor& color) { return color.addToMask(mask); }
color_mask_t operator- (const color_mask_t& mask, const BubbleColor& color) { return color.removeToMask(mask); }
bool operator& (const color_mask_t& mask, const BubbleColor& color) { return color.hasInMask(mask); }






Bubble::Bubble(BubbleModel *const model, TextureManager* texs) :
	LocalAttrAllocator(),
	Transformable(),
	_model(model),
	_exploited(false),
	_allocScenario(),
	_allocCell(),
	_floatingCheckPhase(false),
	_speed(),
	_acceleration(),
	_texs(texs),
    _bcolor(BubbleColor::Colorless),
	_resistence(0),
	_sprite()
{}

Bubble::~Bubble() {}

BubbleIdentifier Bubble::getIdentifier() const
{
	return { _model->name, _bcolor };
}

void Bubble::draw(sf::RenderTarget *const (&g))
{
	_sprite.draw(g);
	//g->draw(*this);
}







BubbleIdentifier BubbleIdentifier::invalid() { return { "", BubbleColor::Colorless }; }

bool BubbleIdentifier::operator== (const BubbleIdentifier& bi2) const { return model == bi2.model && color == bi2.color; }
bool BubbleIdentifier::operator!= (const BubbleIdentifier& bi2) const { return !(*this == bi2); }
bool BubbleIdentifier::operator> (const BubbleIdentifier& bi2) const { auto cmp = model.compare(bi2.model); return cmp > 0 || (cmp == 0 && color > bi2.color); }
bool BubbleIdentifier::operator>= (const BubbleIdentifier& bi2) const { auto cmp = model.compare(bi2.model); return cmp > 0 || (cmp == 0 && color >= bi2.color); }
bool BubbleIdentifier::operator< (const BubbleIdentifier& bi2) const { auto cmp = model.compare(bi2.model); return cmp < 0 || (cmp == 0 && color < bi2.color); }
bool BubbleIdentifier::operator<= (const BubbleIdentifier& bi2) const { auto cmp = model.compare(bi2.model); return cmp <= 0 || (cmp == 0 && color <= bi2.color); }
bool BubbleIdentifier::operator! () const { return model.empty(); }

BubbleIdentifier::operator bool() const { return !model.empty(); }

bool BubbleIdentifier::isValid() const { return !model.empty(); }
void BubbleIdentifier::invalidate() { *this = invalid(); }

Bubble* BubbleIdentifier::createBubble(TextureManager* textureManager, bool editorMode) const
{
	return !(*this) ? nullptr : CreateNewBubble(model, color, textureManager, editorMode);
}






BubbleManager::~BubbleManager()
{
	clear();
}

BubbleModel* BubbleManager::registerBubbleModel(const std::string& name)
{
	if (_models.find(name) != _models.end())
		return nullptr;
	return &(_models[name] = {});
}

void BubbleManager::setDefaultModel(const std::string& name)
{
	auto it = _models.find(name);
	if (it != _models.end())
		_default = &((*it).second);
}

void BubbleManager::deleteBubbleModel(const std::string& name)
{
	auto it = _models.find(name);
	if (it != _models.end())
	{
		BubbleModel* model = &(it->second);
		_models.erase(it);
	}
}

void BubbleManager::clear() { _models.clear(); }


Bubble* CreateNewBubble(const std::string& modelName, const BubbleColor& color, TextureManager* textureManager, bool editorMode)
{
	BubbleModel* model = bubman_hasBubbleModel(modelName)
		? bubman_getBubbleModel(modelName)
		: bubman_getDefaultModel();

	return CreateNewBubble(model, color, textureManager, editorMode);
}
Bubble* CreateNewBubble(BubbleModel* model, const BubbleColor& color, TextureManager* textureManager, bool editorMode)
{
	if (model == nullptr)
		return nullptr;

	Bubble* b = new Bubble{ model, textureManager };
	model->init(b, color.id(), editorMode);
	return b;
}

void DestroyBubble(const Bubble* bub)
{
	delete bub;
}

