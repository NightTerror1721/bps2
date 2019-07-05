#include "py.h"

#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <set>

#include "assets.h"
#include "bubbles.h"
#include "paths.h"

#include <functional>

#define BINDED(type) _##type
#define IF_BINDED(type, binded_var) type* binded_var = BINDED(type); if(binded_var)

#define DEF_PY_FUNC(module, name, function) (module).def((name), &__py__##function)
#define PUSH_PY_FUNC(module, function) DEF_PY_FUNC(module, #function, function)
#define PUSH_PY_FUNC_EXT(module, function, policy) (module).def(#function, &__py__##function, policy)

#define PUSH_PY_MET(py_class, _class, method) (py_class).def(#method, &_class::##method)

PyInterpreter::PyInterpreter() :
	_py_int()
{}

namespace
{
	TextureManager* _TextureManager{ nullptr };
	BubbleHeap* _BubbleHeap{ nullptr };
}

py::object pylib::executePythonScript(const std::string& filepath)
{
	try
	{
		return py::eval_file(filepath);
	}
	catch (py::error_already_set ex)
	{
		std::cout << "Python exception occurs: " << ex.what() << std::endl;
		return py::none();
	}
}

void loadConfigFile(const std::string& filePath, py::dict& locals, )

void pylib::loadConfig()
{
	try
	{
		py::dict locals{};
		py::eval_file(static_cast<std::string>(Path::ConfigFile), py::globals(), locals);

		Config& config = __ConfigInstance();
		for (auto& value : locals)
		{
			try
			{
				std::string key{ value.first.cast<std::string>() };
				if (value.second.is_none())
					config.setNull(key);
				else if (py::isinstance<py::bool_>(value.second))
					config.setBoolean(key, value.second.cast<bool>());
				else if (py::isinstance<py::int_>(value.second))
					config.setInteger(key, value.second.cast<int32>());
				else if (py::isinstance<py::float_>(value.second))
					config.setFloat(key, value.second.cast<float>());
				else if (py::isinstance<py::str>(value.second))
					config.setString(key, value.second.cast<std::string>());
				else config.setNull(key);
			}
			catch (...)
			{
				std::cout << "Invalid Config entry: " << value.first << " => " << value.second << std::endl;
			}
		}
	}
	catch (py::error_already_set ex)
	{
		std::cout << "Python exception occurs: " << ex.what() << std::endl;
	}
}

void pylib::bindTextureManager(TextureManager* const& ptr) { _TextureManager = ptr; }
void pylib::unbindTextureManager() { _TextureManager = nullptr; }

void pylib::bindBubbleHeap(BubbleHeap* const& ptr) { _BubbleHeap = ptr; }
void pylib::unbindBubbleHeap() { _BubbleHeap = nullptr; }


/* PY FUNCTIONS */
namespace
{
	BubbleModel* PY(createBubbleModel) (const std::string& name, std::function<void()> precache)
	{
		if (HasBubbleModel(name))
			return nullptr;

		Ptr<BubbleModel> model = RegisterBubbleModel(name);
		precache();
		return &model;
	}

	bool PY(existsBubbleModel) (const std::string& name)
	{
		return HasBubbleModel(name);
	}

	void PY(loadTexture) (const std::string& filename, const std::string& tag)
	{
		IF_BINDED(TextureManager, texs)
			texs->load(filename, tag);
	}

	void PY(loadTextureFromPart) (const std::string& filename, const std::string& tag, const u32& x, const u32& y, const u32& w, const u32& h)
	{
		IF_BINDED(TextureManager, texs)
			texs->load(filename, tag, x, y, w, h);
	}
}


/* MODULES */

