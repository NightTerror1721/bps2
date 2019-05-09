#include "py.h"

#include "assets.h"
#include "bubbles.h"

#define BINDED(type) _##type
#define IF_BINDED(type, binded_var) type* binded_var = BINDED(type); if(binded_var)

#define DEF_PY_FUNC(module, name, function) (module).def((name), &__py__##function)
#define PUSH_PY_FUNC(module, function) DEF_PY_FUNC(module, #function, function)

PyInterpreter::PyInterpreter() :
	_py_int()
{}

py::object pylib::executePythonScript(const std::string& filepath)
{
	return py::eval_file(filepath);
}


/* PY FUNCTIONS */
namespace
{
	BubbleModel* PY(createBubbleModel) (const std::string& name, std::function<void()> precache)
	{
		IF_BINDED(BubbleModelManager, models)
		{
			Ptr<BubbleModel> model = models->createNew(name);
			precache();
			return &model;
		}
		return nullptr;
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

PYBIND11_EMBEDDED_MODULE(PBS_bubbleModel, m) {
	py::class_<BubbleModel> model{ m, "BubbleModel" };

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


	PUSH_PY_FUNC(m, createBubbleModel);
}

PYBIND11_EMBEDDED_MODULE(PBS_textures, m) {
	PUSH_PY_FUNC(m, loadTexture);
	PUSH_PY_FUNC(m, loadTextureFromPart);
}
