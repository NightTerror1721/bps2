#include "assets.h"
#include "utils.h"
#include "statics.h"

#include <chrono>

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

void __py__load_texture(const std::string& file, const std::string& tag, uint32 x, uint32 y, uint32 width, uint32 height)
{
	auto tm = get_static(texture_manager);
	if (tm)
		tm->loadTexture(file, tag, x, y, width, height);
}

PYBIND11_EMBEDDED_MODULE(__texture_import, m) {
	py::class_<TextureManager> tm(m, "TextureManager");

	tm.def("loadTexture", &__py__load_texture);

	//m.def("registerTexture")
}






void AnimatedSprite::setFrameDimensions(uint32 x, uint32 y, uint32 w, uint32 h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

void AnimatedSprite::setFrameCount(uint32 frames_count)
{
	_frames = frames_count;
}


void AnimatedSprite::setStaticMode()
{
	_mode = SPRITE_MODE_STATIC;
}

void AnimatedSprite::setSequenceMode()
{
	_mode = SPRITE_MODE_SEQUENCE;
}

void AnimatedSprite::setLoopMode()
{
	_mode = SPRITE_MODE_LOOP;
}

void AnimatedSprite::setRandomMode(delta_t min, delta_t max)
{
	_mode = SPRITE_MODE_RANDOM;
	_min = std::min(min, max);
	_max = std::max(min, max);
	
	_rand = std::minstd_rand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	generateCurrent();
}

#define move_it(delta) this->_it += (delta) * this->_speed
#define try_move_it(delta) if(this->_end) { break; } move_it(delta)

void AnimatedSprite::update(delta_t delta)
{
	switch (_mode)
	{
		case SPRITE_MODE_SEQUENCE:
			try_move_it(delta);
			if (state() != 0)
				_end = true;
			break;

		case SPRITE_MODE_LOOP:
			try_move_it(delta);
			updateIterator();
			break;

		case SPRITE_MODE_RANDOM:
			try_move_it(delta);
			if (_current > 0)
				_current -= delta;
			else
			{
				if (state() != 0)
				{
					updateIterator();
					generateCurrent();
				}
			}
			break;
	}
}

void AnimatedSprite::draw(sf::RenderTarget *const (&g))
{
	if(_mode == SPRITE_MODE_STATIC || !_end)
		g->draw(*this);
}


void AnimatedSprite::generateCurrent()
{
	const delta_t maxval = (_max - _min);
	delta_t percentage = (_rand() / static_cast<delta_t>(_rand.max()));
	_current = (maxval * percentage) + _min;
}

void AnimatedSprite::updateIterator()
{
	int current_state;
	while ((current_state = state()) != 0)
	{
		if (current_state > 0)
			_it -= static_cast<delta_t>(_frames);
		else _it += static_cast<delta_t>(_frames);
	}

	if (_it != _oldit)
	{
		int it = static_cast<int>(_it);
		this->setTextureRect({
			static_cast<int>(_x + it),
			static_cast<int>(_y + it),
			static_cast<int>(_w),
			static_cast<int>(_h)
		});
	}
	_oldit = _it;
}
