#ifndef __PY_UTILS_CPP__
#define __PY_UTILS_CPP__

#include <pybind11/embed.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace py::literals;

py::object __run_py_file(const std::string& filename, py::object locals = py::object());

#define run_py_file(filename, ...) __run_py_file((filename), py::dict(__VA_ARGS__))

#endif //__PY_UTILS_CPP__
