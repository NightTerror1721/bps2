#include "py.h"

#include <filesystem>
#include <iostream>

#include "bubbles.h"

py::object run_py_file(const std::string& filename)
{
	try
	{
		return py::eval_file(filename);
	}
	catch (py::error_already_set ex)
	{
		std::cout << ex.what() << std::endl;
		return py::none();
	}
}

/*py::object __py__run_py_file(const std::string& filename, py::object locals)
{
	if (!locals)
		return run_py_file(filename);
	return run_py_file(filename, locals);
}*/


#define T2V(tuple) Vec2f{ (tuple)[0].cast<float>(), (tuple)[1].cast<float>() }
#define V2T(vector) py::make_tuple((vector).x, (vector).y)


__DEF_PY_MODULE__(__DEF__PY_MODULE_STDLIB, m) {
	m.def("runfile", &run_py_file);
	m.attr("current_dir") = std::filesystem::current_path().c_str();
}

__DEF_PY_MODULE__(__DEF__PY_MODULE_TEXTURES, m) {
	py::class_<TextureManager> tm(m, "TextureManager");

	tm.def("loadTexture", static_cast<bool (TextureManager::*)(const std::string&, const std::string&, uint32, uint32, uint32, uint32)>(&TextureManager::loadTexture));
	//tm.def("getTexture", &TextureManager::getTexture);
	tm.def("hasTexture", &TextureManager::hasTexture);

	m.def("GetTextureManager", &__py__GetTextureManager, py::return_value_policy::reference);

	//tm.def("loadTexture", &__py__load_texture);

	//m.def("registerTexture")
}

__DEF_PY_MODULE__(__DEF__PY_MODULE_BUBBLES, m) {

	py::class_<BubbleModel> bm(m, "BubbleModel");

	bm.def_readwrite("requireColor", &BubbleModel::requireColor);
	bm.def_readwrite("onCollide", &BubbleModel::onCollide);
	bm.def_readwrite("onExplode", &BubbleModel::onExplode);
	bm.def_readwrite("onInserted", &BubbleModel::onInserted);
	bm.def_readwrite("onNeighborExplode", &BubbleModel::onNeighborExplode);
	bm.def_readwrite("onNeighborInserted", &BubbleModel::onNeighborInserted);
	bm.def_readwrite("init", &BubbleModel::init);



	m.def("registerBubbleModel", [](const std::string& name) -> BubbleModel* {
		return bubman_registerBubbleModel(name);
	});

	m.def("isRegisteredBubbleModel", [](const std::string& name) -> bool {
		return bubman_hasBubbleModel(name);
	});

	m.def("setDefaultBubbleModel", [](const std::string& name) {
		bubman_setDefaultModel(name);
	});

	m.def("getBubbleModel", [](const std::string& name) -> BubbleModel* {
		return bubman_getBubbleModel(name);
	});

	m.def("getDefaultBubbleModel", []() -> BubbleModel* {
		return bubman_getDefaultModel();
	});



	py::class_<BubbleColor> bc(m, "BubbleColor");

	bc.def_readonly_static("Red", &BubbleColor::Red);
	bc.def_readonly_static("Orange", &BubbleColor::Orange);
	bc.def_readonly_static("Yellow", &BubbleColor::Yellow);
	bc.def_readonly_static("Green", &BubbleColor::Green);
	bc.def_readonly_static("Blue", &BubbleColor::Blue);
	bc.def_readonly_static("Purple", &BubbleColor::Purple);
	bc.def_readonly_static("Gray", &BubbleColor::Gray);
	bc.def_readonly_static("Black", &BubbleColor::Black);
	bc.def_readonly_static("Multicolor", &BubbleColor::Multicolor);
	bc.def_readonly_static("Colorless", &BubbleColor::Colorless);

	bc.def("isNormalColor", &BubbleColor::isNormalColor);
	bc.def("isMulticolor", &BubbleColor::isMulticolor);
	bc.def("isColorless", &BubbleColor::isColorless);

	bc.def("match", &BubbleColor::match);

	bc.def("name", &BubbleColor::name);
	bc.def("__str__", &BubbleColor::name);

	bc.def("id", &BubbleColor::id);
	
	bc.def("__eq__", [](BubbleColor* bc1, BubbleColor* bc2) -> bool { return *bc1 == *bc2; });
	bc.def("__ne__", [](BubbleColor* bc1, BubbleColor* bc2) -> bool { return *bc1 != *bc2; });



	py::class_<Bubble, LocalAttrAllocator, sf::Transformable> b(m, "Bubble", py::multiple_inheritance());

	b.def("setLocalInt", &Bubble::setLocalInt);
	b.def("getLocalInt", &Bubble::getLocalInt, "name"_a, "default_value"_a = 0);
	b.def("deleteLocalInt", &Bubble::deleteLocalInt);

	b.def("setLocalFloat", &Bubble::setLocalFloat);
	b.def("getLocalFloat", &Bubble::getLocalFloat, "name"_a, "default_value"_a = 0.f);
	b.def("deleteLocalFloat", &Bubble::deleteLocalFloat);

	b.def("setLocalString", &Bubble::setLocalString);
	b.def("getLocalString", &Bubble::getLocalString, "name"_a, "default_value"_a = "");
	b.def("deleteLocalString", &Bubble::deleteLocalString);
	
}

