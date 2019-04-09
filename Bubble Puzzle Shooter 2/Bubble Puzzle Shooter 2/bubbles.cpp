#include "bubbles.h"

#include <SFML/Graphics.hpp>

BubbleManager BubbleManager::_instance;

const BubbleColor BubbleColor::Red(0);
const BubbleColor BubbleColor::Orange(1);
const BubbleColor BubbleColor::Yellow(2);
const BubbleColor BubbleColor::Green(3);
const BubbleColor BubbleColor::Blue(4);
const BubbleColor BubbleColor::Purple(5);
const BubbleColor BubbleColor::Gray(6);
const BubbleColor BubbleColor::Black(7);
const BubbleColor BubbleColor::Multicolor(8);
const BubbleColor BubbleColor::Colorless(9);

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






Bubble::Bubble(BubbleModel *const model, const bubble_code_t& code, TextureManager* texs) :
	LocalAttrAllocator(),
	Transformable(),
	_model(model),
	_code(code),
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

void Bubble::draw(sf::RenderTarget *const (&g))
{
	_sprite.draw(g);
	//g->draw(*this);
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

		auto cit = _codes.begin();
		while (cit != _codes.end())
		{
			if (cit->second == model)
				_codes.erase(cit++);
			else cit++;
		}

		_models.erase(it);
	}
}

void BubbleManager::linkCodeToModel(const bubble_code_t& code, const std::string& modelName)
{
	auto it = _models.find(modelName);
	if (it != _models.end())
		_codes[code] = &(it->second);
}

void BubbleManager::clear() { _models.clear(); }


Bubble* CreateNewBubble(const bubble_code_t& code, TextureManager* textureManager)
{
	BubbleModel* model = bubman_hasCode(code)
		? bubman_getBubbleModelFromCode(code)
		: bubman_getDefaultModel();

	return new Bubble{ model, code, textureManager };
}

void DestroyBubble(const Bubble* bub)
{
	delete bub;
}

