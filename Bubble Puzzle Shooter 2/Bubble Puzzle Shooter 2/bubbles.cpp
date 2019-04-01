#include "bubbles.h"
#include "statics.h"

#include <SFML/Graphics.hpp>

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






Bubble::Bubble(const std::string& tag, TextureManager* texs)
	: PhysicalEntity(tag), _exploited(false), _allocScenario(), _allocCell(),
	  _floatingCheckPhase(false), _speed(), _acceleration(), _texs(texs),
      _bcolor(BubbleColor::Colorless), _resistence(0), _sprite() {}

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

void BubbleManager::deleteBubbleModel(const std::string& name)
{
	auto it = _models.find(name);
	if (it != _models.end())
		_models.erase(it);
}

void BubbleManager::clear() { _models.clear(); }





PYBIND11_EMBEDDED_MODULE(__bubbles, m) {

	py::class_<BubbleModel> bm(m, "BubbleModel");

	bm.def_readwrite("onCollide", &BubbleModel::onCollide);
	bm.def_readwrite("onExplode", &BubbleModel::onExplode);
	bm.def_readwrite("onInserted", &BubbleModel::onInserted);
	bm.def_readwrite("onNeighborExplode", &BubbleModel::onNeighborExplode);
	bm.def_readwrite("onNeighborInserted", &BubbleModel::onNeighborInserted);
	bm.def_readwrite("init", &BubbleModel::init);

	m.def("registerBubbleModel", [](const std::string& name) -> BubbleModel* {
		auto bm = get_static(bubble_manager);
		if (bm)
			return bm->registerBubbleModel(name);
		return nullptr;
	});

	m.def("isRegisteredBubbleModel", [](const std::string& name) -> bool {
		auto bm = get_static(bubble_manager);
		return bm && bm->hasBubbleModel(name);
	});

}

