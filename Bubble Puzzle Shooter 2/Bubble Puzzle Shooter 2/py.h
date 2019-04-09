#ifndef __PY_UTILS_CPP__
#define __PY_UTILS_CPP__

#include <pybind11/embed.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace py::literals;

py::object run_py_file(const std::string& filename);

#define __DEF_TO_STRING__(x) #x

#define __DEF_PY_MODULE__(module_identifier, module_var) PYBIND11_EMBEDDED_MODULE(module_identifier, module_var)

#define __DEF__PY_MODULE_STDLIB __stdlib
#define __DEF__PY_MODULE_TEXTURES __texture_import
#define __DEF__PY_MODULE_BUBBLES __bubbles

#define PY_MODULE_STDLIB __DEF_TO_STRING__(__DEF__PY_MODULE_STDLIB)
#define PY_MODULE_TEXTURES __DEF_TO_STRING__(__DEF__PY_MODULE_TEXTURES)
#define PY_MODULE_BUBBLES __DEF_TO_STRING__(__DEF__PY_MODULE_BUBBLES)


#endif //__PY_UTILS_CPP__
