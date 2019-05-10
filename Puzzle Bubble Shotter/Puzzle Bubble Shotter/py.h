#pragma once

#include <pybind11/embed.h>
#include <string>
#include <functional>

#include "config.h"

namespace py = pybind11;

class TextureManager;
class BubbleHeap;
struct BubbleModel;

class PyInterpreter
{
private:
	py::scoped_interpreter _py_int;

public:
	PyInterpreter();
};

#define __BINDED_PTR(type) namespace { type* _##type{ nullptr }; } \
namespace pylib { \
inline void bind##type (type* const& ptr) { _##type = ptr; } \
inline void unbind##type () { _##type = nullptr; } \
}


__BINDED_PTR(TextureManager)
__BINDED_PTR(BubbleHeap)


#undef __BINDED_PTR


namespace pylib
{
	py::object executePythonScript(const std::string& filepath);
}

#define PY(name) __py__##name

namespace // INNER PYTHON FUNCTIONS
{
	BubbleModel* PY(createBubbleModel) (const std::string& name, std::function<void()> precache);
	bool PY(existsBubbleModel) (const std::string& name);

	void PY(loadTexture) (const std::string& filename, const std::string& tag);
	void PY(loadTextureFromPart) (const std::string& filename, const std::string& tag, const u32& x, const u32& y, const u32& w, const u32& h);
}

