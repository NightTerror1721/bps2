#pragma once

#include <string>
#include <array>

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

namespace Path
{
	class path_t;
}

Path::path_t operator""_p(const char* value, std::size_t size);

namespace Path
{
	class path_t
	{
	private:
		std::string _path;

	public:
		path_t(const std::string& path);

		path_t operator+ (const std::string& path) const;
		path_t operator+ (const path_t& path) const;

		path_t& operator+= (const std::string& path);
		path_t& operator+= (const path_t& path);

		path_t& operator<< (const std::string& path);
		path_t& operator<< (const path_t& path);

		operator std::string() const;

		const path_t& operator>> (std::string& path) const;
		const path_t& operator>> (path_t& path) const;
	};

	const path_t Textures{ "data"_p << "textures" };
	const path_t Sounds{ "data"_p << "audio" << "sounds" };
	const path_t Musics{ "data"_p << "audio" << "musics" };
}
