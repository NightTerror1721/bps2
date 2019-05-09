#include "assets.h"

#include <chrono>

#include "paths.h"

TextureManager::TextureManager(TextureManager* const& parent) :
	Manager(parent)
{}

void TextureManager::load(const std::string& texture_path, const std::string& name)
{
	auto tex = create(name);
	tex->loadFromFile(Path::Textures + texture_path);
}

void TextureManager::load(const std::string& texture_path, const std::string& name, const sf::IntRect& dims)
{
	auto tex = create(name);
	tex->loadFromFile(Path::Textures + texture_path, dims);
}

void TextureManager::load(const std::string& texture_path, const std::string& name, const int& x, const int& y, const int& width, const int& height)
{
	auto tex = create(name);
	tex->loadFromFile(Path::Textures + texture_path, { x, y, width, height });
}




void AnimatedSprite::setFrameDimensions(const u32& x, const u32& y, const u32& w, const u32& h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}
void AnimatedSprite::setFrameCount(const u32& frames_count) { _frames = frames_count; }

void AnimatedSprite::setStaticMode() { _mode = Mode::Static; }
void AnimatedSprite::setSequenceMode() { _mode = Mode::Sequence; }
void AnimatedSprite::setLoopMode() { _mode = Mode::Loop; }
void AnimatedSprite::setRandomMode(const delta_t& min, const delta_t& max)
{
	_mode = Mode::Random;
	_min = std::min(min, max);
	_max = std::max(min, max);

	_rand = std::minstd_rand{ static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) };
	generateCurrent();
}

bool AnimatedSprite::isStaticMode() const { return _mode == Mode::Static; }
bool AnimatedSprite::isSequenceMode() const { return _mode == Mode::Sequence; }
bool AnimatedSprite::isLoopMode() const { return _mode == Mode::Loop; }
bool AnimatedSprite::isRandomMode() const { return _mode == Mode::Random; }

bool AnimatedSprite::hasEnded() const { return _end; }

void AnimatedSprite::start() { _end = false; }
void AnimatedSprite::stop() { _end = true; }

void AnimatedSprite::setCurrentFrame(const u32& frame) { _it = static_cast<delta_t>(frame); }
void AnimatedSprite::setExactCurrentFrame(const delta_t& frame) { _it = frame; }

u32 AnimatedSprite::getCurrentFrame() const { return static_cast<u32>(_it); }
delta_t AnimatedSprite::getExactCurrentFrame() const { return _it; }

void AnimatedSprite::rewind() { _it = 0.f; }
void AnimatedSprite::fastForward() { _it = static_cast<delta_t>(_frames); }

u32 AnimatedSprite::getFrameX() const { return _x; }
u32 AnimatedSprite::getFrameY() const { return _y; }
u32 AnimatedSprite::getFrameWidth() const { return _w; }
u32 AnimatedSprite::getFrameHeight() const { return _h; }
u32 AnimatedSprite::getFrameCount() const { return _frames; }

#define try_move_it(delta) if(this->_end) { break; } this->_it += (delta) * this->_speed
void AnimatedSprite::update(const delta_t& delta)
{
	switch (_mode)
	{
		case Mode::Sequence:
			try_move_it(delta);
			if (state())
				_end = true;
			break;

		case Mode::Loop:
			try_move_it(delta);
			updateIterator();
			break;

		case Mode::Random:
			try_move_it(delta);
			if (_current > 0)
				_current -= delta;
			else
			{
				if (state())
				{
					updateIterator();
					generateCurrent();
				}
			}
	}
}
#undef try_move_it

void AnimatedSprite::draw(sf::RenderTarget* const& g)
{
	if (_mode == Mode::Static || !_end)
		g->draw(*this);
}

int AnimatedSprite::state() const { return _it >= static_cast<delta_t>(_frames) ? 1 : _it < 0 ? -1 : 0; }

void AnimatedSprite::generateCurrent()
{
	const delta_t maxval = _max - _min;
	delta_t percentage = (_rand() / static_cast<delta_t>(_rand.max()));
	_current = (maxval * percentage) + _min;
}

void AnimatedSprite::updateIterator()
{
	int current_state;
	while (current_state = state())
	{
		if (current_state > 0)
			_it -= static_cast<delta_t>(_frames);
		else _it += static_cast<delta_t>(_frames);
	}

	if (_it != _oldIt)
	{
		int it = static_cast<int>(_it);
		setTextureRect({
			static_cast<int>(_x + it),
			static_cast<int>(_y + it),
			static_cast<int>(_w),
			static_cast<int>(_h)
		});
	}
	_oldIt = _it;
}
