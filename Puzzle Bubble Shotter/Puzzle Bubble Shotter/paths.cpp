#include "paths.h"

using namespace Path;

path_t::path_t(const std::string& path) :
	_path(path)
{}

path_t path_t::operator+ (const std::string& path) const { return { _path + PATH_SEPARATOR + path }; }
path_t path_t::operator+ (const path_t& path) const { return { _path + PATH_SEPARATOR + path._path }; }

path_t& path_t::operator+= (const std::string& path) { _path += PATH_SEPARATOR + path; return *this; }
path_t& path_t::operator+= (const path_t& path) { _path += PATH_SEPARATOR + path._path; return *this; }

path_t& path_t::operator<< (const std::string& path) { _path += PATH_SEPARATOR + path; return *this; }
path_t& path_t::operator<< (const path_t& path) { _path += PATH_SEPARATOR + path._path; return *this; }

path_t::operator std::string() const { return _path; }

const path_t& path_t::operator>> (std::string& path) const { path = _path; return *this; }
const path_t& path_t::operator>> (path_t& path) const { path = *this; return *this; }

Path::path_t operator""_p(const char* value, std::size_t size) { return path_t{ std::string{ value, size } }; }
