#pragma once

#include <SFML/Graphics.hpp>
#include "utils.h"
#include "py.h"

#include <map>

using sf::Texture;
using sf::Sprite;

class TextureManager
{
private:
	std::map<std::string, Texture*> _tex;

public:
	TextureManager();
	~TextureManager();

	Texture* loadTexture(const std::string& file, const std::string& tag, const sf::IntRect& area);
	inline Texture* loadTexture(const std::string& file, const std::string& tag, uint32 x, uint32 y, uint32 width, uint32 height)
	{
		return loadTexture(file, tag, sf::IntRect(x, y, width, height));
	}

	void __py__loadTexture(std::string& file, std::string& tag, uint32 x, uint32 y, uint32 width, uint32 height)
	{
		loadTexture(file, tag, sf::IntRect(x, y, width, height));
	}

	void destroyTexture(const std::string& tag);

	Texture* getTexture(const std::string& tag) const;
	Texture* operator[] (const std::string& tag) const;

	void destroyAllTextures();
};

void bind(Texture* (&texture));
void unbind();

