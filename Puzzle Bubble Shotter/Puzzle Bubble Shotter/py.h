#pragma once

#include <pybind11/embed.h>

namespace py = pybind11;

class PyInterpreter
{
private:
	py::scoped_interpreter _py_int;

public:
	PyInterpreter();
};