PYBIND11_EMBEDDED_MODULE(PBS_bubbles, m) {
	py::class_<BubbleModel> model{ m, "BubbleModel" };

	model.def(py::self == py::self);
	model.def(py::self != py::self);

	model.def_readonly("name", &BubbleModel::name);

	model.def_readwrite("colorType", &BubbleModel::colorType);

	model.def_readwrite("floating", &BubbleModel::floating);
	model.def_readwrite("destroyInBottom", &BubbleModel::destroyInBottom);
	model.def_readwrite("requireDestroyToClear", &BubbleModel::requireDestroyToClear);

	model.def_readwrite("resistence", &BubbleModel::resistence);

	model.def_readwrite("pointsOfTurnsToDown", &BubbleModel::pointsOfTurnsToDown);

	model.def_readwrite("localInts", &BubbleModel::localInts);
	model.def_readwrite("localFloats", &BubbleModel::localFloats);
	model.def_readwrite("localStrings", &BubbleModel::localStrings);

	model.def_readwrite("init", &BubbleModel::init);

	model.def_readwrite("onCollide", &BubbleModel::onCollide);
	model.def_readwrite("onInserted", &BubbleModel::onInserted);
	model.def_readwrite("onExplode", &BubbleModel::onExplode);
	model.def_readwrite("onNeighborInserted", &BubbleModel::onNeighborInserted);
	model.def_readwrite("onNeighborExplode", &BubbleModel::onNeighborExplode);


	PUSH_PY_FUNC_EXT(m, createBubbleModel, py::return_value_policy::reference);
	PUSH_PY_FUNC(m, existsBubbleModel);



	/* Vector2D */
	py::class_<vec2f> v2f{ m, "Vector2" };

	v2f.def_readwrite("x", &vec2f::x);
	v2f.def_readwrite("y", &vec2f::y);

	v2f.def(py::init<>());
	v2f.def(py::init<float, float>());
	v2f.def(py::init<vec2f>());

	v2f.def(-py::self);
	v2f.def(py::self += py::self);
	v2f.def(py::self -= py::self);
	v2f.def(py::self + py::self);
	v2f.def(py::self - py::self);
	v2f.def(py::self * float{});
	v2f.def(float{} * py::self);
	v2f.def(py::self *= float{});
	v2f.def(py::self / float{});
	v2f.def(py::self /= float{});
	v2f.def(py::self == py::self);
	v2f.def(py::self != py::self);



	/* UniqueObject */
	py::class_<UniqueObject> uo{ m, "UniqueObject" };



	/* UniqueObject */
	py::class_<sf::Transformable> t{ m, "Transformable" };



	/* AnimatedSprite */
	py::class_<AnimatedSprite> as{ m, "AnimatedSprite" };

	as.def("setTexture", [](AnimatedSprite* self, const std::string& textureTag) {
		IF_BINDED(TextureManager, texs)
		{
			if (texs->has(textureTag))
				self->setTexture(*texs->get(textureTag));
		}
	});

	as.def("setFrameDimensions", &AnimatedSprite::setFrameDimensions);
	as.def("setFrameCount", &AnimatedSprite::setFrameCount);

	as.def("setStaticMode", &AnimatedSprite::setStaticMode);
	as.def("setSequenceMode", &AnimatedSprite::setSequenceMode);
	as.def("setLoopMode", &AnimatedSprite::setLoopMode);
	as.def("setRandomMode", &AnimatedSprite::setRandomMode);

	as.def("isStaticMode", &AnimatedSprite::isStaticMode);
	as.def("isSequenceMode", &AnimatedSprite::isSequenceMode);
	as.def("isLoopMode", &AnimatedSprite::isLoopMode);
	as.def("isRandomMode", &AnimatedSprite::isRandomMode);

	as.def("hasEnded", &AnimatedSprite::hasEnded);

	as.def("start", &AnimatedSprite::start);
	as.def("stop", &AnimatedSprite::stop);

	as.def("setCurrentFrame", &AnimatedSprite::setCurrentFrame);
	as.def("setExactCurrentFrame", &AnimatedSprite::setExactCurrentFrame);

	as.def("getCurrentFrame", &AnimatedSprite::getCurrentFrame);
	as.def("getExactCurrentFrame", &AnimatedSprite::getExactCurrentFrame);

	as.def("setFrameSpeed", &AnimatedSprite::setFrameSpeed);
	as.def("getFrameSpeed", &AnimatedSprite::getFrameSpeed);

	as.def("rewind", &AnimatedSprite::rewind);
	as.def("fastForward", &AnimatedSprite::fastForward);

	as.def("getFrameX", &AnimatedSprite::getFrameX);
	as.def("getFrameY", &AnimatedSprite::getFrameY);
	as.def("getFrameWidth", &AnimatedSprite::getFrameWidth);
	as.def("getFrameHeight", &AnimatedSprite::getFrameHeight);
	as.def("getFrameCount", &AnimatedSprite::getFrameCount);



	/* BubbleColor */
	py::class_<BubbleColor> bc{ m, "BubbleColor" };

	bc.def(py::self == py::self);
	bc.def(py::self != py::self);
	bc.def(py::self > py::self);
	bc.def(py::self < py::self);
	bc.def(py::self >= py::self);
	bc.def(py::self <= py::self);

	bc.def_property_readonly("id", [](BubbleColor* self) { return self->id(); });
	bc.def_property_readonly("name", [](BubbleColor* self) { return self->name(); });

	bc.def_static("count", &BubbleColor::count);
	bc.def_static("all", &BubbleColor::all);



	/* ColorType */
	py::enum_<ColorType> ct{ m, "ColorType" };
	ct.value("Colorless", ColorType::Colorless);
	ct.value("NormalColor", ColorType::NormalColor);
	ct.value("Multicolor", ColorType::Multicolor);
	ct.export_values();



	/* Bubble */
	py::class_<Bubble, UniqueObject, sf::Transformable> b{ m, "Bubble", py::multiple_inheritance() };

	b.def("getModel", [](Bubble* self) { return &self->getModel(); }, py::return_value_policy::reference);

	b.def("hasExploited", &Bubble::hasExploited);
	b.def("explode", &Bubble::explode);

	b.def("getPosition", &Bubble::getPosition);
	b.def("getSpeed", &Bubble::getSpeed);
	b.def("getAcceleration", &Bubble::getAcceleration);

	b.def("setColor", &Bubble::setColor);
	b.def("getColor", &Bubble::getColor);
	b.def("getColorType", &Bubble::getColorType);
	b.def("colorMatch", &Bubble::colorMatch);
	b.def("isNormalColor", [](Bubble* self) { return self->getColorType() == ColorType::NormalColor; });
	b.def("isMulticolor", [](Bubble* self) { return self->getColorType() == ColorType::Multicolor; });
	b.def("isColorless", [](Bubble* self) { return self->getColorType() == ColorType::Colorless; });

	b.def("getSprite", static_cast<AnimatedSprite* (Bubble::*)()>(&Bubble::getSprite), py::return_value_policy::reference);
	b.def("updateSpriteScale", &Bubble::updateSpriteScale);

	b.def("getResistence", &Bubble::getResistence);
	b.def("isIndestructible", &Bubble::isIndestructible);
	b.def("isFloating", &Bubble::isFloating);
	b.def("destroyInBottom", &Bubble::destroyInBottom);
	b.def("requireDestroyToClear", &Bubble::requireDestroyToClear);
	b.def("getPointsOfTurnsToDown", &Bubble::getPointsOfTurnsToDown);

	b.def("getLocalInt", &Bubble::getLocalInt);
	b.def("getLocalFloat", &Bubble::getLocalFloat);
	b.def("getLocalString", &Bubble::getLocalString);

	b.def("setLocalInt", &Bubble::setLocalInt);
	b.def("setLocalFloat", &Bubble::setLocalFloat);
	b.def("setLocalString", &Bubble::setLocalString);
}

PYBIND11_EMBEDDED_MODULE(PBS_textures, m) {
	PUSH_PY_FUNC(m, loadTexture);
	PUSH_PY_FUNC(m, loadTextureFromPart);
}
