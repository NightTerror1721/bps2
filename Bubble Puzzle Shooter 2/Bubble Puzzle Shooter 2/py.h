#ifndef __PY_UTILS_CPP__
#define __PY_UTILS_CPP__

#include <pybind11/embed.h>

namespace py = pybind11;

py::object run_py_file(const std::string& filename, py::object locals = py::object());


#endif //__PY_UTILS_CPP__
