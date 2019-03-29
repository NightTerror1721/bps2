#include "py.h"

#include <filesystem>
#include <iostream>

py::object run_py_file(const std::string& filename, py::object locals)
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

PYBIND11_EMBEDDED_MODULE(__stdlib, m) {
	m.def("runfile", &run_py_file, py::arg("filename"), py::arg("locals") = py::dict());
	m.attr("current_dir") = std::filesystem::current_path().c_str();
}

