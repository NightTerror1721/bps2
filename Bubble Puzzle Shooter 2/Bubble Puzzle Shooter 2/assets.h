#pragma once

#include <SFML/Graphics.hpp>
#include "utils.h"
#include "py.h"

#include <map>
#include <random>

using sf::Texture;
using native_sprite = sf::Sprite;

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

	void destroyTexture(const std::string& tag);

	Texture* getTexture(const std::string& tag) const;
	Texture* operator[] (const std::string& tag) const;

	void destroyAllTextures();
};

void bind(Texture* (&texture));
void unbind();


/*class Sprite : public native_sprite
{
public:
	virtual void update(delta_t delta);
	virtual void draw(sf::RenderTarget *const (&g));
};*/

#define SPRITE_MODE_STATIC 0
#define SPRITE_MODE_SEQUENCE 1
#define SPRITE_MODE_LOOP 2
#define SPRITE_MODE_RANDOM 3

class AnimatedSprite : public native_sprite
{
private:
	uint32 _x;
	uint32 _y;
	uint32 _w;
	uint32 _h;
	uint32 _frames;

	uint8 _mode;

	delta_t _min;
	delta_t _max;
	delta_t _current;
	std::minstd_rand _rand;

	delta_t _it;
	delta_t _oldit;
	delta_t _speed;
	bool _end;

public:
	void setFrameDimensions(uint32 x, uint32 y, uint32 w, uint32 h);
	void setFrameCount(uint32 frames_count);

	void setStaticMode();
	void setSequenceMode();
	void setLoopMode();
	void setRandomMode(delta_t min, delta_t max);

	inline bool isStaticMode() const { return _mode == SPRITE_MODE_STATIC; }
	inline bool isSequenceMode() const { return _mode == SPRITE_MODE_SEQUENCE; }
	inline bool isLoopMode() const { return _mode == SPRITE_MODE_LOOP; }
	inline bool isRandomMode() const { return _mode == SPRITE_MODE_RANDOM; }

	inline bool hasEnded() const { return _end; }

	inline void start() { _end = false; }
	inline void stop() { _end = true; }

	inline void setCurrentFrame(uint32 frame) { _it = static_cast<delta_t>(frame); }
	inline void setExactCurrentFrame(delta_t frame) { _it = frame; }

	inline uint32 getCurrentFrame() const { return static_cast<uint32>(_it); }
	inline delta_t getExactCurrentFrame() const { return _it; }

	inline void rewind() { _it = 0.f; }
	inline void fastForward() { _it = static_cast<delta_t>(_frames); }

	inline uint32 getFrameX() const { return _x; }
	inline uint32 getFrameY() const { return _y; }
	inline uint32 getFrameWidth() const { return _w; }
	inline uint32 getFrameHeight() const { return _h; }
	inline uint32 getFrameCount() const { return _frames; }

	void update(delta_t delta);
	void draw(sf::RenderTarget *const (&g));

private:
	inline int state() const { return _it >= 0 ? 1 : _it < 0 ? -1 : 0; }
	void generateCurrent();
	void updateIterator();
};


