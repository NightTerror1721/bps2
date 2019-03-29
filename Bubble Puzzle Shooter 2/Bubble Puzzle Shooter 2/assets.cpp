#include "assets.h"
#include "utils.h"

TextureManager::TextureManager() :_tex() {}
TextureManager::~TextureManager()
{
	destroyAllTextures();
}

Texture* TextureManager::loadTexture(const std::string& file, const std::string& tag, const sf::IntRect& area)
{
	if (_tex.find(tag) != _tex.end())
		return nullptr;
	Texture* tex = new Texture();
	if (tex->loadFromFile(file, area))
	{
		_tex[tag] = tex;
		return tex;
	}
	else return nullptr;
}

void TextureManager::destroyTexture(const std::string& tag)
{
	auto it = _tex.find(tag);
	if (it != _tex.end())
	{
		auto tex = (*it).second;
		_tex.erase(it);
		delete tex;
	}
}

Texture* TextureManager::getTexture(const std::string& tag) const { return _tex.at(tag); }
Texture* TextureManager::operator[] (const std::string& tag) const { return _tex.at(tag); }

void TextureManager::destroyAllTextures()
{
	if (!_tex.empty())
	{
		auto it = _tex.begin();
		while (it != _tex.end())
		{
			auto all = *it;
			auto tex = all.second;
			delete tex;
			it++;
		}
		_tex.clear();
	}
}


void bind(Texture* (&texture)) { Texture::bind(texture); }
void unbind() { Texture::bind(NULL); }

PYBIND11_EMBEDDED_MODULE(__texture_import, m) {
	py::class_<TextureManager> tm(m, "TextureManager");

	tm.def("loadTexture", &TextureManager::__py__loadTexture);

	//m.def("registerTexture")
}
